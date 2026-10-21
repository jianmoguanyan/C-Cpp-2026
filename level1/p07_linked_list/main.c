#include <stdio.h>
#include <stdlib.h>

// 单向链表节点结构体
// value 节点存储的数据
// next 指向下一个节点的指针
typedef struct Node {
    int value;
    struct Node* next;
} Node;

// 创建一个新链表节点
// val 存入节点的整型数据
// return 成功返回新节点地址；内存分配失败终止程序
Node* createNode(int val)
{
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("内存分配失败！\n");
        exit(EXIT_FAILURE);
    }
    newNode->value = val;
    newNode->next = NULL;
    return newNode;
}

// 遍历并打印链表所有节点的值
// head 链表头指针
void printList(Node* head)
{
    if (head == NULL)
    {
        printf("链表为空\n");
        return;
    }
    Node* p = head;
    while (p != NULL)
    {
        printf("%d ", p->value);
        p = p->next;
    }
    printf("\n");
}

// 原地反转单向链表
// head 原链表头指针
// return 反转之后新链表的头指针
Node* reverseList(Node* head)
{
    Node* pre = NULL;
    Node* cur = head;
    Node* nextTmp;
    while (cur != NULL)
    {
        nextTmp = cur->next;
        cur->next = pre;
        pre = cur;
        cur = nextTmp;
    }
    return pre;
}

// 从头开始查找第一个值等于5的节点
// head 链表头指针
// foundNode 输出型参数：带回找到节点的指针，供查找下一个使用
// return 找到返回节点序号（序号从1开始），未找到返回-1
int findFirstFive(Node* head, Node** foundNode)
{
    *foundNode = NULL;
    Node* p = head;
    int idx = 1;
    while (p != NULL)
    {
        if (p->value == 5)
        {
            *foundNode = p;
            return idx;
        }
        p = p->next;
        idx++;
    }
    return -1;
}

// 从给定节点的下一个位置，继续查找下一个值等于5的节点
// head 链表头指针
// lastNode 上一次找到的节点
// foundNode 输出型参数：带回本次找到节点的指针
// return 找到返回节点序号（序号从1开始），未找到返回-1
int findNextFive(Node* head, Node* lastNode, Node** foundNode)
{
    *foundNode = NULL;
    if (lastNode == NULL)
    {
        return -1;
    }
    Node* p = lastNode->next;
    int idx = 1;
    Node* tmp = head;
    // 遍历计数，直接拿到p对应的下标，逻辑清晰
    while (tmp != p && tmp != NULL)
    {
        idx++;
        tmp = tmp->next;
    }

    while (p != NULL)
    {
        if (p->value == 5)
        {
            *foundNode = p;
            return idx;
        }
        p = p->next;
        idx++;
    }
    return -1;
}

// 释放整条链表所有节点内存，避免内存泄漏
// head 链表头指针
void freeList(Node* head)
{
    Node* p = head;
    Node* delNode;
    while (p != NULL)
    {
        delNode = p;
        p = p->next;
        free(delNode);
    }
}

int main(void)
{
    // main函数内创建单向链表：1 -> 3 -> 5 -> 7 -> 5 -> 9
    Node* head = createNode(1);
    head->next = createNode(3);
    head->next->next = createNode(5);
    head->next->next->next = createNode(7);
    head->next->next->next->next = createNode(5);
    head->next->next->next->next->next = createNode(9);

    Node* searchPtr = NULL;
    int ret;

    printf("原始链表：");
    printList(head);

    // 查找第一个5
    ret = findFirstFive(head, &searchPtr);
    printf("第一个值为5的节点序号：%d\n", ret);
    // 查找下一个5
    ret = findNextFive(head, searchPtr, &searchPtr);
    printf("下一个值为5的节点序号：%d\n", ret);
    // 再找下一个（验证边界：没有更多5，返回-1）
    ret = findNextFive(head, searchPtr, &searchPtr);
    printf("再下一个值为5的节点序号：%d\n", ret);

    // 链表反序
    head = reverseList(head);
    printf("\n反转后的链表：");
    printList(head);

    ret = findFirstFive(head, &searchPtr);
    printf("反转后第一个值为5的节点序号：%d\n", ret);
    ret = findNextFive(head, searchPtr, &searchPtr);
    printf("反转后下一个值为5的节点序号：%d\n", ret);

    // 释放内存
    freeList(head);
    head = NULL;

    return 0;
}
