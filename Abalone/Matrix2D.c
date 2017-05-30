#include <stdlib.h>
#include "Matrix2D.h"

void copyMatrix2D(Matrix2D *src, Matrix2D *dst)
{
	dst->n = src->n;
	dst->m = src->m;

	if (length(dst->mat) == length(src->mat))
	{
		for (unsigned int i = 0; i < length(dst->mat); i++)
			dst->mat->m_firstElem[i] = src->mat->m_firstElem[i];
	}
	else
	{
		if (dst->mat->m_firstElem != NULL)
			free(dst->mat->m_firstElem);

		dst->mat->m_firstElem = malloc(sizeof(double)*(src->mat->m_lastElem - src->mat->m_firstElem + 1);

		for (unsigned int i = 0; i < m.m_lastElem - m.m_firstElem + 1; i++)
			dst->mat->m_firstElem[i] = src->mat->m_firstElem[i];

		size_t delta = dst->mat->m_lastElem - dst->mat->m_firstElem;

		dst->mat->m_lastElem = dst->mat->m_firstElem + delta;
	}
}