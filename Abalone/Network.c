#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "Matrix2D.h"
#include "Network.h"

const double Lambda = 1.;

void myRand(double *x)
{
	*x = (double)(rand() - RAND_MAX / 2) / (double)(RAND_MAX / 2);
}

void myRand2D(Matrix2D *x)
{
	applyMatrixElement(myRand, x->mat);
}

void sigmoide(double *x)
{
	*x = 1 / (1 + exp(-(*x)*Lambda));
}
void sigmoide2D(Matrix2D *x)
{
	applyMatrixElement(sigmoide, x->mat);
}

void der_sigmoide(double *x)
{
	*x = Lambda * exp(-(*x)*Lambda) / pow((1 + exp(-(*x)*Lambda)), 2);
}

void der_sigmoide2D(Matrix2D *x)
{
	applyMatrixElement(der_sigmoide, x->mat);
}

Network* newEmptyNetwork()
{
	Network *n = malloc(sizeof(Network));
	n->activated = NULL;
	n->bias = NULL;
	n->size = 0;
	n->weights = 0;

	return n;
}

Network* newNetwork(Matrix* layerSize) 
{
	Network *n = newEmptyNetwork();
	n->size = lengthMatrix(layerSize);

	n->neurons = malloc(sizeof(Matrix2D*)*n->size);
	n->activated = malloc(sizeof(Matrix2D*)*(n->size - 1));
	n->weights = malloc(sizeof(Matrix2D*)*(n->size - 1));
	n->bias = malloc(sizeof(Matrix2D*)*(n->size - 1));

	for (unsigned int i = 0; i < n->size; i++)
		n->neurons[i] = newMatrix2D(getMatrixElement(layerSize,i), 1);
	for (unsigned int i = 0; i < n->size-1; i++)
		n->activated[i] = newMatrix2D(getMatrixElement(layerSize, i+1), 1);
	for (unsigned int i = 0; i < n->size - 1; i++)
		n->bias[i] = newMatrix2D(getMatrixElement(layerSize, i + 1), 1);
	for (unsigned int i = 0; i < n->size - 1; i++)
		n->weights[i] = newMatrix2D(n->bias[i]->n, n->neurons[i]->n);

	applyMatrixElement(myRand2D, n->weights);
	applyMatrixElement(myRand2D, n->bias);
}

Matrix2D* trainNetwork(Network* net, Matrix2D* input, Matrix2D* output)
{
	Matrix2D *activated, *error;

	copyMatrix2D(net->neurons[0], input);
	dotInMatrix2D(net->activated[0], net->weights[0], net->neurons[0]);
	sumInMatrix2D(net->activated[0], net->bias[0]);

	for (unsigned int i = 1; i < net->size - 1; i++)
	{
		copyMatrix2D(net->neurons[i], net->activated[i - 1]);

		sigmoide2D(net->neurons[i]);

		dotInMatrix2D(net->activated[i], net->weights[i], net->neurons[i]);

		sumInMatrix2D(net->activated[i], net->bias[i]);
	}

	activated = net->activated[net->size - 2];

	applyMatrixElement(der_sigmoide2D, activated);

	error = minusMatrix2D(net->neurons[net->size - 1], output);

	hadamardDotInMatrix2D(error, activated);

	return error;
}

void corrErrorNetwork(Network *net, Matrix2D* error)
{
	Matrix2D** delta = malloc(sizeof(Matrix2D*)*(net->size-1));
	Matrix2D ** activated = malloc(sizeof(Matrix2D*)*(net->size - 2));
	int i = 0;

	for (i = 0; i < net->size - 1; i++)
	{
		activated[i] = newEmptyMatrix2D();
		copyMatrix2D(net->activated[i], activated[i]);
		applyMatrixElement(sigmoide, activated[i]->mat);
	}

	delta[net->size-2] = newMatrix2DCopy(error);
	
	for (int i = (int)net->size - 3; i >= 0; i--)
	{
		Matrix2D *t = transp(net->weights[i + 1]);
		dotInMatrix2D(t, t, delta[i + 1]);
		hadamardDotInMatrix2D(t, activated[i]);
		copyMatrix2D(t, delta[i]);
		deleteMatrix2D(t);
	}

	for (unsigned int i = 0; i < net->size-1; i++)
	{
		*net->weights[i] += -Lambda * (*delta[i]) * net->neurons[i]->transp();
		*net->bias[i] += -Lambda * (*delta[i]);
	}

	for (i = 0; i < net->size - 1; i++)
	{
		deleteMatrix2D(delta[i]);
		deleteMatrix2D(activated[i]);
	}

	free(delta);
	free(activated);
}

double trainNetworkManyTime(Network *net, Matrix2D** inputs, Matrix2D** outputs)
{
	Matrix2D* error((unsigned int)outputs.first()->length(), 1);

	for (unsigned int i = 0; i < inputs.length(); i++)
	{
		unsigned int index(rand() % inputs.length());
		Matrix2D<double> er = train(inputs[index], outputs[index]);
		error += er;
		corrError(er);
	}

	error /= (double)inputs.length();

	double mean(0);

	for (unsigned int i = 0; i < error.length(); i++)
		mean += error[i];

	mean /= (double)error.length();

	return mean;
}

Matrix2D* processNetwork(Matrix2D *input)
{
	m_neurons[0] = input;
	*m_activated[0] = (*m_weights[0]) * (*m_neurons[0]) + (*m_bias[0]);

	for (unsigned int i = 1; i < m_activated.length(); i++)
	{
		m_neurons[i] = m_activated[i - 1];

		sigmoide2D(m_neurons[i]);

		*m_activated[i] = (*m_weights[i]) * (*m_neurons[i]) + (*m_bias[i]);
	}

	return *m_neurons.last();
}

void deleteNetwork()
{
	for (unsigned int i = 0; i < m_neurons.length(); i++)
		delete m_neurons[i];
	for (unsigned int i = 0; i < m_activated.length(); i++)
		delete m_activated[i];
	for (unsigned int i = 0; i < m_bias.length(); i++)
		delete m_bias[i];
	for (unsigned int i = 0; i < m_weights.length(); i++)
		delete m_weights[i];
}
