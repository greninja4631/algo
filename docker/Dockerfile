# =========================
# 🏗️ GAFA流・完全成果物保証 Dockerfile
# =========================

FROM arm64v8/gcc:13 AS builder
WORKDIR /app

# コード全体コピー
COPY . .

# 全スクリプトを必ず実行可能化
RUN chmod +x ./scripts/*.sh ./run_ci.sh

# Lint（静的解析）+ Build（Makefile）+ 成果物(バイナリ)検証（ls）
RUN ./scripts/lint.sh && ./scripts/build.sh \
 && ls -l build/bin/ \
 && [ -f build/bin/main ] || (echo "mainバイナリが無い！Makefile/ビルド失敗！パス設定・.dockerignore漏れ等を再確認！" && exit 1)

FROM builder AS ci
RUN ./run_ci.sh

FROM arm64v8/debian:bullseye-slim AS prod
WORKDIR /app
COPY --from=builder /app/build/bin /app/bin
ENTRYPOINT ["/app/bin/main"]