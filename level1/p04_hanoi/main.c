#include <stdio.h>

void hanoi(int n, char source, char helper, char target)
{
    if (n != 0)
    {
        hanoi(n - 1, source, target, helper);
        printf("%c -> %c\n", source, target);
        hanoi(n - 1, helper, source, target);
    }
}

int main()
{
    hanoi(2, 'A', 'B', 'C');
    return 0;
}
//VS C环境