
//basic text processing file for .c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct USER_ARR{
        char twitter_username[1024]; //clarify later 
        int count_of_tweets; 
       
};
//won't be able to know until we have a size of users, but we can probably mal
int temp = 1000;

//can declare this within main 
struct USER_ARR GLOBAL_USERS[20000];

int global_name_pos = 0;

int find_row_pos(char *file_csv){

        FILE *csv_file = fopen(file_csv, "r"); // "r" for read
        
        //todo more error checking later on 
        if (!csv_file) {
                printf("Can't open file\n");
                return -1;
        }
        char buf[1024];
        int row_count = 0;
        int field_count = 0;
        int index_of_field = 0; 
        while (fgets(buf, 1024, csv_file)) {
                field_count = 0;
                row_count++;
                char *field = strtok(buf, ",");
                
                // this is for the first row
                //assuming that header is always row1
                while (field) {

                        printf("%d : %s\n", index_of_field ,field);
                        if (strcmp(field, "name") ==0){
                                global_name_pos = index_of_field;
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
        return global_name_pos;

}
int main(int argc, char* argv[]){


        char* file_name = argv[1];
        
        int res_find_row = find_row_pos(argv[1]);
        
        //couldn't open file
        if (res_find_row ==-1)
        {
                printf("Invalid Input Format");
                return 0;
        }
        printf("Name is at index %d\n", global_name_pos);

        return 0;



}
