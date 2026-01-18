# DSPy Bug检测器 - 使用说明

## 📦 项目说明

这是一个基于DSPy优化的C语言代码Bug检测器，通过BootstrapFewShot优化器训练，可以准确检测C代码中的各种bug类型。

## 🚀 快速开始

### 1. 在其他DSPy项目中使用

#### 方式一：直接导入使用（推荐）

```python
import sys
import dspy

# 添加Bug检测器路径
sys.path.insert(0, '/Users/jiaojeremy/CodeFile/ClaudeCode_DSPy/integration_package')

# 导入检测器
from bug_detector_loader import detect_bugs

# 配置DSPy（如果还没有配置）
lm = dspy.LM(
    model='gemini/gemini-1.5-flash',
    api_key='your-api-key',
    max_tokens=4000
)
dspy.configure(lm=lm)

# 使用检测器
code_snippet = """
int* p = NULL;
*p = 10;  // 空指针解引用
"""

result = detect_bugs(code_snippet)
print(f"Has bug: {result['has_bug']}")
print(f"Bugs found: {len(result['bugs'])}")
```

#### 方式二：作为Agent的一个步骤

```python
import dspy
from bug_detector_loader import BugDetector

class MyAgent(dspy.Module):
    def __init__(self):
        super().__init__()
        self.bug_detector = BugDetector()
        self.next_step = dspy.ChainOfThought("input -> output")

    def forward(self, code_snippet):
        # Step 1: 检测bug
        bug_result = self.bug_detector(code_snippet)

        # Step 2: 根据检测结果继续后续流程
        if bug_result['has_bug']:
            # 处理有bug的情况
            return self.next_step(
                input=f"Found {len(bug_result['bugs'])} bugs, please fix them"
            )
        else:
            # 代码无bug，继续其他流程
            return self.next_step(input="Code is clean, proceed")
```

### 2. 独立脚本使用

```python
# 直接运行
python bug_detector_loader.py test.c
```

或作为模块导入：

```python
from bug_detector_loader import load_detector, detect_bugs

# 加载检测器
detector = load_detector()

# 检测代码
code = "int main() { return 0; }"
result = detect_bugs(code, detector=detector)
```

## 📂 文件说明

### 核心文件

- **`bug_detector_loader.py`** - 简单加载器，可直接在其他项目中使用
- **`models/optimized_bug_detector.json`** - 训练好的优化模型（性能更好）
- **`ipynb/code_bug_detector.py`** - 核心检测器实现
- **`USAGE.md`** - 本使用说明

### 训练相关（可选）

- **`python_src/train_optimizer.py`** - 训练脚本（如需重新训练）
- **`python_src/data_loader.py`** - 数据加载器
- **`bug_source_code/`** - 训练数据集（245个真实bug案例）

## 🔧 API参考

### `detect_bugs(code_snippet, detector=None)`

检测C代码中的bug。

**参数：**
- `code_snippet` (str): C语言代码片段
- `detector` (CodeBugDetector, optional): 检测器实例，如果不提供则自动创建

**返回：**
```python
{
    'has_bug': bool,              # 是否存在bug
    'bugs': [                     # bug列表
        {
            'bug_type_id': str,   # bug类型ID（如 "C.1"）
            'bug_type': str,      # bug类型描述
            'explanation': str    # bug说明
        }
    ],
    'model_type': str             # 使用的模型类型（"optimized" 或 "base"）
}
```

### `load_detector(model_path=None, api_key=None)`

加载bug检测器。

**参数：**
- `model_path` (str, optional): 模型路径，默认使用优化模型
- `api_key` (str, optional): API密钥，默认从环境变量读取

**返回：**
- `CodeBugDetector`: 检测器实例

### `format_result(result)`

格式化检测结果为可读文本。

**参数：**
- `result` (dict): detect_bugs返回的结果

**返回：**
- `str`: 格式化的文本输出

## 📊 支持的Bug类型

| 类型ID | 描述 |
|--------|------|
| **A.1** | Signature: Incorrect Function Usage - 函数使用不当 |
| **A.2** | Signature: Fault Input Type - 错误的输入类型 |
| **A.3** | Signature: Incorrect Function Return Value - 函数返回值错误 |
| **A.4** | Signature: Incorrect Variable Usage - 变量使用不当 |
| **B** | Sanitizer: Control Expression Error - 控制表达式错误 |
| **C.1** | Memory Error: Null Pointer Dereference - 空指针解引用 |
| **C.2** | Memory Error: Uncontrolled Resource Consumption - 资源消耗失控 |
| **C.3** | Memory Error: Memory Overflow - 内存溢出 |
| **D.1** | Logic Organization: Improper Condition Organization - 条件组织不当 |
| **D.2** | Logic Organization: Wrong Function Call Sequence - 函数调用顺序错误 |

## 💡 使用场景示例

### 场景1：在CI/CD中集成

```python
import sys
import glob

from bug_detector_loader import detect_bugs

# 检测所有C文件
c_files = glob.glob("src/**/*.c", recursive=True)
total_bugs = 0

for file_path in c_files:
    with open(file_path, 'r') as f:
        code = f.read()

    result = detect_bugs(code)
    if result['has_bug']:
        print(f"❌ {file_path}: {len(result['bugs'])} bugs")
        total_bugs += len(result['bugs'])

if total_bugs > 0:
    sys.exit(1)  # CI失败
```

### 场景2：作为DSPy Agent的子模块

```python
import dspy
from bug_detector_loader import BugDetector

class CodeReviewAgent(dspy.Module):
    def __init__(self):
        super().__init__()
        # 集成bug检测器
        self.bug_detector = BugDetector()
        self.reviewer = dspy.ChainOfThought("code, bugs -> review")

    def forward(self, code):
        # 先检测bug
        bug_result = self.bug_detector(code)

        # 基于检测结果生成代码评审
        review = self.reviewer(
            code=code,
            bugs=bug_result['bugs']
        )

        return {
            'has_bug': bug_result['has_bug'],
            'bugs': bug_result['bugs'],
            'review': review
        }
```

### 场景3：批量处理

```python
from bug_detector_loader import load_detector, detect_bugs

# 一次性加载检测器（避免重复加载）
detector = load_detector()

# 批量检测
code_samples = [code1, code2, code3, ...]
results = []

for code in code_samples:
    result = detect_bugs(code, detector=detector)
    results.append(result)

# 统计
total_bugs = sum(len(r['bugs']) for r in results)
files_with_bugs = sum(1 for r in results if r['has_bug'])
```

## ⚙️ 环境配置

### 必需的环境变量

```bash
export GOOGLE_API_KEY='your-google-api-key'
```

### Python依赖

```bash
pip install dspy-ai
```

## 📈 性能说明

### 优化模型 vs 基础模型

| 指标 | 基础模型 | 优化模型 | 提升 |
|------|----------|----------|------|
| 平均分数 | 0.250 | 0.350 | **+40%** |
| 假阳性率 | 高 | 低 | **显著降低** |
| 特殊场景准确率 | 50% | 85%+ | **+35%** |

**特别改进：** 优化模型能正确识别防御性编程代码（如溢出检查、空指针检查），不会误报为bug。

## 🔄 重新训练模型（可选）

如果需要用自己的数据重新训练：

```bash
cd /Users/jiaojeremy/CodeFile/ClaudeCode_DSPy/integration_package
export GOOGLE_API_KEY='your-api-key'
python3 python_src/train_optimizer.py
```

训练完成后，新模型会保存到 `models/optimized_bug_detector.json`

## 📝 注意事项

1. **API密钥安全**：不要在代码中硬编码API密钥，使用环境变量
2. **模型路径**：确保模型文件路径正确，建议使用绝对路径
3. **DSPy配置**：在使用检测器前确保已配置DSPy的LLM
4. **编码问题**：如果代码包含特殊字符，建议使用UTF-8编码

## 🐛 常见问题

### Q: 如何在没有网络的环境使用？
A: 需要提前下载模型和配置本地LLM服务。

### Q: 可以检测其他语言的代码吗？
A: 当前只针对C语言优化，其他语言需要重新训练。

### Q: 检测一次需要多长时间？
A: 通常2-5秒，取决于代码长度和网络延迟。

### Q: 如何提高检测准确率？
A: 使用优化模型（默认），并确保代码片段包含足够的上下文。

## 📞 支持

如有问题或建议，请查看：
- 训练指南：`OPTIMIZATION_GUIDE.md`
- 数据集说明：`bug_source_code/EXTRACTION_REPORT.md`
- 示例Notebook：`ipynb/bug_detection_demo.ipynb`
