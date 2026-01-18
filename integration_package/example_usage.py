#!/usr/bin/env python3
"""
DSPy Bug检测器使用示例

展示如何在其他DSPy项目中集成和使用bug检测器
"""

import sys
import dspy

# 添加Bug检测器项目路径（integration_package目录）
sys.path.insert(0, '/Users/jiaojeremy/CodeFile/ClaudeCode_DSPy/integration_package')

from bug_detector_loader import BugDetector, detect_bugs, load_detector, format_result


# =============================================================================
# 示例1：最简单的使用方式
# =============================================================================

def example1_simple():
    """最简单的使用方式：直接调用detect_bugs函数"""
    print("=" * 80)
    print("示例1：最简单的使用方式")
    print("=" * 80)

    code = """
int* p = NULL;
*p = 10;  // 空指针解引用
"""

    # 直接检测（会自动创建检测器）
    result = detect_bugs(code)

    print(format_result(result))
    print()


# =============================================================================
# 示例2：重复使用检测器（推荐）
# =============================================================================

def example2_reuse_detector():
    """重复使用检测器实例，避免重复加载"""
    print("=" * 80)
    print("示例2：重复使用检测器（批量检测）")
    print("=" * 80)

    # 一次性加载检测器
    detector = load_detector()

    # 多个代码片段
    code_samples = [
        """
        void process() {
            int* data = malloc(100 * sizeof(int));
            // 忘记释放内存
        }
        """,
        """
        int safe_add(int a, int b) {
            if (a > 0 && b > INT_MAX - a) return -1;
            return a + b;
        }
        """,
        """
        void copy(char* input) {
            char buf[10];
            strcpy(buf, input);  // 缓冲区溢出
        }
        """
    ]

    # 批量检测
    for i, code in enumerate(code_samples, 1):
        print(f"\n检测代码片段 #{i}:")
        result = detect_bugs(code, detector=detector)
        print(f"Has bug: {result['has_bug']}")
        print(f"Bugs: {len(result['bugs'])}")

    print()


# =============================================================================
# 示例3：作为DSPy Module的一部分
# =============================================================================

class CodeReviewAgent(dspy.Module):
    """
    代码评审Agent，集成了bug检测功能

    这是一个典型的使用场景：在DSPy Agent中集成bug检测器
    """

    def __init__(self):
        super().__init__()

        # 集成bug检测器作为子模块
        self.bug_detector = BugDetector()

        # 定义后续处理步骤
        self.reviewer = dspy.ChainOfThought("code, bug_info -> review_comment")

    def forward(self, code_snippet: str):
        """
        执行代码评审

        Args:
            code_snippet: 要评审的代码

        Returns:
            评审结果
        """
        # Step 1: 检测bug
        bug_result = self.bug_detector(code_snippet)

        # Step 2: 根据bug检测结果生成评审意见
        if bug_result['has_bug']:
            bugs = bug_result['bug_details'].get('bugs', [])
            bug_summary = f"Found {len(bugs)} bugs: " + ", ".join(
                [b.get('bug_type_id', 'Unknown') for b in bugs]
            )
        else:
            bug_summary = "No bugs detected"

        # Step 3: 生成评审意见
        review = self.reviewer(
            code=code_snippet[:200],  # 只传递前200字符
            bug_info=bug_summary
        )

        return {
            'has_bug': bug_result['has_bug'],
            'bugs': bug_result['bug_details'].get('bugs', []),
            'review': review
        }


def example3_as_agent_module():
    """将bug检测器集成到DSPy Agent中"""
    print("=" * 80)
    print("示例3：作为DSPy Agent的子模块")
    print("=" * 80)

    # 创建Agent
    agent = CodeReviewAgent()

    # 测试代码
    code = """
    int get_value(int* ptr) {
        return *ptr;  // 没有检查空指针
    }
    """

    # 执行评审
    print("\n执行代码评审...")
    result = agent(code)

    print(f"\nBug检测结果:")
    print(f"  Has bug: {result['has_bug']}")
    print(f"  Bugs found: {len(result['bugs'])}")
    print(f"\n评审意见:")
    print(f"  {result['review']}")
    print()


# =============================================================================
# 示例4：在Skill中使用（您的场景）
# =============================================================================

class MySkill(dspy.Module):
    """
    自定义Skill，某个步骤调用bug检测器

    这是您描述的场景：在skill的某个步骤调用bug检测，
    生成结果后继续后续流程
    """

    def __init__(self):
        super().__init__()

        # Step 1: Bug检测器
        self.bug_detector = BugDetector()

        # Step 2: 后续处理
        self.fix_suggester = dspy.ChainOfThought("bug_info -> fix_suggestion")

        # Step 3: 最终输出
        self.formatter = dspy.ChainOfThought("fix_suggestion -> final_output")

    def forward(self, code_input: str):
        """
        执行完整的skill流程

        流程：
        1. 调用bug检测器生成结果
        2. 基于检测结果生成修复建议
        3. 格式化最终输出
        """
        # Step 1: 调用bug检测
        print("[Step 1] 调用bug检测器...")
        bug_result = self.bug_detector(code_input)

        # Step 2: 基于结果继续后续流程
        if bug_result['has_bug']:
            print("[Step 2] 发现bug，生成修复建议...")
            bugs = bug_result['bug_details'].get('bugs', [])
            bug_info = {
                'count': len(bugs),
                'types': [b.get('bug_type_id') for b in bugs],
                'descriptions': [b.get('explanation') for b in bugs]
            }

            fix_suggestion = self.fix_suggester(bug_info=str(bug_info))
        else:
            print("[Step 2] 无bug，跳过修复建议...")
            fix_suggestion = "No fixes needed, code is clean."

        # Step 3: 格式化最终输出
        print("[Step 3] 格式化最终输出...")
        final_output = self.formatter(fix_suggestion=str(fix_suggestion))

        return {
            'bug_detection': bug_result,
            'fix_suggestion': str(fix_suggestion),
            'final_output': str(final_output)
        }


def example4_in_skill():
    """在Skill中使用bug检测器（您的使用场景）"""
    print("=" * 80)
    print("示例4：在Skill中使用（多步骤流程）")
    print("=" * 80)

    # 创建Skill
    skill = MySkill()

    # 测试代码
    code = """
    void process(char* input) {
        char buffer[10];
        strcpy(buffer, input);
    }
    """

    # 执行Skill
    print("\n执行Skill...")
    result = skill(code)

    print("\n最终结果:")
    print(f"  Bug检测: {result['bug_detection']['has_bug']}")
    print(f"  修复建议: {result['fix_suggestion']}")
    print(f"  最终输出: {result['final_output']}")
    print()


# =============================================================================
# 示例5：读取文件进行检测
# =============================================================================

def example5_detect_file():
    """从文件读取代码进行检测"""
    print("=" * 80)
    print("示例5：检测C代码文件")
    print("=" * 80)

    # 创建测试文件
    test_file = '/tmp/test_code.c'
    with open(test_file, 'w') as f:
        f.write("""
#include <stdio.h>

int main() {
    int* ptr = NULL;
    *ptr = 42;  // Bug: 空指针解引用
    return 0;
}
""")

    print(f"\n检测文件: {test_file}")

    # 读取并检测
    with open(test_file, 'r') as f:
        code = f.read()

    result = detect_bugs(code)
    print(format_result(result))
    print()


# =============================================================================
# 主函数：运行所有示例
# =============================================================================

def main():
    """运行所有示例"""

    # 配置DSPy（必需）
    api_key = 'AIzaSyCNwoqb59pZ49ef2ZCdmQrHw6nNKKQygdU'  # 您的API密钥
    lm = dspy.LM(
        model='gemini/gemini-3-flash-preview',
        api_key=api_key,
        max_tokens=4000
    )
    dspy.configure(lm=lm)

    print("\n")
    print("*" * 80)
    print("DSPy Bug检测器 - 使用示例集合")
    print("*" * 80)
    print("\n")

    # 运行示例（按需取消注释）
    example1_simple()
    # example2_reuse_detector()
    # example3_as_agent_module()
    # example4_in_skill()
    # example5_detect_file()

    print("\n" + "=" * 80)
    print("所有示例运行完成！")
    print("=" * 80)


if __name__ == '__main__':
    main()
