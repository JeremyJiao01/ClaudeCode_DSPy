"""
命令行接口 (CLI) 工具
"""

import click
import sys
import os
import json
import dspy

# 导入检测器
from dspy_bug_detector import create_detector, BUG_TYPE_MAPPING


@click.group()
@click.version_option(version='1.0.0')
def main():
    """DSPy Bug Detector - C语言代码Bug检测工具"""
    pass


@main.command()
@click.argument('file_path', type=click.Path(exists=True))
@click.option('--model', '-m', default=None, help='优化模型路径')
@click.option('--api-key', '-k', envvar='GOOGLE_API_KEY', help='Google API Key')
@click.option('--output', '-o', type=click.Choice(['text', 'json']), default='text', help='输出格式')
def detect(file_path, model, api_key, output):
    """
    检测C代码文件中的bug

    示例:
        bug-detect detect test.c
        bug-detect detect test.c --output json
        bug-detect detect test.c --model ./my_model.json
    """
    # 配置DSPy
    if api_key:
        lm = dspy.LM(
            model='gemini/gemini-1.5-flash',
            api_key=api_key,
            max_tokens=4000
        )
        dspy.configure(lm=lm)
    else:
        click.echo("错误: 未设置API密钥", err=True)
        click.echo("请设置环境变量 GOOGLE_API_KEY 或使用 --api-key 选项", err=True)
        sys.exit(1)

    # 读取代码文件
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            code_snippet = f.read()
    except Exception as e:
        click.echo(f"错误: 无法读取文件 {file_path}: {e}", err=True)
        sys.exit(1)

    # 创建检测器
    try:
        detector = create_detector(model_path=model)
    except Exception as e:
        click.echo(f"错误: 无法创建检测器: {e}", err=True)
        sys.exit(1)

    # 检测bug
    click.echo(f"正在检测文件: {file_path}")
    if detector.is_optimized:
        click.echo("使用优化模型")
    else:
        click.echo("使用基础模型")
    click.echo()

    try:
        result = detector(code_snippet)

        # 输出结果
        if output == 'json':
            # JSON格式输出
            json_result = {
                'file': file_path,
                'has_bug': result['has_bug'],
                'bugs': result['bug_details'].get('bugs', [])
            }
            click.echo(json.dumps(json_result, indent=2, ensure_ascii=False))
        else:
            # 文本格式输出
            click.echo(detector.format_output(result))

    except Exception as e:
        click.echo(f"错误: 检测失败: {e}", err=True)
        sys.exit(1)


@main.command()
@click.argument('code', required=False)
@click.option('--model', '-m', default=None, help='优化模型路径')
@click.option('--api-key', '-k', envvar='GOOGLE_API_KEY', help='Google API Key')
def quick(code, model, api_key):
    """
    快速检测代码片段（从stdin或参数）

    示例:
        bug-detect quick "int* p = NULL; *p = 10;"
        echo "int main() { return 0; }" | bug-detect quick
    """
    # 配置DSPy
    if api_key:
        lm = dspy.LM(
            model='gemini/gemini-1.5-flash',
            api_key=api_key,
            max_tokens=4000
        )
        dspy.configure(lm=lm)
    else:
        click.echo("错误: 未设置API密钥", err=True)
        sys.exit(1)

    # 获取代码
    if code:
        code_snippet = code
    elif not sys.stdin.isatty():
        code_snippet = sys.stdin.read()
    else:
        click.echo("错误: 请提供代码片段作为参数或通过stdin输入", err=True)
        click.echo("示例: bug-detect quick 'int main() { return 0; }'", err=True)
        sys.exit(1)

    # 创建检测器并检测
    detector = create_detector(model_path=model)
    result = detector(code_snippet)

    # 输出结果
    click.echo(detector.format_output(result))


@main.command()
@click.argument('directory', type=click.Path(exists=True))
@click.option('--model', '-m', default=None, help='优化模型路径')
@click.option('--api-key', '-k', envvar='GOOGLE_API_KEY', help='Google API Key')
@click.option('--pattern', '-p', default='*.c', help='文件匹配模式 (默认: *.c)')
@click.option('--output', '-o', type=click.Path(), help='输出报告到文件')
def batch(directory, model, api_key, pattern, output):
    """
    批量检测目录中的所有C文件

    示例:
        bug-detect batch ./src
        bug-detect batch ./src --pattern "*.c" --output report.json
    """
    import glob

    # 配置DSPy
    if api_key:
        lm = dspy.LM(
            model='gemini/gemini-1.5-flash',
            api_key=api_key,
            max_tokens=4000
        )
        dspy.configure(lm=lm)
    else:
        click.echo("错误: 未设置API密钥", err=True)
        sys.exit(1)

    # 查找所有匹配的文件
    search_pattern = os.path.join(directory, '**', pattern)
    files = glob.glob(search_pattern, recursive=True)

    if not files:
        click.echo(f"未找到匹配的文件: {search_pattern}")
        return

    click.echo(f"找到 {len(files)} 个文件")
    click.echo()

    # 创建检测器
    detector = create_detector(model_path=model)

    # 批量检测
    results = []
    for i, file_path in enumerate(files, 1):
        click.echo(f"[{i}/{len(files)}] 检测: {file_path}")

        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
                code_snippet = f.read()

            result = detector(code_snippet)
            results.append({
                'file': file_path,
                'has_bug': result['has_bug'],
                'bugs': result['bug_details'].get('bugs', [])
            })

            if result['has_bug']:
                click.echo(f"  ✗ 发现 {len(result['bug_details'].get('bugs', []))} 个bug")
            else:
                click.echo(f"  ✓ 未发现bug")

        except Exception as e:
            click.echo(f"  ✗ 检测失败: {e}", err=True)
            results.append({
                'file': file_path,
                'error': str(e)
            })

        click.echo()

    # 输出统计
    total_bugs = sum(len(r.get('bugs', [])) for r in results)
    files_with_bugs = sum(1 for r in results if r.get('has_bug', False))

    click.echo("=" * 80)
    click.echo("批量检测完成")
    click.echo("=" * 80)
    click.echo(f"总文件数: {len(files)}")
    click.echo(f"有bug的文件: {files_with_bugs}")
    click.echo(f"总bug数: {total_bugs}")

    # 保存报告
    if output:
        with open(output, 'w', encoding='utf-8') as f:
            json.dump(results, f, indent=2, ensure_ascii=False)
        click.echo(f"\n报告已保存到: {output}")


@main.command()
def list_types():
    """列出所有支持的bug类型"""
    click.echo("=" * 80)
    click.echo("支持的Bug类型列表")
    click.echo("=" * 80)
    click.echo()

    for bug_id, description in BUG_TYPE_MAPPING.items():
        click.echo(f"{bug_id:6} - {description}")

    click.echo()
    click.echo(f"总计: {len(BUG_TYPE_MAPPING)} 种bug类型")


if __name__ == '__main__':
    main()
