
//basic text processing file for .c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[]){

        // FILE *csv_file = fopen(argv[1], "r"); // "r" for read

        FILE *csv_file = fopen("cl-tweets-short-clean.csv", "r");
        char buf[1024];

        if (!csv_file) {
        printf("Can't open file\n");
        return 1;
        }

        while (fgets(buf, 1024, csv_file)) {
        printf("%s\n", buf);
        }


        fclose(csv_file);

        return 0;




}