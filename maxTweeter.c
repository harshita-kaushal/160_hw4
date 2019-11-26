
//basic text processing file for .c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define MAX_SIZE 180

//have an array of structs, one unique one for each twitter user
//store the username and then increment the count every time we encounter a tweet 


struct USER_ARR{

        
        char twitter_username[1024]; //clarify later 
        int count_of_tweets; 
       
};
//won't be able to know until we have a size of users, but we can probably mal
int temp = 1000;

//can declare this within main 
struct USER_ARR GLOBAL_USERS[20000];


int main(int argc, char* argv[]){

// A valid input csv file has columns separated with “,”’s, but you cannot assume the location of the
// tweeter column will be fixed to a particular location (like 8 for instance). A valid csv file will have
// a header, and the tweeter column will be called “name”, and the items may or may not be
// surrounded by quotes (“/”). You may also assume that the maximum line length of any valid csv
// file will not be longer than 1024 characters, the length of the file will not exceed 20,000 lines.
// Finally, for this assignment, a valid file will not have additional commas inside the tweet.
// However, your program should not crash on any input, even invalid ones.

        FILE *csv_file = fopen(argv[1], "r"); // "r" for read


        if (!csv_file) {
                printf("Can't open file\n");
                return -1;
        }

        char buf[1024];
        int row_count = 0;
        int field_count = 0;
        int index_of_field = 0; 
        int name_pos = 0;
        while (fgets(buf, 1024, csv_file)) {
                field_count = 0;
                row_count++;

                // if (row_count == 1) {
                // continue;
                // }

                char *field = strtok(buf, ",");
                while (field) {

                        // this is only for the first one
                        //assuming that header is always row1

                
                printf("%d : %s\n", index_of_field ,field);
                if (strcmp(field, "name") ==0){
                       
                        name_pos = index_of_field;
                        printf("Name is at index %d\n", name_pos);
                        break;

                }
                index_of_field++;

                field = strtok(NULL, ",");

                field_count++;
                }
                // printf("\n");
                break;
        }


        fclose(csv_file);

        return 0;




}
