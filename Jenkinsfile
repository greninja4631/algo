#!/usr/bin/env bash
# ==========================================================
# local_build.sh – マルチアーキ build & run ワンライナー
#   • Apple-Silicon → arm64 イメージをビルド＆実行
#   • Intel/WSL      → amd64 イメージをビルド＆実行
#   • --arch=amd64 / arm64 で強制指定も可
# ==========================================================
set -euo pipefail

# ---------- 🔧 CLI オプション ----------
ARCH_OVERRIDE=""
while [[ $# -gt 0 ]]; do
  case "$1" in
    --arch=*)
      ARCH_OVERRIDE="${1#*=}"         # amd64 / arm64
      ;;
    -h|--help)
      echo "Usage: $0 [--arch=amd64|arm64]"
      exit 0
      ;;
    *)
      echo "Unknown option: $1" >&2
      exit 1
      ;;
  esac
  shift
done

# ---------- 🖥️ ホスト CPU からプラットフォーム決定 ----------
HOST_ARCH_RAW=$(uname -m)            # arm64 / aarch64 / x86_64 …
HOST_ARCH=${HOST_ARCH_RAW/aarch64/arm64}

TARGET_ARCH=${ARCH_OVERRIDE:-$HOST_ARCH}
case "$TARGET_ARCH" in
  arm64|aarch64)   PLATFORM="linux/arm64"  ;;
  amd64|x86_64)    PLATFORM="linux/amd64"  ;;
  *)
    echo "❌ Unsupported arch: $TARGET_ARCH" >&2
    exit 2
    ;;
esac

IMAGE_TAG="yourapp:${TARGET_ARCH}"

echo "🛠️  Building $IMAGE_TAG for $PLATFORM …"
docker buildx build \
  --platform "$PLATFORM" \
  -t  "$IMAGE_TAG" \
  -f docker/Dockerfile . \
  --load

echo "🚀 Running $IMAGE_TAG …"
docker run --rm -it --platform "$PLATFORM" "$IMAGE_TAG"