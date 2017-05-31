#ifndef MATRIX_H
#define MATRIX_H
/*
**Gère un tableau de taille size en mémoire.
*/

typedef struct Matrix
{
	double* m_firstElem;
	double* m_lastElem;
}Matrix;

Matrix* newEmptyMatrix();

Matrix* newMatrix(unsigned int size, ...);

void copyMatrix(Matrix *src, Matrix *dst);

unsigned int lengthMatrix(Matrix *m);

double firstMatrix(Matrix *m);

double lastMatrixElement(Matrix *m);

void applyMatrixElement(void(*f)(double *), Matrix *m);

double getMatrixElement(Matrix *m, unsigned int index);

void deleteMatrix(Matrix *m);

#endif