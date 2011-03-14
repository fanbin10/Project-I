/* @(#)relaxation.h
 */

#ifndef _RELAXATION_H
#define _RELAXATION_H 1
#define PI = 3.14159265358


int gs(double *a, double* source,  double* compare,int row, int col);
int jac(double *a, double* source,  double* compare,int row, int col);
int overRelaxation(double *a, double* source, double* compare, int row, int col,double w);

#endif /* _RELAXATION_H */

