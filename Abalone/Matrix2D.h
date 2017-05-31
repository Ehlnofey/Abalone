#ifndef MATRIX2D_H
#define MATRIX2D_H

#include "Matrix.h"

typedef struct Matrix2D {
	unsigned int n, m;
	Matrix *mat;
}Matrix2D;

Matrix2D* newEmptyMatrix2D();
Matrix2D* newMatrix2D(unsigned int n, unsigned int m);
void copyMatrix2D(Matrix2D *src, Matrix2D *dst);
Matrix2D* transp(Matrix2D *m);
void sumInMatrix2D(Matrix2D *m1, Matrix2D *m2);
double* getMatrix2DElement(Matrix2D *m, unsigned int i, unsigned int j);
void deleteMatrix2D(Matrix2D *m);
Matrix2D* dotMatrix2D(Matrix2D *m1, Matrix2D *m2);
void dotInMatrix2D(Matrix2D *dst, Matrix2D *m1, Matrix2D *m2);
void dotScalarMatrix2D(double s, Matrix2D *m);
void divisionScalarMatrix2D(double s, Matrix2D *m);
Matrix2D* sumMatrix2D(Matrix2D *m1, Matrix2D *m2);
Matrix2D* minusMatrix2D(Matrix2D *m1, Matrix2D *m2);
Matrix2D* hadamardDotMatrix2D(Matrix2D *m1, Matrix2D *m2); 
void hadamardDotInMatrix2D(Matrix2D *m1, Matrix2D *m2);

#endif