#include <stdlib.h>
#include <stdio.h>

typedef struct Node {
    int data;
    struct Node *next;
    struct Node *prev;
} Node;

// 新しいノードを作成する関数
Node* create_node(int val) {
    Node *node = malloc(sizeof(Node));
    if (!node) exit(1);
    node->data = val;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

// ノードをリストの先頭に追加する関数      head = push(head, 10);でvalの中身が10で確定しているため、 create_node(val);のvalも関数名は異なるが、ローカルスコープ内でvalの値を継承するため、val == 10;が成立する。
Node* push(Node *head, int val) {    
    Node *node = create_node(val); //この２２行目のcreate_node(val);自体も関数が起動するコードとして扱われる。　mallocでヒープ領域にアドレスを確保する宣言をしたポインターをNode* push関数宣言の後に、ポインター変数として宣言する。
    if (head) {
        node->next = head;
        head->prev = node;
    }
    return node;
}



// スタック pop (先頭から削除)
Node* pop(Node *head) {
    if (!head) return NULL;
    Node *tmp = head;
    head = head->next;
    if (head) head->prev = NULL;
    free(tmp);
    return head;
}

// キュー enqueue (末尾に追加)
Node* enqueue(Node *tail, int val) {
    Node *node = create_node(val);
    if (!tail) return node;
    tail->next = node;
    node->prev = tail;
    return node;
}

// キュー dequeue (先頭から削除)
Node* dequeue(Node *head) {
    return pop(head);
}

// 循環リスト化 (tail->next = head)
void make_circular(Node *head, Node *tail) {
    if (head && tail) {
        tail->next = head;
        head->prev = tail;
    }
}

// リスト表示（循環対応）
void print_list(Node *head) {
    if (!head) { puts("(empty)"); return; }
    Node *cur = head;
    do {
        printf("%d -> ", cur->data);
        cur = cur->next;
    } while (cur && cur != head);
    puts(cur == head ? "(circular back to head)" : "NULL");
}

// 全解放（循環も対応）
void free_all(Node *head) {
    if (!head) return;
    Node *cur = head, *next;
    do {
        next = cur->next;
        free(cur);
        cur = next;
    } while (cur && cur != head);
}

// 配列操作サンプル
void array_operations() {
    int array[5] = {1, 2, 3, 4, 5};
    printf("配列の中身: ");
    for (int i = 0; i < 5; i++) {
        printf("%d ", array[i]);
    }
    puts("");
}

// Undo/Redo操作
Node* undo(Node **undo_stack, Node **redo_stack, Node *head) {
    if (!*undo_stack) return head;
    *redo_stack = push(*redo_stack, head->data);
    head = pop(head);
    *undo_stack = pop(*undo_stack);
    return head;
}

Node* redo(Node **undo_stack, Node **redo_stack, Node *head) {
    if (!*redo_stack) return head;
    head = push(head, (*redo_stack)->data);
    *undo_stack = push(*undo_stack, (*redo_stack)->data);
    *redo_stack = pop(*redo_stack);
    return head;
}

// プロセスキューシミュレーション（ラウンドロビン）
void simulate_round_robin(Node *head, int time_slice) {
    if (!head) return;
    Node *cur = head;
    printf("\nラウンドロビン実行順:\n");
    do {
        printf("プロセス %d を %d秒実行\n", cur->data, time_slice);
        cur = cur->next;
    } while (cur && cur != head);
}

// --- メイン ---
int main() {
    Node *head = NULL, *tail = NULL;
    Node *undo_stack = NULL, *redo_stack = NULL;

    puts("\n配列操作サンプル");
    array_operations();

    puts("\nスタック操作");
    head = push(head, 10);
    undo_stack = push(undo_stack, 10);
    head = push(head, 20);
    undo_stack = push(undo_stack, 20);
    head = push(head, 30);
    undo_stack = push(undo_stack, 30);
    print_list(head);

    puts("\nUndo実行");
    head = undo(&undo_stack, &redo_stack, head);
    print_list(head);

    puts("\nRedo実行");
    head = redo(&undo_stack, &redo_stack, head);
    print_list(head);

    puts("\nキュー操作");
    tail = head;
    while (tail && tail->next) tail = tail->next;
    tail = enqueue(tail, 40);
    tail = enqueue(tail, 50);
    print_list(head);

    head = dequeue(head);
    print_list(head);

    puts("\n循環リスト化");
    make_circular(head, tail);
    print_list(head);

    puts("\nラウンドロビンプロセススケジューション");
    simulate_round_robin(head, 3);

    puts("\nメモリ解放");
    free_all(head);
    free_all(undo_stack);
    free_all(redo_stack);
    return 0;
}

// 🌟 ステージ1：「配列を使ってデータをまとめる」

// ⸻

// 何をしている？
// 	•	配列 [1, 2, 3, 4, 5] を作り、それを順番に出力する。

// 本質的に行っている計算
// 	•	「固定長データ（順番が決まったデータ）」をforループで順番に1個ずつ取り出して、表示するだけ。

// イメージ
// 	•	机に並べた5個のカードを、左から順番に読み上げていく感じ！

// ⸻

// 🌟 ステージ2：「スタックを使ってデータを積み上げる（push）」

// ⸻

// 何をしている？
// 	•	10, 20, 30を1個ずつ積み上げる（push）

// 本質的に行っている計算
// 	•	新しいデータが来たら、既存の先頭にくっつけてリストの順番を上書きしている。
// 	•	つまり、先に来たデータを押し込めて、後から来たデータが表面に見える構造を作っている。

// イメージ
// 	•	本を重ねて積んでいく。
// 	•	最後に置いた本（30）が一番上にいる！

// ⸻

// 🌟 ステージ3：「スタックからデータを取り出す（pop）」

// ⸻

// 何をしている？
// 	•	一番上（最新）の30を取り出して消す。

// 本質的に行っている計算
// 	•	一番先頭のノード（30）を削除して、次のノード（20）に頭を移動させる。

// イメージ
// 	•	本の山から一番上の本（30）だけをパッと抜き取る！

// ⸻

// 🌟 ステージ4：「キューにデータを追加する（enqueue）」

// ⸻

// 何をしている？
// 	•	40と50をリストの末尾にくっつける。

// 本質的に行っている計算
// 	•	リストの最後までたどって、新しいノード（40や50）をその後ろに追加している。

// イメージ
// 	•	行列（レジ待ち）に新しく人（40、50）が並ぶ感じ！

// ⸻

// 🌟 ステージ5：「キューからデータを取り出す（dequeue）」

// ⸻

// 何をしている？
// 	•	一番先頭（10）を取り出して削除する。

// 本質的に行っている計算
// 	•	リストの最初の要素を取り出し、次の人（20）を先頭にする。

// イメージ
// 	•	レジ待ちで一番前の人（10）が会計を終えて列から抜ける！

// ⸻

// 🌟 ステージ6：「リストを循環させる（make_circular）」

// ⸻

// 何をしている？
// 	•	リストの最後（50）の次を、リストの最初（20）に繋げる。

// 本質的に行っている計算
// 	•	tail->next = head として、リストをぐるっと円状につなぐ！

// イメージ
// 	•	リレーのバトンみたいに、最後の人が最初の人にバトンを渡す構造を作る！

// ⸻

// 🌟 ステージ7：「UndoとRedoの操作」

// ⸻

// 何をしている？
// 	•	Undo：
// 	•	直前の操作（pushしたデータ）を取り消してリストから消す。
// 	•	Redo：
// 	•	Undoで消したデータを元に戻してリストに復活させる。

// 本質的に行っている計算
// 	•	Undoは「直前の操作を記憶しておいて、1つ巻き戻す」
// 	•	Redoは「巻き戻した操作を再実行する」

// イメージ
// 	•	ゲームで「1手戻る」「やっぱり戻った手をまた進める」ボタン！

// ⸻

// 🌟 ステージ8：「ラウンドロビンプロセスキューシミュレーション」

// ⸻

// 何をしている？
// 	•	リストをぐるぐる回しながら、各プロセス（20, 40, 50）を順番に3秒ずつ実行する。

// 本質的に行っている計算
// 	•	各ノード（プロセス）に同じだけの「時間切り出し」を与えて、順番に実行しているだけ。
// 	•	完全なラウンドロビン型スケジューリング！

// イメージ
// 	•	遊園地の乗り物で、全員が3分ずつ順番に乗るルール！

// ⸻

// 🎯 本質をまとめると

// ステージ	本質
// 配列出力	決まった順番で固定データを読み出す
// スタック操作	先入れ後出し（LIFO）構造を作る
// キュー操作	先入れ先出し（FIFO）構造を作る
// 双方向・循環リスト	前後移動やループ可能なリストを作る
// Undo/Redo	操作履歴を管理して巻き戻し・やり直しを行う
// プロセススケジューラ	公平に順番でリストを回すシステムを作る



// ⸻

// ✅ この流れを完全に体感できると…
// 	•	リスト、スタック、キューの本質と違いが本当に腹落ちします。
// 	•	SNSアプリ、チャットアプリ、ゲーム設計、OS設計に即応用できます！
// 	•	さらに難しい「ツリー構造」「グラフ構造」もすんなり理解できるようになります。












// 🎯 まず結論

// この「配列・単方向リスト・双方向リスト・循環リスト・スタック・キュー」は、
// ➔ コンピュータサイエンス（CS）・アプリ開発・アルゴリズム設計の「絶対的土台」だからです。

// これらを「感覚レベル」で使いこなせるようになると、
// SNSアプリ開発でも、AI開発でも、ゲーム開発でも、ビジネスでも圧倒的に有利になります。

// ⸻

// ✅ 1. コンピュータサイエンス（CS）の観点から

// 項目	理由
// 計算機内部の動きがわかる	プログラム内部で「どんな順番でデータが動いているか」を完全にイメージできる
// メモリ管理が直感で理解できる	malloc/freeを体得できるので、C以外の言語（Python, Go, Rust）にも応用できる
// 「抽象化」の感覚が身につく	リスト・スタック・キューは「抽象データ型（ADT）」の原点。抽象化能力が鍛えられる

// ✅ ➔ だから**CSの全科目（OS、DB、ネットワーク、AI）**に強くなれる！

// ⸻

// ✅ 2. データ構造の観点から

// 項目	理由
// 実装型で覚えられる	暗記ではなく、手を動かして構造を完全に理解できる
// データ構造の「トレードオフ」を体で覚えられる	配列vsリスト、スタックvsキューの違いを状況に応じて使い分けられる
// 巨大データの処理に強くなる	ビッグデータ時代に必要な「効率的なデータ操作力」が身につく

// ✅ ➔ だから高性能アプリ・AI・大規模DB設計もできるようになる！

// ⸻

// ✅ 3. アルゴリズムの観点から

// 項目	理由
// ソート・探索・スケジューリングの基礎	配列・リスト・スタック・キューは全ての基本アルゴリズムに直結
// 計算量（Big-O）が感覚で理解できる	なぜ配列アクセスがO(1)で、リストアクセスがO(n)なのか肌でわかる
// 問題解決力が爆伸びする	アルゴリズム問題（AtCoder, LeetCode）を自力で解けるようになる

// ✅ ➔ だから競技プログラミング・Google面接対策・AI最適化問題にも強くなる！

// ⸻

// ✅ 4. SNSアプリ開発の観点から

// 項目	理由
// タイムライン設計に必須	ユーザー投稿をリスト・スタック・キューで管理する設計感覚が身につく
// リアルタイムチャット・通知機能設計	FIFO（キュー構造）でチャット・通知のリアルタイム制御が自然にできる
// Undo/Redo実装（投稿編集・取り消し機能）	双方向リストベースで履歴管理ができるようになる
// 高速スクロール・遅延ロード設計	リスト＋メモリ最適化テクニックで超軽量SNSを設計できるようになる

// ✅ ➔ だからInstagram級の高性能SNSアプリを本気で作れるレベルに到達できる！

// ⸻

// 🎯 超最終まとめ

// 分野	この学びの意義
// コンピュータサイエンス（CS）	すべての技術の土台を作る
// データ構造	大規模データ時代を生き抜く力をつける
// アルゴリズム	問題解決力と最適化力を飛躍的に高める
// SNSアプリ開発	本当に売れる・使われるアプリを作る力がつく



