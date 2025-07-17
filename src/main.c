#include <stdio.h>
#include <stdbool.h>
#include <stdio.h>
#include "ds/statistics.h"
#include "data_structures.h" 
#include "util/logger.h" 
#include <assert.h>


#ifndef TESTING        /* ←★ テストビルド時は main() を除去 */

int main(const ds_allocator_t* alloc)
{
    /* デフォルトロガーのまま起動メッセージだけ出力 */
    ds_log(DS_LOG_LEVEL_INFO,
           "Data-Structures Library %s — production run",
           DS_API_VERSION);

    /* TODO: 本番 CLI / サーバー起動コードをここへ */
    return 0;
}

#endif /* TESTING */
