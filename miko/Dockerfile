# ✅ Jenkins LTS + JDK11ベースイメージ
FROM jenkins/jenkins:lts-jdk11

# rootユーザーで必要なツール群インストール
USER root

RUN apt-get update && apt-get install -y \
    gcc \
    make \
    valgrind \
    docker.io \
    git \
    openssh-client \
    bash \
    && apt-get clean

# Jenkinsユーザーへ切り替え
USER jenkins

# 作業ディレクトリ設定（実際はJenkins/docker run時に-mount推奨）
WORKDIR /app

# 注意：Jenkins本番運用時は「COPY」「VOLUME」などをDockerfileに書かず、Jenkins側でワークスペースをマウントする！