"""
DSPy Code Bug Detection Module

这个模块实现了基于DSPy的C语言代码bug检测功能。
"""

import dspy
from typing import List, Dict, Optional
import json


# Bug类型映射字典（从EXTRACTION_REPORT.ipynb导入）
BUG_TYPE_MAPPING = {
    "BT001": "内存泄漏 (Memory Leak) - 动态分配的内存未被正确释放",
    "BT002": "缓冲区溢出 (Buffer Overflow) - 写入数据超出缓冲区边界",
    "BT003": "空指针解引用 (Null Pointer Dereference) - 试图访问空指针指向的内存",
    "BT004": "使用未初始化变量 (Use of Uninitialized Variable) - 使用了未赋初值的变量",
    "BT005": "数组越界 (Array Index Out of Bounds) - 数组索引超出有效范围",
    "BT006": "悬空指针 (Dangling Pointer) - 使用指向已释放内存的指针",
    "BT007": "整数溢出 (Integer Overflow) - 整数运算结果超出类型表示范围",
    "BT008": "格式化字符串漏洞 (Format String Vulnerability) - printf等函数的格式字符串可被控制",
    "BT009": "资源泄漏 (Resource Leak) - 文件句柄、套接字等资源未正确关闭",
    "BT010": "竞态条件 (Race Condition) - 多线程环境下的并发访问问题",
    "BT011": "除零错误 (Division by Zero) - 除数为零导致的运行时错误",
    "BT012": "类型转换错误 (Type Casting Error) - 不安全或不正确的类型转换",
    "BT013": "逻辑错误 (Logic Error) - 程序逻辑不符合预期",
    "BT014": "off-by-one错误 (Off-by-One Error) - 循环或数组访问的边界差一错误",
    "BT015": "内存重复释放 (Double Free) - 同一块内存被释放多次",
    "BT016": "符号错误 (Sign Error) - 有符号和无符号整数混用导致的问题",
    "BT017": "栈溢出 (Stack Overflow) - 栈空间使用超出限制",
    "BT018": "死锁 (Deadlock) - 多线程相互等待导致程序挂起",
    "BT019": "未检查返回值 (Unchecked Return Value) - 未检查函数返回值导致错误被忽略",
    "BT020": "其他 (Other) - 其他类型的bug"
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

                bug_type_id = bug.get('bug_type_id', 'BT020')
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
