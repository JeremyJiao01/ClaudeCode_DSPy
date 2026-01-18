#!/usr/bin/env python3
"""
DSPy Bug检测器 - 简单加载器

这个文件提供了最简单的方式来使用训练好的bug检测模型。
可以直接在其他DSPy项目中导入使用。

使用示例：
    from bug_detector_loader import detect_bugs

    result = detect_bugs("int* p = NULL; *p = 10;")
    print(result)
"""

import os
import sys
import dspy
from typing import Dict, List, Optional

# =============================================================================
# 路径配置 - 自动处理
# =============================================================================

# 获取当前文件所在目录
_CURRENT_DIR = os.path.dirname(os.path.abspath(__file__))

# 获取项目根目录（上一级）
_PROJECT_ROOT = os.path.dirname(_CURRENT_DIR)

# 添加ipynb目录到路径
_IPYNB_DIR = os.path.join(_PROJECT_ROOT, 'ipynb')
if _IPYNB_DIR not in sys.path:
    sys.path.insert(0, _IPYNB_DIR)

# 默认模型路径（在integration_package/models/下）
DEFAULT_MODEL_PATH = os.path.join(_CURRENT_DIR, 'models', 'optimized_bug_detector.json')

# =============================================================================
# 核心类和函数
# =============================================================================

class BugDetector(dspy.Module):
    """
    Bug检测器包装类

    这是一个简化的包装类，可以直接在DSPy Module中使用。

    示例：
        class MyAgent(dspy.Module):
            def __init__(self):
                super().__init__()
                self.bug_detector = BugDetector()

            def forward(self, code):
                result = self.bug_detector(code)
                return result
    """

    def __init__(self, model_path: Optional[str] = None):
        """
        初始化Bug检测器

        Args:
            model_path: 模型路径，默认使用优化模型
        """
        super().__init__()

        # 导入CodeBugDetector
        from code_bug_detector import CodeBugDetector

        # 使用默认模型或指定模型
        if model_path is None:
            model_path = DEFAULT_MODEL_PATH if os.path.exists(DEFAULT_MODEL_PATH) else None

        # 创建检测器
        self._detector = CodeBugDetector(optimized_model_path=model_path)
        self.is_optimized = self._detector.is_optimized

    def forward(self, code_snippet: str) -> Dict:
        """
        检测代码中的bug

        Args:
            code_snippet: C语言代码片段

        Returns:
            检测结果字典
        """
        return self._detector(code_snippet)

    def __call__(self, code_snippet: str) -> Dict:
        """允许直接调用实例"""
        return self.forward(code_snippet)


def load_detector(model_path: Optional[str] = None, api_key: Optional[str] = None) -> BugDetector:
    """
    加载bug检测器

    Args:
        model_path: 模型路径，默认使用优化模型
        api_key: API密钥，如果没有配置DSPy则必需

    Returns:
        BugDetector实例

    示例：
        detector = load_detector()
        result = detector("int main() { return 0; }")
    """
    # 如果提供了api_key，配置DSPy
    if api_key:
        lm = dspy.LM(
            model='gemini/gemini-3-flash-preview',
            api_key=api_key,
            max_tokens=4000
        )
        dspy.configure(lm=lm)

    # 创建并返回检测器
    return BugDetector(model_path=model_path)


def detect_bugs(code_snippet: str, detector: Optional[BugDetector] = None) -> Dict:
    """
    检测代码中的bug（便捷函数）

    Args:
        code_snippet: C语言代码片段
        detector: 检测器实例（可选，如果不提供则创建新实例）

    Returns:
        检测结果字典，格式：
        {
            'has_bug': bool,              # 是否存在bug
            'bugs': [                     # bug列表
                {
                    'bug_type_id': str,   # bug类型ID
                    'bug_type': str,      # bug类型描述
                    'explanation': str    # bug说明
                }
            ],
            'model_type': str             # 模型类型
        }

    示例：
        result = detect_bugs("int* p = NULL; *p = 10;")
        if result['has_bug']:
            print(f"Found {len(result['bugs'])} bugs")
    """
    # 如果没有提供检测器，创建新的
    if detector is None:
        detector = load_detector()

    # 检测bug
    raw_result = detector(code_snippet)

    # 格式化返回结果
    return {
        'has_bug': raw_result['has_bug'],
        'bugs': raw_result['bug_details'].get('bugs', []),
        'model_type': 'optimized' if detector.is_optimized else 'base'
    }


def format_result(result: Dict) -> str:
    """
    格式化检测结果为可读文本

    Args:
        result: detect_bugs返回的结果

    Returns:
        格式化的文本输出

    示例：
        result = detect_bugs(code)
        print(format_result(result))
    """
    lines = []
    lines.append("=" * 80)
    lines.append("Bug检测结果")
    lines.append("=" * 80)

    if not result['has_bug']:
        lines.append("\n✓ 未检测到bug，代码看起来正常。\n")
    else:
        lines.append(f"\n✗ 检测到 {len(result['bugs'])} 个bug：\n")

        for i, bug in enumerate(result['bugs'], 1):
            lines.append(f"Bug #{i}:")
            lines.append(f"  类型: {bug.get('bug_type_id', 'Unknown')} - {bug.get('bug_type', 'Unknown')}")
            lines.append(f"  说明: {bug.get('explanation', 'N/A')}")
            lines.append("")

    lines.append(f"模型类型: {result['model_type']}")
    lines.append("=" * 80)

    return "\n".join(lines)


# =============================================================================
# 命令行接口（如果直接运行此文件）
# =============================================================================

def main():
    """命令行主函数"""
    import argparse

    parser = argparse.ArgumentParser(description='DSPy Bug检测器')
    parser.add_argument('file', nargs='?', help='要检测的C代码文件')
    parser.add_argument('--model', '-m', help='模型路径')
    parser.add_argument('--api-key', '-k', help='API密钥', default=os.getenv('GOOGLE_API_KEY'))

    args = parser.parse_args()

    # 检查API密钥
    if not args.api_key:
        print("错误: 未设置API密钥")
        print("请设置环境变量 GOOGLE_API_KEY 或使用 --api-key 选项")
        sys.exit(1)

    # 配置DSPy
    lm = dspy.LM(
        model='gemini/gemini-1.5-flash',
        api_key=args.api_key,
        max_tokens=4000
    )
    dspy.configure(lm=lm)

    # 读取代码
    if args.file:
        if not os.path.exists(args.file):
            print(f"错误: 文件不存在: {args.file}")
            sys.exit(1)

        with open(args.file, 'r', encoding='utf-8') as f:
            code = f.read()

        print(f"检测文件: {args.file}\n")
    else:
        # 从stdin读取
        print("请输入C代码（按Ctrl+D结束）:")
        code = sys.stdin.read()

    # 加载检测器
    detector = load_detector(model_path=args.model)

    # 检测bug
    print("检测中...\n")
    result = detect_bugs(code, detector=detector)

    # 输出结果
    print(format_result(result))


if __name__ == '__main__':
    main()
