#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

int main(void)
{
    int number;
    printf("Input A Positive Integer :");
    scanf("%d", &number);

    if (number == 1)
    {
        printf("%d is not a prime number.", number);
    }
    else if (number == 2)
    {
        printf("%d is a prime number.", number);
    }
    else if (number % 2 == 0)
    {
        printf("%d is not a prime number.", number);
    }
    //到此已排除1、2、偶数三种特殊情况
    else
    {
        int right = 0;
        double count = sqrt(number);
        for (int k = 3; k <= count; k = k + 2)
        {
            if (number % k == 0)
            {
                right++;
                break;
            }
        }
        if (right == 0)
        {
            printf("%d is a prime number.", number);
        }
        else
        {
            printf("%d is not a prime number.", number);
        }
    }
    return 0;
}
//VS C环境
