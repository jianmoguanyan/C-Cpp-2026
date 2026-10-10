#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <time.h>

int isPrime(int num)
{
    if(num < 2)
        return 0;
    if(num == 2)
        return 1;
    if(num % 2 == 0)
        return 0;
    // 只遍历奇数，循环到平方根，优化效率
    int sqrt_num = sqrt(num);
    for(int k = 3; k <= sqrt_num; k += 2)
    {
        if(num % k == 0)
        {
            return 0;
        }
    }
    return 1;
}

int main(void)
{
    clock_t start, end;
    printf("all the prime number from 2 to 1000：\n");

    start = clock();

    for(int number = 2; number <= 1000; number++)
    {
        if(isPrime(number))
        {
            printf("%d ", number);
        }
    }

    end = clock();
    double alltime = (double)(end - start) / CLOCKS_PER_SEC;
    printf("\nThe count consume %.6f seconds", alltime);

    return 0;
}

//VS C环境