/* @(#)main.c
 */


#include "relaxation.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <math.h>

int initialize(double *, int ,int, int);
int writeMatrix(FILE *, double*, int, int);
int compareDecide(double* com, int row, int col,  double* maximum, double* avg);

int main(int argc, char *argv[])
{

    FILE *pipe = popen("gnuplot -persist","w");
    FILE *fp;
    FILE *result, *fmax, *favg;
    int num_iter = 100000; /* default maximum iteration number is 10,000*/
    double tolerance = 0.0005; /* default tolerance is 0.05%*/
    int i,j,k,l;
    int col , row ;
    double *mesh;
    double *source;
    double *compare;
    int c, pflag;
    char *algorithm = "gs"; /* set algorithm to gauss-siedal defaultly*/
    double max, avg;
    int initial = 0 ; /*we set initial guess to 0 defaultly*/
    double w = 1.2;

    pflag = 0;
    while ((c = getopt(argc, argv, "pia:n:t:w:")) != -1)
      switch(c)
	{
	case 'p':
	  pflag = 1;
	  break;
	case'a':
	  algorithm = optarg;
	  break;
	case 'n':
	  num_iter = atoi(optarg);
	  break;
	case 't':
	  tolerance = atof(optarg)/100;
	  printf("Tolerance is set to be: %lf\n", tolerance);
	  break;
	case 'i':
	  initial = 1;
	  break;
	case 'w':
	  w = atof(optarg);
	  break;

	case '?':
	  if (optopt == 'n'){
	    printf("Option -%c requires an argument.\n", optopt);
	    return 1;
	  }
	  if (optopt == 'a'){
	    printf("Option -%c requires an argument.\n", optopt);
	    return 1;
	  }
	  if (optopt == 'w'){
	    printf("Option -%c requires an argument.\n", optopt);
	    return 1;
	  }	  
	}

    

    fmax = fopen("max.txt","w");
    favg = fopen("average.txt","w");

    if ( (result=fopen("result.txt","w")) == NULL){
        fprintf(stderr, "Cannot open output file\n");
        exit(1);
    }
   



    col = 40;
    row = 20;
    mesh = (double *)malloc(sizeof(double)*row*col);
    source = (double *)malloc(sizeof(double)*row*col);
    compare = (double *)malloc(sizeof(double)*row*col);
    
    for (j=0; j<col*row; j++)
        source[j]=0;
    source[(int)round(row/2)*col+(int)round(col/2)]=-6.28; 
    initialize(mesh, row, col,initial);

    for (k=0; k<num_iter; k++){

      if ( strcmp(algorithm , "jacobi") == 0)
        jac(mesh, source, compare, row, col);
      else if(strcmp(algorithm , "over") == 0)
        overRelaxation(mesh, source, compare, row, col, w);
      else if(strcmp(algorithm, "gs") == 0)
        gs(mesh, source, compare, row, col);
      else
	gs(mesh, source, compare, row, col);
       
      compareDecide(compare, row, col, &max, &avg );
	printf("Correction : max:%f, avg:%f\n",max, avg);

	fprintf(favg, "%d\t%lf\n",k,avg);

      if (max <= tolerance && avg <= tolerance) {
	printf("Convering condition is met and running 100 more steps to check stability...\n");
	for(l=0; l<100; l++){ 
	  gs(mesh, source, compare, row, col);
	}
	compareDecide(compare, row, col, &max, &avg);
	if (max <= tolerance && avg <= tolerance){
	  printf("Converging condition met and stability checked: %d iterations used\n",k);
	  break;
	}
      }



      if (pflag == 1){
	if ( ((fp = fopen("plot.dat","w")) == NULL) )
	  { 
	    printf("Error can't open plot.dat for writing.\n");
	    exit(1);
	  }

	for (i = 0; i < row; i++)
	  {
	    for (j=0; j<col; j++)
	      fprintf(fp,"%lf  ",mesh[i*col+j] );
	    fprintf(fp, "\n");
	  }
	fflush(fp);
	usleep(100000);
	fprintf(pipe,"splot 'plot.dat' matrix with lines\n");
	fflush(pipe);
		usleep(100000);
      }
    }

    fclose(fp);
    writeMatrix(result, mesh, row, col);
    
    return 0;
}

int initialize(double *mesh, int row, int col, int flag){

    int i;
    srand((unsigned)(time(0)));
    for (i = 0; i < col*row; i++)
      if(flag == 0)
	mesh[i]=0;
      else if (flag == 1 )
	mesh[i] = (rand()/(RAND_MAX+1.0));
      
    for (i = 0; i < col; i++){
        mesh[i]=0;
        mesh[(row-1)*col+i]=1;
    }

    for (i = 0; i < row; i++){
        mesh[i*col] = 0;
        mesh[(i+1)*col-1] = 1;
    }
    return 0;
}

int writeMatrix(FILE *fp, double *mesh, int row, int col){
    int i, j;
    for (i=0; i<row; i++){
        for (j=0; j<col; j++){
            fprintf(fp,"%lf\t", mesh[i*col+j]);
        }
        fprintf(fp, "\n");
    }
    return 0;
}


int compareDecide(double* com, int row, int col,  double* maximum, double* avg){
  int i,j; 
  *avg = 0;
  *maximum = com[col+2]; 
  for (i=1; i<row-1; i++){
    for (j=1; j<col-1; j++){
      *avg += com[i*col+j];
      if (com[i*col+j] > *maximum)
	*maximum = com[i*col+j];
    }
  }
  *avg /= (row-1)*(col-1);
  return 1;
}

