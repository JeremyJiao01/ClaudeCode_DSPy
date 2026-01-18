"""
REST API服务
使用FastAPI实现的Web API服务
"""

from fastapi import FastAPI, HTTPException, BackgroundTasks
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel, Field
from typing import List, Dict, Optional
import os
import dspy
import uvicorn

from dspy_bug_detector import create_detector, BUG_TYPE_MAPPING


# 创建FastAPI应用
app = FastAPI(
    title="DSPy Bug Detector API",
    description="基于DSPy的C语言代码Bug检测API",
    version="1.0.0"
)

# 添加CORS支持
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# 全局检测器实例（延迟初始化）
_detector = None


def get_detector():
    """获取或创建检测器实例"""
    global _detector
    if _detector is None:
        # 配置DSPy
        api_key = os.getenv('GOOGLE_API_KEY')
        if api_key:
            lm = dspy.LM(
                model='gemini/gemini-1.5-flash',
                api_key=api_key,
                max_tokens=4000
            )
            dspy.configure(lm=lm)

        _detector = create_detector()
    return _detector


# 请求/响应模型
class DetectRequest(BaseModel):
    """Bug检测请求"""
    code: str = Field(..., description="要检测的C语言代码片段")
    model_path: Optional[str] = Field(None, description="可选的模型路径")

    class Config:
        json_schema_extra = {
            "example": {
                "code": "int* p = NULL;\n*p = 10;",
                "model_path": None
            }
        }


class BugInfo(BaseModel):
    """Bug信息"""
    line_number: Optional[int] = Field(None, description="Bug所在行号")
    code_line: Optional[str] = Field(None, description="Bug所在代码行")
    bug_type_id: str = Field(..., description="Bug类型ID")
    explanation: str = Field(..., description="Bug说明")


class DetectResponse(BaseModel):
    """Bug检测响应"""
    has_bug: bool = Field(..., description="是否存在bug")
    bugs: List[BugInfo] = Field(default_factory=list, description="Bug列表")
    model_type: str = Field(..., description="使用的模型类型（optimized/base）")


class BugTypeInfo(BaseModel):
    """Bug类型信息"""
    bug_type_id: str
    description: str


class HealthResponse(BaseModel):
    """健康检查响应"""
    status: str
    version: str
    model_loaded: bool


# API端点
@app.get("/", response_model=Dict[str, str])
async def root():
    """API根路径"""
    return {
        "message": "DSPy Bug Detector API",
        "version": "1.0.0",
        "docs": "/docs"
    }


@app.get("/health", response_model=HealthResponse)
async def health():
    """健康检查"""
    detector = get_detector()
    return HealthResponse(
        status="healthy",
        version="1.0.0",
        model_loaded=detector is not None and detector.is_optimized
    )


@app.post("/detect", response_model=DetectResponse)
async def detect_bugs(request: DetectRequest):
    """
    检测代码中的bug

    Args:
        request: 包含代码和可选模型路径的请求

    Returns:
        检测结果
    """
    try:
        # 获取检测器
        if request.model_path:
            detector = create_detector(model_path=request.model_path)
        else:
            detector = get_detector()

        # 检测bug
        result = detector(request.code)

        # 格式化响应
        bugs = []
        for bug in result['bug_details'].get('bugs', []):
            bugs.append(BugInfo(
                line_number=bug.get('line_number'),
                code_line=bug.get('code_line'),
                bug_type_id=bug.get('bug_type_id', 'Unknown'),
                explanation=bug.get('explanation', '')
            ))

        return DetectResponse(
            has_bug=result['has_bug'],
            bugs=bugs,
            model_type="optimized" if detector.is_optimized else "base"
        )

    except Exception as e:
        raise HTTPException(status_code=500, detail=f"检测失败: {str(e)}")


@app.get("/bug-types", response_model=List[BugTypeInfo])
async def list_bug_types():
    """列出所有支持的bug类型"""
    return [
        BugTypeInfo(bug_type_id=bug_id, description=desc)
        for bug_id, desc in BUG_TYPE_MAPPING.items()
    ]


@app.post("/batch-detect")
async def batch_detect(requests: List[DetectRequest], background_tasks: BackgroundTasks):
    """
    批量检测多个代码片段

    Args:
        requests: 包含多个检测请求的列表

    Returns:
        批量检测结果
    """
    detector = get_detector()
    results = []

    for i, req in enumerate(requests):
        try:
            result = detector(req.code)

            bugs = []
            for bug in result['bug_details'].get('bugs', []):
                bugs.append({
                    'line_number': bug.get('line_number'),
                    'code_line': bug.get('code_line'),
                    'bug_type_id': bug.get('bug_type_id', 'Unknown'),
                    'explanation': bug.get('explanation', '')
                })

            results.append({
                'index': i,
                'has_bug': result['has_bug'],
                'bugs': bugs,
                'success': True
            })

        except Exception as e:
            results.append({
                'index': i,
                'error': str(e),
                'success': False
            })

    return {
        'total': len(requests),
        'results': results
    }


def start_server(host="0.0.0.0", port=8000, reload=False):
    """启动API服务器"""
    print(f"启动DSPy Bug Detector API服务器...")
    print(f"访问 http://{host}:{port}/docs 查看API文档")

    uvicorn.run(
        "dspy_bug_detector.api:app",
        host=host,
        port=port,
        reload=reload
    )


if __name__ == "__main__":
    start_server(reload=True)
