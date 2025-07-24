# =========================
# 🏗️ GAFA流・完全成果物保証 Dockerfile【2025年最新版・multiarch対応】
# =========================

# -------- Build Stage（multiarch両対応） --------
FROM gcc:12 AS builder     
WORKDIR /app

# コード全体コピー
COPY . .

# ビルド前に全成果物を完全消去
RUN rm -rf build/ docs/ coverage/

# 全スクリプトを必ず実行可能化
RUN chmod +x ./scripts/*.sh ./run_ci.sh || true

# Lint + Build + mainバイナリ存在チェック
RUN ./scripts/lint.sh && ./scripts/build.sh \
 && echo "------ build/bin/ディレクトリ ------" \
 && ls -lh build/bin || echo "build/bin ディレクトリ無し" \
 && [ -d build/bin ] || (echo "❌ build/bin ディレクトリが無い！" && exit 1) \
 && [ -f build/bin/main ] && chmod +x build/bin/main || (echo "❌ mainバイナリが無い！ビルド失敗！" && exit 1)

# -------- 🧪 CI/CD Stage（必要な場合だけ） --------
FROM builder AS ci
RUN ./run_ci.sh

# -------- Production Stage（multiarch両対応） --------
FROM debian:bullseye-slim AS prod   
WORKDIR /app
COPY --from=builder /app/build/bin /app/bin

# 本番バイナリの唯一のエントリポイント
ENTRYPOINT ["/app/bin/main"]