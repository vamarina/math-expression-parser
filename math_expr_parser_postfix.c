#include "math_expr_parser_postfix.h"

// Function to calculate the result of
// the correct expression
double calc(char *str)
{
    int len = strlen(str);
    int is_number = 0;
    int num_count = 0;
    int ops_count = 0;

    int ops_len = 0;
    int vals_len = 0;

    //Get numbers' and operatots' count
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

    // Arrays to store values and operators
    double values[num_count];
    char ops[ops_count];

    for(int i = 0; i < len; i++)
    {
        // Current symbol is an opening
        // brace, add it to 'ops'
        if(str[i] == '(')
        {
            ops[ops_len++] = str[i];
        }

        // Get number and add it to values' array
        else if(is_digit(str[i]))
        {
            int val = 0;

            while(i < len && is_digit(str[i]))
            {
                val = (val*10) + (str[i]-'0');
                i++;
            }
            values[vals_len++] = val;
            // decrease the value of i because it already
            // points to the character next to the digit,
            // and the for loop will increase the i
            i--;
        }

        // Closing brace encountered, solve
        // entire brace.
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

            // Remove opening brace
            if(ops_len > 0)
            {
                ops[--ops_len] = 0;
            }
        }
        else
        {
            // While the last operator in 'ops' has same or greater
            // precedence to the current character, which
            // is an operator. Apply the last operator
            // of 'ops' to the last two values
            while(ops_len > 0 && precedence(ops[ops_len - 1]) >= precedence(str[i]))
            {
                char op = ops[--ops_len];
                ops[ops_len] = 0;

                double val2 = values[--vals_len];
                values[vals_len] = 0;

                double val1 = values[vals_len - 1];
                values[vals_len - 1] = apply_op(val1, val2, op);
            }
            // Add current character to 'ops'
            ops[ops_len++] = str[i];
        }
    }

    // Entire expression has been parsed at this
    // point, apply remaining ops to remaining
    // values.
    while(ops_len > 0)
    {
        char op = ops[--ops_len];
        ops[ops_len] = 0;

        double val2 = values[--vals_len];
        values[vals_len] = 0;

        double val1 = values[vals_len - 1];
        values[vals_len - 1] = apply_op(val1, val2, op);
    }

    // The last value is the result
    return values[vals_len - 1];
}

// Function to check whether string is correct math expression,
// remove spaces and return constructed string if str is
// correct or 0 if str is incorrect math expression
char *get_str_wo_spaces(char *str)
{
    char *str_wo_spaces = 0;
    int len = 0;
    int i = 0;
	int is_bracket = 0;
    int brackets_count = 0;
    int st_ind = -1;

    if (str == 0 || *str == '\n')
    {
        printf("Error: empty string or there is no string\n");
        return str_wo_spaces;
    }

    // str contains \n, because getline is used for getting string
    // get string length without spaces to allocate memory
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

        // increase number of brackets to check whether every ( has the matching )
        if (str[i] == '(')
        {
            is_bracket = 1;
            brackets_count++;
        }

        // increase len to store '0' character before unary - sign
        // (1st number after ( is negative)
        else if (str[i] == '-' && is_bracket)
        {
            len++;
        }
        // decrease number of brackets to check whether every ( has the matching )
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

    // ( and ) mismatch
    if (brackets_count != 0)
    {
        printf("Error: different number of ( and )\n");
        return str_wo_spaces;
    }

    // add 2 chars for 0+ at the beginning of every exppression and 1 char for \0
    if (!(str_wo_spaces = (char *) malloc(sizeof(char)*(len + 3))))
    {
        printf("Memory allocation error\n");
        return str_wo_spaces;
    }

    // initialize memory with 0s
    memset(str_wo_spaces, 0, len + 3);

    str_wo_spaces[0] = '0';
    str_wo_spaces[1] = '+';
    int j = 1;
    is_bracket = 0;
    i = 0;

    if (str[0] == ' ' || str[0] == '-')
    {
        // skip spaces
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
            // expr starts from - and there is a digit or ( after -
            str_wo_spaces[j] = '-';
        }
    }

    // str contains \n, because getline is used for getting string
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

        // skip spaces
        while (str[i] == ' ')
        {
            i++;
        }

        // current character is operator
        if (is_curr_op)
        {
            if (!is_digit(str_wo_spaces[j]) && (str[i] == '-' && str_wo_spaces[j] != '('))
            {
                free(str_wo_spaces);
                str_wo_spaces = 0;
                printf("Error: there is no number before operator or ( before unary -\n");
                return str_wo_spaces;
            }
            // add 0 before unary -
            if (str[i]  == '-' && str_wo_spaces[j] == '(')
            {
                str_wo_spaces[++j] = '0';
            }
        }

        // current character is digit
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
        // current character is (
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
        // current character is )
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
        // add character to output string
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
    // return constructed string
	return str_wo_spaces;
}

// Function to find operator's precedence
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

// Function to perform arithmetic operations
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

// Function to check whether a character is operator or not
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

// Function to check whether a character is numeric or not
int is_digit (const char c)
{
    return (c >= '0' && c <= '9');
}

// For expression calculation algorithm from
// https://www.geeksforgeeks.org/expression-evaluation/ page is used
