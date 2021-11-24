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

            printf("\nEnter the math expression or q to exit: ");
            if (-1 != getline(&str, &sz, stdin))
            {
                if (*str == 'q')
                {
                    free(str);
                    str = 0;
                    return 0;
                }
                char *str_wo_spaces = 0;

                // check whether string is correct or not, add 0-
                // for unary -, remove spaces and get the chnaged
                // string or 0 if something was wrong
                str_wo_spaces = get_str_wo_spaces(str);
                free(str);
                str = 0;

                if(str_wo_spaces)
                {
                    printf ("Result: %.2f\n",calc(str_wo_spaces));
                    free(str_wo_spaces);
                    str_wo_spaces = 0;
                }
            }
        }
	}
    return 0;
}
