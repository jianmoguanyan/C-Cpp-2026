#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

int number = 0;

int box1[99999];
int box2[99999];
int box3[99999];

int main(void)
{   
	double count = 0;
	printf("Input A Positive Integer :");
	scanf("%d", &number);

    count = sqrt(number);
	int k = 0;
	for (int i=1;i < count;i++)
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
		if(box1[m]%2!=0)
		{
			box2[n] = box1[m];
		    n++;
		}
	}
    
	//for (int p = 0;p < n;p++)
	//printf("%d ", box2[p]);
	//可以使用此代码测试奇数盒box2是否正常录入数字

    double real = 0;
	for(int e=0;e<n;e++)
	{
		box3[e] = number % box2[e];
		if (box3[e] != 0)
		{
			real = real + 1;
		}
		else
		{
			real = real + 0;
		}
	}

	if (real == 0)
	{
		printf("%d is a prime number.", number);
	}
	else if(real!=0)
	{
		printf("%d is not a prime number.", number);
	}
	
	return 0;
}

//使用Visual Studio C语言环境完成
