/**
 * @file test_rpn_calculator.c
 * @brief 逆ポーランド記法（RPN）電卓 ADTユニットテスト
 * @version 1.0.0
 * @date 2025-05-28
 * 
 * 【設計意図】
 * - RPN電卓APIの正常系・異常系すべて検証
 * - assertによる失敗即検知（CI/CD想定）
 * - サンプル数式を網羅的にテストし、実装品質を担保
 */

 #include "../include/data_structures.h"
 #include <stdio.h>
 #include <assert.h>
 #include <string.h>

 
 int main(void) {
     printf("=== RPN Calculator ADT Unit Test Start ===\n");
 
     // 1. 電卓インスタンス生成
     ds_rpn_calculator_t* calc = ds_rpn_calculator_create();
     assert(calc != NULL);
 
     // 2. 基本的な足し算
     double result = 0.0;
     assert(ds_rpn_calculator_evaluate(calc, "3 4 +", &result) == DS_SUCCESS);
     assert(result == 7.0);
 
     // 3. 複数演算子・優先度のテスト (3 + 4 * 2 = 11)
     assert(ds_rpn_calculator_evaluate(calc, "3 4 2 * +", &result) == DS_SUCCESS);
     assert(result == 11.0);
 
     // 4. 括弧つき数式 ((3 + 4) * 2 = 14)
     assert(ds_rpn_calculator_evaluate(calc, "3 4 + 2 *", &result) == DS_SUCCESS);
     assert(result == 14.0);
 
     // 5. 引き算・割り算 (10 - 2 / 4 = 9.5)
     assert(ds_rpn_calculator_evaluate(calc, "10 2 4 / -", &result) == DS_SUCCESS);
     assert(result == 9.5);
 
     // 6. 負の数やゼロ割りエラー
     assert(ds_rpn_calculator_evaluate(calc, "5 0 /", &result) == DS_ERROR_INVALID_ARGUMENT);
 
     // 7. 入力異常（空文字列、無効な文字列）
     assert(ds_rpn_calculator_evaluate(calc, "", &result) == DS_ERROR_INVALID_ARGUMENT);
     assert(ds_rpn_calculator_evaluate(calc, "3 4 x", &result) == DS_ERROR_INVALID_ARGUMENT);
 
     // 8. 大きな数式の連続計算
     assert(ds_rpn_calculator_evaluate(calc, "1 2 + 3 4 + *", &result) == DS_SUCCESS); // (1+2)*(3+4)=21
     assert(result == 21.0);
 
     // 9. リセット操作の検証（複数回使えるか）
     assert(ds_rpn_calculator_reset(calc) == DS_SUCCESS);
     assert(ds_rpn_calculator_evaluate(calc, "6 2 /", &result) == DS_SUCCESS);
     assert(result == 3.0);
 
     // 10. NULLポインタ等の異常系
     assert(ds_rpn_calculator_evaluate(NULL, "3 4 +", &result) == DS_ERROR_NULL_POINTER);
     assert(ds_rpn_calculator_evaluate(calc, NULL, &result) == DS_ERROR_NULL_POINTER);
     assert(ds_rpn_calculator_evaluate(calc, "3 4 +", NULL) == DS_ERROR_NULL_POINTER);
 
     // 11. 破棄処理のテスト
     assert(ds_rpn_calculator_destroy(calc) == DS_SUCCESS);
 
     printf("=== RPN Calculator ADT Unit Test Passed ===\n");
     return 0;
 }
 
 /**
  * 【テスト設計補足】
  * - 全APIの正常系・異常系パターンを網羅
  * - 計算結果と返却エラーコードを逐一チェック
  * - main()はテスト専用、実装は分離
  * - assert/コメント徹底、現場レベルのテスト自動化容易性
  */