"""
DSPy Code Bug Detection Module

这个模块实现了基于DSPy的C语言代码bug检测功能。
"""

import dspy
from typing import List, Dict, Optional
import json


# Bug类型映射字典（基于实际数据集bug_source_code/metadata.json）
BUG_TYPE_MAPPING = {
    # A类: Signature错误（函数签名相关）
    "A.1": "Signature: Incorrect Function Usage - 函数使用不当",
    "A.2": "Signature: Fault Input Type - 错误的输入类型",
    "A.3": "Signature: Incorrect Function Return Value - 函数返回值错误",
    "A.4": "Signature: Incorrect Variable Usage - 变量使用不当",

    # B类: Sanitizer错误
    "B": "Sanitizer: Control Expression Error - 控制表达式错误",

    # C类: Memory错误（内存相关）
    "C.1": "Memory Error: Null Pointer Dereference - 空指针解引用",
    "C.2": "Memory Error: Uncontrolled Resource Consumption - 资源消耗失控",
    "C.3": "Memory Error: Memory Overflow - 内存溢出",

    # D类: Logic Organization错误（逻辑组织）
    "D.1": "Logic Organization: Improper Condition Organization - 条件组织不当",
    "D.2": "Logic Organization: Wrong Function Call Sequence - 函数调用顺序错误"
}


class BugDetectionSignature(dspy.Signature):
    """
    代码bug检测的DSPy Signature

    输入：C语言代码片段
    输出：bug检测结果，包括是否存在bug、bug位置、bug类型等
    """

    code_snippet = dspy.InputField(
        desc="需要检测的C语言代码片段"
    )

    has_bug = dspy.OutputField(
        desc="代码是否存在bug (True/False)"
    )

    bug_details = dspy.OutputField(
        desc="如果存在bug，提供详细信息的JSON字符串，格式：{\"bugs\": [{\"line_number\": int, \"code_line\": str, \"bug_type_id\": str, \"explanation\": str}]}"
    )


class CodeBugDetector(dspy.Module):
    """
    代码bug检测器模块

    使用DSPy的ChainOfThought来分析代码并检测潜在的bug
    """

    def __init__(self):
        super().__init__()
        self.detect = dspy.ChainOfThought(BugDetectionSignature)

    def forward(self, code_snippet: str) -> Dict:
        """
        检测代码中的bug

        Args:
            code_snippet: C语言代码片段

        Returns:
            包含bug检测结果的字典
        """
        # 构建提示信息，包含bug类型映射
        bug_types_info = "\n".join([
            f"{bug_id}: {desc}"
            for bug_id, desc in BUG_TYPE_MAPPING.items()
        ])

        enhanced_prompt = f"""请分析以下C语言代码片段，检测是否存在bug。

可用的Bug类型ID及描述：
{bug_types_info}

代码片段：
{code_snippet}

请仔细分析代码，并：
1. 判断是否存在bug (返回True或False)
2. 如果存在bug，请提供详细信息，包括：
   - 每个bug所在的行号（从1开始计数）
   - 该行的代码内容
   - bug类型ID（从上述列表中选择最合适的）
   - 对该bug的简要解释

请以JSON格式返回bug_details，格式如下：
{{"bugs": [{{"line_number": 行号, "code_line": "代码内容", "bug_type_id": "BT00X", "explanation": "解释"}}]}}

如果没有bug，bug_details应该返回：{{"bugs": []}}
"""

        # 调用DSPy进行推理
        result = self.detect(code_snippet=enhanced_prompt)

        # 解析结果
        has_bug = str(result.has_bug).lower() in ['true', 'yes', '1', 'True']

        try:
            bug_details = json.loads(result.bug_details)
        except:
            # 如果JSON解析失败，尝试从文本中提取信息
            bug_details = {"bugs": [], "raw_response": result.bug_details}

        return {
            "has_bug": has_bug,
            "bug_details": bug_details,
            "raw_response": result
        }

    def format_output(self, detection_result: Dict) -> str:
        """
        格式化输出检测结果

        Args:
            detection_result: 检测结果字典

        Returns:
            格式化的字符串输出
        """
        output = []
        output.append("=" * 80)
        output.append("代码Bug检测结果")
        output.append("=" * 80)

        if not detection_result["has_bug"]:
            output.append("\n✓ 未检测到bug，代码看起来正常。\n")
        else:
            output.append("\n✗ 检测到以下bug：\n")

            bugs = detection_result["bug_details"].get("bugs", [])

            for i, bug in enumerate(bugs, 1):
                output.append(f"Bug #{i}:")
                output.append(f"  位置: 第 {bug.get('line_number', 'N/A')} 行")
                output.append(f"  代码: {bug.get('code_line', 'N/A')}")

                bug_type_id = bug.get('bug_type_id', 'D.1')
                bug_type_desc = BUG_TYPE_MAPPING.get(bug_type_id, "未知Bug类型")
                output.append(f"  类型: {bug_type_desc}")

                output.append(f"  说明: {bug.get('explanation', 'N/A')}")
                output.append("")

        output.append("=" * 80)

        return "\n".join(output)


def detect_bugs_in_code(code_snippet: str, detector: Optional[CodeBugDetector] = None) -> Dict:
    """
    便捷函数：检测代码中的bug

    Args:
        code_snippet: C语言代码片段
        detector: 可选的CodeBugDetector实例，如果未提供则创建新实例

    Returns:
        包含bug检测结果的字典
    """
    if detector is None:
        detector = CodeBugDetector()

    return detector(code_snippet)


if __name__ == "__main__":
    # 示例使用
    print("代码Bug检测器已加载")
    print("请在notebook或其他Python脚本中导入并使用此模块")
