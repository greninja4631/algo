pipeline {
    agent {
        dockerfile {
            filename 'Dockerfile'
            dir '.'    // Dockerfileの場所（カレントディレクトリ）
        }
    }
    stages {
        stage('Build') {
            steps {
                echo 'ビルド開始'
                sh 'make'
            }
        }
        stage('Test') {
            steps {
                echo 'テスト実行'
                sh 'make test'
            }
        }
        stage('Memcheck') {
            steps {
                echo 'Valgrindでメモリ検査'
                sh 'make memcheck'
            }
        }
        stage('Clean') {
            steps {
                echo 'クリーンアップ'
                sh 'make clean'
            }
        }
    }
    post {
        always {
            archiveArtifacts artifacts: '*.log', fingerprint: true
        }
    }
}