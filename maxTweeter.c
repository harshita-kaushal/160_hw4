
//basic text processing file for .c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_FILE_SIZE 20000
#define MAX_LINE_LEN 1024
struct USER_ARR{
        char twitter_username[MAX_LINE_LEN]; //clarify later
        int count_of_tweets;

};
//won't be able to know until we have a size of users, but we can probably mal
int temp = 1000;

//can declare this within main
struct USER_ARR* GLOBAL_USERS_ARR[MAX_FILE_SIZE];

int global_name_pos = 0;
int global_num_structs= 0;
//finds position of name within the header (i.e first line of csv)
int find_name_pos(char *file_csv){

        FILE *csv_file = fopen(file_csv, "r"); // "r" for read

        //todo more error checking later on
        //checks if file exists
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

                        // printf("%d : %s\n", index_of_field ,field);
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


int processing_file(char *file_csv){

        FILE *csv_file = fopen(file_csv, "r"); // "r" for read
       
        //todo more error checking later on
        //checks if file exists
        if (!csv_file) {
                printf("Can't open file\n");
                return -1;
        }
        char buf[1024];
        int row_count = 0;
        int field_count = 0;

        while (fgets(buf, 1024, csv_file)) {
               
                field_count = 0;
                row_count++;
                int comma_count = 0;
                
                int cur_char = 0;
                while (cur_char< 1024){
                
                        char cpy = buf[cur_char];
                        // if (strcmp(&cpy, ",")==0)
                        if (cpy== ',')
                        {
                                printf("cpy %c is %d\n",cpy, comma_count);
                                cur_char+=1;
                                comma_count+=1;
                                if (comma_count == 8){
                                        printf("comma count is %d\n", comma_count);
                                        break;
                        }                
                                        
                        }
                        else{
                                // printf("buf[%d] is %c\n",cur_char, cpy);
                                cur_char+=1; 
                                }
                        }
                            
                
        }


                return 0;

                        }

        




char* our_strcmp(char* char_pass){

        char *first = &char_pass[0];
        return first;

}




int main(int argc, char* argv[]){


        // char* file_name = argv[1];

        int res_find_row = find_name_pos(argv[1]);

        int process_rest_file  = processing_file(argv[1]);
        // printf("processing is taking a long time");
        //couldn't open file
        if (res_find_row ==-1 || process_rest_file ==-1)
        {
                printf("Invalid Input Format");
                return 0;
        }

        // printf("Name is at index %d\n", global_name_pos);


        return 0;



}
