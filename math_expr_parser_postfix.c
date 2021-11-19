#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *get_str_wo_spaces(char *str);
int is_operator(const char c);
int is_digit(const char c);
int precedence(char op);
double apply_op(double a, double b, char op);
double calc(char *str);


int main(int argc, char *argv[])
{
    (void)argv;
	if(argc == 1)
	{
        while (1)
        {
            char *str = 0;
            long unsigned int sz = 0;

            printf("Enter the math expression or q to exit: ");
            if (-1 != getline(&str, &sz, stdin))
            {
                if (*str == 'q')
                {
                    free(str);
                    str = 0;
                    return 0;
                }
                printf ("Result: %.2f\n",calc(str));
                free(str);
                str = 0;
            }
        }
	}
    return 0;
}

double calc(char *str_original)
{

    char *str = 0;
    str = get_str_wo_spaces(str_original);


    int len = strlen(str);
    int is_number = 0;
    int num_count = 0;
    int ops_count = 0;

    int ops_len = 0;
    int vals_len = 0;

    for (int i = 0; i < len; i++)
    {
        if (is_digit(str[i]))
        {
            if(is_number == 0)
            {
                num_count++;
                is_number = 1;
            }
        }
        else
        {
            is_number = 0;
            if (str[i] != ' ')
            {
                ops_count++;
            }
        }
    }
    double values[num_count];
    char ops[ops_count];

    for(int i = 0; i < len; i++)
    {
        if(str[i] == '(')
        {
            ops[ops_len++] = str[i];
        }
        else if(is_digit(str[i]))
        {
            int val = 0;

            while(i < len && is_digit(str[i]))
            {
                val = (val*10) + (str[i]-'0');
                i++;
            }
            values[vals_len++] = val;
            i--;
        }
        else if(str[i] == ')')
        {
            while(ops_len > 0 && ops[ops_len - 1] != '(')
            {
                char op = ops[--ops_len];
                ops[ops_len] = 0;

                double val2 = values[--vals_len];
                values[vals_len] = 0;

                double val1 = values[vals_len - 1];
                values[vals_len - 1] = apply_op(val1, val2, op);
            }
            if(ops_len > 0)
            {
                ops[--ops_len] = 0;
            }
        }
        else
        {
            while(ops_len > 0 && precedence(ops[ops_len - 1]) >= precedence(str[i]))
            {
                char op = ops[--ops_len];
                ops[ops_len] = 0;

                double val2 = values[--vals_len];
                values[vals_len] = 0;

                double val1 = values[vals_len - 1];
                values[vals_len - 1] = apply_op(val1, val2, op);
            }
            ops[ops_len++] = str[i];
        }
    }

    while(ops_len > 0)
    {
        char op = ops[--ops_len];
        ops[ops_len] = 0;

        double val2 = values[--vals_len];
        values[vals_len] = 0;

        double val1 = values[vals_len - 1];
        values[vals_len - 1] = apply_op(val1, val2, op);
    }
    if (str)
    {
        free(str);
        str = 0;
    }

    return values[vals_len - 1];
}

int precedence(char op)
{
    if(op == '+' || op == '-')
    {
        return 1;
    }
    if(op == '*' || op == '/')
    {
        return 2;
    }
    return 0;
}

double apply_op(double a, double b, char op)
{
    switch(op)
    {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            if (b != 0)
            {
                return a / b;
            }
            else
            {
                printf("Error: division by 0. Result is incorrect\n");
                return 0;
            }
    }
}

char *get_str_wo_spaces(char *str)  //return 0 if str is incorrect math expression, and return pointer to string if it is correct math expression
{
    char *str_wo_spaces = 0;
    int len = 0;
    int i = 0;
	int is_bracket = 0;
    int brackets_count = 0;
    int st_ind = -1;

    if (str == 0 || *str == '\0')
    {
        printf("Error: empty string or there is no string\n");
        return str_wo_spaces;
    }
    while (str[i] != '\n')
    {
        if (str[i] == ' ')
        {
            i++;
            continue;
        }
        if (st_ind == -1)
        {
            st_ind = i;
            if (str[i] != '(' && str[i] != '-' && !is_digit(str[i]))
            {
                printf("Error: math expression starts from something which differs from (, - or digit\n");
                return str_wo_spaces;
            }
        }
        if (str[i] == '(')
        {
            is_bracket = 1;
            brackets_count++;
        }
        else if (str[i] == '-' && is_bracket)
        {
            len++;      //will add 0 before unary - sign
        }
        else if (str[i] == ')')
        {
            is_bracket = 0;
            brackets_count--;
        }
        else if (is_bracket)
        {
            is_bracket = 0;
        }
        len++;
        i++;
    }
    if (brackets_count != 0)
    {
        printf("Error: different number of ( and )\n");
        return str_wo_spaces;
    }
    if (!(str_wo_spaces = (char *) malloc(sizeof(char)*(len + 3))))   //added 2 char for 0+ at the beginning of every exppression and 1 char for \0
    {
        printf("Memory allocation error\n");
        return str_wo_spaces;
    }
    memset(str_wo_spaces, 0, len + 3);

    str_wo_spaces[0] = '0';
    str_wo_spaces[1] = '+';
    int j = 1;
    is_bracket = 0;
    i = 0;

    if (str[0] == ' ' || str[0] == '-')
    {
        while (i <= len && str[i] == ' ')
        {
            i++;
        }
        if (i <= len && str[i] == '-')
        {
            i++;
            if(i <= len && !is_digit(str[i]))
            {
                while (i <= len && str[i] == ' ')
                {
                    i++;
                }
                if (i <= len && str[i] != '(')
                {
                    free(str_wo_spaces);
                    str_wo_spaces = 0;
                    printf("Error: there is no digit or ( after unary - sign\n");
                    return str_wo_spaces;
                }
                else if (str[i] == '(')
                {
                    is_bracket = 1;
                }
            }
            str_wo_spaces[j] = '-';
        }
    }

	while (str[i] != '\n')
	{
        int is_curr_digit = (str[i] >= '0' && str[i] <= '9');
        int is_curr_op = is_operator(str[i]);

        if (str[i] != ' ' && !is_curr_digit && !is_curr_op && str[i] != '(' && str[i] != ')')
        {
            free(str_wo_spaces);
            str_wo_spaces = 0;
            printf("Error: usage of incorrect symbol. It is allowed only digits, spaces, () and -, +, *, / signs\n");
            return str_wo_spaces;
		}
        while (str[i] == ' ')
        {
            i++;
        }
        if (is_curr_op)
        {
            if (!is_digit(str_wo_spaces[j]) && (str[i] == '-' && str_wo_spaces[j] != '('))
            {
                free(str_wo_spaces);
                str_wo_spaces = 0;
                printf("Error: there is no number before operator\n");
                return str_wo_spaces;
            }
            if (str[i]  == '-' && str_wo_spaces[j] == '(')
            {
                str_wo_spaces[++j] = '0';     //replace every unary - sign with 0- to have only bynary -
            }
        }

		if (is_curr_digit)
        {
            if ((str_wo_spaces[j] != '(' && !is_digit(str_wo_spaces[j]) && !is_operator(str_wo_spaces[j]))
                    || (is_digit(str_wo_spaces[j]) && i > 0 && str[i - 1] == ' '))   // space between digits
            {
                free(str_wo_spaces);
                str_wo_spaces = 0;
                printf("Error: something which is differ from digit, ( or +, -, *, / signs before digit\n");
                return str_wo_spaces;
            }
        }
        if (str[i] == '(')
        {
            if (!is_operator(str_wo_spaces[j]) && str_wo_spaces[j] != '(')
            {
                free(str_wo_spaces);
                str_wo_spaces = 0;
                printf("Error: there is no operator or ( before (\n");
                return str_wo_spaces;
            }
        }
        if (str[i] == ')')
        {
            if (!is_digit(str_wo_spaces[j]) && str_wo_spaces[j] != ')')
            {
                free(str_wo_spaces);
                str_wo_spaces = 0;
                printf("Error: there is no number before )\n");
                return str_wo_spaces;
            }
        }
        str_wo_spaces[++j] = str[i];
        i++;
	}
    if (!is_digit(str_wo_spaces[j]) && str_wo_spaces[j] != ')')   // last part is not a number
    {
        free(str_wo_spaces);
        str_wo_spaces = 0;
        printf("Error: there is no number or ) at the end of expression\n");
        return str_wo_spaces;
    }
	return str_wo_spaces;
}

int is_operator (const char c)
{
	char op[] = {'+', '-', '*', '/'};

	for (long unsigned int i = 0; i < sizeof(op); i++)
	{
		if (c == op[i])
		{
			return 1;
		}
	}
	return 0;
}

int is_digit (const char c)
{
    return (c >= '0' && c <= '9');
}
