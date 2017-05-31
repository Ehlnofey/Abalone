#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "Matrix2D.h"
#include "Network.h"

const double Lambda = 1.;

void myRand(double *x)
{
	*x = (double)(rand() - RAND_MAX / 2) / (double)(RAND_MAX / 2);
}

void sigmoide(double *x)
{
	*x = 1 / (1 + exp(-(*x)*Lambda));
}

void der_sigmoide(double *x)
{
	*x = Lambda * exp(-(*x)*Lambda) / pow((1 + exp(-(*x)*Lambda)), 2);
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
	unsigned int i;
	Network *n = newEmptyNetwork();
	n->size = lengthMatrix(layerSize);

	n->neurons = malloc(sizeof(Matrix2D*)*n->size);
	n->activated = malloc(sizeof(Matrix2D*)*(n->size - 1));
	n->weights = malloc(sizeof(Matrix2D*)*(n->size - 1));
	n->bias = malloc(sizeof(Matrix2D*)*(n->size - 1));

	for (i = 0; i < n->size; i++)
		n->neurons[i] = newMatrix2D((unsigned int)getMatrixElement(layerSize,i), 1);
	for (i = 0; i < n->size-1; i++)
		n->activated[i] = newMatrix2D((unsigned int)getMatrixElement(layerSize, i+1), 1);
	for (i = 0; i < n->size - 1; i++)
		n->bias[i] = newMatrix2D((unsigned int)getMatrixElement(layerSize, i + 1), 1);
	for (i = 0; i < n->size - 1; i++)
		n->weights[i] = newMatrix2D(n->bias[i]->n, n->neurons[i]->n);

	for (i = 0; i < n->size - 1; i++)
		applyMatrixElement(myRand, n->weights[i]->mat);
	for (i = 0; i < n->size - 1; i++)
		applyMatrixElement(myRand, n->bias[i]->mat);

	return n;
}

Matrix2D* trainNetwork(Network* net, Matrix2D* input, Matrix2D* output)
{
	Matrix2D *activated, *error;
	unsigned int i;

	copyMatrix2D(net->neurons[0], input);
	dotInMatrix2D(net->activated[0], net->weights[0], net->neurons[0]);
	sumInMatrix2D(net->activated[0], net->bias[0]);

	for (i = 1; i < net->size - 1; i++)
	{
		copyMatrix2D(net->neurons[i], net->activated[i - 1]);

		applyMatrixElement(sigmoide, net->neurons[i]->mat);

		dotInMatrix2D(net->activated[i], net->weights[i], net->neurons[i]);

		sumInMatrix2D(net->activated[i], net->bias[i]);
	}

	activated = net->activated[net->size - 2];

	applyMatrixElement(der_sigmoide, activated->mat);

	error = minusMatrix2D(net->neurons[net->size - 1], output);

	hadamardDotInMatrix2D(error, activated);

	return error;
}

void corrErrorNetwork(Network *net, Matrix2D* error)
{
	Matrix2D** delta = malloc(sizeof(Matrix2D*)*(net->size-1));
	Matrix2D ** activated = malloc(sizeof(Matrix2D*)*(net->size - 2));
	unsigned int i = 0;
	int j = 0;

	for (i = 0; i < net->size - 1; i++)
	{
		activated[i] = newEmptyMatrix2D();
		copyMatrix2D(net->activated[i], activated[i]);
		applyMatrixElement(sigmoide, activated[i]->mat);
	}

	delta[net->size - 2] = newEmptyMatrix2D();
	copyMatrix2D(error, delta[net->size - 2]);
	
	for (j = (int)net->size - 3; j >= 0; j--)
	{
		Matrix2D *t = transp(net->weights[j + 1]);
		dotInMatrix2D(t, t, delta[j + 1]);
		hadamardDotInMatrix2D(t, activated[j]);
		copyMatrix2D(t, delta[j]);
		deleteMatrix2D(t);
	}

	for (i = 0; i < net->size-1; i++)
	{
		Matrix2D *t = transp(net->neurons[i]);
		dotInMatrix2D(delta[i], delta[i], t);
		dotScalarMatrix2D(-Lambda, delta[i]);
		sumInMatrix2D(net->weights[i], delta[i]);
		sumInMatrix2D(net->bias[i], delta[i]);
	}

	for (i = 0; i < net->size - 1; i++)
	{
		deleteMatrix2D(delta[i]);
		deleteMatrix2D(activated[i]);
	}

	free(delta);
	free(activated);
}

double trainNetworkManyTime(Network *net, Matrix2D** inputs, Matrix2D** outputs, unsigned int inputLength)
{
	Matrix2D* error = newMatrix2D(lengthMatrix(outputs[0]->mat), 1);
	double mean = 0;
	unsigned int i;

	for (i = 0; i < inputLength; i++)
	{
		unsigned int index = rand() % inputLength;
		
		Matrix2D* er = trainNetwork(net, inputs[index], outputs[index]);
		sumInMatrix2D(error, er);
		corrErrorNetwork(net, er);
		deleteMatrix2D(er);
	}

	divisionScalarMatrix2D(inputLength, error);

	for (i = 0; i < lengthMatrix(error->mat); i++)
		mean += getMatrixElement(error->mat, i);
	
	mean /= lengthMatrix(error->mat);

	return mean;
}

Matrix2D* processNetwork(Network *net, Matrix2D *input)
{
	Matrix2D *p = dotMatrix2D(net->weights[0], net->neurons[0]);
	unsigned int i;

	copyMatrix2D(input, net->neurons[0]);

	copyMatrix2D(p, net->activated[0]);
	sumInMatrix2D(net->activated[0],net->bias[0]);

	deleteMatrix2D(p);
	
	for (i = 1; i < net->size-1; i++)
	{
		net->neurons[i] = net->activated[i - 1];

		applyMatrixElement(sigmoide, net->neurons[i]->mat);
		
		p = dotMatrix2D(net->weights[i], net->neurons[i]);
		copyMatrix2D(p, net->activated[i]);
		sumInMatrix2D(net->activated[i], net->bias[i]);
		deleteMatrix2D(p);
	}

	return net->neurons[net->size-1];
}

void deleteNetwork(Network *net)
{
	for (unsigned int i = 0; i < net->size; i++)
		deleteMatrix2D(net->neurons[i]);
	for (unsigned int i = 0; i < net->size; i++)
		deleteMatrix2D(net->activated[i]);
	for (unsigned int i = 0; i < net->size; i++)
		deleteMatrix2D(net->bias[i]);
	for (unsigned int i = 0; i < net->size; i++)
		deleteMatrix2D(net->weights[i]);
}
