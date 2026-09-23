#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

#define TEXT "A"
#define SCREEN_WIDTH 80
#define SPEED_MS 50

int main(void)
{
	int pos = 0;        // 当前字符起始位置
	int dir = 1;        // 移动方向 1向右，-1向左
	int textLen = strlen(TEXT);
	int maxPos = SCREEN_WIDTH - textLen;  // 字符能到达的最右侧坐标

	while (1)
	{
		system("cls");
		// 打印前置空格实现字符偏移
		for (int i = 0; i < pos; i++)
		{
			putchar(' ');
		}
		printf("%s\n", TEXT);

		pos += dir;

		// 碰到右边界，反向向左
		if (pos >= maxPos)
		{
			pos = maxPos;
			dir = -1;
		}
		// 碰到左边界，反向向右
		if (pos <= 0)
		{
			pos = 0;
			dir = 1;
		}

		Sleep(SPEED_MS);
	}
	return 0;
}

//VS C环境