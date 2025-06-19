#include "data_structures.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* --- ラウンドロビン・スケジューラ構造体 --- */
//remaining_time = 120分 ソープランドのサービス時間
struct ds_round_robin_scheduler {
    int time_quantum;     // 各プロセスに割り当てる時間（タイムスライス）何分ごとに、ソープランドの回転を行うかどうか
    ds_queue_t* queue;    // 実行待ちプロセスを保存するキュー
}; ds_round_robin_scheduler_t;

/* --- メモリ/ログ関数の依存性注入 --- */
static ds_malloc_func_t ds_malloc = malloc;
static ds_free_func_t ds_free = free;
static ds_log_func_t ds_log = NULL;

static void default_log(ds_log_level_t level, const char* fmt, ...) {
    (void)level;
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
}

/* --- 内部：プロセス構造体のディープコピー --- */
static ds_process_t* clone_process(const ds_process_t* src) {
    if (!src) return NULL;
    ds_process_t* copy = (ds_process_t*)ds_malloc(sizeof(ds_process_t));
    if (!copy) return NULL;
    memcpy(copy, src, sizeof(ds_process_t));
    return copy;
}


/* --- 公開API --- */

/**
 * @brief スケジューラ生成
 */

//「新しい皿（プロセス）をレーン（queue）に追加」は「新しく来たプロセスを、順番待ちの列の後ろに並べる」その操作をプログラムでは ds_queue_enqueue() で表現している
ds_round_robin_scheduler_t* ds_round_robin_scheduler_create(int time_quantum) {
    if (time_quantum <= 0) return NULL;
    ds_round_robin_scheduler_t* sched = (ds_round_robin_scheduler_t*)ds_malloc(sizeof(ds_round_robin_scheduler_t));
    if (!sched) {
        if (ds_log) ds_log(DS_LOG_ERROR, "[ds_round_robin_scheduler_create] メモリ確保失敗\n");
        return NULL;
    }
    sched->queue = ds_queue_create();
    if (!sched->queue) {
        ds_free(sched);
        return NULL;
    }
    // queue =「列（順番待ちの箱）」＝データをしまっておく場所
    sched->time_quantum = time_quantum;
    return sched;
}

/**
 * @brief スケジューラ破棄
 */
//dequeue ＝「1番前の配列から取り出す」
ds_error_t ds_round_robin_scheduler_destroy(ds_round_robin_scheduler_t* scheduler) {
    if (!scheduler) return DS_ERR_NULL_POINTER;
    void* proc;
    ds_error_t ret;
    // エラー内容を変数に保存
    while ((ret = ds_queue_dequeue(scheduler->queue, &proc)) == DS_SUCCESS) {
        ds_free(proc);
    }
    if (ret != DS_ERR_EMPTY) {
        // 「空」以外のエラーならログ・エラー通知など（ここでreturnしてもよい）
        // 例: fprintf(stderr, "Queue error: %d\n", ret);
        ds_queue_destroy(scheduler->queue);
        ds_free(scheduler);
        return ret;  // エラーコードを返す
    }
    ds_queue_destroy(scheduler->queue);
    ds_free(scheduler);
    return DS_SUCCESS;
}


/**
 * @brief プロセスをスケジューラへ追加
 *///processは実行タスクと同じ意味である。enqueue＝「キュー（queue）の一番後ろにデータを追加する」
ds_error_t ds_round_robin_scheduler_add_process(ds_round_robin_scheduler_t* scheduler, const ds_process_t* process) {
    if (!scheduler || !process) return DS_ERR_NULL_POINTER;
    ds_process_t* proc_copy = clone_process(process);
    if (!proc_copy) return DS_ERR_ALLOC;
    return ds_queue_enqueue(scheduler->queue, proc_copy);
}

/**
 * @brief 次に実行するプロセスを取得
 */
////dequeue ＝「1番前の配列から取り出す」
        // return DS_ERR_EMPTY;
        // •	パターンA： if文 残り3分、タイムクォンタム10分
        // •	if条件はtrue → exec_time=3
        // •	実行後、remaining_time=0

        // •	パターンB：if else文 残り15分、タイムクォンタム10分
        // •	if条件はfalse → exec_time=10
        // •	実行後、remaining_time=5
        ds_error_t ds_round_robin_scheduler_get_next_process(ds_round_robin_scheduler_t* scheduler, ds_process_t* process_out) {
            if (!scheduler || !process_out) return DS_ERR_NULL_POINTER;
            void* proc_ptr;
            if (ds_queue_dequeue(scheduler->queue, &proc_ptr) != DS_SUCCESS) {
                // return DS_ERR_EMPTY; ← ここでエラー返すのが一般的
                return DS_ERR_EMPTY;
            }
        
            ds_process_t* proc = (ds_process_t*)proc_ptr;
            int exec_time;
            if (proc->remaining_time < scheduler->time_quantum) {
                exec_time = proc->remaining_time;
            } else {
                exec_time = scheduler->time_quantum;
            }
        
            proc->remaining_time -= exec_time;

            //proc->remaining_time = 12; exec_time = 5;
            //// ＝ proc->remaining_time = proc->remaining_time - exec_time;
            // ＝ proc->remaining_time = 12 - 5;
            // ＝ proc->remaining_time = 7;
        
            // 結果をprocess_outにコピー
            memcpy(process_out, proc, sizeof(ds_process_t));
            //*「proc（計算中のプロセス）の内容を、process_out（外部に渡す変数）に丸ごとコピーする」**という意味になります。
        
            // 残り時間に応じて次の処理
            if (proc->remaining_time > 0) {
                ds_queue_enqueue(scheduler->queue, proc);
            } else {
                ds_free(proc);
            }
        
            return DS_SUCCESS;
        }


/**
 * @brief プロセスの実行完了通知
 */

//  ✅ 「このプロセス、完食したで！」
// queue から該当 　process_id のプロセスを探して削除する。
ds_error_t ds_round_robin_scheduler_complete_process(ds_round_robin_scheduler_t* scheduler, int process_id) {
    if (!scheduler) return DS_ERR_NULL_POINTER;
    ds_queue_t* queue = scheduler->queue;
    size_t n = ds_queue_size(queue);
    bool found = false;
    //i++はループ本体が1回終わるごとに加算される
    for (size_t i = 0; i < n; ++i) {
        void* proc_ptr;
        if (ds_queue_dequeue(queue, &proc_ptr) != DS_SUCCESS) break;
        ds_process_t* proc = (ds_process_t*)proc_ptr;
        if (proc->process_id == process_id) {

            //【マクドナルドで例える】スタッフが「注文ボックスから最大n枚だけ順番にチケットを確認」
            //毎回「この注文番号は店長が探している番号と一致するか？」を確認

            //proc->process_id：今カウンターで見てるバーガーセットの伝票番号（＝それぞれの注文番号） 
            //process_id：店員が呼び出しマイクで「◯◯番のお客様」と呼んでいる番号（＝渡したい注文番号）
          
            ds_free(proc);
            found = true;
            continue; //「continue;」は「このバーガーは処理終わったから、次のバーガーに進んで！」という合図はfor文の指定回数文行う
        }
        ds_queue_enqueue(queue, proc); //forループが成功していた場合は、第二引数を第１引数にすべてコピー&ペーストし、!= DS_SUCCESS) break;が発生した場合は、for文でループが成功していた要素の数まで,trueの時と同じようにコピペする
    
    }
    return found ? DS_SUCCESS : DS_ERR_NOT_FOUND;
}

// ✅ 1. 「プロセスが終わった」＝もうCPUで実行する必要がない
// 	•	割り当て時間（remaining_time）が0になったプロセスは、全ての仕事が終わったという状態です。
// 	•	もう順番待ちのキューに残しておく意味がありません。

// ⸻

// ✅ 2. 無駄なメモリ・リソースを消費しないため
// 	•	使い終わったプロセス（終わった皿）は“片付ける”のがOS流儀です。
// 	•	そのままキューに残しておくと、「ゴミ（使い終わった仕事）」がどんどん溜まり、
// システムのメモリが無駄に消費されてしまいます。

// ⸻

// ✅ 3. キューを“正常な順番待ちリスト”に保つため
// 	•	「すでに完了しているプロセス」がキューに残っていると、
// “順番待ち”の意味がなくなる（=実行しても何もすることがないプロセスが順番に回ってしまう）。
// 	•	OSスケジューラは「まだ実行すべき仕事だけを公平に回す」のが使命です。

// ⸻

// ✅ 4. process_idで探して削除する理由
// 	•	「完了したプロセスはプロセスIDで一意に管理されている」から
// 	•	他のプロセス（まだ残ってるやつ）はそのまま残し、終わった特定のプロセスだけ削除できる
// 	•	IDで指定することで、「間違って他のプロセスを消さない」という安全設計になる




/* --- 依存性注入 --- */
void ds_set_log_function(ds_log_func_t func) { ds_log = func ? func : default_log; }
void ds_set_memory_functions(ds_malloc_func_t malloc_func, ds_free_func_t free_func) {
    ds_malloc = malloc_func ? malloc_func : malloc;
    ds_free = free_func ? free_func : free;
}