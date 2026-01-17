# DSPy 代码Bug检测系统

基于DSPy框架实现的C语言代码bug自动检测系统。

## 功能特性

- 🔍 自动检测C语言代码中的潜在bug
- 🎯 精确定位bug所在行号和代码片段
- 📊 支持20种常见C语言bug类型分类
- 🤖 基于大语言模型的智能分析
- 📝 详细的bug解释和修复建议

## 支持的Bug类型

系统支持检测以下20种常见C语言bug类型：

| Bug类型ID | 描述 |
|-----------|------|
| BT001 | 内存泄漏 (Memory Leak) |
| BT002 | 缓冲区溢出 (Buffer Overflow) |
| BT003 | 空指针解引用 (Null Pointer Dereference) |
| BT004 | 使用未初始化变量 (Use of Uninitialized Variable) |
| BT005 | 数组越界 (Array Index Out of Bounds) |
| BT006 | 悬空指针 (Dangling Pointer) |
| BT007 | 整数溢出 (Integer Overflow) |
| BT008 | 格式化字符串漏洞 (Format String Vulnerability) |
| BT009 | 资源泄漏 (Resource Leak) |
| BT010 | 竞态条件 (Race Condition) |
| BT011 | 除零错误 (Division by Zero) |
| BT012 | 类型转换错误 (Type Casting Error) |
| BT013 | 逻辑错误 (Logic Error) |
| BT014 | off-by-one错误 (Off-by-One Error) |
| BT015 | 内存重复释放 (Double Free) |
| BT016 | 符号错误 (Sign Error) |
| BT017 | 栈溢出 (Stack Overflow) |
| BT018 | 死锁 (Deadlock) |
| BT019 | 未检查返回值 (Unchecked Return Value) |
| BT020 | 其他 (Other) |

## 项目结构

```
ClaudeCode_DSPy/
├── DPSy_tes.ipynb              # DSPy模型初始化
├── EXTRACTION_REPORT.ipynb     # Bug类型定义和统计
├── code_bug_detector.py        # 核心检测模块
├── bug_detection_demo.ipynb    # 完整使用示例
├── requirements.txt            # Python依赖
└── PROJECT_README.md           # 本文档
```

## 安装

1. 克隆仓库：
```bash
git clone <repository-url>
cd ClaudeCode_DSPy
```

2. 安装依赖：
```bash
pip install -r requirements.txt
```

3. 配置API密钥：
```bash
# 设置环境变量
export ANTHROPIC_API_KEY="your-api-key-here"
# 或者使用OpenAI
export OPENAI_API_KEY="your-api-key-here"
```

## 快速开始

### 方式1：使用Jupyter Notebook（推荐）

1. 启动Jupyter Notebook：
```bash
jupyter notebook
```

2. 打开 `bug_detection_demo.ipynb`

3. 按照notebook中的步骤执行代码

### 方式2：使用Python脚本

```python
import dspy
import os
from code_bug_detector import CodeBugDetector

# 配置DSPy模型
lm = dspy.LM(
    model='anthropic/claude-3-5-sonnet-20241022',
    api_key=os.getenv('ANTHROPIC_API_KEY'),
    max_tokens=4000
)
dspy.configure(lm=lm)

# 创建检测器
detector = CodeBugDetector()

# 待检测的C代码
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

# 执行检测
result = detector(code)

# 格式化输出
print(detector.format_output(result))
```

## 输出示例

```
================================================================================
代码Bug检测结果
================================================================================

✗ 检测到以下bug：

Bug #1:
  位置: 第 2 行
  代码: int* data = (int*)malloc(100 * sizeof(int));
  类型: 内存泄漏 (Memory Leak) - 动态分配的内存未被正确释放
  说明: 函数中分配了内存但在所有退出路径上都没有调用free()释放内存

================================================================================
```

## 高级用法

### 自定义Bug类型

你可以在 `code_bug_detector.py` 中修改 `BUG_TYPE_MAPPING` 字典来添加或修改bug类型：

```python
BUG_TYPE_MAPPING = {
    "BT001": "内存泄漏 (Memory Leak) - 动态分配的内存未被正确释放",
    "BT021": "你的自定义Bug类型",
    # ... 更多类型
}
```

### 批量检测

```python
code_samples = [code1, code2, code3]

for i, code in enumerate(code_samples, 1):
    print(f"检测代码片段 #{i}")
    result = detector(code)
    print(detector.format_output(result))
```

### 获取原始检测数据

```python
result = detector(code)

# 访问检测结果
has_bug = result['has_bug']
bug_details = result['bug_details']
bugs = bug_details.get('bugs', [])

for bug in bugs:
    print(f"Line {bug['line_number']}: {bug['bug_type_id']}")
    print(f"Explanation: {bug['explanation']}")
```

## 工作原理

1. **DSPy Signature定义**：`BugDetectionSignature` 定义了输入（代码片段）和输出（bug检测结果）的结构

2. **Chain of Thought推理**：使用 `dspy.ChainOfThought` 让模型进行逐步推理，提高检测准确性

3. **结构化输出**：检测结果以JSON格式返回，包含bug的详细信息：
   - 行号
   - 代码片段
   - Bug类型ID
   - 解释说明

4. **类型映射**：将Bug类型ID映射到人类可读的描述

## 配置选项

### 使用不同的模型

在 `DPSy_tes.ipynb` 中，你可以切换不同的模型：

```python
# OpenAI GPT-4
lm = dspy.OpenAI(model='gpt-4', api_key=os.getenv('OPENAI_API_KEY'))

# Claude 3.5 Sonnet
lm = dspy.LM(
    model='anthropic/claude-3-5-sonnet-20241022',
    api_key=os.getenv('ANTHROPIC_API_KEY')
)

# Claude 3 Opus
lm = dspy.LM(
    model='anthropic/claude-3-opus-20240229',
    api_key=os.getenv('ANTHROPIC_API_KEY')
)
```

### 调整模型参数

```python
lm = dspy.LM(
    model='anthropic/claude-3-5-sonnet-20241022',
    api_key=os.getenv('ANTHROPIC_API_KEY'),
    max_tokens=4000,        # 最大输出token数
    temperature=0.1         # 温度参数（0-1，越低越确定）
)
```

## 注意事项

1. 需要有效的API密钥（Anthropic或OpenAI）
2. API调用会产生费用，请注意使用量
3. 检测结果依赖于语言模型的能力，可能存在误报或漏报
4. 建议将检测结果作为代码审查的辅助工具，而非完全替代人工审查

## 贡献

欢迎提交Issue和Pull Request来改进这个项目。

## 许可证

MIT License

## 联系方式

如有问题或建议，请提交Issue。
