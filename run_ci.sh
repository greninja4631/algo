#!/bin/bash echo "[INFO] Clean & Build" make clean make build

echo "[INFO] Run valgrind memcheck"
make memcheck || { echo "[ERROR] valgrind 失敗"; exit 1; }

echo "[INFO] CI完了 ✅（メモリリークなし)"

