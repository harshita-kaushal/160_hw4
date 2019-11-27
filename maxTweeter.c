
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


// //@key - > at namepos within the line
// int hash_index_search(int key) {

//   int hashIndex = hashCode(key);
//   printf("hash Index is %d\n", hashIndex);
//   //move in array until an empty
//   while(GLOBAL_USERS_ARR[hashIndex] != NULL) {
// // 	  printf("at hashIndex %d\n", hashIndex);
//       if(GLOBAL_USERS_ARR[hashIndex]->twitter_username == key)
//         {
//             return hashIndex;
//             // return hashArray[hashIndex];
//         }

//       //go to next cell
//       ++hashIndex;

//       //wrap around the table
//       hashIndex %= MAX_SIZE;
//   }

//     //if not found
//   return -1;
// }

//TODO uncomment
// int struct_match_search(char* username){

//         for (int index = 0; index<global_num_structs; index++) {
//                 if (GLOBAL_USERS_ARR[index]->twitter_username!=NULL){
//                         if (strcmp(GLOBAL_USERS_ARR[index]->twitter_username, username)==0)
//                         {
//                                 // GLOBAL_USERS_ARR[i]->count_of_tweets +=1;
//                                 return index;
//                         }

//                 }
//          }
//         return -1;

// }

//TODO UNCOMMENT
// void append_to_arr(){



//         // if hashcode_match
//         //         struct.count +=1


//         // else
//         //         global_num_structs++
//         //         GLOBAL_USERS_ARR[global_num_structs] = struct



// }



char* our_strcmp(char* char_pass){

        char *first = &char_pass[0];
        return first;
        
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
        int index_of_field = 0;

        while (fgets(buf, 1024, csv_file)) {
               
                char *ptr = buf; //same as char *ptr = &arr[0]
                field_count = 0;
                row_count++;
                int comma_count = 0;
                
                int name_index = 0;
                int cur_char = 0;

                printf("%s\n", buf);
                
               
                char cpy = buf[2];
                if (strcmp(&cpy, ",")==0)
                {
                        printf("comma is present is %c\n",buf[2]);
                }
                // printf("buf at index %d is %c\n", cur_char,  ptr[cur_char]);
                cur_char++;
                break;
                        }

        return 0;
        
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
