#include <stdio.h>
int m = 2;
int func(int n)
{
    if(n == 1234567)
    {
        printf("%d\n", n);
        return;
    }
    else
    {
        printf("%d\n",n);
        func(n*10 + m++);
        printf("%d\n",n);
    }
}
int main()
{
    func(1);
    return 0;
}