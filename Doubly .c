#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===============================
// 基本構造体定義
// ===============================

// ノード構造体
typedef struct Node {
    int data;
    struct Node* prev;
    struct Node* next;
} Node;

// 双方向リスト構造体
typedef struct DoublyLinkedList {
    Node* head;
    Node* tail;
} DoublyLinkedList;

// ===============================
// 関数プロトタイプ
// ===============================
DoublyLinkedList* create_list();
void append(DoublyLinkedList* list, int data);
void prepend(DoublyLinkedList* list, int data);
void delete_node(DoublyLinkedList* list, int data);
void print_forward(DoublyLinkedList* list);
void print_backward(DoublyLinkedList* list);
void free_list(DoublyLinkedList* list);

// ===============================
// 実装
// ===============================

DoublyLinkedList* create_list() {
    DoublyLinkedList* list = (DoublyLinkedList*)malloc(sizeof(DoublyLinkedList));
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void append(DoublyLinkedList* list, int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = data;
    new_node->prev = list->tail;
    new_node->next = NULL;

    if (list->tail) {
        list->tail->next = new_node;
    } else {
        list->head = new_node;
    }
    list->tail = new_node;
}

void prepend(DoublyLinkedList* list, int data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    new_node->data = data;
    new_node->next = list->head;
    new_node->prev = NULL;

    if (list->head) {
        list->head->prev = new_node;
    } else {
        list->tail = new_node;
    }
    list->head = new_node;
}

void delete_node(DoublyLinkedList* list, int data) {
    Node* current = list->head;
    while (current) {
        if (current->data == data) {
            if (current->prev) current->prev->next = current->next;
            else list->head = current->next;

            if (current->next) current->next->prev = current->prev;
            else list->tail = current->prev;

            free(current);
            return;
        }
        current = current->next;
    }
}

void print_forward(DoublyLinkedList* list) {
    Node* current = list->head;
    printf("Forward: ");
    while (current) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("\n");
}

void print_backward(DoublyLinkedList* list) {
    Node* current = list->tail;
    printf("Backward: ");
    while (current) {
        printf("%d ", current->data);
        current = current->prev;
    }
    printf("\n");
}

void free_list(DoublyLinkedList* list) {
    Node* current = list->head;
    while (current) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

// ===============================
// メイン関数（動作テスト）
// ===============================

int main() {
    DoublyLinkedList* list = create_list();

    append(list, 10);
    append(list, 20);
    append(list, 30);
    prepend(list, 5);
    print_forward(list);
    print_backward(list);

    delete_node(list, 20);
    print_forward(list);
    print_backward(list);

    free_list(list);
    return 0;
}  