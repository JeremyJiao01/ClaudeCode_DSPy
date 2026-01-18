"""
DSPy Bug Detector - C语言代码Bug检测工具

这个包提供了基于DSPy优化的C语言bug检测功能。
"""

__version__ = "1.0.0"
__author__ = "Your Name"

# 导入核心类和函数
import sys
import os

# 添加ipynb路径到sys.path
_package_dir = os.path.dirname(os.path.abspath(__file__))
_ipynb_dir = os.path.join(os.path.dirname(_package_dir), 'ipynb')
if _ipynb_dir not in sys.path:
    sys.path.insert(0, _ipynb_dir)

from code_bug_detector import (
    CodeBugDetector,
    BUG_TYPE_MAPPING,
)

# 提供便捷函数
def create_detector(model_path=None, api_key=None):
    """
    创建bug检测器实例

    Args:
        model_path: 优化模型路径（可选）
        api_key: API密钥（可选，如果环境变量中没有设置）

    Returns:
        CodeBugDetector实例
    """
    import dspy

    # 配置DSPy模型（如果提供了api_key）
    if api_key:
        lm = dspy.LM(
            model='gemini/gemini-1.5-flash',
            api_key=api_key,
            max_tokens=4000
        )
        dspy.configure(lm=lm)

    # 如果没有指定模型路径，使用默认路径
    if model_path is None:
        default_model = os.path.join(_package_dir, '..', 'models', 'optimized_bug_detector.json')
        if os.path.exists(default_model):
            model_path = default_model

    # 创建检测器
    return CodeBugDetector(optimized_model_path=model_path)


def detect_bugs(code_snippet, detector=None, api_key=None):
    """
    便捷函数：检测代码中的bug

    Args:
        code_snippet: C语言代码片段
        detector: CodeBugDetector实例（可选，如果没有提供则创建新实例）
        api_key: API密钥（可选）

    Returns:
        包含bug检测结果的字典
    """
    if detector is None:
        detector = create_detector(api_key=api_key)

    return detector(code_snippet)


# 导出公共API
__all__ = [
    'CodeBugDetector',
    'BUG_TYPE_MAPPING',
    'create_detector',
    'detect_bugs',
    '__version__',
]
