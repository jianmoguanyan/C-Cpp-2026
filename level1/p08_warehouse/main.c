#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "inventory.dat"
#define MODEL_LEN 32

// 货物节点结构体，保存型号、库存数量
// model 货物型号
// count 库存数量
// next 指向下一个货物节点
typedef struct GoodsNode {
    char model[MODEL_LEN];
    int count;
    struct GoodsNode* next;
} GoodsNode;

// 创建一个新货物节点
// model 货物型号字符串
// count 初始数量
// return 成功返回新节点指针；分配失败退出程序
GoodsNode* createGoods(const char* model, int count)
{
    GoodsNode* newNode = (GoodsNode*)malloc(sizeof(GoodsNode));
    if (newNode == NULL)
    {
        printf("内存分配失败！\n");
        exit(EXIT_FAILURE);
    }
    strncpy(newNode->model, model, MODEL_LEN - 1);
    newNode->model[MODEL_LEN - 1] = '\0';
    newNode->count = count;
    newNode->next = NULL;
    return newNode;
}

// 在链表中按型号查找货物
// head 链表头
// model 待查找型号
// return 找到返回节点指针，找不到返回NULL
GoodsNode* findGoods(GoodsNode* head, const char* model)
{
    GoodsNode* p = head;
    while (p != NULL)
    {
        if (strcmp(p->model, model) == 0)
        {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

// 显示全部存货列表
// head 链表头
void showInventory(GoodsNode* head)
{
    if (head == NULL)
    {
        printf("==== 当前库存为空 ====\n");
        return;
    }
    printf("==== 存货列表 ====\n");
    printf("%-20s %8s\n", "型号", "数量");
    GoodsNode* p = head;
    while (p != NULL)
    {
        printf("%-20s %8d\n", p->model, p->count);
        p = p->next;
    }
    printf("==================\n");
}

// 入库：已有型号增加数量；新型号新增一条记录
// head 链表头指针的地址（二级指针，支持头插新增）
void stockIn(GoodsNode** head)
{
    char model[MODEL_LEN];
    int addNum;
    printf("请输入货物型号：");
    scanf("%s", model);
    printf("请输入入库数量：");
    scanf("%d", &addNum);

    if (addNum <= 0)
    {
        printf("入库数量必须大于0！\n");
        return;
    }

    GoodsNode* target = findGoods(*head, model);
    if (target != NULL)
    {
        target->count += addNum;
        printf("入库成功！%s 当前库存：%d\n", model, target->count);
    }
    else
    {
        GoodsNode* newNode = createGoods(model, addNum);
        newNode->next = *head;
        *head = newNode;
        printf("新增货物，入库成功！\n");
    }
}

// 出库：减少对应型号库存，校验库存充足
// head 链表头
void stockOut(GoodsNode** head)
{
    char model[MODEL_LEN];
    int outNum;
    printf("请输入货物型号：");
    scanf("%s", model);
    printf("请输入出库数量：");
    scanf("%d", &outNum);

    if (outNum <= 0)
    {
        printf("出库数量必须大于0！\n");
        return;
    }

    GoodsNode* target = findGoods(*head, model);
    if (target == NULL)
    {
        printf("该型号货物不存在！\n");
        return;
    }
    if (target->count < outNum)
    {
        printf("库存不足！现有库存：%d\n", target->count);
        return;
    }
    target->count -= outNum;
    printf("出库成功！%s 当前库存：%d\n", model, target->count);
}

// 从文件读取库存数据，构建链表
// return 加载完成的链表头指针
GoodsNode* loadFromFile()
{
    FILE* fp = fopen(FILE_NAME, "rb");
    if (fp == NULL)
    {
        // 文件不存在，首次运行，返回空链表
        return NULL;
    }
    GoodsNode* head = NULL;
    GoodsNode temp;
    while (fread(&temp, sizeof(GoodsNode), 1, fp) == 1)
    {
        GoodsNode* newNode = createGoods(temp.model, temp.count);
        newNode->next = head;
        head = newNode;
    }
    fclose(fp);
    return head;
}

// 将链表库存全部写入文件保存
// head 链表头
void saveToFile(GoodsNode* head)
{
    FILE* fp = fopen(FILE_NAME, "wb");
    if (fp == NULL)
    {
        printf("文件打开失败，无法保存库存！\n");
        return;
    }
    GoodsNode* p = head;
    while (p != NULL)
    {
        fwrite(p, sizeof(GoodsNode), 1, fp);
        p = p->next;
    }
    fclose(fp);
    printf("库存数据已保存至文件！\n");
}

// 释放整条链表内存
// head 链表头
void freeList(GoodsNode* head)
{
    GoodsNode* p = head;
    GoodsNode* del;
    while (p != NULL)
    {
        del = p;
        p = p->next;
        free(del);
    }
}

// 打印主菜单
void showMenu()
{
    printf("\n===== 简单进销存系统 =====\n");
    printf("1. 显示存货列表\n");
    printf("2. 入库\n");
    printf("3. 出库\n");
    printf("4. 退出程序\n");
    printf("==========================\n");
    printf("请输入选择：");
}

int main(void)
{
    // 程序启动，读取文件加载库存
    GoodsNode* inventory = loadFromFile();
    int select;

    while (1)
    {
        showMenu();
        scanf("%d", &select);
        switch (select)
        {
        case 1:
            showInventory(inventory);
            break;
        case 2:
            stockIn(&inventory);
            break;
        case 3:
            stockOut(&inventory);
            break;
        case 4:
            // 退出前保存数据
            saveToFile(inventory);
            freeList(inventory);
            printf("程序退出\n");
            return 0;
        default:
            printf("输入选项无效，请重新选择！\n");
            break;
        }
    }
}
