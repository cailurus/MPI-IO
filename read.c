#include <stdio.h>
#include <stdlib.h>

int main(void)
   {
       FILE * fp;
       char * line = NULL;
       size_t len = 0;
       ssize_t read;

       fp = fopen("/Users/jinyangzhou/Documents/Github/MPI-IO/data", "r");
       if (fp == NULL)
           exit(EXIT_FAILURE);

       while ((read = getline(&line, &len, fp)) != -1) {
           printf("Retrieved line of length %zu :\n", read);
           printf("%d\n", atoi(line));
       }

       if (line)
           free(line);
       exit(EXIT_SUCCESS);
   }