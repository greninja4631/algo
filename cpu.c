#include <stdio.h>

// レジスタ構造体
typedef struct {
    int A;      // レジスタA
    int B;      // レジスタB
    int RESULT; // 計算結果
    int PC;     // プログラムカウンタ
} CPU;

// 命令セットInstructionCode;
typedef enum {
    LOAD_A = 1,   // Aに値をロード
    LOAD_B,       // Bに値をロード
    ADD,          // A + B を計算してRESULTに格納
    PRINT,        // 結果を表示
    HALT          // 終了
} InstructionCode;

// 命令構造体
typedef struct {
    InstructionCode code;
    int operand;
} Instruction;

// 命令メモリ（プログラム）
Instruction program[] = {
    { LOAD_A, 10 },      // A = 10
    { LOAD_B, 32 },      // B = 32
    { ADD, 0 },          // A + B
    { PRINT, 0 },        // 結果を表示
    { HALT, 0 }          // 終了
};

// 命令を実行する関数（フェッチ→デコード→実行）
void run(CPU *cpu, Instruction *program) {
    while (1) {
        Instruction inst = program[cpu->PC];  // フェッチ
        cpu->PC++;                            // 次の命令へ（しおりを進める）

        switch (inst.code) {                  // デコード→実行
            case LOAD_A:
                cpu->A = inst.operand;
                break;
            case LOAD_B:
                cpu->B = inst.operand;
                break;
            case ADD:
                cpu->RESULT = cpu->A + cpu->B;
                break;
            case PRINT:
                printf("結果: %d\n", cpu->RESULT);
                break;
            case HALT:
                return;  // 実行終了
        }
    }
}

int main() {
    CPU cpu = {0};  // 全レジスタ初期化
    run(&cpu, program);
    return 0;
}

//a