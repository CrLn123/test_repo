#include <stdio.h>
void factorial()
{
    printf("请输入一个整数\n");
    int num;
    scanf("%d", &num);
    int sum[1000] = {1};
    int len = 1;
    for(int i = 2; i <= num; i++)
    {
        int carry = 0;         
        
        for (int j = 0; j < len; j++)
        {
            int temp = sum[j] * i + carry;
            carry = temp/10;
            sum[j] = temp % 10;
        }
        while (carry > 0)
        {
            sum[len] = carry % 10;
            carry /= 10;
            len++;
        }
    }
    printf("阶乘为:", sum);
    for(int j = len - 1; j >= 0; j--)
    {
        printf("%d",sum[j]);
    }
    printf("\n");

}
void judge()
{
    printf("请输入一个整数\n");
    int num;
    scanf("%d", &num);
    int index = 0;
    
    for (int i = 2; i < num/2 + 1; i++)
    {
        if(num % i == 0)
        {
            index = 1;
            break;
        }
    }
    if(num == 1 || num == 2)
    {
        printf("是一个质数\n");
    }
    else if (index)
    {
        printf("不是一个质数\n");
    }
    else
    {
        printf("是一个质数\n");
    }
}
void showFun()
{
    while(1)
    {
        printf("1代表退出，2代表求阶乘，3代表括判别质数\n");
        int select;
        scanf("%d", &select);
        switch(select)
        {
            case 1:
                return;
            case 2:
                factorial();
                break;
            case 3:
                judge();
                break;
            default:
                printf("请选择一个选项来操作 1代表退出，2代表求阶乘，3代表括判别质数\n");
            
        }
    }
}
int main()
{
    showFun();
    return 0;
}