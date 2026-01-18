"""
DSPy Bug Detector - C语言代码Bug检测工具
"""

from setuptools import setup, find_packages

with open("README.md", "r", encoding="utf-8") as fh:
    long_description = fh.read()

setup(
    name="dspy-bug-detector",
    version="1.0.0",
    author="Your Name",
    author_email="your.email@example.com",
    description="基于DSPy的C语言代码bug检测工具",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/yourusername/dspy-bug-detector",
    packages=find_packages(),
    classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Developers",
        "Topic :: Software Development :: Quality Assurance",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
    ],
    python_requires=">=3.8",
    install_requires=[
        "dspy-ai>=2.0.0",
        "fastapi>=0.100.0",
        "uvicorn>=0.23.0",
        "pydantic>=2.0.0",
        "click>=8.0.0",
    ],
    entry_points={
        "console_scripts": [
            "bug-detect=dspy_bug_detector.cli:main",
            "bug-detect-server=dspy_bug_detector.api:start_server",
        ],
    },
    package_data={
        "dspy_bug_detector": [
            "models/*.json",
            "models/*.pkl",
        ],
    },
    include_package_data=True,
)
