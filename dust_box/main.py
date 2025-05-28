# main.py
# ----------------------------------------------
# モダンAPIサーバー (FastAPI + DI + Prometheus + Clean Arch)
# Firebase/Flutter + CI/CD/クラウド部署 + Web3/情勢分析接続前提
# ----------------------------------------------

from fastapi import FastAPI, Depends, HTTPException
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel, Field
from typing import List, Literal, Union
import logging
import statistics
import enum

# ------------------------
# 観測性の追加 (Prometheus)
# ------------------------
try:
    from prometheus_fastapi_instrumentator import Instrumentator
    ENABLE_METRICS = True
except ImportError:
    ENABLE_METRICS = False

# ------------------------
# ログ設定
# ------------------------
logging.basicConfig(level=logging.INFO, format='[%(levelname)s] %(message)s')
logger = logging.getLogger("statistics")

# ------------------------
# FastAPI アプリケーション
# ------------------------
app = FastAPI(
    title="Statistics API",
    description="CI/CD + Firebase 連携用統計ロジックAPI",
    version="2.0.0"
)

# ------------------------
# グローバルコール + Web3 + FlutterWeb3
# ------------------------
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

if ENABLE_METRICS:
    Instrumentator().instrument(app).expose(app)

# ------------------------
# Enum 型 統計タイプ
# ------------------------
class StatisticType(str, enum.Enum):
    sum = "sum"
    min = "min"
    max = "max"
    average = "average"
    median = "median"
    mode = "mode"

# ------------------------
# Request/Response 型 (Pydantic + OpenAPI拡張)
# ------------------------
class StatisticsRequest(BaseModel):
    values: List[int] = Field(..., description="統計計算する数列", example=[1, 2, 3, 3, 4, 5])

class StatisticsResponse(BaseModel):
    sum: int
    min: int
    max: int
    average: float
    median: float
    mode: int

# ------------------------
# 統計ロジック (DI + Clean Arch)
# ------------------------
class StatisticsCalculator:
    def calculate(self, data: List[int]) -> StatisticsResponse:
        if not data:
            raise ValueError("input data is empty")
        sorted_data = sorted(data)
        freq = {x: data.count(x) for x in data}
        most = max(freq.items(), key=lambda x: x[1])[0]

        return StatisticsResponse(
            sum=sum(data),
            min=min(data),
            max=max(data),
            average=round(sum(data) / len(data), 2),
            median=statistics.median(data),
            mode=most
        )

# ------------------------
# DI コンテナでの渡し組み
# ------------------------
def get_calculator():
    return StatisticsCalculator()

# ------------------------
# エンドポイント (async対応 / DI導入 / 型明示)
# ------------------------
@app.post("/statistics", response_model=StatisticsResponse)
async def compute_statistics(
    payload: StatisticsRequest,
    calculator: StatisticsCalculator = Depends(get_calculator)
):
    try:
        return calculator.calculate(payload.values)
    except Exception as e:
        logger.error(f"[ERROR] /statistics failed: {e}")
        raise HTTPException(status_code=400, detail="統計処理中にエラーが発生しました")

# ------------------------
# Web3ウォレットチェック用API
# ------------------------
@app.get("/wallet-check")
async def wallet_check():
    return {"status": "ok", "message": "Web3ウォレット確認用API"}

# ------------------------
# ヘルスチェック用 (Cloud Run / Firebase Hosting用)
# ------------------------
@app.get("/")
async def health():
    return {"status": "ok", "version": "2.0.0"}

# ------------------------
# 単体テスト (CI用)
# ------------------------
def test_module():
    calc = StatisticsCalculator()
    s = calc.calculate([1, 2, 2, 3, 4])
    assert s.sum == 12
    assert s.min == 1
    assert s.max == 4
    assert s.average == 2.4
    assert s.median == 2.0
    assert s.mode == 2


# ✅ 人間が 覚えて理解すべき 部分（重要知識・汎用力）

# カテゴリ	内容	なぜ覚えるべきか
# 1. HTTPとREST APIの基礎	GETとPOSTの違い、ステータスコードの意味（200/400/500など）	全てのAPI設計・デバッグの土台。全サービス共通。
# 2. Pydantic / BaseModel の意味と使い方	型チェック、データバリデーション、OpenAPIの自動生成に直結	クライアントからの入力を防御し、型安全性を担保する。
# 3. FastAPIの DI（Depends）構造	Depends(get_service) のような依存性注入の構造	OOP的な責務分離、テスト、再利用性の中核。
# 4. 統計関数の意味（平均・中央値・最頻値）	median, mode, average の計算ロジック	統計APIの核であり、CSの基本教養でもある。
# 5. ロギングと例外処理の意味	try-except や logger.error の使い方	実運用時にバグや障害を追跡するための最低限スキル。
# 6. JSONとDictの違い	Pythonの辞書（dict）とJSONの関係（序列/フォーマット）	API設計・通信のフォーマット理解に必要不可欠。
# 7. Enumの意義	Enumを使って返す型を明示し、誤った文字列返却を防ぐ	型安全・コード補完・Swaggerドキュメントの質に直結。


# ⸻

# 🤖 AIに 丸投げしてよい部分（毎回手で書く必要はない）

# カテゴリ	内容	なぜAIに任せてOKか
# 1. BaseModel の定義の boilerplate	class Request(BaseModel): values: List[int] など	Pydantic構文は反復が多く、AIがミスなく補完できる。
# 2. OpenAPI用の Field(description=..., example=...)	Swagger用のコメント記述	説明文の生成はAIの得意領域。あとから編集可能。
# 3. Prometheusの導入コード	Instrumentator().instrument(app).expose(app)	一度構成すれば毎回同じ。定型処理なので暗記不要。
# 4. テスト関数（pytest対応）	def test_statistics(): assert calc.sum([1,2,3]) == 6	テストコードは冗長になりがちなので生成で十分。
# 5. DockerfileやCI用の.yml構成	FROM python:3.11 ... や GitHub Actionsワークフロー	インフラ構成は複雑でもパターン化されておりAIが得意。
# 6. クライアントFlutter側のDio + Freezedコード	@freezed class Statistics with _$Statistics など	型付けAPI呼び出しコードは生成が最速・確実。
# 7. Swagger UI向けの補助説明文の自動生成	自然言語説明や例文	要件から説明を自動で出すのはAIが一番効率的。


# ⸻

# 🧠 総まとめ：覚える vs 任せるフロー

# 判断基準	具体的な考え方
# ✅ CSの概念に関わるもの → 覚える	例: HTTP, 統計処理, DI, バリデーションの設計思想
# ✅ トラブル時に読む必要があるログや例外処理 → 覚える	例: logger.error(), try-except の意図
# 🤖 定型化された構成コード → AIに任せる	例: docker-compose.yml, BaseModelの定義など
# 🤖 Swagger用の例文や説明文 → AIに書かせる	例: description="平均値を返す" など文章系

