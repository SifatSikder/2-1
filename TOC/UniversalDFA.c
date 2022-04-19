#include<stdio.h>
#include<stdlib.h>
int mystrlen (char a[]){
    int i;
    for(i=1;;i++)
    {
        if(a[i]=='\0')break;
    }
    return i;
}

int main() {
  int num_of_states, num_of_inputs;
  scanf("%d %d",&num_of_states,&num_of_inputs);
  
  int start_state, end_state;
  scanf("%d %d",&start_state,&end_state);
  char s=start_state+65;
  int* transition_table =(int *)malloc ((num_of_states*num_of_inputs)*sizeof(int));
//   printf("%d %d",num_of_states,num_of_inputs);
 for(int i=0;i<num_of_states;i++){
     for(int j=0;j<num_of_inputs;j++){
         
          scanf("%d",&transition_table[i*num_of_inputs+j]);
     }
 }
 
// //printing the transition table  
//   for(int i=0;i<num_of_states;i++){
//      for(int j=0;j<num_of_inputs;j++){
//           printf("%d ",transition_table[i*num_of_inputs+j]);
//      }
//      printf("\n");
//  }
getchar();
char A[100];
    printf("Enter a String:- ");
    gets(A);
    int l=mystrlen(A);
    // printf("%d",l);
    int presentState=start_state;
    int start=1;
    
     for (int i = 0; i < l; i++){ //string iterator
         for (int j = 0; j< num_of_inputs; j++){ //j=input iterator
                
            if(A[i]-48==j){
                 for (int k = 0; k < num_of_states; k++){ //k=state iteratpr
                    if(k==presentState && start==0){
                        printf(" ->%c ",65+transition_table[k*num_of_inputs+j]);
                        presentState=transition_table[k*num_of_inputs+j];
                        break;
                    }
                    else if (k==presentState && start==1)
                    {
                        printf("%c ->%c",s,65+transition_table[k*num_of_inputs+j]);
                        presentState=transition_table[k*num_of_inputs+j];
                        start=0;
                        break;
                    }
                    
                }
            }
         }
     
     }
    
    
      if (presentState==end_state)
        printf("\nString has been Accepted");
    else printf("\nString has not been Accepted");
    return 0;
 
}