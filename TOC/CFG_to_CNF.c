#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct variable_info
{
  char variable_name;
  char **variable_transitions;
  int number_of_transition;
  int reachable_from_S;
} variable_info;

typedef struct new_non_terminal_storage
{
  char s[3];
  char c;
} new_non_terminal_storage;

typedef struct new_terminal_storage
{
  char terminal;
  char non_terminal;
} new_terminal_storage;

new_non_terminal_storage new_storage[10];

new_terminal_storage new_ter_storage[10];

int number_of_non_terminal;
variable_info b[100];
variable_info new_start;

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
    b[i].reachable_from_S = 0;
    b[i].number_of_transition = number_of_or(a) + 1;
    b[i].variable_transitions = substring(a, 3);
  }
  number_of_non_terminal = i;
}

void cnf_output()
{
  if (new_start.variable_name == 'Q')
  {
    printf("S`-> ");
    for (int i = 0; i < new_start.number_of_transition; i++)
    {
      printf("%s|", new_start.variable_transitions[i]);
    }
  }
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

// returns 1 if found a duplicate
int duplicate_checker(char *string, char **array_of_strings, int number_of_transitions)
{
  for (int i = 0; i < number_of_transitions; i++)
  {
    if (!strcmp(array_of_strings[i], string))
      return 1;
  }
  return 0;
}

int is_variable(char ch)
{
  if (ch >= 65 && ch <= 90)
    return 1;
  return 0;
}

int is_terminal(char ch)
{
  if (ch >= 97 && ch <= 122)
    return 1;
  return 0;
}

void make_reachable(char ch)
{
  for (int i = 0; i < number_of_non_terminal; i++)
  {
    if (ch == b[i].variable_name)
    {
      b[i].reachable_from_S = 1;
    }
  }
}

void unit_production()
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
  if (new_start.variable_name == 'Q')
  {
    new_start.variable_transitions = (char **)malloc(b[0].number_of_transition * sizeof(char *));
    for (int j = 0; j < b[0].number_of_transition; j++)
      new_start.variable_transitions[j] = (char *)malloc(b[0].number_of_transition * sizeof(char));

    int i;
    for (i = 0; i < b[0].number_of_transition; i++)
    {
      strcpy(new_start.variable_transitions[i], b[0].variable_transitions[i]);
      // printf("%s",new_start.variable_transitions[i]);
    }
    new_start.number_of_transition = i;
  }
}

void conversion_to_cnf()
{
  // check weather the start symbol S appears in the right side or not
  int flag = 0;
  for (int i = 0; i < number_of_non_terminal; i++)
  {
    for (int j = 0; j < b[i].number_of_transition; j++)
    {

      for (int k = 0; k < strlen(b[i].variable_transitions[j]); k++)
      {
        if (b[i].variable_transitions[j][k] == 'S')
        {
          new_start.number_of_transition = 1;
          new_start.reachable_from_S = 1;
          new_start.variable_name = 'Q';
          new_start.variable_transitions = substring("Q->S", 3);
          flag = 1;
          break;
        }
      }
      if (flag)
        break;
    }
    if (flag)
      break;
  }

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

void remove_3_or_more_variables()
{
  char new_non_terminal = 'X';
  int l = 0;
  for (int i = 0; i < new_start.number_of_transition; i++)
  {
    if (strlen(new_start.variable_transitions[i]) > 2)
    {
      int count = strlen(new_start.variable_transitions[i]);
      for (int k = count; k > 2; k--)
      {

        char arr[2];
        arr[0] = new_start.variable_transitions[i][k - 2];
        arr[1] = new_start.variable_transitions[i][k - 1];

        int got_duplicate = 0, m;
        for (m = 0; m < 10; m++)
        {
          if (!strcmp(new_storage[m].s, arr))
          {
            got_duplicate = 1;
            break;
          }
        }

        if (got_duplicate)
        {
          new_start.variable_transitions[i][k - 2] = new_storage[m].c;
          new_start.variable_transitions[i][k - 1] = '\0';
        }
        else
        {
          new_start.variable_transitions[i][k - 2] = new_non_terminal + l;
          new_start.variable_transitions[i][k - 1] = '\0';

          strcpy(new_storage[l].s, arr);
          new_storage[l].c = new_non_terminal + l;

          b[number_of_non_terminal].variable_name = new_non_terminal + l;
          l++;
          b[number_of_non_terminal].number_of_transition = 1;

          b[number_of_non_terminal].variable_transitions = (char **)malloc(sizeof(char *));
          for (int j = 0; j < 1; j++)
            b[number_of_non_terminal].variable_transitions[j] = (char *)malloc(2 * sizeof(char));

          strcpy(b[number_of_non_terminal].variable_transitions[0], arr);
          number_of_non_terminal++;
        }
      }
    }
  }

  for (int i = 0; i < number_of_non_terminal; i++)
  {
    for (int j = 0; j < b[i].number_of_transition; j++)
    {
      if (strlen(b[i].variable_transitions[j]) > 2)
      {
        int count = strlen(b[i].variable_transitions[j]);
        for (int k = count; k > 2; k--)
        {
          char arr[3];
          arr[0] = b[i].variable_transitions[j][k - 2];
          arr[1] = b[i].variable_transitions[j][k - 1];

          int got_duplicate = 0, m;
          for (m = 0; m < 10; m++)
          {
            if (!strcmp(new_storage[m].s, arr))
            {
              got_duplicate = 1;
              break;
            }
          }

          if (got_duplicate)
          {
            b[i].variable_transitions[j][k - 2] = new_storage[m].c;
            b[i].variable_transitions[j][k - 1] = '\0';
          }

          else
          {
            b[i].variable_transitions[j][k - 2] = new_non_terminal + l;
            b[i].variable_transitions[j][k - 1] = '\0';

            strcpy(new_storage[l].s, arr);
            new_storage[l].c = new_non_terminal + l;

            b[number_of_non_terminal].variable_name = new_non_terminal + l;
            l++;
            b[number_of_non_terminal].number_of_transition = 1;

            b[number_of_non_terminal].variable_transitions = (char **)malloc(sizeof(char *));
            for (int j = 0; j < 1; j++)
              b[number_of_non_terminal].variable_transitions[j] = (char *)malloc(2 * sizeof(char));

            strcpy(b[number_of_non_terminal].variable_transitions[0], arr);
            number_of_non_terminal++;
          }
        }
      }
    }
  }
}

void remove_mixed_symbols()
{
  char new_non_terminal = 'K';
  int l = 0;
  int q = 0;
  for (int i = 0; i < new_start.number_of_transition; i++)
  {
    if (strlen(new_start.variable_transitions[i]) == 2 && (is_terminal(new_start.variable_transitions[i][0]) || is_terminal(new_start.variable_transitions[i][1])))
    {
      if (is_terminal(new_start.variable_transitions[i][0]))
      {

        int got_duplicate = 0, m;
        for (m = 0; m < 10; m++)
        {
          if (new_ter_storage[m].terminal == new_start.variable_transitions[i][0])
          {
            got_duplicate = 1;
            break;
          }
        }

        if (got_duplicate)
        {

          new_start.variable_transitions[i][0] = new_ter_storage[m].non_terminal;
        }
        else
        {

          new_ter_storage[q].terminal = new_start.variable_transitions[i][0];
          new_ter_storage[q].non_terminal = new_non_terminal + l;
          new_start.variable_transitions[i][0] = new_non_terminal + l;

          b[number_of_non_terminal].variable_name = new_non_terminal + l;
          l++;
          b[number_of_non_terminal].number_of_transition = 1;

          b[number_of_non_terminal].variable_transitions = (char **)malloc(sizeof(char *));
          for (int j = 0; j < 1; j++)
            b[number_of_non_terminal].variable_transitions[j] = (char *)malloc(2 * sizeof(char));

          b[number_of_non_terminal].variable_transitions[0][0] = new_ter_storage[q].terminal;
          number_of_non_terminal++;
          q++;
        }
      }
      else
      {
        int got_duplicate = 0, m;
        for (m = 0; m < 10; m++)
        {
          if (new_ter_storage[m].terminal == new_start.variable_transitions[i][1])
          {
            got_duplicate = 1;
            break;
          }
        }

        if (got_duplicate)
        {

          new_start.variable_transitions[i][1] = new_ter_storage[m].non_terminal;
        }
        else
        {

          new_ter_storage[q].terminal = new_start.variable_transitions[i][1];
          new_ter_storage[q].non_terminal = new_non_terminal + l;
          new_start.variable_transitions[i][1] = new_non_terminal + l;

          b[number_of_non_terminal].variable_name = new_non_terminal + l;
          l++;
          b[number_of_non_terminal].number_of_transition = 1;

          b[number_of_non_terminal].variable_transitions = (char **)malloc(sizeof(char *));
          for (int j = 0; j < 1; j++)
            b[number_of_non_terminal].variable_transitions[j] = (char *)malloc(2 * sizeof(char));

          b[number_of_non_terminal].variable_transitions[0][0] = new_ter_storage[q].terminal;
          number_of_non_terminal++;
          q++;
        }
      }
    }
  }

  for (int i = 0; i < number_of_non_terminal; i++)
  {
    for (int j = 0; j < b[i].number_of_transition; j++)
    {
      if (strlen(b[i].variable_transitions[j]) == 2 && (is_terminal(b[i].variable_transitions[j][0]) || is_terminal(b[i].variable_transitions[j][1])))
      {
        if (is_terminal(b[i].variable_transitions[j][0]))
        {

          int got_duplicate = 0, m;
          for (m = 0; m < 10; m++)
          {
            if (new_ter_storage[m].terminal == b[i].variable_transitions[j][0])
            {
              got_duplicate = 1;
              break;
            }
          }

          if (got_duplicate)
          {

            b[i].variable_transitions[j][0] = new_ter_storage[m].non_terminal;
          }
          else
          {

            new_ter_storage[q].terminal = b[i].variable_transitions[j][0];
            new_ter_storage[q].non_terminal = new_non_terminal + l;
            b[i].variable_transitions[j][0] = new_non_terminal + l;

            b[number_of_non_terminal].variable_name = new_non_terminal + l;
            l++;
            b[number_of_non_terminal].number_of_transition = 1;

            b[number_of_non_terminal].variable_transitions = (char **)malloc(sizeof(char *));
            for (int j = 0; j < 1; j++)
              b[number_of_non_terminal].variable_transitions[j] = (char *)malloc(2 * sizeof(char));

            b[number_of_non_terminal].variable_transitions[0][0] = new_ter_storage[q].terminal;
            number_of_non_terminal++;
            q++;
          }
        }
        else
        {
          int got_duplicate = 0, m;
          for (m = 0; m < 10; m++)
          {
            if (new_ter_storage[m].terminal == b[i].variable_transitions[j][1])
            {
              got_duplicate = 1;
              break;
            }
          }

          if (got_duplicate)
          {

            b[i].variable_transitions[j][1] = new_ter_storage[m].non_terminal;
          }
          else
          {

            new_ter_storage[q].terminal = b[i].variable_transitions[j][1];
            new_ter_storage[q].non_terminal = new_non_terminal + l;
            b[i].variable_transitions[j][1] = new_non_terminal + l;

            b[number_of_non_terminal].variable_name = new_non_terminal + l;
            l++;
            b[number_of_non_terminal].number_of_transition = 1;

            b[number_of_non_terminal].variable_transitions = (char **)malloc(sizeof(char *));
            for (int j = 0; j < 1; j++)
              b[number_of_non_terminal].variable_transitions[j] = (char *)malloc(2 * sizeof(char));

            b[number_of_non_terminal].variable_transitions[0][0] = new_ter_storage[q].terminal;
            number_of_non_terminal++;
            q++;
          }
        }
      }
    }
  }
}
int main(void)
{
  cfg_input();
  conversion_to_cnf();
  unit_production();
  remove_3_or_more_variables();
  remove_mixed_symbols();
  cnf_output();

  return 0;
}