#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include "Matrix.h"

Matrix* newEmptyMatrix()
{
	Matrix* m = malloc(sizeof(Matrix));
	m->m_firstElem = NULL;
	m->m_lastElem = NULL;

	return m;
}

Matrix* newMatrix(unsigned int size, ...)
{
	Matrix *m = newEmptyMatrix();
	m->m_firstElem = malloc(sizeof(double)*size);
	m->m_lastElem = m->m_firstElem + size - 1;

	va_list vl;
	va_start(vl, size);

	for (unsigned int i = 0; i < size; i++)
		m->m_firstElem[i] = va_arg(vl, unsigned int);

	va_end(vl);

	return m;
}

void copyMatrix(Matrix *src, Matrix *dst)
{
	if (src->m_firstElem != NULL)
	{
		dst->m_firstElem = malloc(sizeof(double)*(src->m_lastElem - src->m_firstElem + 1));

		for (unsigned int i = 0; i < lengthMatrix(src); i++)
			dst->m_firstElem[i] = src->m_firstElem[i];

		size_t delta = src->m_lastElem - src->m_firstElem;

		dst->m_lastElem = dst->m_firstElem + delta;
	}
}

unsigned int lengthMatrix(Matrix *m)
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

double getMatrixElement(Matrix *m, unsigned int index)
{
	if (index < 0 || index >= lengthMatrix(m))
	{
		printf("Bah index in a Matrix.");
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
