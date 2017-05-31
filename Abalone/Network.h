#ifndef NETWORK_H
#define NETWORK_H

typedef struct Network
{
	Matrix2D **neurons, **activated, **weights, **bias;
	unsigned int size;
}Network;

#endif