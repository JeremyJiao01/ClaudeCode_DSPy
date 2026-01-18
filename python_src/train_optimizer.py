"""
DSPy Bug检测器优化训练脚本

使用BootstrapFewShot优化器训练bug检测模型
"""

import dspy
from dspy.teleprompt import BootstrapFewShot
import os
import sys
import json

# 添加项目路径
current_dir = os.path.dirname(os.path.abspath(__file__))
project_root = os.path.dirname(current_dir)
sys.path.insert(0, os.path.join(project_root, 'ipynb'))

from code_bug_detector import CodeBugDetector, BUG_TYPE_MAPPING
from data_loader import load_bug_detection_dataset


def setup_dspy_model():
    """配置DSPy模型"""
    print("=" * 80)
    print("配置DSPy模型")
    print("=" * 80)

    # 使用环境变量中的API key
    api_key = os.getenv('GOOGLE_API_KEY')
    if not api_key:
        print("错误: 未找到 GOOGLE_API_KEY 环境变量")
        print("请设置: export GOOGLE_API_KEY='your-api-key'")
        return False

    try:
        lm = dspy.LM(
            model='gemini/gemini-3-flash-preview',
            api_key=api_key,
            max_tokens=4000
        )

        dspy.configure(lm=lm)
        print("✓ DSPy模型配置完成 (Gemini 1.5 Flash)\n")
        return True
    except Exception as e:
        print(f"✗ 配置模型失败: {e}")
        return False


def bug_detection_metric(example, prediction, trace=None):
    """
    评估指标函数

    评估模型预测的准确性：
    1. has_bug 是否正确
    2. 如果有bug，bug类型是否正确

    Args:
        example: 真实样本
        prediction: 模型预测
        trace: 追踪信息（可选）

    Returns:
        分数 (0-1之间)
    """
    score = 0.0

    try:
        # 解析预测的has_bug
        pred_has_bug = str(prediction.has_bug).lower() in ['true', 'yes', '1']
        true_has_bug = example.has_bug

        # 检查has_bug是否正确 (权重: 0.5)
        if pred_has_bug == true_has_bug:
            score += 0.5

        # 如果真实样本有bug，检查bug类型是否正确 (权重: 0.5)
        if true_has_bug:
            try:
                pred_details = json.loads(prediction.bug_details)
                true_details = json.loads(example.bug_details)

                pred_bugs = pred_details.get('bugs', [])
                true_bugs = true_details.get('bugs', [])

                # 如果预测也有bug，检查类型
                if pred_bugs and true_bugs:
                    pred_type = pred_bugs[0].get('bug_type_id', '')
                    true_type = true_bugs[0].get('bug_type_id', '')

                    # 类型完全匹配
                    if pred_type == true_type:
                        score += 0.5
                    # 类型部分匹配（主类别相同）
                    elif pred_type.split('.')[0] == true_type.split('.')[0]:
                        score += 0.25
            except:
                # JSON解析失败，不加分
                pass
        else:
            # 真实样本无bug，如果预测也正确识别为无bug，加分
            try:
                pred_details = json.loads(prediction.bug_details)
                if not pred_details.get('bugs', []):
                    score += 0.5
            except:
                pass
    except Exception as e:
        # 如果出现任何错误，返回0分
        return 0.0

    return score


def validate_model(model, valset, num_samples=20):
    """
    验证模型性能

    Args:
        model: 要验证的模型
        valset: 验证集
        num_samples: 验证样本数量

    Returns:
        平均分数
    """
    print(f"\n验证模型性能 (使用 {min(num_samples, len(valset))} 个样本)...")

    total_score = 0.0
    correct_predictions = 0

    for i, example in enumerate(valset[:num_samples]):
        try:
            # 使用detect模块直接调用，返回DSPy Prediction对象
            prediction = model.detect(code_snippet=example.code_snippet)
            score = bug_detection_metric(example, prediction)
            total_score += score

            # 判断has_bug是否正确
            pred_has_bug = str(prediction.has_bug).lower() in ['true', 'yes', '1']
            if pred_has_bug == example.has_bug:
                correct_predictions += 1

            # 显示进度
            if (i + 1) % 5 == 0:
                print(f"  已验证 {i+1}/{min(num_samples, len(valset))} 个样本...")
        except Exception as e:
            print(f"  警告: 验证样本 {i} 时出错: {e}")

    avg_score = total_score / min(num_samples, len(valset))
    accuracy = correct_predictions / min(num_samples, len(valset))

    print(f"\n验证结果:")
    print(f"  平均分数: {avg_score:.3f}")
    print(f"  Has_bug准确率: {accuracy:.1%}")

    return avg_score


def train_with_bootstrap_fewshot(trainset, valset, num_examples=8, max_bootstraps=4):
    """
    使用BootstrapFewShot优化器训练模型

    Args:
        trainset: 训练集
        valset: 验证集
        num_examples: Few-shot示例数量
        max_bootstraps: Bootstrap最大轮数

    Returns:
        优化后的模型
    """
    print("=" * 80)
    print("开始模型优化训练")
    print("=" * 80)
    print(f"训练集大小: {len(trainset)}")
    print(f"验证集大小: {len(valset)}")
    print(f"Few-shot示例数: {num_examples}")
    print(f"Bootstrap轮数: {max_bootstraps}")
    print()

    # 创建基础模型
    base_model = CodeBugDetector()

    # 在优化前验证基础模型
    print("评估基础模型性能...")
    baseline_score = validate_model(base_model, valset, num_samples=10)

    # 配置BootstrapFewShot优化器
    print("\n" + "=" * 80)
    print("配置BootstrapFewShot优化器")
    print("=" * 80)

    teleprompter = BootstrapFewShot(
        metric=bug_detection_metric,
        max_bootstrapped_demos=num_examples,
        max_labeled_demos=num_examples,
        max_rounds=max_bootstraps
    )

    # 编译优化模型
    print("\n开始优化训练（这可能需要几分钟）...")
    print("注意: 优化过程会调用LLM多次，请耐心等待\n")

    try:
        optimized_model = teleprompter.compile(
            base_model,
            trainset=trainset[:50]  # 使用前50个训练样本
        )

        print("\n✓ 模型优化完成！")

        # 验证优化后的模型
        print("\n评估优化后模型性能...")
        optimized_score = validate_model(optimized_model, valset, num_samples=10)

        # 显示改进
        improvement = (optimized_score - baseline_score) / baseline_score * 100 if baseline_score > 0 else 0
        print("\n" + "=" * 80)
        print("优化结果对比")
        print("=" * 80)
        print(f"基础模型分数: {baseline_score:.3f}")
        print(f"优化模型分数: {optimized_score:.3f}")
        print(f"性能提升: {improvement:+.1f}%")
        print("=" * 80)

        return optimized_model

    except Exception as e:
        print(f"\n✗ 优化训练失败: {e}")
        print("返回基础模型")
        return base_model


def main():
    """主训练流程"""
    print("\n" + "=" * 80)
    print("DSPy Bug检测器优化训练")
    print("=" * 80)
    print()

    # 1. 配置DSPy模型
    if not setup_dspy_model():
        return

    # 2. 加载数据集
    print("=" * 80)
    print("加载训练数据")
    print("=" * 80)

    try:
        trainset, valset = load_bug_detection_dataset(max_samples=100)
        print(f"✓ 成功加载数据集")
        print(f"  训练集: {len(trainset)} 样本")
        print(f"  验证集: {len(valset)} 样本\n")

        if len(trainset) == 0:
            print("✗ 训练集为空，无法进行训练")
            return

    except Exception as e:
        print(f"✗ 加载数据集失败: {e}")
        return

    # 3. 训练优化模型
    optimized_model = train_with_bootstrap_fewshot(
        trainset=trainset,
        valset=valset,
        num_examples=6,  # 使用6个few-shot示例
        max_bootstraps=3  # 3轮bootstrap
    )

    # 4. 保存优化后的模型
    print("\n" + "=" * 80)
    print("保存优化模型")
    print("=" * 80)

    model_dir = os.path.join(project_root, 'models')
    # 使用JSON格式更安全
    model_path = os.path.join(model_dir, 'optimized_bug_detector.json')

    try:
        optimized_model.save_optimized_model(model_path)
        print(f"\n✓ 训练完成！优化模型已保存至: {model_path}")
    except Exception as e:
        print(f"\n✗ 保存模型失败: {e}")

    print("\n" + "=" * 80)
    print("训练流程完成")
    print("=" * 80)
    print("\n你现在可以在 bug_detection_demo.ipynb 中使用优化后的模型了！")
    print(f"使用方法: detector = CodeBugDetector('{model_path}')\n")


if __name__ == "__main__":
    main()
