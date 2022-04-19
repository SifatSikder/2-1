#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define OLD_VARIABLE 1
#define NEW_VARIABLE 2

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
char start_variable;
char non_generating[10];

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
    scanf("%c",&start_variable);
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

int is_existing_nonterminal(char ch)
{
	for (int i = 0; i < number_of_non_terminal; i++)
	{
		if (b[i].variable_name == ch)
			return 1;
	}
	return 0;
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

char get_new_non_terminal()
{
	char ch = 'A';

	while (1)
	{

		if (!is_existing_nonterminal(ch))
		{
			return ch;
		}
		ch++;
	}
}

void introduce_new_transition()
{
	char ch = 'A';
	for (int i = 0; i < number_of_terminal;)
	{

		if (!is_existing_nonterminal(ch))
		{
			b[number_of_non_terminal].variable_name = ch;
			b[number_of_non_terminal].number_of_transition = 1;
			b[number_of_non_terminal].old_or_new_variable = NEW_VARIABLE;

			b[number_of_non_terminal].variable_transitions = (char **)malloc(sizeof(char *));
			for (int j = 0; j < 1; j++)
				b[number_of_non_terminal].variable_transitions[j] = (char *)malloc(sizeof(char));
			b[number_of_non_terminal++].variable_transitions[0][0] = terminal[i];
			ch++;
			i++;
		}
		else
		{
			while (1)
			{
				ch++;
				if (!is_existing_nonterminal(ch))
					break;
			}
		}
	}
}

char get_non_terminal_of_a_terminal(char ch)
{
	for (int i = 0; i < number_of_non_terminal; i++)
	{

		for (int j = 0; j < b[i].number_of_transition; j++)
		{

			if ((b[i].number_of_transition == 1) && strlen(b[i].variable_transitions[j]) == 1 && b[i].variable_transitions[j][0] == ch)
				return b[i].variable_name;
		}
	}
}

void replace_new_variable()
{
	for (int i = 0; i < number_of_non_terminal; i++)
	{

		if (b[i].old_or_new_variable == OLD_VARIABLE)
		{
			for (int j = 0; j < b[i].number_of_transition; j++)
			{
				if (!(strlen(b[i].variable_transitions[j]) == 1))
				{
					for (int k = 0; k < strlen(b[i].variable_transitions[j]); k++)
					{
						if (is_existing_terminal(b[i].variable_transitions[j][k]))
						{
							b[i].variable_transitions[j][k] = get_non_terminal_of_a_terminal(b[i].variable_transitions[j][k]);
						}
					}
				}
			}
		}
	}
}

int is_existing_only_2_length_variable(char *arr)
{
	for (int i = 0; i < number_of_non_terminal; i++)
	{

		if (b[i].old_or_new_variable == NEW_VARIABLE && b[i].number_of_transition == 1 && (strlen(b[i].variable_transitions[0]) == 2))
		{
			if (!strcmp(arr, b[i].variable_transitions[0]))
				return 1;
		}
	}
	return 0;
}

void remove_3_or_more_variables()
{
	for (int i = 0; i < number_of_non_terminal; i++)
	{

		if (b[i].old_or_new_variable == OLD_VARIABLE)
		{
			for (int j = 0; j < b[i].number_of_transition; j++)
			{
				if ((strlen(b[i].variable_transitions[j]) > 2))
				{
					int string_length = strlen(b[i].variable_transitions[j]);

					for (int k = string_length; k > 2; k--)
					{

						char arr[3];
						arr[0] = b[i].variable_transitions[j][k - 2];
						arr[1] = b[i].variable_transitions[j][k - 1];
						arr[2] = '\0';

						if (is_existing_only_2_length_variable(arr))
						{
							for (int l = 0; l < number_of_non_terminal; l++)
							{

								if (b[l].old_or_new_variable == NEW_VARIABLE && b[l].number_of_transition == 1 && (strlen(b[l].variable_transitions[0]) == 2))
								{
									if (!strcmp(arr, b[l].variable_transitions[0]))
									{
										b[i].variable_transitions[j][k - 2] = b[l].variable_name;
										b[i].variable_transitions[j][k - 1] = '\0';
									}
								}
							}
						}
						else
						{
							char new_non_terminal = get_new_non_terminal();

							b[i].variable_transitions[j][k - 2] = new_non_terminal;
							b[i].variable_transitions[j][k - 1] = '\0';

							b[number_of_non_terminal].variable_name = new_non_terminal;
							b[number_of_non_terminal].number_of_transition = 1;
							b[number_of_non_terminal].old_or_new_variable = NEW_VARIABLE;

							b[number_of_non_terminal].variable_transitions = (char **)malloc(sizeof(char *));
							for (int j = 0; j < 1; j++)
								b[number_of_non_terminal].variable_transitions[j] = (char *)malloc(2 * sizeof(char));
							strcpy(b[number_of_non_terminal++].variable_transitions[0], arr);
						}
					}
				}
			}
		}
	}
}

int find_a_char_in_string(char *string, char find)
{
	int found = 0;
	int size = strlen(string);
	for (int i = 0; i < size; i++)
	{
		if (string[i] == find)
		{
			found = 1;
			break;
		}
	}
	return found;
}

char *possible_strings_without_an_index(char *string, int index)
{
	int size = strlen(string);
	char *result = (char *)malloc(size);

	for (int i = 0, j = 0; i < size; i++)
	{
		if (i != index)
		{
			result[j++] = string[i];
		}
	}
	return result;
}

int duplicate_checker(char *string, char **array_of_strings, int number_of_transitions)
{
	for (int i = 0; i < number_of_transitions; i++)
	{
		if (!strcmp(array_of_strings[i], string))
			return 1;
	}
	return 0;
}

void remove_all_null_production()
{
	// check weather the start symbol S appears in the right side or not
	// int flag = 0;
	// for (int i = 0; i < number_of_non_terminal; i++)
	// {
	//   for (int j = 0; j < b[i].number_of_transition; j++)
	//   {

	//     for (int k = 0; k < strlen(b[i].variable_transitions[j]); k++)
	//     {
	//       if (b[i].variable_transitions[j][k] == 'S')
	//       {
	//         new_start.number_of_transition = 1;
	//         new_start.reachable_from_S = 1;
	//         new_start.variable_name = 'Q';
	//         new_start.variable_transitions = substring("Q->S", 3);
	//         flag = 1;
	//         break;
	//       }
	//     }
	//     if (flag)
	//       break;
	//   }
	//   if (flag)
	//     break;
	// }

	// NOW ITS TIME TO REMOVE THE NULL PRODUCTIONS
	while (1)
	{
		// first find a target variable which provides a null production
		char target_variable;
		int got_target = 0;
		for (int i = 0; i < number_of_non_terminal; i++)
		{
			for (int j = 0; j < b[i].number_of_transition; j++)
			{
				if (!strcmp("NULL", b[i].variable_transitions[j]))
				{
					target_variable = b[i].variable_name;
					got_target = 1;
					b[i].number_of_transition--;
					break;
				}
			}
			if (got_target)
				break;
		}

		// Now remove that variable

		if (got_target)
		{
			for (int i = 0; i < number_of_non_terminal; i++)
			{
				for (int j = 0; j < b[i].number_of_transition; j++)
				{

					if (find_a_char_in_string(b[i].variable_transitions[j], target_variable))
					{
						if (strlen(b[i].variable_transitions[j]) == 1)
						{
							strcpy(b[i].variable_transitions[b[i].number_of_transition++], "NULL");
						}
						else
						{

							for (int k = 0; k < strlen(b[i].variable_transitions[j]); k++)
							{
								if (b[i].variable_transitions[j][k] == target_variable)
								{
									char *store = possible_strings_without_an_index(b[i].variable_transitions[j], k);
									if (!duplicate_checker(store, b[i].variable_transitions, b[i].number_of_transition))
									{
										b[i].variable_transitions[b[i].number_of_transition++] = store;
									}
								}
							}
						}
					}
				}
			}
		}
		else
			break;
	}
	// Null productions removed successfully now its time to remove unit productions
}

void remove_unit_production()
{

	// S->S type productions removed
	while (1)
	{
		int same_unit_variable = 0;
		for (int i = 0; i < number_of_non_terminal; i++)
		{
			for (int j = 0; j < b[i].number_of_transition; j++)
			{

				if (strlen(b[i].variable_transitions[j]) == 1 && is_variable(b[i].variable_transitions[j][0]) && b[i].variable_transitions[j][0] == b[i].variable_name)
				{
					same_unit_variable = 1;
					char ch = b[i].variable_transitions[j][0];
					for (int k = j + 1; k < b[i].number_of_transition; k++)
					{
						strcpy(b[i].variable_transitions[k - 1], b[i].variable_transitions[k]);
					}
					b[i].number_of_transition--;
				}
			}
		}
		if (!same_unit_variable)
			break;
	}

	// every unit prouction removed
	while (1)
	{
		int got_unit_variable = 0;
		for (int i = 0; i < number_of_non_terminal; i++)
		{
			for (int j = 0; j < b[i].number_of_transition; j++)
			{

				if (strlen(b[i].variable_transitions[j]) == 1 && is_variable(b[i].variable_transitions[j][0]))
				{
					got_unit_variable = 1;
					char ch = b[i].variable_transitions[j][0];
					for (int k = 0; k < number_of_non_terminal; k++)
					{
						if (b[k].variable_name == ch)
						{
							for (int l = 0; l < b[k].number_of_transition; l++)
							{

								if (l == 0)
								{
									strcpy(b[i].variable_transitions[j], b[k].variable_transitions[l]);
								}
								else
								{
									// b[i].variable_transitions=(char **)malloc(sizeof(char *));
									b[i].variable_transitions[b[i].number_of_transition] = (char *)malloc(strlen(b[k].variable_transitions[l]) * sizeof(char));
									strcpy(b[i].variable_transitions[b[i].number_of_transition++], b[k].variable_transitions[l]);
								}
							}
						}
					}
				}
			}
		}
		if (!got_unit_variable)
			break;
	}

	// new start variable unit production removed
	// if (new_start.variable_name == 'Q')
	// {
	//   new_start.variable_transitions = (char **)malloc(b[0].number_of_transition * sizeof(char *));
	//   for (int j = 0; j < b[0].number_of_transition; j++)
	//     new_start.variable_transitions[j] = (char *)malloc(b[0].number_of_transition * sizeof(char));

	//   int i;
	//   for (i = 0; i < b[0].number_of_transition; i++)
	//   {
	//     strcpy(new_start.variable_transitions[i], b[0].variable_transitions[i]);
	//     // printf("%s",new_start.variable_transitions[i]);
	//   }
	//   new_start.number_of_transition = i;
	// }
}

int is_generating(char ch)
{
	for (int i = 0; i < number_of_non_terminal; i++)
	{
		if (b[i].variable_name==ch && b[i].generating==1)
		{
			return 1;
		}
		
	}
	return 0; 
	
}

void variable_delete(char ch)
{
    for (int i = 0; i < number_of_non_terminal; i++)
    {
        if (ch==b[i].variable_name)
        {
            for (int j = i+1; j < number_of_non_terminal; j++)
            {
                b[j-1].generating=b[j].generating;
                b[j-1].number_of_transition=b[j].number_of_transition;
                b[j-1].old_or_new_variable=b[j].old_or_new_variable;
                b[j-1].reachable_from_start=b[j].reachable_from_start;
                b[j-1].variable_name=b[j].variable_name;
                

                b[j-1].variable_transitions = (char **)malloc(b[j].number_of_transition*sizeof(char *));
			for (int q = 0; q < b[j].number_of_transition; q++)
				b[j-1].variable_transitions[q] = (char *)malloc(strlen(b[j].variable_transitions[q])*sizeof(char));

                for (int x = 0; x < b[j].number_of_transition; x++)
                {
                    strcpy(b[j-1].variable_transitions[x],b[j].variable_transitions[x]);
                }
                

            }
            number_of_non_terminal--;
            
        }
        
    }
    
}

void delete(char ch)
{
    for (int i = 0; i < number_of_non_terminal; i++)
	{
		
		if (b[i].variable_name==ch)
		{
			variable_delete(b[i].variable_name);
		}
		else
		{
			for (int j = 0; j < b[i].number_of_transition; j++)
			{
				int need_to_delete=0;
                for (int k = 0; k < strlen(b[i].variable_transitions[j]); k++)
				{
                    if (b[i].variable_transitions[j][k]==ch)
                    {
                        need_to_delete=1;
                        break;
                    }
                }
                if (need_to_delete)
                {
                   for (int l = j+1; l < b[i].number_of_transition; l++)
                   {
                       strcpy(b[i].variable_transitions[l-1],b[i].variable_transitions[l]);
                   }
                   b[i].number_of_transition--;
                    
                }
                
            }
		}
		
			
    }
	
}

void make_reachable(char ch)
{
    
    for (int i = 0; i < number_of_non_terminal; i++)
	{
        if (b[i].variable_name==ch)
        {
            b[i].reachable_from_start=1;
            break;
        }
       
        
    }

}

int is_reachable(char ch)
{
    for (int i = 0; i < number_of_non_terminal; i++)
    {
        if (b[i].variable_name==ch && b[i].reachable_from_start==1 )
        {
            return 1;
        }
        
    }
    return 0;
    

}



int already_stored(char ch)
{
	for (int i = 0; i < 10; i++)
	{
		if (non_generating[i]==ch)
		{
			return 1;
		}
		
	}
	return 0;
	
}

void remove_all_useless_symbols()
{
	
	int index=0;
	// delete all non-generating function

	// base case of finding the generating variables
	for (int i = 0; i < number_of_non_terminal; i++)
	{

		for (int j = 0; j < b[i].number_of_transition; j++)
		{
			if ((strlen(b[i].variable_transitions[j]) == 1) && is_terminal(b[i].variable_transitions[j][0]))
			{
				b[i].generating = 1;
				break;
			}
		}
	}

	for (int i = 0; i < number_of_non_terminal; i++)
	{
		int flag = 0;
		if (!is_generating(b[i].variable_name))
		{
			for (int j = 0; j < b[i].number_of_transition; j++)
			{
				for (int k = 0; k < strlen(b[i].variable_transitions[j]); k++)
				{
					if (is_existing_nonterminal(b[i].variable_transitions[j][k]) && is_generating(b[i].variable_transitions[j][k]))
					{
						b[i].generating = 1;
						flag = 1;
						break;
					}
				}

				if (flag)
					break;
			}
		}
	}

	for (int i = 0; i < number_of_non_terminal; i++)
	{
			for (int j = 0; j < b[i].number_of_transition; j++)
			{
				for (int k = 0; k < strlen(b[i].variable_transitions[j]); k++)
				{
					if (is_variable(b[i].variable_transitions[j][k]) && (!is_generating(b[i].variable_transitions[j][k])) && (!already_stored(b[i].variable_transitions[j][k])))
					{
						non_generating[index++]=b[i].variable_transitions[j][k];
					}
				}
			}
	}
	
	//delete all rhs of transitions of non generating
	for (int i = 0; i < index; i++)
	{
		delete(non_generating[i]);
	}


	

	// finding all variable which is reachable from start
    for (int i = 0; i < number_of_non_terminal; i++)
    {
        if (b[i].variable_name==start_variable)
        {
            b[i].reachable_from_start=1;
            for (int j = 0; j < b[i].number_of_transition; j++)
            {
                for (int k = 0; k < strlen(b[i].variable_transitions[j]); k++)
                {
                    
                    if (is_existing_nonterminal(b[i].variable_transitions[j][k]))
                    {
                       make_reachable(b[i].variable_transitions[j][k]);
                    }
                    
                }
                
               
            }
            
        }
        

        else if(b[i].reachable_from_start==1)
        {
            for (int j = 0; j < b[i].number_of_transition; j++)
            {
                for (int k = 0; k < strlen(b[i].variable_transitions[j]); k++)
                {
                    
                    if (is_existing_nonterminal(b[i].variable_transitions[j][k]))
                    {
                       make_reachable(b[i].variable_transitions[j][k]);
                    }
                    
                }
                
               
            }
        }
        
    }

    //deleting all unreachable variables
    for (int i = 0; i < number_of_non_terminal; i++)
    {
        if (b[i].reachable_from_start==0)
        {
            variable_delete(b[i].variable_name);
        }
        
    }
    
    
    
}

int main(void)
{

	cfg_input();
	remove_all_null_production();
	remove_unit_production();
	remove_all_useless_symbols();
	find_all_terminal_variables();
	introduce_new_transition();
	replace_new_variable();
	remove_3_or_more_variables();
	cnf_output();
}
