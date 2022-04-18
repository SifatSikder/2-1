#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct variable_info
{
    char variable_name;
    char **variable_transitions;
    int number_of_transition;
} variable_info;
variable_info b[100];

int number_of_non_terminal;
int number_of_terminal=0;
char terminal[100];

char **substring(char *string, int starting_index)
{

    int size = strlen(string);
    char **result = (char **)malloc(size * sizeof(char *));
    for (int i = 0; i < size; i++)
        result[i] = (char *)malloc(size * sizeof(char));

    for (int i = starting_index, j = 0, k = 0; i < size; i++)
    {
        if (string[i] != '|')
        {
            result[j][k++] = string[i];
        }
        else
        {
            result[j][k] = '\0';
            k = 0;
            j++;
        }
    }

    return result;
}

int number_of_or(char *string)
{
    int size = strlen(string);
    int count = 0;
    for (int i = 0; i < size; i++)
    {
        if (string[i] == '|')
            count++;
    }
    return count;
}

void cfg_input()
{
    char a[100] = {0};

    printf("Enter the CFG(write done when finished entering):-\n");
    int i;
    for (i = 0;; i++)
    {
        gets(a);
        if (!strcmp(a, "done"))
            break;
        b[i].variable_name = a[0];
        b[i].number_of_transition = number_of_or(a) + 1;
        b[i].variable_transitions = substring(a, 3);
    }
    number_of_non_terminal = i;
}

void cnf_output()
{
    // if (new_start.variable_name == 'Q')
    // {
    //     printf("S`-> ");
    //     for (int i = 0; i < new_start.number_of_transition; i++)
    //     {
    //         printf("%s|", new_start.variable_transitions[i]);
    //     }
    // }
    printf("\n");

    for (int i = 0; i < number_of_non_terminal; i++)
    {

        printf("%c-> ", b[i].variable_name);
        for (int j = 0; j < b[i].number_of_transition; j++)
        {
            printf("%s|", b[i].variable_transitions[j]);
        }
        printf("\n");
    }
}

int is_variable(char ch)
{
    if (ch >= 65 && ch <= 90)
        return 1;
    return 0;
}

int is_terminal(char ch)
{
    if (ch >= 65 && ch <= 90)
        return 0;
    return 1;
}

int is_duplicate(char ch)
{
    for (int i = 0; i < number_of_terminal ; i++)
    {
        if (terminal[i]==ch) return 1;
    }
    return 0; 
    
}

void find_all_terminal_variables()
{
     int index=0;
     for (int i = 0; i < number_of_non_terminal; i++)
    {

        
        for (int j = 0; j < b[i].number_of_transition; j++)
        {
            for (int k = 0; k < strlen(b[i].variable_transitions[j]); k++)
            {
                if (is_terminal(b[i].variable_transitions[j][k]))
                {
                    if (!is_duplicate(b[i].variable_transitions[j][k]))
                    {
                        terminal[index++]=b[i].variable_transitions[j][k];
                        number_of_terminal++;
                    }
                    
                }
                
            }
            
        }
       
    } 
}


int main(void)
{

    cfg_input();
    find_all_terminal_variables();
    cnf_output();

}