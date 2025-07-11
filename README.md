# test
## API開発／テスト／CI自動化フロー

- 全APIの新規実装・テストは src/, include/, tests/ に規約通り追加
- main関数は tests/test_main.c に1つのみ（各テストは void test_xxx_all(void) として分離）
- `make test` で全API一括テスト
- CI/CDや自動検証は run_ci.sh (またはMakefile)に自動化フローを全記載
- Jenkins/GitHubActions/Travisは run_ci.sh や make test を実行するだけでOK