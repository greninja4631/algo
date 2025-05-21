# ✅ ベース：Jenkins + JDK（CI/CD実行環境として安定）
FROM jenkins/jenkins:lts-jdk11

# ✅ root権限で追加インストール
USER root

# ⛏️ 必要なツール群をインストール（C開発＋Docker CLI）
RUN apt-get update && apt-get install -y \
    gcc \
    make \
    valgrind \
    docker.io \
    bash \
    && apt-get clean

# ✅ Jenkinsユーザーに戻す（セキュリティ考慮）
USER jenkins

# ✅ 作業ディレクトリ（後からJenkinsでマウントされる想定）
WORKDIR /app

# デフォルトコマンドは Jenkinsが制御するため不要

# デフォルトコマンドは Jenkinsが制御するため不要
# 🧱 1行目：ベース環境を決める
# •	FROM：この環境は「どのOS・開発環境」からスタートするかを決める命令
# •	gcc：C言語用コンパイラ（GNU Compiler Collection）が入った公式Docker環境
# •	latest：その最新版（タグ）。gccの最新バージョンが入る

# 🧱 2行目：valgrind をインストール
# •	RUN：このコマンドを実行しといて！というDockerへの命令
# •	apt update：パッケージ一覧を最新に更新
# •	apt install -y valgrind：valgrindを「はい」って自動回答しながらインストール

# 🧱 3行目：作業フォルダを指定
# •	WORKDIR：ここを作業場所（cdする先）に設定してね
# •	/app：コンテナ内にできる仮想フォルダ

# 🧱 4行目：コードをコンテナにコピー
# •	COPY：ローカルからファイルをコピーする命令
# •	.：カレントディレクトリ（今いる場所）
# •	/app：コンテナの中の作業場所

# 🧱 5行目：起動時に bash を開く
# •	CMD：このコンテナが起動したときに、これを実行してね
# •	bash：Linuxのコマンド入力画面（ターミナル）を開く