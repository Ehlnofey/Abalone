#ifndef MATRIX2D_H
#define MATRIX2D_H

#include "Matrix.h"

typedef struct Matrix2D {
	unsigned int n, m;
	Matrix *mat;
}Matrix2D;

Matrix2D* newEmptyMatrix2D()
{
	Matrix2D *m = malloc(sizeof(Matrix2D));
	m->n = 0;
	m->m = 0;
	m->mat = newEmptyMatrix();
}

Matrix2D* newMatrix2D(unsigned int n, unsigned int m) : Matrix<T>(n*m), m_n(n), m_m(m)
{

}

void copyMatrix2D(Matrix2D *src, Matrix2D *dst);

Matrix2D* transp(Matrix2D *m)
{
	Matrix2D t(m_m, m_n);

	for (unsigned int i = 0; i < m_n; i++)
		for (unsigned int j = 0; j < m_m; j++)
			t.m_firstElem[i*t.m_n + j] = m_firstElem[j*m_n + i];

	return t;
}

Matrix2D& operator+=(Matrix2D &m2)
{
	if (m_m != m2.m() || m_n != m2.n())
		throw std::exception("Dim of Matrix2D incompatible.");

	for (unsigned int i = 0; i < m_n; i++)
		for (unsigned int j = 0; j < m_m; j++)
			m_firstElem[j*m_n + i] += m2(i, j);

	return *this;
}
Matrix2D& operator/=(T m2)
{
	for (unsigned int i = 0; i < m_n; i++)
		for (unsigned int j = 0; j < m_m; j++)
			m_firstElem[j*m_n + i] /= m2;

	return *this;
}
Matrix2D& operator*=(T m2)
{
	for (unsigned int i = 0; i < m_n; i++)
		for (unsigned int j = 0; j < m_m; j++)
			m_firstElem[j*m_n + i] *= m2;

	return *this;
}

virtual void init2D(unsigned int n, unsigned int m)
{
	init(n*m);
	m_n = n;
	m_m = m;
}

T& operator()(unsigned int i, unsigned int j)
{
	if (m_firstElem + j*m_n + i > m_lastElem)
		throw std::exception("Out of range in Matrix2D");
	return m_firstElem[j*m_n + i];
}

const unsigned int& n()
{
	return m_n;
}

const unsigned int& m()
{
	return m_m;
}

virtual ~Matrix2D()
{
	if (m_firstElem != NULL)
		delete m_firstElem;

	m_firstElem = NULL;
}

template <typename T>
Matrix2D<T> operator*(Matrix2D<T> &m1, Matrix2D<T> &m2)
{
	if (m1.m() != m2.n())
		throw std::exception("Dim of Matrix2D incompatible.");

	Matrix2D<T> res(m1.n(), m2.m());

	for (unsigned int i = 0; i < res.n(); i++)
		for (unsigned int j = 0; j < res.m(); j++)
			for (unsigned int k = 0; k < m1.m(); k++)
				res(i, j) += m1(i, k) * m2(k, j);

	return res;
}
template <typename T, typename S>
Matrix2D<T> operator*(S &m1, const Matrix2D<T> &m2)
{
	Matrix2D<T> res(m2);

	for (unsigned int i = 0; i < res.n(); i++)
		for (unsigned int j = 0; j < res.m(); j++)
			res(i, j) *= m1;

	return res;
}
template <typename T>
Matrix2D<T> operator+(Matrix2D<T> &m1, Matrix2D<T> &m2)
{
	if (m1.m() != m2.m() || m1.n() != m2.n())
		throw std::exception("Dim of Matrix2D incompatible.");

	Matrix2D<T> res(m1.n(), m1.m());

	for (unsigned int i = 0; i < res.n(); i++)
		for (unsigned int j = 0; j < res.m(); j++)
			res(i, j) = m1(i, j) + m2(i, j);

	return res;
}
template <typename T>
Matrix2D<T> operator-(Matrix2D<T> &m1, Matrix2D<T> &m2)
{
	if (m1.m() != m2.m() || m1.n() != m2.n())
		throw std::exception("Dim of Matrix2D incompatible.");

	Matrix2D<T> res(m1.n(), m1.m());

	for (unsigned int i = 0; i < res.n(); i++)
		for (unsigned int j = 0; j < res.m(); j++)
			res(i, j) = m1(i, j) - m2(i, j);

	return res;
}


template <typename T>
Matrix2D<T> hadamard(Matrix2D<T> &m1, Matrix2D<T> &m2)
{
	if (m1.m() != m2.m() || m1.n() != m2.n())
		throw std::exception("Dim of Matrix2D incompatible.");

	Matrix2D<T> res(m1.n(), m2.m());

	for (unsigned int i = 0; i < res.n(); i++)
		for (unsigned int j = 0; j < res.m(); j++)
			res(i, j) = m1(i, j)*m2(i, j);

	return res;
}

#endif