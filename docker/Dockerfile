# =========================================================
# 2025-Ready  Multi-Arch Dockerfile  (GLIBC 2.38 / bookworm)
# =========================================================
# ▶︎ arm64 / amd64 を同一ファイルでビルド可能
# ▶︎ scripts/ci_tasks.sh からも直接呼び出しやすい

# BuildKit 機能を使う宣言       ★ これが無いと $TARGETPLATFORM が効かない
# syntax=docker/dockerfile:1.4

################## 🔨  Build Stage  ##################
ARG TARGETPLATFORM     # ★ FROM の前で宣言必須
FROM --platform=$TARGETPLATFORM gcc:12-bookworm AS builder   

WORKDIR /app
COPY . .

RUN rm -rf build/ docs/ coverage/ \
 && chmod +x ./scripts/*.sh ./run_ci.sh || true \
 && ./scripts/lint.sh \
 && ./scripts/build.sh \
 && test -f build/bin/main \
 && chmod +x build/bin/main

################## 🧪  (optional) CI Stage ############
FROM builder AS ci
RUN ./run_ci.sh

################## 🚀  Production Stage  ##############
ARG TARGETPLATFORM
FROM --platform=$TARGETPLATFORM debian:bookworm-slim AS prod   

WORKDIR /app
COPY --from=builder /app/build/bin /app/bin
# RUN strip /app/bin/main   # サイズを削りたい場合はコメント解除
ENTRYPOINT ["/app/bin/main"]