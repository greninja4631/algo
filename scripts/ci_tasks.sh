#!/usr/bin/env bash
# ==========================================================
# ci_tasks.sh – buildx multi-arch ＋ Makefile build/test
#   * --arch=amd64|arm64 で強制指定（省略で自動判定）
#   * practice/ ディレクトリの写経コードもビルド実行
# ==========================================================
set -euo pipefail

# ---------- 0. CLI オプション ----------
ARCH_OVERRIDE=""
while [[ $# -gt 0 ]]; do
  case "$1" in
    --arch=*) ARCH_OVERRIDE="${1#*=}" ;;
    -h|--help)
      echo "Usage: $0 [--arch=amd64|arm64]"; exit 0 ;;
    *) echo "Unknown option: $1" >&2; exit 1 ;;
  esac
  shift
done

# ---------- 1. 環境情報 ----------
echo "[INFO] Working dir → $(pwd)"
for d in src tests practice; do
  if [[ -d $d ]]; then
    echo "[INFO] $d:"; ls -l "$d"
  else
    echo "[WARN] $d directory not found"
  fi
done

# ---------- 2. Makefile build / test ----------
make -k clean   || true            # clean が無くても続ける
make -k format  || true            # format が無くても続ける
make build
make test
make -k memcheck || true           # Valgrind が無ければスキップ

# ---------- 3. practice/ 写経コード ----------
if [[ -d practice ]]; then
  for c in practice/*.c; do
    [[ -e $c ]] || continue
    out=${c%.c}.out
    gcc -Wall -Wextra -Werror -g "$c" -o "$out"
    echo "[RUN] $out"; "./$out" || true
  done
fi

# ---------- 4. buildx multi-arch イメージ ----------
ARCH_RAW=$(uname -m)           # arm64 / x86_64 など
HOST_ARCH=${ARCH_RAW/aarch64/arm64}
TARGET_ARCH=${ARCH_OVERRIDE:-$HOST_ARCH}

case "$TARGET_ARCH" in
  arm64) PLATFORM=linux/arm64 ;;
  amd64|x86_64) PLATFORM=linux/amd64; TARGET_ARCH=amd64 ;;
  *) echo "❌ unknown arch $TARGET_ARCH"; exit 2 ;;
esac

IMAGE=yourapp:$TARGET_ARCH
echo "[INFO] Docker buildx → $IMAGE ($PLATFORM)"

docker buildx build \
  --platform "$PLATFORM" \
  -t "$IMAGE" \
  -f docker/Dockerfile . \
  --load

echo "[INFO] Docker run → $IMAGE"
docker run --rm -t --platform "$PLATFORM" "$IMAGE"

echo "[SUCCESS] All CI tasks passed 🎉"