/*
Do not modify this file.
Make all of your changes to main.c instead.
*/

#include "program.h"

#include <stdio.h>
#include <stdlib.h>

static int compare_bytes( const void *pa, const void *pb )
{
	int a = *(char*)pa;
	int b = *(char*)pb;

	if(a<b) {
		return -1;
	} else if(a==b) {
		return 0;
	} else {
		return 1;
	}

}

void access_sequential( char *data, int length )
{
	for (int i = 0; i < length; i++) {
		data[i] = 0;
	}
}

void access_random( char *data, int length )
{
	int num;
	for (int i = 0; i < length; i++) {
		num = (rand()%(length + 1));
		data[i] = 0;
	}
}

void access_uniform( char *data, int length )
{
	// TODO: Implementar
}
