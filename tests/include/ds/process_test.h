#ifndef DS_PROCESS_TEST_H
#define DS_PROCESS_TEST_H

#include "data_structures.h"

// テスト用：本体定義（本番にはincludeしない！）
struct ds_process {
    int id;
    int burst_time;
};

#endif // DS_PROCESS_TEST_H

// 2. 使う人も用途も全然違う
// 	•	process_test.hはテストの実装者（構造体いじる人）しか読まない
// 	•	test_process.hはテストを実行したい人／テストスイートが全部読む
// 	•	公開範囲も違う！
