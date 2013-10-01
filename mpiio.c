#include "mpi.h"
#include "mpio.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef SIZE
#define SIZE 100
#endif

int main(int argc, char  **argv)
{
	int *buf, i, rank, nints, len;
	char *filename, *tmp;
	MPI_File fh;
	MPI_Status status;

	FILE * fp;
	char * line = NULL;
	size_t slen = 0;
	ssize_t sread;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(rank == 0){
		i = 1;
		while((i < argc) && strcmp("-fname", *argv)){
			i++;
			argv++;
		}
		if(i >= argc){
			fprintf(stderr, "sss\n");
			MPI_Abort(MPI_COMM_WORLD, 1);
		}
		argv++;
		len = strlen(*argv);
		filename = (char *)malloc(len+10);
		strcpy(filename, *argv);
		MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);
		MPI_Bcast(filename, len+10, MPI_CHAR, 0, MPI_COMM_WORLD);
	}
	else{
		MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);
		filename = (char *) malloc(len+10);
		MPI_Bcast(filename, len+10, MPI_CHAR, 0, MPI_COMM_WORLD);
	}
	buf = (int *)malloc(SIZE);
//	nints = SIZE/sizeof(int);
	fp = fopen("/Users/jinyangzhou/Documents/Github/MPI-IO", "r");

	while((sread = getline(&line, &slen, fp)) != -1){
//		printf("%d\n", atoi(line));
		for(i = rank*3; i<rank+3; i++){
			buf[i] = atoi(line);
		}
	}
/*	for(i = 0; i<nints, i++){
		buf[i] = rank;
	}
*/
	tmp = (char *) malloc(len+10);
	strcpy(tmp, filename);
	sprintf(filename, "%s.%d", tmp, rank);

	MPI_File_open(MPI_COMM_SELF, filename, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
	MPI_File_write(fh, buf, 3, MPI_INT, &status);
	MPI_File_close(&fh);

	for(i = rank*3; i<rank+3; i++)
		buf[i] = buf[i] + 10;
	MPI_File_open(MPI_COMM_SELF, filename, MPI_MODE_CREATE + MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
	MPI_File_write(fh, buf, 3, MPI_INT, &status);
	MPI_File_close(&fh);

	for(i = rank*3; i<rank+3; i++)
		printf("%d\n", buf[i]);

	free(buf);
	free(filename);
	free(tmp);

	MPI_Finalize();
	return 0;
}