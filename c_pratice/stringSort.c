#include <stdio.h>
#include <string.h>

void sortStr(char str[][100],int count)
{
    for (int i = 0; i < count; i++)
    {
        int max = 0;
        for (int j = 0; j < count - i; j++)
        {
            if(strcmp(str[j], str[max]) > 0)
            {
                max = j;
            }
        }
        char temp[100] = {0};
        strcpy(temp, str[max]);
        strcpy(str[max], str[count - 1 - i]);
        strcpy(str[count - 1 - i],temp);
    }
}

int main()
{
    char str[10][100];
    char temp[100];
    int count = 0;
    while(1)
    {
        printf("\"O\"represents input data\n");
        printf("\"s\"represents sort data\n");
        printf("\"E\"represents exit\n");
        printf("Please select the function:\n");
        char select;
        scanf("%c",&select);
        if(select == 'O')
        {
            char* et = "#";
            for (int i = count; i < 10 ; i++)
            {
                printf("please input the %d's string:",i+1);
                scanf("%s", temp);
                if(!strcmp(temp,et))
                {
                    break;
                }
                strcpy(str[i],temp);
                count++;
                temp[0] = '\0';
            }
        }
        else if(select == 's')
        {
            
            sortStr(str,count);
            for(int i = 0; i < count; i++)
            {
                printf("%s\n",str[i]);
            }
        }
        else if(select == 'E')
        {
            break;
        }
        else
        {
            printf("Unavailable function\n");
        }
    }
    return 0;
}