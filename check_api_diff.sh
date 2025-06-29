#!/bin/bash

echo "=== APIズレ自動チェック ==="

FILES=(
  "history_system"
  "doubly_list"
  "lru_cache"
  "stack"
  "queue"
  "circular_list"
  "url_shortener"
  "round_robin"
  "rpn_calculator"
  "statistics"
  "next_next_practice"
)

for f in "${FILES[@]}"; do
  echo "[$f]"
  # ヘッダから関数宣言のみ抽出（先頭が関数名で始まる行）
  grep "^${f}_" include/ds/$f.h | sed 's/;//' | sort > /tmp/header_$f.txt
  # .cファイルから関数定義のみ抽出（同じく先頭が関数名）
  grep "^${f}_" src/ds/$f.c | sed 's/ {//' | sort > /tmp/impl_$f.txt
  # 差分出力
  diff /tmp/header_$f.txt /tmp/impl_$f.txt && echo "  → OK" || echo "  → 差分あり!"
done

echo "=== 完了 ==="
