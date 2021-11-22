#include "math_expr_parser_postfix.h"

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
