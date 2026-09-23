#include <stdio.h>
#include <windows.h>

int times = 0;
int k= 0;
int i;

int main() {
	while (times <= 30)
	{
		for (int i = 0; i<= times;i++)		
		{
			printf(" ");
		}
		fflush(stdout);
		printf("A");
		Sleep(200);
		fflush(stdout);
		printf("\r                                             \r");
		times++;
	}
	//现在times=31 i=0//
	while (k<=30)
	{
		for (i=30; i>= k;i--)
		{
			printf(" ");
		}
		fflush(stdout);
		printf("A");
		printf("                                                     \r");
		Sleep(200);
		fflush(stdout);
		printf("                                             \r");
		k++;
	}

	printf("over");
	return 0;
}
