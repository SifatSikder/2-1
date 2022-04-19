#include <stdio.h>
int mystrlen (char a[]){
    int i;
    for(i=1;;i++)
    {
        if(a[i]=='\0')break;
    }
    return i;
}
int main (){
    char A[100];
    printf("Enter a String:- ");
    gets(A);
    int l=mystrlen(A);
    int q0=0,q1=1,q2=2;
    int presentState=q0;
    int start=1;
    for (int i = 0; i < l; i++)
    {
        if (A[i]=='0'&& presentState==q0)
        {
            if (start)
            {
                printf("A->B");
                presentState=q1;
                start=0;
            }
            else
            {
                printf("->B");
                presentState=q1;
            }
            
            
        }
        else if(A[i]=='1'&& presentState==q0)
        {
            if (start)
            {
                printf("A->A");
                presentState=q0;
                start=0;
            }
            else
            {
                printf("->A");
                presentState=q0;
            }
        }
        else if (A[i]=='0'&& presentState==q1){
                printf("->C");
                presentState=q2;
        }
        else if(A[i]=='1'&& presentState==q1){
                printf("->A");
                presentState=q0;
        }
        else if (A[i]=='0'&& presentState==q2){
            printf("->C");
                presentState=q2;
        }
        else
        {
            printf("->A");
                presentState=q0;
        }
        
    }
    if (presentState==q2)
        printf("\nString has been Accepted");
    else printf("\nString has not been Accepted");
    return 0;
}