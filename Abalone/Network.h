#ifndef NETWORK_H
#define NETWORK_H

typedef struct Network
{
	Matrix2D **neurons, **activated, **weights, **bias;
	unsigned int size;
}Network;

void deleteNetwork(Network *net);
Matrix2D* processNetwork(Network *net, Matrix2D *input);
double trainNetworkManyTime(Network *net, Matrix2D** inputs, Matrix2D** outputs, unsigned int inputLength);
void corrErrorNetwork(Network *net, Matrix2D* error);
Matrix2D* trainNetwork(Network* net, Matrix2D* input, Matrix2D* output);
Network* newNetwork(Matrix* layerSize);
Network* newEmptyNetwork();
void der_sigmoide(double *x);
void sigmoide(double *x);
void myRand(double *x);

#endif