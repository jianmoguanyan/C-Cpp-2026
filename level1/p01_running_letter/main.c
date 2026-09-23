#include <stdio.h>
#include <string.h>
#include <windows.h>

#define TEXT "A"
#define SPEED 50

//获取控制台当前可用的宽度（列数）
int getConsoleWidth(void)
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOut == INVALID_HANDLE_VALUE)
		return 0;
	if (!GetConsoleScreenBufferInfo(hOut, &info))
		return 0;
	//可视区域的列数，随窗口拉伸实时变化
	return info.srWindow.Right - info.srWindow.Left + 1;
}

int main(void)
{
	int pos = 0, dir = 1;
	int textLen = (int)strlen(TEXT);

	while (1)
	{
		int width = getConsoleWidth();
		//窗口太窄或取不到宽度时退到默认值
		if (width < textLen + 1)
			width = textLen + 1;

		int maxPos = width - textLen - 1;
		// 让字母碰到可视右边界时反弹

		system("cls");

		// 通过设置光标位置，避免字符堆积
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD coord = { (SHORT)pos, 0 };
		SetConsoleCursorPosition(hOut, coord);
		printf("%s", TEXT);

		pos += dir;
		if (pos >= maxPos) { pos = maxPos; dir = -1; }
		if (pos <= 0) { pos = 0;    dir = 1; }

		Sleep(SPEED);
	}
	return 0;
}
//VS C环境