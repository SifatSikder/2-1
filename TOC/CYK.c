#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define OLD_VARIABLE 1
#define NEW_VARIABLE 2

typedef struct
{
    char *members; // pointer to dynamically allocated array of the set members
    int length;    // the number of members of the set
} Set;

typedef struct variable_info
{
    char variable_name;
    char **variable_transitions;
    int number_of_transition;
    int old_or_new_variable;
    int generating;
    int reachable_from_start;
} variable_info;
variable_info b[100];

int number_of_non_terminal;
int number_of_terminal = 0;
char terminal[100];
Set *s[100];
char start_variable;
char string[100];

// returns a pointer to a new Set allocated on the heap
Set *init()
{
    // allocate space for the set
    Set *new_set = malloc(sizeof(Set));

    // initially the set will be empty with no members, so set length to 0
    new_set->length = 0;

    // allocate enough space to store 1 member, we'll expand this as needed
    new_set->members = malloc(sizeof(char));

    // return the new Set (or more specifically, a pointer to it)
    return new_set;
}

bool is_member(Set *set, char value)
{
    // if we can find the value in the set's members, it is in the set
    for (int i = 0; i < set->length; i++)
        if (set->members[i] == value)
            return true;

    // if after checking all the set's members we can't find the value, it is
    // not a member of the set
    return false;
}

void insert(Set *set, char member)
{
    // check to make the member value is not in the set already
    if (!is_member(set, member))
    {
        // allocate space to store the *new* amount of members in the set
        set->members =
            realloc(set->members, sizeof(int) * (set->length + 1));

        // put the member into the set at the next available index
        set->members[set->length] = member;

        // increment the set length to acknowledge the new length of the set
        set->length = set->length + 1;
    }
}

int is_valid_transition(char c1, char c2)
{
    for (int i = 0; i < number_of_non_terminal; i++)
    {
        for (int j = 0; j < b[i].number_of_transition; j++)
        {
            if (strlen(b[i].variable_transitions[j]) == 2 && b[i].variable_transitions[j][0] == c1 && b[i].variable_transitions[j][1] == c2)
            {
                return 1;
            }
        }
    }
    return 0;
}

void variable_of_a_valid_transition(char c1, char c2, Set *set)
{
    for (int i = 0; i < number_of_non_terminal; i++)
    {
        for (int j = 0; j < b[i].number_of_transition; j++)
        {
            if (strlen(b[i].variable_transitions[j]) == 2 && b[i].variable_transitions[j][0] == c1 && b[i].variable_transitions[j][1] == c2)
            {
                insert(set, b[i].variable_name);
            }
        }
    }
}

void cartesian(Set *A, Set *B, Set *set)
{
    for (int i = 0; i < A->length; i++)
    {
        for (int j = 0; j < B->length; j++)
        {
            if (is_valid_transition(A->members[i], B->members[j]))
            {
                variable_of_a_valid_transition(A->members[i], B->members[j], set);
            }
        }
    }
}

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

    printf("Enter the start symbol of CFG :- ");
    scanf("%c", &start_variable);
    getchar();

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
        b[i].old_or_new_variable = OLD_VARIABLE;
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

int is_terminal(char ch)
{
    if (ch >= 65 && ch <= 90)
        return 0;
    return 1;
}

int is_duplicate(char ch)
{
    for (int i = 0; i < number_of_terminal; i++)
    {
        if (terminal[i] == ch)
            return 1;
    }
    return 0;
}

void find_all_terminal_variables()
{
    int index = 0;
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
                        terminal[index++] = b[i].variable_transitions[j][k];
                        number_of_terminal++;
                    }
                }
            }
        }
    }
}

void make_set()
{
    for (int k = 0; k < number_of_terminal; k++)
    {
        s[k] = init();
        for (int i = 0; i < number_of_non_terminal; i++)
        {
            for (int j = 0; j < b[i].number_of_transition; j++)
            {
                if (strlen(b[i].variable_transitions[j]) == 1 && b[i].variable_transitions[j][0] == terminal[k])
                {
                    insert(s[k], b[i].variable_name);
                }
            }
        }
    }
}

void string_to_check_input()
{
    printf("Enter the string you want to test the membership of\n");
    gets(string);
}

int is_present_start_variable(Set *set)
{
    for (int i = 0; i < set->length; i++)
    {
        if (set->members[i] == start_variable)
        {
            return 1;
        }
    }
    return 0;
}

Set *get_set_of_a_terminal(char ch)
{
    for (int i = 0; i < number_of_terminal; i++)
    {
        if (terminal[i] == ch)
        {
            return s[i];
        }
    }
}

int is_existing_terminal(char ch)
{
    for (int i = 0; i < number_of_terminal; i++)
    {
        if (terminal[i] == ch)
            return 1;
    }
    return 0;
}

int CYK()
{

    int size = strlen(string);
    for (int i = 0; i < size; i++)
    {
        if (!is_existing_terminal(string[i]))
        {
            printf("\n%s is a not member of the given CNF\n", string);
            return 0;
        }
    }

    Set *dp_array[size + 1][size + 1];

    for (int i = 1; i <= size; i++)
    {
        dp_array[i][i] = get_set_of_a_terminal(string[i - 1]);
    }

    for (int d = 1; d < size; d++)
    {
        for (int i = 1; i <= size - d; i++)
        {
            int j = i + d;
            Set *set = init();
            for (int k = i; k < j; k++)
            {

                cartesian(dp_array[i][k], dp_array[k + 1][j], set);
            }
            dp_array[i][j] = set;
        }
    }

    if (is_present_start_variable(dp_array[1][size]))
    {
        printf("\n%s is a member of the given CNF\n", string);
        return 1;
    }
    else
    {
        printf("\n%s is a not member of the given CNF\n", string);
        return 0;
    }
}

int main(void)
{
    cfg_input();
    find_all_terminal_variables();
    make_set();
    string_to_check_input();
    CYK();
}