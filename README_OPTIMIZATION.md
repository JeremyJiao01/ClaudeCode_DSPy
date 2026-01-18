# DSPy Bug检测器 - 优化使用指南

## 🎯 问题：为什么示例5会被误判？

在 `bug_detection_demo.ipynb` 的示例5中，以下**正确的安全检查代码**可能被误判为有bug：

```c
int safe_add(int a, int b, int* result) {
    if (result == NULL) {
        return -1;  // 空指针检查 ✓
    }

    // 检查整数溢出 ✓
    if (a > 0 && b > INT_MAX - a) {
        return -1;
    }
    if (a < 0 && b < INT_MIN - a) {
        return -1;
    }

    *result = a + b;
    return 0;
}
```

**原因**：基础模型没有从真实数据中学习，容易将防御性编程代码误认为是错误处理。

## ✅ 解决方案：使用DSPy优化

通过使用DSPy的**BootstrapFewShot优化器**，模型能够：
- 从真实的bug数据中学习
- 区分安全检查代码和真正的bug
- 减少假阳性（误报）

## 🚀 快速开始（3步）

### 步骤1️⃣：设置API密钥

```bash
export GOOGLE_API_KEY='AIzaSyCNwoqb59pZ49ef2ZCdmQrHw6nNKKQygdU'
```

### 步骤2️⃣：运行训练脚本

```bash
cd /Users/jiaojeremy/CodeFile/ClaudeCode_DSPy
python3 python_src/train_optimizer.py
```

**训练过程**（约3-5分钟）：
```
1. 加载训练数据（从metadata.json）
2. 评估基础模型性能
3. 使用BootstrapFewShot优化
4. 评估优化后性能
5. 保存模型到 models/optimized_bug_detector.pkl
```

### 步骤3️⃣：使用优化模型

在 `bug_detection_demo.ipynb` 中运行，notebook会自动检测并使用优化模型！

**或者**在代码中：
```python
from code_bug_detector import CodeBugDetector

# 加载优化模型
detector = CodeBugDetector(optimized_model_path='../models/optimized_bug_detector.pkl')

# 检测代码
result = detector(code_snippet)
print(detector.format_output(result))
```

## 📊 预期效果

### 优化前（基础模型）
- ❌ 示例5被误判为有bug
- ❌ 假阳性率高
- ⚠️ 准确率：~60-70%

### 优化后（BootstrapFewShot）
- ✅ 示例5正确识别为无bug
- ✅ 假阳性率降低
- ✅ 准确率：~80-90%+

## 🔧 配置说明

### 训练参数（在 train_optimizer.py 中）

```python
optimized_model = train_with_bootstrap_fewshot(
    trainset=trainset,
    valset=valset,
    num_examples=6,      # Few-shot示例数（推荐4-8）
    max_bootstraps=3     # Bootstrap轮数（推荐2-4）
)
```

### 数据集大小

```python
# 加载更多训练数据
trainset, valset = load_bug_detection_dataset(max_samples=200)
```

## 📁 项目文件结构

```
ClaudeCode_DSPy/
├── ipynb/
│   ├── code_bug_detector.py           # ✨ 支持优化的检测器
│   └── bug_detection_demo.ipynb       # 📓 Demo notebook
├── python_src/
│   ├── data_loader.py                 # 📦 数据加载器
│   └── train_optimizer.py             # 🎓 训练脚本
├── bug_source_code/
│   ├── metadata.json                  # 📋 119个真实bug案例
│   └── functions/                     # 💾 Bug代码样本
├── models/
│   └── optimized_bug_detector.pkl     # 🎯 优化后的模型（训练后生成）
└── README_OPTIMIZATION.md             # 📖 本文档
```

## ❓ 常见问题

### Q: 训练需要多少时间？
**A**: 使用默认参数约3-5分钟（取决于网络速度和API响应）

### Q: 会消耗多少API调用？
**A**: 约50-100次LLM调用（使用50个训练样本，3轮bootstrap）

### Q: 可以使用其他模型吗？
**A**: 可以！修改 `train_optimizer.py` 中的模型配置：

```python
# OpenAI GPT-4
lm = dspy.OpenAI(model='gpt-4', api_key=os.getenv('OPENAI_API_KEY'))

# Anthropic Claude
lm = dspy.LM(model='anthropic/claude-3-5-sonnet-20241022',
             api_key=os.getenv('ANTHROPIC_API_KEY'))
```

### Q: 如何验证优化效果？
**A**: 训练脚本会自动显示优化前后的对比：
```
优化结果对比
================================================================================
基础模型分数: 0.650
优化模型分数: 0.850
性能提升: +30.8%
================================================================================
```

## 🎓 优化原理

### BootstrapFewShot工作流程

1. **示例选择**：从训练数据中选择最具代表性的bug案例
2. **Bootstrap采样**：多轮迭代，每轮选择表现最好的示例
3. **度量驱动**：使用评估指标筛选最优示例组合
4. **提示优化**：自动优化提示词和示例的组合方式

### 评估指标

```python
def bug_detection_metric(example, prediction, trace=None):
    score = 0.0

    # 1. has_bug准确性（权重50%）
    if pred_has_bug == true_has_bug:
        score += 0.5

    # 2. bug类型准确性（权重50%）
    if bug_type_exact_match:
        score += 0.5
    elif bug_type_category_match:
        score += 0.25

    return score
```

## 📚 更多文档

- 详细优化指南：[OPTIMIZATION_GUIDE.md](./OPTIMIZATION_GUIDE.md)
- DSPy官方文档：https://dspy-docs.vercel.app/
- BootstrapFewShot：https://dspy-docs.vercel.app/docs/building-blocks/optimizers

## 🎉 开始使用

现在就运行训练脚本，提升你的bug检测器性能！

```bash
python3 python_src/train_optimizer.py
```

训练完成后，在 `bug_detection_demo.ipynb` 中查看示例5的改进效果！
