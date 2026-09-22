#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>

int main(void)
{  
   printf("all the prime number from 2 to 1000：");
   for(int number=2;number<1001;number++)
   {
    int count = 0;

    if (number == 2) 
    {
        printf("2 ");
    }
    else if (number % 2 == 0)
    {continue;}
    //到此已排除2、偶数两种特殊情况
    else
    {   int right=0;
        count = sqrt(number);
        for(int k=3;k<=count;k=k+2)
        { 
            if(number%k==0)
            {
                right++;
                break;
            }
        }

        if (right == 0)
        { printf("%d ", number);}
    }
    
   }
    return 0;
}
