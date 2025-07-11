<<<<<<< HEAD
# =======================
# Dockerfile (CI/CD統合)
# =======================

FROM gcc:13.2.0

# 1. 作業ディレクトリの作成
WORKDIR /workspace

# 2. プロジェクトの全ファイルをコンテナにコピー
COPY . .

# 3. 必要なツールのインストール
=======
# --- 1. ベースはgcc最新版&最軽量 ---
FROM gcc:13.2.0

# --- 2. プロジェクトルートに移動 ---
WORKDIR /workspace

# --- 3. プロジェクト全体コピー（src/, include/, tests/, Makefile, run_ci.sh など全て） ---
COPY . .

# --- 4. 必要ツール類（静的解析・テスト全て）一括インストール ---
>>>>>>> feature
RUN apt-get update && \
    apt-get install -y make git doxygen cppcheck clang-tidy valgrind && \
    rm -rf /var/lib/apt/lists/*

<<<<<<< HEAD
# 4. make test で全テスト自動実行（Makefile必須！）
RUN make clean && make test

# 5. 対話用シェル（失敗してもbashが起動）
CMD ["bash"]
=======
# --- 5. デフォルトでテスト&静的解析&ドキュメント&メモリチェック自動実行（run_ci.shが推奨）---
RUN bash run_ci.sh

# --- 6. 手動デバッグ用bash起動もサポート ---
CMD ["bash"]
>>>>>>> feature
