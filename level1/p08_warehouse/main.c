#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h> // 用于获取 INT_MAX 检查溢出

#define FILE_NAME "inventory.dat"
#define MODEL_LEN 32

// 货物节点结构体
typedef struct GoodsNode
{
    char model[MODEL_LEN];
    int count;
    struct GoodsNode* next;
} GoodsNode;

// 用于文件读写的纯净结构体（绝对不包含指针！）
// 防止将无意义的内存地址写入文件导致数据损坏
typedef struct FileRecord
{
    char model[MODEL_LEN];
    int count;
} FileRecord;

// 清空输入缓冲区，防止非法输入导致死循环
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 【P8检查输入】安全地读取一个整数
int readSafeInt(const char* prompt)
{
    int val;
    printf("%s", prompt);
    while (scanf("%d", &val) != 1) // 如果读取失败
    {
        clearInputBuffer();
        printf("输入无效，请输入数字：");
    }
    clearInputBuffer(); // 清除数字后面的回车或多余字符
    return val;
}

// 【P8限制型号长度】安全地读取字符串
void readSafeString(const char* prompt, char* buffer, int maxLen)
{
    printf("%s", prompt);
    // 构造安全格式化字符串，例如 maxLen=32 则生成 "%31s"，确保留出 '\0' 的位置
    char format[16];
    sprintf(format, "%%%ds", maxLen - 1);
    scanf(format, buffer);
    clearInputBuffer();
}

// 创建一个新货物节点
GoodsNode* createGoods(const char* model, int count)
{
    GoodsNode* newNode = (GoodsNode*)malloc(sizeof(GoodsNode));
    if (newNode == NULL)
    {
        printf("内存分配失败！\n");
        exit(EXIT_FAILURE);
    }
    // 使用 strncpy 保证安全，并强制补 '\0'
    strncpy(newNode->model, model, MODEL_LEN - 1);
    newNode->model[MODEL_LEN - 1] = '\0';
    newNode->count = count;
    newNode->next = NULL;
    return newNode;
}

// 在链表中按型号查找货物
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
void stockIn(GoodsNode** head)
{
    char model[MODEL_LEN];
    readSafeString("请输入货物型号：", model, MODEL_LEN);
    int addNum = readSafeInt("请输入入库数量：");

    if (addNum <= 0)
    {
        printf("入库数量必须大于0！\n");
        return;
    }

    GoodsNode* target = findGoods(*head, model);
    if (target != NULL)
    {
        // 【P8检查库存溢出】防止 int 类型数据溢出变成负数
        if (target->count > INT_MAX - addNum)
        {
            printf("入库失败：加上该数量后库存将超出系统最大限制！\n");
            return;
        }
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
void stockOut(GoodsNode** head)
{
    char model[MODEL_LEN];
    readSafeString("请输入货物型号：", model, MODEL_LEN);
    int outNum = readSafeInt("请输入出库数量：");

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
GoodsNode* loadFromFile()
{
    FILE* fp = fopen(FILE_NAME, "rb");
    if (fp == NULL)
    {
        return NULL; // 文件不存在，返回空链表
    }
    GoodsNode* head = NULL;
    GoodsNode* tail = NULL; // 使用尾插法，确保读出来的顺序和存进去的一致
    FileRecord temp;        // 使用不含指针的结构体读取

    while (fread(&temp, sizeof(FileRecord), 1, fp) == 1)
    {
        GoodsNode* newNode = createGoods(temp.model, temp.count);
        if (head == NULL)
        {
            head = newNode;
            tail = newNode;
        }
        else
        {
            tail->next = newNode;
            tail = newNode;
        }
    }
    fclose(fp);
    return head;
}

// 【P8检查写入结果】将链表库存写入文件保存
void saveToFile(GoodsNode* head)
{
    FILE* fp = fopen(FILE_NAME, "wb");
    if (fp == NULL)
    {
        printf("文件打开失败，无法保存库存！\n");
        return;
    }
    GoodsNode* p = head;
    FileRecord temp;
    int isSuccess = 1; // 标记是否全部写入成功

    while (p != NULL)
    {
        // 只拷贝数据部分，不拷贝指针
        strncpy(temp.model, p->model, MODEL_LEN);
        temp.count = p->count;

        // 检查 fwrite 返回值
        if (fwrite(&temp, sizeof(FileRecord), 1, fp) != 1)
        {
            isSuccess = 0;
            break;
        }
        p = p->next;
    }

    if (fclose(fp) != 0) // 关闭文件也可能失败（比如缓冲区刷新到磁盘时出错）
    {
        isSuccess = 0;
    }

    if (isSuccess)
    {
        printf("库存数据已保存至文件！\n");
    }
    else
    {
        printf("【警告】保存数据时发生错误，部分数据可能丢失！\n");
    }
}

// 释放整条链表内存
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
}

int main(void)
{
    GoodsNode* inventory = loadFromFile();

    while (1)
    {
        showMenu();
        int select = readSafeInt("请输入选择："); // 使用安全读取

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