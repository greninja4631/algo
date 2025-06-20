# ベースは gcc の最新版（軽量で互換性あり）
FROM gcc:13-bullseye

# 作業ディレクトリをプロジェクトルートに設定
WORKDIR /workspace

# プロジェクトのすべてのファイルをイメージ内にコピー
COPY . .

# 必要なツール類をすべてインストール（make, git, doxygen, cppcheck, clang-tidy）
RUN apt-get update && \
    apt-get install -y make git doxygen cppcheck clang-tidy && \
    rm -rf /var/lib/apt/lists/*

# デフォルトでテストビルド（Makefileが存在し、testターゲットがあることが前提）
RUN make test

# デバッグや手動操作のため、bashを起動
CMD [ "bash" ]
