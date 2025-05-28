#include <stdio.h>
#include <string.h>

char message[100] = "Hello, world!";

void get_message() {
    printf("GET /message: %s\n", message);
}

// message：コピーされる側（書き込む先）new_msg：コピーする側（データの元）
//*new_msgで先頭の１文字ずつnew_msgに代入して、messageに置き換えている

void post_message(const char *new_msg) {
    strncpy(message, new_msg, sizeof(message) - 1);
    message[sizeof(message) - 1] = '\0';
    printf("POST /message: %s\n", message);
}

int main() {
    get_message();
    post_message("ぼっち・ざ・ホロリス！");
    get_message();
    return 0;
}