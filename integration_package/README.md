# DSPy Bug检测器 - 集成包

这个文件夹包含了在其他DSPy项目中使用Bug检测器所需的所有文件。

## 📦 文件说明

| 文件 | 说明 | 优先级 |
|------|------|--------|
| **`bug_detector_loader.py`** | 主加载器文件 | ⭐⭐⭐ 必需 |
| **`QUICKSTART.md`** | 快速开始指南（5分钟上手） | ⭐⭐⭐ 建议先读 |
| **`USAGE.md`** | 详细使用文档 | ⭐⭐ 需要时查阅 |
| **`example_usage.py`** | 完整示例代码（5个示例） | ⭐⭐ 参考学习 |
| **`INTEGRATION_SUMMARY.md`** | 集成方案总结 | ⭐ 总体了解 |
| **`models/`** | 训练好的优化模型 | ⭐⭐⭐ 必需 |

## 🚀 快速开始（3步）

### 1. 在您的项目中添加路径

```python
import sys
sys.path.insert(0, '/Users/jiaojeremy/CodeFile/ClaudeCode_DSPy/integration_package')
```

### 2. 导入并配置DSPy

```python
import dspy
from bug_detector_loader import detect_bugs

lm = dspy.LM(
    model='gemini/gemini-3-flash-preview',
    api_key='your-api-key',
    max_tokens=4000
)
dspy.configure(lm=lm)
```

### 3. 使用检测器

```python
code = "int* p = NULL; *p = 10;"
result = detect_bugs(code)

print(f"Has bug: {result['has_bug']}")
print(f"Bugs found: {len(result['bugs'])}")
```

## 🎯 在Agent/Skill中使用

```python
from bug_detector_loader import BugDetector

class MyAgent(dspy.Module):
    def __init__(self):
        super().__init__()
        self.bug_detector = BugDetector()

    def forward(self, code):
        # 调用bug检测
        result = self.bug_detector(code)

        # 继续后续流程
        if result['has_bug']:
            return self.handle_bugs(result)
        else:
            return self.continue_processing()
```

## 📂 目录结构

```
integration_package/
├── README.md                      ← 本文件
├── bug_detector_loader.py         ← 主加载器
├── QUICKSTART.md                  ← 快速开始
├── USAGE.md                       ← 详细文档
├── example_usage.py               ← 示例代码
├── INTEGRATION_SUMMARY.md         ← 集成总结
└── models/
    └── optimized_bug_detector.json  ← 训练好的模型
```

## 🔑 环境要求

```bash
# Python依赖
pip install dspy-ai

# 环境变量
export GOOGLE_API_KEY='your-api-key'
```

## 📚 推荐阅读顺序

1. 📖 **`QUICKSTART.md`** - 5分钟快速上手
2. 💡 **`example_usage.py`** - 查看完整示例
3. 📝 **`USAGE.md`** - 需要详细API时查阅
4. 📋 **`INTEGRATION_SUMMARY.md`** - 了解全貌

## ⚡ 最小使用示例

```python
import sys
sys.path.insert(0, '/Users/jiaojeremy/CodeFile/ClaudeCode_DSPy/integration_package')

from bug_detector_loader import detect_bugs

result = detect_bugs("int main() { return 0; }")
print(result)
```

## 🎊 开始使用

现在您可以：
1. 阅读 **`QUICKSTART.md`** 快速上手
2. 运行 **`example_usage.py`** 查看效果
3. 在您的项目中导入使用

祝您使用愉快！🚀
