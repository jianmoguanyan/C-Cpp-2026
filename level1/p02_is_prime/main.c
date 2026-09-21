#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

int number = 0;

int box1[99999];
int box2[99999];
int box3[99999];

int main(void)
{   
	int count = 0;
	printf("Input A Positive Integer :");
	scanf("%d", &number);

	if (number == 1)
	{
		printf("%d is not a prime number.", number);
		return 0;
	}
	else if (number == 2) 
	{
		printf("%d is a prime number.", number);
		return 0;
	}
    else if (number % 2 == 0)
	{
		printf("%d is not a prime number.", number);
		return 0;
	}
	//到此已排除1、2、偶数三种特殊情况
	else 
	{
		count = sqrt(number);
		int k = 0;
		for (int i = 2;i <= count;i++)
		{
			box1[k] = i;
			k++;
		}

		//for (int w = 0;w < k;w++)
		//printf("%d ", box1[w]);
		//printf("%d ", k);
		//可以使用此代码测试整数盒box1是否正常录入数字

		int n = 0;
		for (int m = 0;m < k;m++)
		{
			if (box1[m] % 2 != 0)
			{
				box2[n] = box1[m];
				n++;
			}
		}

		//for (int p = 0;p < n;p++)
		//printf("%d ", box2[p]);
		//可以使用此代码测试奇数盒box2是否正常录入数字

		int right;
		double real = 0;
		for (int e = 0;e < n;e++)
		{
			box3[e] = number % box2[e];
			if (box3[e] == 0)
			{
				printf("%d is  not a prime number.", number);
				right++;
				break;
			}

		}

		if (right != 0)
		{
			return 0;
		}
		else
		{
			printf("%d is a prime number.", number);
		}
	}
	return 0;
}
//使用VS C语言环境完成//
