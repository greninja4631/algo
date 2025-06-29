# =======================
# Dockerfile (CI/CD統合)
# =======================

FROM gcc:13.2.0

# 1. 作業ディレクトリの作成
WORKDIR /workspace

# 2. プロジェクトの全ファイルをコンテナにコピー
COPY . .

# 3. 必要なツールのインストール
RUN apt-get update && \
    apt-get install -y make git doxygen cppcheck clang-tidy valgrind && \
    rm -rf /var/lib/apt/lists/*

# 4. make test で全テスト自動実行（Makefile必須！）
RUN make clean && make test

# 5. 対話用シェル（失敗してもbashが起動）
CMD ["bash"]
