#include "square.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <rpc/rpc.h>

int main(int argc, char* argv[])
{
	CLIENT* c1;
	square_in* in;
	square_out* outp;

	c1 = clnt_create(argv[1], SQUARE_PROG, SQUARE_VERS, "tcp");

	in->arg1 = atol(argv[2]);
	outp = squareproc_1(in, c1);

	printf("Result: %ld\n", outp->res1);
	return 0;
}