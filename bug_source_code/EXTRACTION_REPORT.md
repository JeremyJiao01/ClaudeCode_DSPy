# Bug 源代码提取报告

提取时间: 2026-01-17 13:23:05

## 统计信息

- 总 Bug 数: 245
- 成功提取: 245
- 提取失败: 0
- 成功率: 100.0%

## 数据集结构

```
bug_source_code/
├── metadata.json              # 所有 bug 的元数据
├── source_files/              # 完整源文件
│   └── {bug_id}/
│       ├── buggy.c            # 修复前的源码
│       └── fixed.c            # 修复后的源码
├── diffs/                     # Unified diff 文件
│   └── {bug_id}.diff
├── functions/                 # 函数级别代码（如果有位置信息）
│   └── {bug_id}/
│       ├── buggy_function.c
│       └── fixed_function.c
└── EXTRACTION_REPORT.md       # 本报告
```

## 按项目统计

| 项目 | Bug 数量 |
|------|----------|
| llvm___llvm-project | 143 |
| danmar___cppcheck | 32 |
| CESNET___libyang | 15 |
| fmtlib___fmt | 14 |
| KhronosGroup___SPIRV-Tools | 12 |
| apache___arrow | 9 |
| CLIUtils___CLI11 | 3 |
| facebook___rocksdb | 3 |
| libevent___libevent | 3 |
| nanomsg___nng | 3 |
| skypjack___entt | 2 |
| uncrustify___uncrustify | 2 |
| DynamoRIO___dynamorio | 1 |
| SOCI___soci | 1 |
| awslabs___aws-c-common | 1 |
| zeromq___libzmq | 1 |

## 按 Bug 类型统计

| Bug 类型 ID | 数量 |
|-------------|------|
| D.1 | 66 |
| B | 64 |
| A.4 | 25 |
| D.2 | 20 |
| A.3 | 19 |
| A.1 | 19 |
| A.2 | 12 |
| C.2 | 9 |
| C.1 | 6 |
| C.3 | 5 |

## 使用说明

### 训练数据格式

`metadata.json` 包含所有 bug 的详细信息，每个条目包括：

- **bug_id**: 唯一标识符
- **bug_type_id**: bug 类型 (A.1-A.4, B, C.1-C.3, D.1-D.2)
- **files**: 源码文件的相对路径
- **location**: bug 在源文件中的位置（行号）
- **repository**: GitHub 仓库

### 离线使用

整个 `bug_source_code/` 目录可以直接打包用于离线训练：

```bash
tar -czf bug_dataset.tar.gz bug_source_code/
```

### 加载数据示例 (Python)

```python
import json

# 读取元数据
with open('bug_source_code/metadata.json', 'r') as f:
    bugs = json.load(f)

# 访问第一个 bug
bug = bugs[0]
print(f"Bug ID: {bug['bug_id']}")
print(f"Type: {bug['bug_type_id']}")

# 读取源代码
buggy_file = f"bug_source_code/{bug['files']['buggy_full']}"
with open(buggy_file, 'r') as f:
    buggy_code = f.read()
```
