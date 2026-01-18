# DSPy Bug检测器优化指南

## 概述

本项目实现了基于DSPy的C语言代码bug检测系统，支持使用DSPy的**BootstrapFewShot优化器**来提升检测准确性。

## 为什么需要优化？

未优化的基础模型可能存在以下问题：
- ❌ **假阳性（False Positive）**：将正确的代码误判为有bug
  - 例如：将安全检查代码（溢出检测、空指针检查）误认为是错误处理
- ❌ **检测不准确**：缺乏从真实数据中学习的经验
- ❌ **缺乏领域知识**：没有针对特定bug类型的示例

优化后的模型：
- ✅ **更高准确率**：从真实bug数据中学习
- ✅ **减少假阳性**：能够区分安全检查代码和真正的bug
- ✅ **Few-Shot学习**：使用典型bug示例来指导检测

## 优化原理

### BootstrapFewShot优化器

DSPy的BootstrapFewShot优化器通过以下方式提升性能：

1. **Few-Shot示例选择**：从训练数据中选择最具代表性的bug示例
2. **Bootstrap采样**：多轮迭代，逐步改进示例质量
3. **度量驱动**：使用自定义评估指标来筛选最优示例
4. **提示工程自动化**：自动优化提示词和示例组合

### 评估指标

我们的评估指标 `bug_detection_metric` 包含：
- **has_bug准确性**（权重50%）：是否正确判断代码有无bug
- **bug类型准确性**（权重50%）：如果有bug，类型是否正确
  - 完全匹配（如 `C.1` = `C.1`）：+0.5分
  - 部分匹配（如 `C.1` vs `C.2`，主类别相同）：+0.25分

## 快速开始

### 1. 环境准备

确保已安装依赖：
```bash
pip install dspy-ai
```

设置API密钥：
```bash
export GOOGLE_API_KEY='your-google-api-key'
```

### 2. 训练优化模型

运行训练脚本：
```bash
cd python_src
python train_optimizer.py
```

训练过程：
1. 加载训练数据（从 `bug_source_code/metadata.json`）
2. 评估基础模型性能
3. 使用BootstrapFewShot优化（约3-5分钟）
4. 评估优化后模型性能
5. 保存优化模型到 `models/optimized_bug_detector.pkl`

### 3. 使用优化模型

在notebook中：
```python
from code_bug_detector import CodeBugDetector

# 自动加载优化模型（如果存在）
detector = CodeBugDetector(optimized_model_path='../models/optimized_bug_detector.pkl')

# 检测代码
result = detector(code_snippet)
print(detector.format_output(result))
```

或直接运行 `bug_detection_demo.ipynb`，它会自动检测并使用优化模型。

## 项目结构

```
ClaudeCode_DSPy/
├── ipynb/
│   ├── code_bug_detector.py      # Bug检测器模块（支持优化）
│   └── bug_detection_demo.ipynb  # 演示notebook
├── python_src/
│   ├── data_loader.py            # 数据加载器
│   └── train_optimizer.py        # 训练脚本
├── bug_source_code/
│   ├── metadata.json             # Bug数据集元信息
│   ├── functions/                # Bug函数代码
│   └── source_files/             # 完整源文件
├── models/
│   └── optimized_bug_detector.pkl # 优化后的模型（训练后生成）
└── OPTIMIZATION_GUIDE.md         # 本文档
```

## 训练参数调优

在 `train_optimizer.py` 中可以调整以下参数：

```python
optimized_model = train_with_bootstrap_fewshot(
    trainset=trainset,
    valset=valset,
    num_examples=6,      # Few-shot示例数量（推荐4-8）
    max_bootstraps=3     # Bootstrap轮数（推荐2-4）
)
```

参数说明：
- `num_examples`：每次推理使用的Few-shot示例数量
  - 太少：学习不充分
  - 太多：token消耗大，可能引入噪声
  - 推荐：6个
- `max_bootstraps`：Bootstrap迭代轮数
  - 更多轮数：更好的示例质量
  - 更多轮数：训练时间更长
  - 推荐：3轮

## 数据集说明

### 训练数据来源

数据来自 `bug_source_code/metadata.json`，包含：
- **119个真实bug案例**（来自开源项目）
- **Bug类型分布**：
  - A类（Signature）：函数签名相关错误
  - B类（Sanitizer）：控制表达式错误
  - C类（Memory）：内存相关错误（空指针、溢出等）
  - D类（Logic Organization）：逻辑组织错误

### 样本结构

每个bug案例包含：
- `buggy_function.c`：有bug的代码
- `fixed_function.c`：修复后的代码
- `bug_type_id`：bug类型ID（如 `C.1`）
- `bug_type`：bug类型描述

训练时，我们同时使用：
- Buggy代码作为正样本（has_bug=True）
- Fixed代码作为负样本（has_bug=False）

这样可以让模型学习区分有bug和无bug的代码。

## 性能对比

### 示例：示例5（安全检查代码）

**代码：**
```c
int safe_add(int a, int b, int* result) {
    if (result == NULL) {
        return -1;  // 空指针检查
    }

    if (a > 0 && b > INT_MAX - a) {
        return -1;  // 溢出检查
    }

    *result = a + b;
    return 0;
}
```

**基础模型**：
- ❌ 可能误判为有bug（将安全检查误认为错误处理）

**优化模型**：
- ✅ 正确识别为无bug（理解这是防御性编程）

## 常见问题

### Q: 训练需要多长时间？
A: 使用默认参数（6个示例，3轮bootstrap，50个训练样本），约3-5分钟。

### Q: 训练会调用多少次API？
A: 大约50-100次LLM调用，取决于训练集大小和bootstrap轮数。

### Q: 可以使用其他LLM吗？
A: 可以！修改 `train_optimizer.py` 中的模型配置：
```python
# 使用OpenAI
lm = dspy.OpenAI(model='gpt-4', api_key=os.getenv('OPENAI_API_KEY'))

# 使用Claude
lm = dspy.LM(model='anthropic/claude-3-5-sonnet-20241022',
             api_key=os.getenv('ANTHROPIC_API_KEY'))
```

### Q: 优化模型可以重用吗？
A: 可以！优化模型保存为pickle文件，可以在不同会话中重复使用。

### Q: 如何评估模型性能？
A: 训练脚本会自动在验证集上评估，显示：
- 平均分数（0-1）
- Has_bug准确率
- 优化前后对比

## 进阶优化

### 使用更多训练数据

修改 `train_optimizer.py`：
```python
trainset, valset = load_bug_detection_dataset(max_samples=200)  # 增加到200个样本
```

### 调整评估指标

在 `train_optimizer.py` 中修改 `bug_detection_metric` 函数来调整评估逻辑。

### 使用MIPRO优化器

MIPRO是DSPy的另一个强大优化器：
```python
from dspy.teleprompt import MIPRO

teleprompter = MIPRO(
    metric=bug_detection_metric,
    num_candidates=10,
    init_temperature=1.0
)
```

## 参考资源

- [DSPy官方文档](https://dspy-docs.vercel.app/)
- [BootstrapFewShot说明](https://dspy-docs.vercel.app/docs/building-blocks/optimizers#bootstrapfewshot)
- [DSPy GitHub仓库](https://github.com/stanfordnlp/dspy)

## 联系与反馈

如有问题或建议，请提交Issue或PR。
