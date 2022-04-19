#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct variable_info
{
    char variable_name;
    char *variable_transitions;
    int transition_length;
} variable_info;
variable_info b[100];

int number_of_non_terminal;
char text[1000];

char *substring(char *string, int starting_index, int length)
{

    int size = strlen(string);
    char *result = (char *)malloc(size);

    for (int i = starting_index, j = 0; i < starting_index + length; i++, j++)
        result[j] = string[i];
    return result;
}

void naive_string_matcher()
{

    while (1)
    {
        int n = strlen(text);
        int need_to_check_next = 0;
        for (int i = 0; i <= n - 3; i++)
        {

            int l;
            int need_to_shift = 0;
            for (l = 0; l < number_of_non_terminal; l++)
            {

                char *store = substring(text, i, b[l].transition_length);

                if (!strcmp(b[l].variable_transitions, store))

                {
                    need_to_check_next = 1;
                    need_to_shift = 1;
                    break;
                }
            }

            if (need_to_shift)
            {
                text[i] = b[l].variable_name;

                int j;
                for (j = i + b[l].transition_length; j < n; j++)
                {
                    text[j - b[l].transition_length + 1] = text[j];
                }
                text[j - b[l].transition_length + 1] = '\0';
            }
        }

        if (strlen(text) == 1)
        {
            printf("Accepted\n");
            break;
        }
        if (strlen(text) != 1 && need_to_check_next == 0)
        {
            printf("rejected\n");
            break;
        }
    }
}

void cfg_input()
{
    printf("Enter the text:-\n");
    gets(text);

    char a[100];
    int i;
    for (i = 0;; i++)
    {
        gets(a);
        if (!strcmp(a, "done"))
            break;

        b[i].variable_name = a[0];
        b[i].transition_length = strlen(a) - 3;
        b[i].variable_transitions = substring(a, 3, b[i].transition_length);
    }
    number_of_non_terminal = i;
}

int main(void)
{
    cfg_input();
    naive_string_matcher();

    return 0;
}