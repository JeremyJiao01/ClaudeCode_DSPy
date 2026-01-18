"""
数据加载器模块

从metadata.json和source_files中加载训练数据用于DSPy优化
"""

import json
import os
from typing import List, Dict, Tuple
import dspy


def create_bug_example(code_snippet: str, has_bug: bool, bug_details: str):
    """创建Bug检测训练样本"""
    # 使用dspy.Example创建样本，并标记输入字段
    example = dspy.Example(
        code_snippet=code_snippet,
        has_bug=has_bug,
        bug_details=bug_details
    ).with_inputs('code_snippet')
    return example


class BugDatasetLoader:
    """Bug检测数据集加载器"""

    def __init__(self, metadata_path: str, base_dir: str):
        """
        初始化数据加载器

        Args:
            metadata_path: metadata.json文件路径
            base_dir: bug_source_code目录的基础路径
        """
        self.metadata_path = metadata_path
        self.base_dir = base_dir
        self.metadata = self._load_metadata()

    def _load_metadata(self) -> List[Dict]:
        """加载metadata.json"""
        with open(self.metadata_path, 'r', encoding='utf-8') as f:
            return json.load(f)

    def _read_code_file(self, relative_path: str) -> str:
        """读取代码文件"""
        full_path = os.path.join(self.base_dir, relative_path)
        if not os.path.exists(full_path):
            return ""

        with open(full_path, 'r', encoding='utf-8', errors='ignore') as f:
            return f.read()

    def load_training_data(self, max_samples: int = None) -> Tuple[List, List]:
        """
        加载训练数据和验证数据

        Args:
            max_samples: 最大样本数量，None表示全部加载

        Returns:
            (训练集, 验证集) 元组
        """
        examples = []

        for i, bug_entry in enumerate(self.metadata):
            if max_samples and i >= max_samples:
                break

            try:
                # 读取buggy代码（有bug的版本）
                buggy_function_path = bug_entry['files'].get('buggy_function', '')
                buggy_code = self._read_code_file(buggy_function_path)

                if not buggy_code.strip():
                    continue

                # 创建bug详情
                bug_type_id = bug_entry.get('bug_type_id', 'D.1')
                bug_type = bug_entry.get('bug_type', 'Unknown')

                # 构建bug_details JSON
                bug_details = {
                    "bugs": [{
                        "bug_type_id": bug_type_id,
                        "bug_type": bug_type,
                        "explanation": f"This code contains {bug_type}"
                    }]
                }

                # 添加有bug的样本
                examples.append(create_bug_example(
                    code_snippet=buggy_code,
                    has_bug=True,
                    bug_details=json.dumps(bug_details)
                ))

                # 读取fixed代码（无bug的版本）
                fixed_function_path = bug_entry['files'].get('fixed_function', '')
                fixed_code = self._read_code_file(fixed_function_path)

                if fixed_code.strip():
                    # 添加无bug的样本
                    examples.append(create_bug_example(
                        code_snippet=fixed_code,
                        has_bug=False,
                        bug_details=json.dumps({"bugs": []})
                    ))

            except Exception as e:
                print(f"Warning: Failed to load bug entry {i}: {e}")
                continue

        # 划分训练集和验证集 (80/20)
        split_idx = int(len(examples) * 0.8)
        trainset = examples[:split_idx]
        valset = examples[split_idx:]

        return trainset, valset

    def get_bug_statistics(self) -> Dict:
        """获取数据集统计信息"""
        bug_types = {}
        for entry in self.metadata:
            bug_type = entry.get('bug_type', 'Unknown')
            bug_types[bug_type] = bug_types.get(bug_type, 0) + 1

        return {
            'total_bugs': len(self.metadata),
            'bug_types': bug_types
        }


def load_bug_detection_dataset(base_dir: str = None, max_samples: int = None) -> Tuple[List, List]:
    """
    便捷函数：加载bug检测数据集

    Args:
        base_dir: bug_source_code目录路径，默认为当前目录的bug_source_code
        max_samples: 最大样本数量

    Returns:
        (训练集, 验证集) 元组
    """
    if base_dir is None:
        # 自动检测项目根目录
        current_dir = os.path.dirname(os.path.abspath(__file__))
        project_root = os.path.dirname(current_dir)
        base_dir = os.path.join(project_root, 'bug_source_code')

    metadata_path = os.path.join(base_dir, 'metadata.json')

    loader = BugDatasetLoader(metadata_path, base_dir)
    return loader.load_training_data(max_samples)


if __name__ == "__main__":
    # 测试数据加载
    print("加载Bug检测数据集...")

    trainset, valset = load_bug_detection_dataset(max_samples=10)

    print(f"训练集大小: {len(trainset)}")
    print(f"验证集大小: {len(valset)}")

    if trainset:
        print(f"\n第一个训练样本:")
        print(f"Has bug: {trainset[0].has_bug}")
        print(f"Code snippet (前200字符):")
        print(trainset[0].code_snippet[:200])
