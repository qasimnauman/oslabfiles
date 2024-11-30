#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

struct v {
   int i; /* row */
   int j; /* column */
   int x;
};

struct matrixProp {
   int row;
   int column;
   int high;
   int low;
};

void *runner(void *param); /* the thread */

void *matrixA (void *prop);
void *matrixB (void *prop);

int main(int argc, char *argv[]) {
   int w,x,y,z,high,low;
    if (argc > 6)
        {
        cout <<"You specified more than six arguments."<< endl;
        cout <<"Additional parameters will be ignored"<< endl;
        }

        else if (argc < 6)
    {
       cout <<"You did not specify enough arguments"<< endl;
       exit(0);
    }

   w = atoi(argv[1]);
   x = atoi(argv[2]);
   y = atoi(argv[3]);
   z = atoi(argv[4]);
   high = atoi(argv[5]);
   low = atoi(argv[6]);

   int A[w][x];
   int B[y][z];
   int C[w][z];

   struct matrixProp *matA = (struct matrixProp *) malloc(sizeof(struct matrixProp));
   matA->high = high;
   matA->low = low;
   matA->row = w;
   matA->column = x;

   /* Now create the thread A */
  pthread_t tid;       //Thread ID
   pthread_attr_t attr; //Set of thread attributes
   //Get the default attributes
   pthread_attr_init(&attr);
   //Create thread
   pthread_create(&tid,&attr,matrixA,matA);

   struct matrixProp *matB = (struct matrixProp *) malloc(sizeof(struct matrixProp));
   matB->high = high;
   matB->low = low;
   matB->row = y;
   matB->column = z;
   pthread_attr_init(&attr);
   //Create thread
   pthread_create(&tid,&attr,matrixB,matB);

   int i,j, count = 0;
   for(i = 0; i < w; i++) {
      for(j = 0; j < z; j++) {
             //struct to pass parameters
         struct v *data = (struct v *) malloc(sizeof(struct v));
         data->i = i; //row
         data->j = j; // column
         data->x = x;
         /* Now create the thread passing it data as a parameter */
         pthread_t tid;       //Thread ID
         pthread_attr_t attr; //Set of thread attributes
         //Get the default attributes
         pthread_attr_init(&attr);
         //Create thread
         pthread_create(&tid,&attr,runner,data);
         //Parent waits for all threads to finish
         pthread_join(tid, NULL);
         count++;
      }
   }

   //print matrix C
   for(i = 0; i < w; i++) {
      for(j = 0; j < z; j++) {
         printf("%d ", C[i][j]);
      }
      printf("\n");
   }
}

//thread is created with parameters from matrix A & B
void *runner(void *param) {
   struct v *data = (v*)param;
   int n, x, sum = 0; // n is counter and sum stores calculation
   x = data->x;
   //Row X Column
   for(n = 0; n< x; n++){
      sum += A[data->i][n] * B[n][data->j];
   }
   //store the sum in its cell in matrix C
   C[data->i][data->j] = sum;

   //exit without error
   pthread_exit(0);
}

void *matrixA(void *prop) {
   struct matrixProp *matA = (matrixProp*)prop;
   int i, j, row, column, high, low, cell = 0;

   row = matA->row;
   column = matA->column;
   high = matA->high;
   low = matA->low;
   //Row X Column
   for(i = 0; i < row; i++) {
      for(j=0; j < column; j++); {
             cell = rand() % high + low;
             A[i][j] = cell;
          }

   }
   //store the sum in its cell in matrix C
     //exit without error
   pthread_exit(0);
}

void *matrixB(void *prop) {
   struct matrixProp *matB = (matrixProp*)prop;
   int i, j, row, column, high, low, cell = 0;

   row = matB->row;
   column = matB->column;
   high = matB->high;
   low = matB->low;
   //Row X Column
   for(i = 0; i < column; i++) {
      for(j=0; j < row; j++); {
             cell = rand() % high + low;
             B[i][j] = cell;
          }

   }
     //exit without error
   pthread_exit(0);
}