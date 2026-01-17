"""
简单的测试脚本，用于测试代码bug检测器

运行方式：
python test_bug_detector.py
"""

import dspy
import os
from ipynb.code_bug_detector import CodeBugDetector, BUG_TYPE_MAPPING


def setup_dspy_model():
    """配置DSPy模型"""
    print("正在配置DSPy模型...")

    # 检查API密钥
    api_key = os.getenv('ANTHROPIC_API_KEY') or os.getenv('OPENAI_API_KEY')
    if not api_key:
        print("错误：未找到API密钥")
        print("请设置环境变量：")
        print("  export ANTHROPIC_API_KEY='your-key'")
        print("  或")
        print("  export OPENAI_API_KEY='your-key'")
        return False

    try:
        if os.getenv('ANTHROPIC_API_KEY'):
            lm = dspy.LM(
                model='anthropic/claude-3-5-sonnet-20241022',
                api_key=os.getenv('ANTHROPIC_API_KEY'),
                max_tokens=4000
            )
            print("✓ 使用Claude 3.5 Sonnet模型")
        else:
            lm = dspy.OpenAI(
                model='gpt-4',
                api_key=os.getenv('OPENAI_API_KEY')
            )
            print("✓ 使用GPT-4模型")

        dspy.configure(lm=lm)
        print("✓ DSPy模型配置完成\n")
        return True
    except Exception as e:
        print(f"✗ 配置模型时出错: {e}")
        return False


def test_memory_leak():
    """测试案例1：内存泄漏"""
    code = """
void process_data() {
    int* data = (int*)malloc(100 * sizeof(int));
    if (data == NULL) {
        return;
    }

    for (int i = 0; i < 100; i++) {
        data[i] = i * 2;
    }

    // 忘记释放内存！
}
"""
    return code


def test_buffer_overflow():
    """测试案例2：缓冲区溢出"""
    code = """
void copy_string(char* input) {
    char buffer[10];
    strcpy(buffer, input);  // 没有检查input的长度！
    printf("Copied: %s\\n", buffer);
}
"""
    return code


def test_null_pointer():
    """测试案例3：空指针解引用"""
    code = """
int get_value(int* ptr) {
    return *ptr;  // 没有检查ptr是否为NULL！
}

int main() {
    int* p = NULL;
    int value = get_value(p);
    printf("Value: %d\\n", value);
    return 0;
}
"""
    return code


def test_array_bounds():
    """测试案例4：数组越界"""
    code = """
void fill_array() {
    int arr[10];
    for (int i = 0; i <= 10; i++) {  // off-by-one错误！
        arr[i] = i;
    }
}
"""
    return code


def test_correct_code():
    """测试案例5：正确的代码"""
    code = """
int safe_add(int a, int b, int* result) {
    if (result == NULL) {
        return -1;
    }

    if (a > 0 && b > INT_MAX - a) {
        return -1;  // 防止溢出
    }
    if (a < 0 && b < INT_MIN - a) {
        return -1;  // 防止下溢
    }

    *result = a + b;
    return 0;
}
"""
    return code


def run_tests():
    """运行所有测试"""
    if not setup_dspy_model():
        return

    detector = CodeBugDetector()
    print("✓ Bug检测器已初始化\n")

    test_cases = [
        ("内存泄漏", test_memory_leak()),
        ("缓冲区溢出", test_buffer_overflow()),
        ("空指针解引用", test_null_pointer()),
        ("数组越界", test_array_bounds()),
        ("正确代码", test_correct_code())
    ]

    print("=" * 80)
    print("开始测试代码Bug检测器")
    print("=" * 80)
    print()

    for i, (test_name, code) in enumerate(test_cases, 1):
        print(f"\n{'='*80}")
        print(f"测试 {i}/{len(test_cases)}: {test_name}")
        print(f"{'='*80}")
        print("\n代码片段：")
        print(code)
        print("\n检测中...")

        try:
            result = detector(code)
            print(detector.format_output(result))
        except Exception as e:
            print(f"✗ 检测出错: {e}")

        if i < len(test_cases):
            input("\n按Enter继续下一个测试...")

    print("\n" + "=" * 80)
    print("所有测试完成！")
    print("=" * 80)


def interactive_mode():
    """交互模式：让用户输入代码进行检测"""
    if not setup_dspy_model():
        return

    detector = CodeBugDetector()
    print("✓ Bug检测器已初始化\n")

    print("=" * 80)
    print("交互式代码Bug检测")
    print("=" * 80)
    print("请输入C语言代码片段，输入完成后按Ctrl+D (Unix/Mac) 或 Ctrl+Z (Windows)：")
    print()

    while True:
        try:
            print("请输入代码（多行输入，结束请按Ctrl+D或Ctrl+Z）：")
            lines = []
            while True:
                try:
                    line = input()
                    lines.append(line)
                except EOFError:
                    break

            code = "\n".join(lines)

            if not code.strip():
                print("未输入代码，退出。")
                break

            print("\n检测中...")
            result = detector(code)
            print(detector.format_output(result))

            again = input("\n是否继续检测其他代码？(y/n): ")
            if again.lower() != 'y':
                break

        except KeyboardInterrupt:
            print("\n\n检测已中断。")
            break


def show_bug_types():
    """显示所有支持的bug类型"""
    print("=" * 80)
    print("支持的Bug类型列表")
    print("=" * 80)
    for bug_id, description in BUG_TYPE_MAPPING.items():
        print(f"{bug_id}: {description}")
    print("=" * 80)


def main():
    """主函数"""
    print("\n欢迎使用DSPy代码Bug检测器！\n")
    print("请选择运行模式：")
    print("1. 运行预设测试案例")
    print("2. 交互式检测（输入自定义代码）")
    print("3. 显示支持的Bug类型")
    print("4. 退出")

    while True:
        try:
            choice = input("\n请选择 (1-4): ").strip()

            if choice == '1':
                run_tests()
                break
            elif choice == '2':
                interactive_mode()
                break
            elif choice == '3':
                show_bug_types()
            elif choice == '4':
                print("再见！")
                break
            else:
                print("无效选择，请输入1-4")

        except KeyboardInterrupt:
            print("\n\n程序已退出。")
            break
        except Exception as e:
            print(f"发生错误: {e}")


if __name__ == "__main__":
    main()
