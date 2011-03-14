/* @(#)relaxation.c
 */
#include "relaxation.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int copyMesh(double*, double*, int, int);

int gs(double *a, double* source, double* compare, int row, int col){
    int i, j;
    double *b = (double *)malloc(sizeof(double)*row*col);
    for (i=0; i<row; i++){
        for (j=0; j<col; j++){
            b[i*col+j] = a[i*col+j];
        }
    }
    
    /*if (sizeof(a) != sizeof(double)*row*col){
        printf("Array transfer error in Gauss-sidel\n");
        return 1;
        }*/
    
    for (i=1; i<row-1; i++){
        for (j=1; j<col-1; j++){
            b[i*col+j] = (1.0/4)*( a[(i-1)*col+j]+ a[(i+1)*col+j]+ a[i*col+j+1]+ a[i*col+j-1]+source[i*col+j]);
            compare[i*col+j]=fabs(b[i*col+j]-a[i*col+j]);
        }
    }

    copyMesh(b,a,row,col);
    free(b);
    return 0;
}

int jac(double *a, double* source, double* compare, int row, int col){
    int i, j;
    double *b = (double *)malloc(sizeof(double)*row*col);
    /*
    if (sizeof(a) != sizeof(double)*row*col){
        printf("Array transfer error in Jac\n");
        return 1;
    }
    */
    for (i=0; i<row; i++){
        for (j=0; j<col; j++){
            b[i*col+j] = a[i*col+j];
        }
    }
    
    for (i=1; i<row-1; i++){
        for (j=1; j<col-1; j++){
            b[i*col+j] = (1.0/4)*( b[(i-1)*col+j]+ a[(i+1)*col+j]+ a[i*col+j+1]+ b[i*col+j-1] + source[i*col+j] );
            compare[i*col+j]=fabs(b[i*col+j]-a[i*col+j]);
        }
    }
    copyMesh(b,a,row,col);
    free(b);
    return 0;
}



int overRelaxation(double *a, double* source, double* compare, int row, int col,  double w){
    int i, j;
    double *b = (double *)malloc(sizeof(double)*row*col);
    /*
    if (sizeof(a) != sizeof(double)*row*col){
        printf("Array transfer error in over-relaxation\n");
        return 1;
    }
    */
    for (i=0; i<row; i++){
        for (j=0; j<col; j++){
            b[i*col+j] = a[i*col+j];
        }
    }
    
    for (i=1; i<row-1; i++){
        for (j=1; j<col-1; j++){
            b[i*col+j] =(1-w)*a[i*col+j] + (w/4)*( b[(i-1)*col+j]+ a[(i+1)*col+j]+ a[i*col+j+1]+ b[i*col+j-1] + source[i*col+j] );
            compare[i*col+j]=fabs(b[i*col+j]-a[i*col+j]);
                        
        }
    }


    copyMesh(b,a,row,col);
    free(b);
    return 0;
}

int copyMesh(double *a, double *b, int row, int col){
  int i,j;
  for (i=0; i<row; i++){
    for (j=0; j<col; j++){
      b[i*col+j] = a[i*col+j];
    }
  }
  return 0;
}
