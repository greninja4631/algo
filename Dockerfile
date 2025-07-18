<<<<<<< HEAD
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
=======
>>>>>>> feature
FROM gcc:13.2.0

WORKDIR /workspace

COPY . .

<<<<<<< HEAD
# --- 4. 必要ツール類（静的解析・テスト全て）一括インストール ---
>>>>>>> feature
=======
>>>>>>> feature
RUN apt-get update && \
    apt-get install -y make git doxygen cppcheck clang-tidy clang-format valgrind && \
    rm -rf /var/lib/apt/lists/*

<<<<<<< HEAD
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
=======
# ここで一度必ずクリーンしてからCI実行（.o, .d残り対策）
RUN make clean

RUN bash run_ci.sh

CMD ["bash"]
>>>>>>> feature
