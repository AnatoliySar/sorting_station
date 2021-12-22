#include <stdio.h>
#include <stdlib.h>

typedef struct Stack
{
	double value;
	struct Stack* next;
} Stack;
Stack* initStack(double val)
{
	Stack* newStack = (Stack*)malloc(sizeof(Stack));
	newStack->value = val;
	newStack->next = NULL;
	return newStack;
}
Stack* append_s(Stack* stack, double val)
{
	Stack* newStack = (Stack*)malloc(sizeof(Stack));
	newStack->value = val;
	newStack->next = stack;
	return newStack;
}
Stack* pop_s(Stack* stack)
{
	Stack* tmp = stack;
	stack = stack->next;
	free(tmp);
	return stack;
}
double top_s(Stack* stack)
{
	return stack->value;
}

double charToNum(char* charNum)
{
	double value = 0;
	double power = 1;
	int len = 0;
	int lenF = 0;
	int i, j;
	char d;
	int flag = 0;
	if (charNum[0] == '-')
	{
		flag = 1;
		charNum = charNum + 1;
	}
	while (charNum[len] != '\0' && charNum[len] != '.' && charNum[len] != ',')
	{
		len++;
	}
	for (i = len - 1; i >= 0; i--) 
	{
		d = charNum[i];
		value += power * (d - '0');
		power *= 10;
	}
	
	if (charNum[len] == '\0')
	{
		if (flag == 1)
			value *= -1;
		return value;
	}		
	len++;
	while (charNum[len] != '\0')
	{
		len++;
		lenF++;
	}
	power = 0.1; 
	len = len - lenF;
	for (i = 0; i < lenF; i++)
	{
		d = charNum[len + i];
		value += power * (d - '0');
		power /= 10;
	}
	if (flag == 1)
		value *= -1;
	return value;
}

double countPolish(char* exp)
{
	int i = 0, j = 0;
	char charNum[15];
	double num;
	Stack* stackOfNums = NULL; 
	double top;
	double num1, num2;
	while (exp[i] != '\0')
	{
		if (exp[i] == ' ')
			i++;
		if (exp[i] == '+')
		{
			num2 = top_s(stackOfNums);
			stackOfNums = pop_s(stackOfNums);
			num1 = top_s(stackOfNums);
			stackOfNums = pop_s(stackOfNums);
			stackOfNums = append_s(stackOfNums, num1 + num2);
			i += 1;
		}
		if (exp[i] == '-' && (exp[i + 1] < '0' || exp[i + 1] > '9'))
		{
			num2 = top_s(stackOfNums);
			stackOfNums = pop_s(stackOfNums);
			num1 = top_s(stackOfNums);
			stackOfNums = pop_s(stackOfNums);
			stackOfNums = append_s(stackOfNums, num1 - num2);
			i += 1;
		}
		if (exp[i] == '*')
		{
			num2 = top_s(stackOfNums);
			stackOfNums = pop_s(stackOfNums);
			num1 = top_s(stackOfNums);
			stackOfNums = pop_s(stackOfNums);
			stackOfNums = append_s(stackOfNums, num1 * num2);
			i += 1;
		}
		if (exp[i] == '/')
		{
			num2 = top_s(stackOfNums);
			stackOfNums = pop_s(stackOfNums);
			num1 = top_s(stackOfNums);
			stackOfNums = pop_s(stackOfNums);
			stackOfNums = append_s(stackOfNums, num1 / num2);
			i += 1;
		}
		if (exp[i] >= '0' && exp[i] <= '9' || exp[i] == '-' && exp[i + 1] && exp[i + 1] >= '0' && exp[i + 1] <= '9')
		{
			while (exp[i] >= '0' && exp[i] <= '9' || exp[i] == '.' || exp[i] == ',' || exp[i] == '-')
			{
				charNum[j] = exp[i];
				i++;
				j++;
			}
			charNum[j] = '\0';
			num = charToNum(charNum);
			stackOfNums = append_s(stackOfNums, num);
			i++;
			j = 0;
		}
	}
	num = top_s(stackOfNums);
	pop_s(stackOfNums);
	return num;
}

char* toPolish(char* exp)
{
	int i = 0, j = 0;
	double num;
	char charNum[15];
	double top;
	char* out = (char*)malloc(sizeof(char)*400);
	Stack* stackOfActs = NULL;
	while (exp[i] != '\0')
	{
		if (exp[i] == ' ')
			i++;
		else if ((exp[i] >= '0' && exp[i] <= '9') || 
			(exp[i] == '-' && (exp[i + 1] >= '0' && exp[i + 1] <= '9') && //'-' - это часть числа, если за ним идет цифра
				(i == 0 || exp[i - 1] == '(' || exp[i - 1] == ' ' && exp[i - 2] == '(' || // и при этом '-' стоит в начале либо после '('
				exp[i - 1] == '+' || exp[i - 1] == ' ' && exp[i - 2] == '+' || // или после '+'
				exp[i - 1] == '-' || exp[i - 1] == ' ' && exp[i - 2] == '-' || // или после '-'
				exp[i - 1] == '*' || exp[i - 1] == ' ' && exp[i - 2] == '*' || // или после '*'
				exp[i - 1] == '/' || exp[i - 1] == ' ' && exp[i - 2] == '/'))) // или после '/' (либо после этих символов и ' ')
		{
			if (exp[i] == '-')
			{
				out[j] = '-';
				j++;
				i++;
			}
			while (exp[i] >= '0' && exp[i] <= '9' || exp[i] == '.' || exp[i] == ',')
			{
				out[j] = exp[i];
				i++;
				j++;
			}
			out[j] = ' ';
			j++;
		}
		else if (exp[i] == '(')
		{
			stackOfActs = append_s(stackOfActs, 4);
			i++;
		}
		else if (exp[i] == ')')
		{
			while (1)
			{
				if (stackOfActs == NULL)
				{
					printf("Issues with brackets");
					return "0";
				}
				top = stackOfActs->value;
				if (top == 0)
				{
					out[j] = '+';
					j++;
					out[j] = ' ';
					j++;
					stackOfActs = pop_s(stackOfActs);
				}
				if (top == 1)
				{
					out[j] = '-';
					j++;
					out[j] = ' ';
					j++;
					stackOfActs = pop_s(stackOfActs);
				}
				if (top == 2)
				{
					out[j] = '*';
					j++;
					out[j] = ' ';
					j++;
					stackOfActs = pop_s(stackOfActs);
				}
				if (top == 3)
				{
					out[j] = '/';
					j++;
					out[j] = ' ';
					j++;
					stackOfActs = pop_s(stackOfActs);
				}
				if (top == 4)
				{
					stackOfActs = pop_s(stackOfActs);
					break;
				}
			}
			i++;
		}
		else if (exp[i] == '+')
		{
			while (stackOfActs != NULL && (stackOfActs->value == 0 || stackOfActs->value == 1
				|| stackOfActs->value == 2	|| stackOfActs->value == 3))
			{
				top = top_s(stackOfActs);
				if (top == '0')
				{
					out[j] = '+';
					j++;
					out[j] = ' ';
					j++;
				}
				if (top == '1')
				{
					out[j] = '-';
					j++;
					out[j] = ' ';
					j++;
				}
				if (top == '2')
				{
					out[j] = '*';
					j++;
					out[j] = ' ';
					j++;
				}
				if (top == '3')
				{
					out[j] = '/';
					j++;
					out[j] = ' ';
					j++;
				}
			}
			stackOfActs = append_s(stackOfActs, 0);
			i++;
		}
		else if (exp[i] == '-')
		{
			while (stackOfActs != NULL && (stackOfActs->value == 0 || stackOfActs->value == 1
				|| stackOfActs->value == 2 || stackOfActs->value == 3))
			{
				top = top_s(stackOfActs);
				if (top == '0')
				{
					out[j] = '+';
					j++;
					out[j] = ' ';
					j++;
				}
				if (top == '1')
				{
					out[j] = '-';
					j++;
					out[j] = ' ';
					j++;
				}
				if (top == '2')
				{
					out[j] = '*';
					j++;
					out[j] = ' ';
					j++;
				}
				if (top == '3')
				{
					out[j] = '/';
					j++;
					out[j] = ' ';
					j++;
				}
			}
			stackOfActs = append_s(stackOfActs, 1);
			i++;
		}
		else if (exp[i] == '*')
		{
			while (stackOfActs != NULL && (stackOfActs->value == 2 || stackOfActs->value == 3))
			{
				top = top_s(stackOfActs);
				if (top == '2')
				{
					out[j] = '*';
					j++;
					out[j] = ' ';
					j++;
				}
				if (top == '3')
				{
					out[j] = '/';
					j++;
					out[j] = ' ';
					j++;
				}
			}
			stackOfActs = append_s(stackOfActs, 2);
			i++;
		}
		else if (exp[i] == '/')
		{
			while (stackOfActs != NULL && (stackOfActs->value == 2 || stackOfActs->value == 3))
			{
				top = top_s(stackOfActs);
				if (top == '2')
				{
					out[j] = '*';
					j++;
					out[j] = ' ';
					j++;
				}
				if (top == '3')
				{
					out[j] = '/';
					j++;
					out[j] = ' ';
					j++;
				}
			}
			stackOfActs = append_s(stackOfActs, 3);
			i++;
		}

	}
	while (stackOfActs != NULL)
	{
		if (stackOfActs->value == 0)
		{
			out[j] = '+';
			j++;
			out[j] = ' ';
			j++;
			stackOfActs = pop_s(stackOfActs);
		}
		else if (stackOfActs->value == 1)
		{
			out[j] = '-';
			j++;
			out[j] = ' ';
			j++;
			stackOfActs = pop_s(stackOfActs);
		}
		else if (stackOfActs->value == 2)
		{
			out[j] = '*';
			j++;
			out[j] = ' ';
			j++;
			stackOfActs = pop_s(stackOfActs);
		}
		else if (stackOfActs->value == 3)
		{
			out[j] = '/';
			j++;
			out[j] = ' ';
			j++;
			stackOfActs = pop_s(stackOfActs);
		}
	}
	out[j - 1] = '\0';
	return out;
}

int main()
{
	char* e = toPolish("-5-2*(-9*-7)");
	printf("%s\n", e);
	double b = countPolish(e);
	printf("%lf\n", b);	
	return 0;
}