#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char string[100];

#define epsilon -2
#define Z0 -1
#define q0 0
#define q1 1
#define q2 2

int initial_state=q0,final_state=q2;
int current_state=q0;
int stack_symbol=Z0;



typedef struct Stack 
{ 
 int size; 
 int top; 
 int *S; 
} Stack; 

Stack st;

void create(int size) 
{ 
 
 st.size=size; 
 st.top=-1; 
 st.S=(int *)malloc(st.size*sizeof(int)); 
}

void Display() 
{ 
 int i; 
 for(i=st.top;i>=0;i--) 
 printf("%d ",st.S[i]); 
 printf("\n"); 
  
} 

void push(int x) 
{ 
 if(st.top==st.size-1) 
 printf("Stack overflow\n");  
 else 
 { 
 st.top++; 
 st.S[st.top]=x; 
 } 
 
} 

int pop() 
{ 
 int x=-1; 
  
 if(st.top==-1) 
 printf("Stack Underflow\n");  else 
 { 
 x=st.S[st.top--]; 
 } 
 return x; 
} 

int peek(int index) { 
 int x=-1; 
 if(st.top-index+1<0) 
 printf("Invalid Index \n");  x=st.S[st.top-index+1]; 
  
 return x; 
} 

int isEmpty() { 
 if(st.top==-1) 
 return 1; 
 return 0; 
} 

int isFull() 
{ 
 return st.top==st.size-1; } 

int stackTop() { 
 if(!isEmpty(st))
 return st.S[st.top];  return -1; 
} 

void string_input()
{
    printf("Enter the string you wanna check:- (must contain only 0s and 1s)");
    scanf("%s",string);
    getchar();
}

void push_transition(int input)
{
    if (current_state==q0 && input==0 && stack_symbol==Z0)
    {
        push(input);
        stack_symbol=input;
    }
    else  if (current_state==q0 && input==1 && stack_symbol==Z0)
    {
        push(input);
        stack_symbol=input;
    }
    else  if (current_state==q0 && input==0 && stack_symbol==0)
    {
        push(input);
        stack_symbol=input;
    }
      else  if (current_state==q0 && input==1 && stack_symbol==0)
    {
        push(input);
        stack_symbol=input;
    }
         else  if (current_state==q0 && input==0 && stack_symbol==1)
    {
        push(input);
        stack_symbol=input;
    }
         else  if (current_state==q0 && input==1 && stack_symbol==1)
    {
        push(input);
        stack_symbol=input;
    }
}

void state_transition()
{
     if (current_state==q0 && (stack_symbol==Z0||stack_symbol==0||stack_symbol==1))
    {
        current_state=q1;
    }
}

int pop_transition(int input)
{

        if (current_state==q1 && input==0 && stack_symbol==0)
    {
        pop(input);
        stack_symbol=stackTop();
        return 1;
    }
    else if (current_state==q1 && input==1 && stack_symbol==1)
    {
        pop(input);
        stack_symbol=stackTop();
        return 1;
    }
    return 0;


}

void final_transition()
{
  if (current_state==q1 && stack_symbol==Z0)
    {
        current_state=q2;
    }
}

int is_even_palindrome()
{ 
    create(strlen(string));
    push(Z0);
    int palindrome=0;
    for (int i = 1; i < strlen(string); i++)
    {
        
        current_state=q0;
        st.top=0;
        for (int j = 0; j < i; j++)
        {
            push_transition(string[j]-48);
        }
        state_transition();
        for (int j = i; j < strlen(string); j++)
        {
            
            int c=pop_transition(string[j]-48);
            if (c==0)
            {
                palindrome=c;
                break;
            }
            palindrome=c;

        }
        if (palindrome)
        {
            final_transition();
        }
        
         if (current_state==q2 && palindrome==1)break;
    }
    if (palindrome==1 && current_state==q2 && stack_symbol==Z0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int main(void)
{

  string_input();
  if (is_even_palindrome())
  {
      printf("acc\n");
  }
  else
  {
      printf("rej\n");
  }
}
