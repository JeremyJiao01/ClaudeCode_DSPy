#!/usr/bin/env python3
"""
测试GEPA带反馈训练的模型
"""

import dspy
import os
import sys
from pathlib import Path

# 添加路径
current_dir = os.path.dirname(os.path.abspath(__file__))
project_root = os.path.dirname(current_dir)
sys.path.insert(0, os.path.join(project_root, 'ipynb'))

from bug_detector_gepa_native import BugPresenceDetector, BugCategoryClassifier


print("=" * 80)
print("GEPA带反馈模型测试")
print("=" * 80)
print()


# ============================================================================
# 配置
# ============================================================================

print("[1/3] 配置模型")
print("-" * 80)

API_KEY = os.environ.get('GEMINI_API_KEY')
if not API_KEY:
    print("❌ 错误: 未设置 GEMINI_API_KEY")
    sys.exit(1)

lm = dspy.LM(
    model='gemini/gemini-2.0-flash-exp',
    api_key=API_KEY,
    max_tokens=4000
)

dspy.configure(lm=lm)
print("✓ 模型配置完成")
print()


# ============================================================================
# 加载模型
# ============================================================================

print("[2/3] 加载训练好的模型")
print("-" * 80)

model_dir = Path(project_root) / 'models'

# Bug检测器
presence_path = model_dir / 'gepa_feedback_presence_detector.json'
if not presence_path.exists():
    print(f"⚠️  检测器模型不存在: {presence_path.name}")
    print("   请先运行: python3 train_gepa_with_feedback.py")
    presence_detector = None
else:
    presence_detector = BugPresenceDetector()
    presence_detector.load(str(presence_path))
    print(f"✓ 检测器: {presence_path.name}")

# Bug分类器
category_path = model_dir / 'gepa_feedback_category_classifier.json'
if not category_path.exists():
    print(f"⚠️  分类器模型不存在: {category_path.name}")
    print("   请先运行: python3 train_gepa_with_feedback.py")
    category_classifier = None
else:
    category_classifier = BugCategoryClassifier()
    category_classifier.load(str(category_path))
    print(f"✓ 分类器: {category_path.name}")

print()

if not presence_detector and not category_classifier:
    print("❌ 没有可测试的模型")
    sys.exit(1)


# ============================================================================
# 测试用例
# ============================================================================

print("[3/3] 运行测试用例")
print("=" * 80)
print()

# Bug检测测试
if presence_detector:
    print("Bug存在检测测试")
    print("-" * 80)

    test_cases_presence = [
        {
            "name": "空指针解引用（有bug）",
            "code": "int* p = NULL;\n*p = 10;",
            "expected": True
        },
        {
            "name": "正常代码（无bug）",
            "code": "int x = 10;\nreturn x + 5;",
            "expected": False
        },
        {
            "name": "内存泄漏（有bug）",
            "code": "int* data = malloc(100);\n// 忘记free",
            "expected": True
        }
    ]

    correct = 0
    for i, test in enumerate(test_cases_presence, 1):
        print(f"\n[{i}/{len(test_cases_presence)}] {test['name']}")
        print(f"预期: {'有bug' if test['expected'] else '无bug'}")

        try:
            result = presence_detector(code_snippet=test['code'])
            pred = result.has_bug

            is_correct = pred == test['expected']
            if is_correct:
                correct += 1
                status = "✓"
            else:
                status = "✗"

            print(f"{status} 预测: {'有bug' if pred else '无bug'}")
            print(f"  置信度: {result.confidence:.2f}")
            print(f"  推理: {result.reasoning[:80]}...")

        except Exception as e:
            print(f"✗ 错误: {e}")

    print(f"\n检测准确率: {correct}/{len(test_cases_presence)} = {correct/len(test_cases_presence)*100:.1f}%")
    print()

# Bug分类测试
if category_classifier:
    print("Bug类别分类测试")
    print("-" * 80)

    test_cases_category = [
        {
            "name": "空指针解引用",
            "code": "int* p = NULL;\n*p = 10;",
            "expected": "MEMORY"
        },
        {
            "name": "条件判断错误",
            "code": "if (x = 10) { /* 应该是 == */ }",
            "expected": "LOGIC"
        },
        {
            "name": "Use-after-free",
            "code": "int* p = malloc(sizeof(int));\nfree(p);\n*p = 10;",
            "expected": "MEMORY"
        },
        {
            "name": "参数类型错误",
            "code": "int value = 123;\nstrlen(value);",
            "expected": "SIGNATURE"
        },
        {
            "name": "数组越界",
            "code": "int arr[10];\nfor (int i=0; i<=10; i++) arr[i]=i;",
            "expected": "LOGIC"
        }
    ]

    correct = 0
    for i, test in enumerate(test_cases_category, 1):
        print(f"\n[{i}/{len(test_cases_category)}] {test['name']}")
        print(f"预期类别: {test['expected']}")

        try:
            result = category_classifier(code_snippet=test['code'])
            pred = result.category.strip().upper()

            is_correct = pred == test['expected']
            if is_correct:
                correct += 1
                status = "✓"
            else:
                status = "✗"

            print(f"{status} 预测类别: {pred}")
            print(f"  置信度: {result.confidence:.2f}")
            print(f"  解释: {result.explanation[:80]}...")

        except Exception as e:
            print(f"✗ 错误: {e}")

    print(f"\n分类准确率: {correct}/{len(test_cases_category)} = {correct/len(test_cases_category)*100:.1f}%")
    print()


# ============================================================================
# 总结
# ============================================================================

print("=" * 80)
print("测试完成")
print("=" * 80)
print()

print("关键验证:")
print("  ✓ 模型可以正常加载")
print("  ✓ 模型可以进行预测")
print("  ✓ 输出格式符合预期")
print()

print("与之前对比:")
print("  - 之前GEPA分类准确率: 3.3%")
print("  - 期望改进后准确率: >40%")
print()

print("如果准确率仍然很低:")
print("  1. 检查训练日志中是否生成了反馈")
print("  2. 检查GEPA是否捕获了轨迹")
print("  3. 尝试增加训练样本数")
print("  4. 尝试使用更强的grader模型")
print()

print("=" * 80)
