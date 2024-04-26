#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    int data;
    struct Node *next;
}Node;


int mydata;
int main(){
    struct Node *temp, *newNode, *head;
    int choice = 1;

    while(choice == 1){
        newNode = (struct Node*)malloc(sizeof(Node));
        head == NULL;
        printf("Enter data: ");
        scanf("%d",&newNode->data);

        newNode->next = NULL;
        if(head == NULL){
            head = temp = newNode;
        }else{
            temp->next = newNode;
            temp = newNode;
        }

        printf("Continue? : ");
        scanf("%d",&choice);
    }
    temp = head;
    while(temp != NULL){
        printf("%d", temp->data);
        temp = temp->next;
    }
    return 0;
}