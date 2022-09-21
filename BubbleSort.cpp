#include<iostream>
using namespace std;

void Bubblesort(int a[], int n){

  int temp=0, i, j;
  for (i=0; i<n; i++) 
  {
    for(j=i+1; j<n; j++) 
    {
       if(a[j] < a[i]) 
       {
          temp=a[i];
          a[i]=a[j];
          a[j]=temp;
       }
    }
  }        
}

int main(){
  int loop=0;
  int a[] = { 3, 5, 6, 0, 1 };
  printf("Array before sorting : ");
  for(loop=0; loop<5; loop++)
    printf("%d\t", a[loop]);
  printf("\n");
  Bubblesort(a, 5);
  printf("Array after sorting : ");
  for(loop=0; loop<5; loop++)
    printf("%d\t", a[loop]);
}
