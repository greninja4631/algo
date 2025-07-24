#!/usr/bin/env bash
set -euo pipefail

# ① arm64
docker buildx build \
  --platform linux/arm64 \
  -t yourapp:arm64 \
  -f Dockerfile . \
  --load

docker run --rm -it --platform linux/arm64 yourapp:arm64

# ② amd64
docker buildx build \
  --platform linux/amd64 \
  -t yourapp:amd64 \
  -f Dockerfile . \
  --load

docker run --rm -it --platform linux/amd64 yourapp:amd64