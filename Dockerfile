# ベースはgcc最新版（軽量・高互換）
FROM gcc:13-bullseye

# 作業ディレクトリをプロジェクトルートに
WORKDIR /workspace

# すべてのファイルをコピー
COPY . .

# 必要に応じてdoxygenやcppcheck, clang-tidyも入れる（品質担保）
RUN apt-get update && apt-get install -y doxygen cppcheck clang-tidy && rm -rf /var/lib/apt/lists/*

# デフォルトはテストビルド
RUN make test

# コンテナ起動時はシェル起動（実験・デバッグ用）
CMD [ "bash" ]