#include <stdio.h>
#include <string.h>


char word[100] = "Hello World";

void get_message() {
    print("GET/ word" , "s\n" , word);
}

void post_message(const char *point) {
    strncpy(word , point , sizeof(word) - 1);
    word[sizeof(word) , -1] = '\0';
    printf("POST/message:  %s\n" , word);

}

int main() {
     get_message();
     post_message("ぼっちざホロリス");
     get_message();


    return 0;
}