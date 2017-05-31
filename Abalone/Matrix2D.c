#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "Matrix2D.h"

Matrix2D* newEmptyMatrix2D()
{
	Matrix2D *m = malloc(sizeof(Matrix2D));
	m->n = 0;
	m->m = 0;
	m->mat = newEmptyMatrix();

	return m;
}

Matrix2D* newMatrix2D(unsigned int n, unsigned int m)
{
	Matrix2D *m2D = malloc(sizeof(Matrix2D));
	m2D->n = n;
	m2D->m = m;
	m2D->mat = newMatrix(n*m);

	return m2D;
}

void copyMatrix2D(Matrix2D *src, Matrix2D *dst)
{
	dst->n = src->n;
	dst->m = src->m;

	if (lengthMatrix(dst->mat) == lengthMatrix(src->mat))
	{
		for (unsigned int i = 0; i < lengthMatrix(dst->mat); i++)
			dst->mat->m_firstElem[i] = src->mat->m_firstElem[i];
	}
	else
	{
		if (dst->mat->m_firstElem != NULL)
			free(dst->mat->m_firstElem);

		dst->mat->m_firstElem = malloc(sizeof(double)*(src->mat->m_lastElem - src->mat->m_firstElem + 1));

		for (unsigned int i = 0; i < lengthMatrix(src->mat); i++)
			dst->mat->m_firstElem[i] = src->mat->m_firstElem[i];

		size_t delta = dst->mat->m_lastElem - dst->mat->m_firstElem;

		dst->mat->m_lastElem = dst->mat->m_firstElem + delta;
	}
}

Matrix2D* transp(Matrix2D *m)
{
	Matrix2D *t = newEmptyMatrix2D(m->n, m->m);

	for (unsigned int i = 0; i < m->n; i++)
		for (unsigned int j = 0; j < m->m; j++)
			t->mat->m_firstElem[i*t->n + j] = m->mat->m_firstElem[j*m->n + i];

	return t;
}
void sumInMatrix2D(Matrix2D *m1, Matrix2D *m2)
{
	if (m1->m != m2->m || m1->n != m2->n)
	{
		printf("Dim of Matrix2D incompatible.");
		assert(0);
	}

	for (unsigned int i = 0; i < m1->n; i++)
		for (unsigned int j = 0; j < m1->m; j++)
			*getMatrix2DElement(m1, i, j) = *getMatrix2DElement(m1, i, j) + *getMatrix2DElement(m2, i, j);
}

double* getMatrix2DElement(Matrix2D *m, unsigned int i, unsigned int j)
{
	if (m->mat->m_firstElem + j*m->n + i > m->mat->m_lastElem)
	{
		printf("Out of range in Matrix2D");
		assert(0);
	}

	return &m->mat->m_firstElem[j*m->n + i];
}
void deleteMatrix2D(Matrix2D *m)
{
	if (m != NULL)
	{
		if (m->mat != NULL)
		{
			if (m->mat->m_firstElem != NULL)
				free(m->mat->m_firstElem);
			free(m->mat);
		}
		free(m);
	}
}
Matrix2D* dotMatrix2D(Matrix2D *m1, Matrix2D *m2)
{
	if (m1->m != m2->n)
	{
		printf("Dim of Matrix2D incompatible.");
		assert(0);
	}

	Matrix2D *res = newEmptyMatrix2D(m1->n, m2->m);

	for (unsigned int i = 0; i < res->n; i++)
		for (unsigned int j = 0; j < res->m; j++)
			for (unsigned int k = 0; k < m1->m; k++)
				*getMatrix2DElement(res, i, j) += *getMatrix2DElement(m1, i, k) * *getMatrix2DElement(m2, k, j);

	return res;
}

void dotInMatrix2D(Matrix2D *dst, Matrix2D * m1, Matrix2D * m2)
{
	if (m1->m != m2->n)
	{
		printf("Dim of Matrix2D incompatible.");
		assert(0);
	}

	for (unsigned int i = 0; i < dst->n; i++)
		for (unsigned int j = 0; j < dst->m; j++)
			for (unsigned int k = 0; k < m1->m; k++)
				*getMatrix2DElement(dst, i, j) += *getMatrix2DElement(m1, i, k) * *getMatrix2DElement(m2, k, j);
}

void dotScalarMatrix2D(double s, Matrix2D *m)
{
	for (unsigned int i = 0; i < m->n; i++)
		for (unsigned int j = 0; j < m->m; j++)
			*getMatrix2DElement(m, i, j) *= s;
}
void divisionScalarMatrix2D(double s, Matrix2D *m)
{
	for (unsigned int i = 0; i < m->n; i++)
		for (unsigned int j = 0; j < m->m; j++)
			*getMatrix2DElement(m, i, j) /= s;
}

Matrix2D* sumMatrix2D(Matrix2D *m1, Matrix2D *m2)
{
	if (m1->m != m2->m || m1->n != m2->n)
	{
		printf("Dim of Matrix2D incompatible.");
		assert(0);
	}

	Matrix2D* res = newMatrix2D(m1->n, m1->m);

	for (unsigned int i = 0; i < res->n; i++)
		for (unsigned int j = 0; j < res->m; j++)
			*getMatrix2DElement(res, i, j) = *getMatrix2DElement(m1, i, j) + *getMatrix2DElement(m2, i, j);

	return res;
}
Matrix2D* minusMatrix2D(Matrix2D *m1, Matrix2D *m2)
{
	if (m1->m != m2->m || m1->n != m2->n)
	{
		printf("Dim of Matrix2D incompatible.");
		assert(0);
	}

	Matrix2D* res = newMatrix2D(m1->n, m1->m);

	for (unsigned int i = 0; i < res->n; i++)
		for (unsigned int j = 0; j < res->m; j++)
			*getMatrix2DElement(res, i, j) = *getMatrix2DElement(m1, i, j) - *getMatrix2DElement(m2, i, j);

	return res;
}

Matrix2D* hadamardDotMatrix2D(Matrix2D *m1, Matrix2D *m2)
{
	if (m1->m != m2->m || m1->n != m2->n)
	{
		printf("Dim of Matrix2D incompatible.");
		assert(0);
	}

	Matrix2D* res = newMatrix2D(m1->n, m1->m);

	for (unsigned int i = 0; i < res->n; i++)
		for (unsigned int j = 0; j < res->m; j++)
			*getMatrix2DElement(res, i, j) = *getMatrix2DElement(m1, i, j) * *getMatrix2DElement(m2, i, j);

	return res;
}

void hadamardDotInMatrix2D(Matrix2D * m1, Matrix2D * m2)
{
	if (m1->m != m2->m || m1->n != m2->n)
	{
		printf("Dim of Matrix2D incompatible.");
		assert(0);
	}

	for (unsigned int i = 0; i < m1->n; i++)
		for (unsigned int j = 0; j < m1->m; j++)
			*getMatrix2DElement(m1, i, j) = *getMatrix2DElement(m1, i, j) * *getMatrix2DElement(m2, i, j);
}
