#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node;

void insertEnd(Node **head, int value){
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return;
    }
    newNode->data = value;
    newNode->next = NULL;

    if (*head == NULL) { // If the list is empty, make the new node the head
        *head = newNode;
    } else {
        Node *temp = *head;
        while (temp->next != NULL) { // Traverse to the end of the list
            temp = temp->next;
        }
        temp->next = newNode; // Link the new node as the last node
    }
}

void insertBeginning(Node **head, int value) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return;
    }
    newNode->data = value;
    newNode->next = *head;
    *head = newNode;
}

int main() {
    Node *head = NULL;
    int value, choice;

    while (1) {
        printf("Where do you want to insert? 1 for beginning, 2 for end, other for exit: ");
        scanf("%d", &choice);

        if (choice == 1 || choice == 2) {
            printf("Enter data: ");
            scanf("%d", &value);
        }

        if (choice == 1) {
            insertBeginning(&head, value);
        } else if (choice == 2) {
            insertEnd(&head, value);
        } else {
            break; // Exit the loop
        }
    }

    // Print the linked list
    Node *temp = head;
    printf("Linked list: ");
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");

    // Free the allocated memory
    temp = head;
    while (temp != NULL) {
        Node *next = temp->next;
        free(temp);
        temp = next;
    }

    return 0;
}
