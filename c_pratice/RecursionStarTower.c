#include<stdio.h>

void p(int first, int end, int col)
{
	if(first == 0)
	{
		return;
	}
	for (int i = 1; i <= col; i++)
	{
		if(i < first || i > end)
		{
			printf(" ");
		}
		else
		{
			printf("*");
		}
	}
	printf("\n");
	p(first - 1,end + 1,col);
}
int main()
{
	int row;
	scanf("%d", &row);
	int col = 2*row-1;
	int first = row, end = row;
	p(first,end,col);

	return 0;
}