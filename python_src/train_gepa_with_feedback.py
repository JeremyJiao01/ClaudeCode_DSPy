#!/usr/bin/env python3
"""
GEPA训练脚本 - 带自然语言反馈

参考 demos/gepa/main.py 架构的关键改进：
1. Metric返回 dspy.Prediction(score=float, feedback=str)
2. 使用grader_lm生成诊断性反馈
3. 支持GEPA的reflective evolution机制

这是我们之前失败的根本原因！
"""

import dspy
from dspy import GEPA
import os
import sys
import json
from collections import defaultdict
from pathlib import Path

# 添加项目路径
current_dir = os.path.dirname(os.path.abspath(__file__))
project_root = os.path.dirname(current_dir)
sys.path.insert(0, os.path.join(project_root, 'ipynb'))

from bug_detector_gepa_native import (
    BugPresenceDetector,
    BugCategoryClassifier,
    map_bug_type_to_category
)


print("=" * 80)
print("GEPA训练 - 带自然语言反馈")
print("=" * 80)
print()
print("关键改进：")
print("  ✓ Metric返回 dspy.Prediction(score=, feedback=)")
print("  ✓ 使用LLM生成诊断性反馈")
print("  ✓ 支持GEPA的reflective evolution")
print()
print("=" * 80)
print()


# ============================================================================
# 1. 配置模型
# ============================================================================

def setup_dspy_models():
    """配置DSPy模型"""
    print("[1/6] 配置模型")
    print("-" * 80)

    # 主模型（用于预测）
    gemini_api_key = ''
    if not gemini_api_key:
        print("❌ 错误: 未设置 GEMINI_API_KEY")
        print("   请运行: export GEMINI_API_KEY='your-api-key'")
        sys.exit(1)

    base_lm = dspy.LM(
        model='gemini/gemini-2.0-flash-exp',
        api_key=gemini_api_key,
        max_tokens=4000
    )
    dspy.configure(lm=base_lm)
    print("✓ 主模型: Gemini 2.0 Flash")

    # Grader模型（用于生成反馈和反思）
    claude_api_key = ''
    if not claude_api_key:
        print("⚠️  未设置 ANTHROPIC_API_KEY")
        print("   使用Gemini作为grader（效果会差一些）")
        print("   建议: export ANTHROPIC_API_KEY='your-key'")
        grader_lm = base_lm  # 降级使用Gemini
    else:
        grader_lm = dspy.LM(
            model="anthropic/claude-opus-4-5",
            api_key=claude_api_key,
            temperature=1.0,
            max_tokens=8000
        )
        print("✓ Grader模型: Claude Opus 4.5")

    print()
    return base_lm, grader_lm


# ============================================================================
# 2. 带反馈的评估指标（关键改进！）
# ============================================================================

class BugPresenceAssessment(dspy.Signature):
    """评估bug检测结果并提供诊断反馈"""

    code_snippet: str = dspy.InputField(desc="待分析的代码片段")
    expected_has_bug: bool = dspy.InputField(desc="真实答案：代码是否有bug")
    predicted_has_bug: bool = dspy.InputField(desc="模型预测：代码是否有bug")
    predicted_reasoning: str = dspy.InputField(desc="模型的推理过程")

    feedback: str = dspy.OutputField(desc="诊断性反馈，解释预测正确/错误的原因，提供改进建议")


class BugCategoryAssessment(dspy.Signature):
    """评估bug分类结果并提供诊断反馈"""

    code_snippet: str = dspy.InputField(desc="待分析的代码片段")
    expected_category: str = dspy.InputField(desc="真实的bug类别（MEMORY/LOGIC/SIGNATURE）")
    predicted_category: str = dspy.InputField(desc="模型预测的bug类别")
    predicted_explanation: str = dspy.InputField(desc="模型的解释")

    feedback: str = dspy.OutputField(desc="诊断性反馈，解释分类正确/错误的原因，提供类别特征说明")


def make_presence_metric(grader_lm: dspy.LM, debug: bool = False):
    """
    创建带反馈的Bug存在检测指标

    参考 demos/gepa/main.py 的 make_metric 模式

    关键：返回 dspy.Prediction(score=, feedback=) 而不是 float
    """

    def metric(gold, pred, trace=None, pred_name=None, pred_trace=None):
        try:
            # 1. 计算分数
            gold_has_bug = gold.has_bug
            pred_has_bug = pred.has_bug

            score = 1.0 if pred_has_bug == gold_has_bug else 0.0

            # 2. 生成自然语言反馈（关键！）
            # 使用grader_lm分析预测结果
            with dspy.context(lm=grader_lm):
                assessor = dspy.Predict(BugPresenceAssessment)

                assessment = assessor(
                    code_snippet=gold.code_snippet[:500],  # 限制长度
                    expected_has_bug=gold_has_bug,
                    predicted_has_bug=pred_has_bug,
                    predicted_reasoning=getattr(pred, 'reasoning', 'No reasoning provided')
                )

                feedback_text = assessment.feedback

            # 3. 返回 dspy.Prediction 对象（这是GEPA需要的格式！）
            if debug:
                print(f"\n[Metric] Score: {score:.1f}")
                print(f"[Metric] Feedback: {feedback_text[:100]}...")

            return dspy.Prediction(
                score=score,
                feedback=feedback_text
            )

        except Exception as e:
            # 发生错误时返回简单反馈
            if debug:
                print(f"[Metric Error] {e}")

            return dspy.Prediction(
                score=0.0,
                feedback=f"Evaluation error: {str(e)[:100]}"
            )

    return metric


def make_category_metric(grader_lm: dspy.LM, debug: bool = False):
    """
    创建带反馈的Bug分类指标

    参考 demos/gepa/main.py 的 make_metric 模式
    """

    # Bug类别特征（用于反馈生成）
    CATEGORY_HINTS = {
        "MEMORY": "涉及指针、内存分配/释放、空指针解引用、Use-after-free、内存泄漏",
        "LOGIC": "涉及条件判断、控制流、循环逻辑、边界检查、赋值vs比较",
        "SIGNATURE": "涉及函数调用、参数类型、API使用、返回值处理"
    }

    def metric(gold, pred, trace=None, pred_name=None, pred_trace=None):
        try:
            # 1. 计算分数
            gold_category = gold.bug_category
            pred_category = pred.category

            score = 1.0 if pred_category == gold_category else 0.0

            # 2. 生成自然语言反馈
            with dspy.context(lm=grader_lm):
                assessor = dspy.Predict(BugCategoryAssessment)

                assessment = assessor(
                    code_snippet=gold.code_snippet[:500],
                    expected_category=gold_category,
                    predicted_category=pred_category,
                    predicted_explanation=getattr(pred, 'explanation', 'No explanation provided')
                )

                # 增强反馈：添加类别特征提示
                if score < 1.0:
                    hint = CATEGORY_HINTS.get(gold_category, "")
                    feedback_text = (
                        f"{assessment.feedback}\n\n"
                        f"提示：{gold_category} 类型的bug通常{hint}。"
                    )
                else:
                    feedback_text = assessment.feedback

            if debug:
                print(f"\n[Metric] Score: {score:.1f}")
                print(f"[Metric] Expected: {gold_category}, Predicted: {pred_category}")
                print(f"[Metric] Feedback: {feedback_text[:100]}...")

            return dspy.Prediction(
                score=score,
                feedback=feedback_text
            )

        except Exception as e:
            if debug:
                print(f"[Metric Error] {e}")

            return dspy.Prediction(
                score=0.0,
                feedback=f"Evaluation error: {str(e)[:100]}"
            )

    return metric


# ============================================================================
# 3. 数据加载
# ============================================================================

def load_dataset_for_presence(metadata_path, max_samples=150):
    """加载Bug存在检测数据集"""
    print("[2/6] 加载Bug检测数据集")
    print("-" * 80)

    with open(metadata_path, 'r') as f:
        metadata = json.load(f)

    # 有bug样本
    bug_examples = []
    for item in metadata[:max_samples]:
        func_path = os.path.join(project_root, 'bug_source_code',
                                item['files']['buggy_function'])

        try:
            with open(func_path, 'r') as f:
                code_snippet = f.read()

            example = dspy.Example(
                code_snippet=code_snippet,
                has_bug=True
            ).with_inputs('code_snippet')

            bug_examples.append(example)
        except Exception as e:
            continue

    # 无bug样本（fixed版本）
    no_bug_examples = []
    for item in metadata[:max_samples//3]:
        func_path = os.path.join(project_root, 'bug_source_code',
                                item['files']['fixed_function'])
        try:
            with open(func_path, 'r') as f:
                code_snippet = f.read()

            example = dspy.Example(
                code_snippet=code_snippet,
                has_bug=False
            ).with_inputs('code_snippet')

            no_bug_examples.append(example)
        except:
            continue

    all_examples = bug_examples + no_bug_examples
    print(f"✓ 总样本: {len(all_examples)} (有bug: {len(bug_examples)}, 无bug: {len(no_bug_examples)})")

    # 8:2分割
    split_point = int(len(all_examples) * 0.8)
    trainset = all_examples[:split_point]
    valset = all_examples[split_point:]

    print(f"✓ 训练集: {len(trainset)}")
    print(f"✓ 验证集: {len(valset)}")
    print()

    return trainset, valset


def load_dataset_for_category(metadata_path, max_samples=150):
    """加载Bug分类数据集"""
    print("[3/6] 加载Bug分类数据集")
    print("-" * 80)

    with open(metadata_path, 'r') as f:
        metadata = json.load(f)

    bugs_by_category = defaultdict(list)

    for item in metadata[:max_samples]:
        bug_type_id = item['bug_type_id']
        category = map_bug_type_to_category(bug_type_id)

        func_path = os.path.join(project_root, 'bug_source_code',
                                item['files']['buggy_function'])

        try:
            with open(func_path, 'r') as f:
                code_snippet = f.read()

            example = dspy.Example(
                code_snippet=code_snippet,
                has_bug=True,
                bug_category=category,
                bug_type_id=bug_type_id
            ).with_inputs('code_snippet')

            bugs_by_category[category].append(example)
        except:
            continue

    print("类别分布:")
    for category in sorted(bugs_by_category.keys()):
        print(f"  {category}: {len(bugs_by_category[category])} 个")

    all_examples = []
    for examples in bugs_by_category.values():
        all_examples.extend(examples)

    print(f"✓ 总样本: {len(all_examples)}")

    split_point = int(len(all_examples) * 0.8)
    trainset = all_examples[:split_point]
    valset = all_examples[split_point:]

    print(f"✓ 训练集: {len(trainset)}")
    print(f"✓ 验证集: {len(valset)}")
    print()

    return trainset, valset


# ============================================================================
# 4. 训练函数
# ============================================================================

def train_presence_detector(trainset, valset, base_lm, grader_lm):
    """训练Bug存在检测器"""
    print("[4/6] 训练Bug存在检测器")
    print("-" * 80)

    # 创建检测器
    detector = BugPresenceDetector()

    # 创建带反馈的metric
    metric = make_presence_metric(grader_lm, debug=False)

    # 配置GEPA（参考demos架构）
    print("配置GEPA优化器...")
    optimizer = GEPA(
        metric=metric,  # 使用带反馈的metric！
        auto="heavy",   # 或 "light"/"medium"
        num_threads=4,
        track_stats=True,
        reflection_minibatch_size=3,  # 参考demos
        reflection_lm=grader_lm  # 使用grader进行反思
    )
    print("✓ GEPA配置完成")
    print()

    # 开始训练
    print("开始GEPA优化...")
    print("(这可能需要10-20分钟)")
    print()

    try:
        optimized_detector = optimizer.compile(
            detector,
            trainset=trainset[:50],  # 使用较小子集
            valset=valset[:20]
        )

        print("✓ 优化完成！")
        print()

        return optimized_detector

    except Exception as e:
        print(f"✗ 优化失败: {e}")
        import traceback
        traceback.print_exc()
        return detector


def train_category_classifier(trainset, valset, base_lm, grader_lm):
    """训练Bug分类器"""
    print("[5/6] 训练Bug分类器")
    print("-" * 80)

    # 创建分类器
    classifier = BugCategoryClassifier()

    # 创建带反馈的metric
    metric = make_category_metric(grader_lm, debug=False)

    # 配置GEPA
    print("配置GEPA优化器...")
    optimizer = GEPA(
        metric=metric,
        auto="heavy",
        num_threads=4,
        track_stats=True,
        reflection_minibatch_size=3,
        reflection_lm=grader_lm
    )
    print("✓ GEPA配置完成")
    print()

    # 开始训练
    print("开始GEPA优化...")
    print("(这可能需要10-20分钟)")
    print()

    try:
        optimized_classifier = optimizer.compile(
            classifier,
            trainset=trainset[:50],
            valset=valset[:20]
        )

        print("✓ 优化完成！")
        print()

        return optimized_classifier

    except Exception as e:
        print(f"✗ 优化失败: {e}")
        import traceback
        traceback.print_exc()
        return classifier


# ============================================================================
# 5. 主流程
# ============================================================================

def main():
    """主训练流程"""
    print()
    print("=" * 80)
    print("开始训练")
    print("=" * 80)
    print()

    # 1. 配置模型
    base_lm, grader_lm = setup_dspy_models()

    # 2. 加载数据
    metadata_path = os.path.join(project_root, 'bug_source_code', 'metadata.json')

    presence_trainset, presence_valset = load_dataset_for_presence(
        metadata_path,
        max_samples=150
    )

    category_trainset, category_valset = load_dataset_for_category(
        metadata_path,
        max_samples=150
    )

    # 3. 训练Bug存在检测器
    optimized_presence = train_presence_detector(
        presence_trainset,
        presence_valset,
        base_lm,
        grader_lm
    )

    # 4. 训练Bug分类器
    optimized_category = train_category_classifier(
        category_trainset,
        category_valset,
        base_lm,
        grader_lm
    )

    # 5. 保存模型
    print("[6/6] 保存模型")
    print("-" * 80)

    model_dir = Path(project_root) / 'models'
    model_dir.mkdir(exist_ok=True)

    presence_path = model_dir / 'gepa_feedback_presence_detector.json'
    category_path = model_dir / 'gepa_feedback_category_classifier.json'

    try:
        optimized_presence.save(str(presence_path))
        print(f"✓ Bug检测器: {presence_path}")

        optimized_category.save(str(category_path))
        print(f"✓ Bug分类器: {category_path}")
    except Exception as e:
        print(f"✗ 保存失败: {e}")

    print()
    print("=" * 80)
    print("训练完成！")
    print("=" * 80)
    print()

    print("关键改进验证:")
    print("  ✓ Metric返回 dspy.Prediction(score=, feedback=)")
    print("  ✓ 使用grader_lm生成自然语言反馈")
    print("  ✓ GEPA可以进行reflective evolution")
    print()

    print("下一步:")
    print("  1. 检查训练日志中的反馈生成")
    print("  2. 验证GEPA是否捕获了轨迹")
    print("  3. 对比优化前后的性能")
    print()

    print("=" * 80)


if __name__ == "__main__":
    main()
