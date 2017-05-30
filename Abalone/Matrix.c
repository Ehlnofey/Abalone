#include <stdlib.h>
#include "Matrix.h"

Matrix* newEmptyMatrix()
{
	Matrix* m = malloc(sizeof(Matrix));
	m->m_firstElem = NULL;
	m->m_lastElem = NULL;
}

Matrix* newMatrix(unsigned int size)
{
	Matrix *m = newEmptyMatrix();
	m->m_firstElem = malloc(sizeof(double)*size);
	m->m_lastElem = m->m_firstElem + size - 1;
}

void copyMatrix(Matrix *src, Matrix *dst)
{
	if (src->m_firstElem != NULL)
	{
		dst->m_firstElem = malloc(sizeof(double)*(src->m_lastElem - src->m_firstElem + 1));

		for (unsigned int i = 0; i < src->m_lastElem - src->m_firstElem + 1; i++)
			dst->m_firstElem[i] = src->m_firstElem[i];

		size_t delta = src->m_lastElem - src->m_firstElem;

		dst->m_lastElem = dst->m_firstElem + delta;
	}
}

int lengthMatrix(Matrix *m)
{
	return m->m_lastElem - m->m_firstElem + 1;
}

double firstMatrix(Matrix *m)
{
	return *m->m_firstElem;
}

double lastMatrixElement(Matrix *m)
{
	return *m->m_lastElem;
}

void applyMatrixElement(void(*f)(double *), Matrix *m)
{
	for (double* ptr = m->m_firstElem;ptr <= m->m_lastElem;ptr++)
		(*f)(ptr);
}

double getMatrixElement(unsigned int index, Matrix *m)
{
	if (index < 0 || index >= length(m))
	{
		stderrf("Bah index in a Matrix.");
		assert(0);
	}

	return m->m_firstElem[index];
}

void deleteMatrix(Matrix *m)
{
	if (m->m_firstElem != NULL)
		free(m->m_firstElem);

	m->m_firstElem = NULL;
}
