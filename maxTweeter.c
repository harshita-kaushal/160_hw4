//basic text processing file for .c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_FILE_SIZE 20000
#define MAX_LINE_LEN 1024

struct USER_ARR
{
  char twitter_username[MAX_LINE_LEN]; //clarify later
  int count_of_tweets;
  int unique_id;
};

int GLOBAL_FIELD_QUOTED[100] = {0};

//can declare this within main
struct USER_ARR GLOBAL_USERS_ARR[MAX_FILE_SIZE];
struct USER_ARR GLOBAL_TOP_TEN[10];

int global_name_pos = 0;
int global_header_comma_count = 0;
int unique_user_count = 0;

//finds position of name within the header (i.e first line of csv)
int find_name_pos(char *file_csv)
{

  FILE *csv_file = fopen(file_csv, "r"); // "r" for read

  //todo more error checking later on
  //checks if file exists
  if (!csv_file)
  {
    printf("Can't open file\n");
    return -1;
  }
  char buf[1024];
  int row_count = 0;
  int field_count = 0;
  int index_of_field = 0;

  while (fgets(buf, 1024, csv_file))
  {
    field_count = 0;
    row_count++;
    char *field = strtok(buf, ",");

    //this is for the first row
    //assuming that header is always row1
    while (field)
    {

      // printf("%d : %s\n", index_of_field ,field);
      if (strcmp(field, "name") == 0)
      {
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

//function used to check whether a field has quotes, no quotes, or are invalid
int check_quotes(char* buf, int last_char, int first_char)
{
  //invalid quotes
  if ((buf[first_char] == '\"' && buf[last_char]  != '\"' )  || (buf[first_char] != '\"' && buf[last_char]  == '\"' ))
  {
    return -1;
  }

  //field has quotes
  else if((buf[first_char] == '\"' && buf[last_char]  == '\"' ))
  {
    return 1;
  }

  //field has no quotes
  else
  {
    return 0;
  }
}

//right now, it checks for a consistent number of counts for each row
int processing_file(char *file_csv)
{

  FILE *csv_file = fopen(file_csv, "r"); // "r" for read

  if (!csv_file)
  {
    printf("Can't open file\n");
    return -1;
  }

  char buf[2048];
  int row_count = 0;

  while (fgets(buf, 2048, csv_file))
  {
    if(strlen(buf) > 1024){
      return -1;
    }

    row_count++;
    //int comma_count = 0;
    int row_comma_count = 0;

    int cur_char = 0;

    int field_count = 0;
    int first_char = 0;
    int last_char = 0;
    //bool first_comma = false;
    bool valid = false;
    int quote_status = 0;

    while (cur_char < strlen(buf))
    {

      //** case of header , will also retrieve global comma count
      if (row_count == 1)
      {
        char cpy = buf[cur_char];

        //see comma
        if (cpy == ',')
        {
          global_header_comma_count += 1;

          last_char = cur_char-1;

          quote_status = check_quotes(buf,last_char,first_char);

          if(quote_status == -1){
            return -1;
          }
          else if(quote_status == 1){
            valid = true;
            GLOBAL_FIELD_QUOTED[field_count] = 1;
            //printf("Field count array at %d: %d\n",field_count,GLOBAL_FIELD_QUOTED[field_count]);
          }
          else{
            valid = true;
          }

          field_count +=1;    // increment for every field that we see
          first_char = last_char + 2;
          cur_char += 1;
          //printf("global_header_comma_count is %d\n", global_header_comma_count);
        }

        //when we don't see comma
        else
        {
          cur_char += 1;
        }

      }

      //for all other rows excluding header
      else
      {
        char cpy = buf[cur_char];

        if (cpy == ',')
        {
          row_comma_count += 1;

          last_char = cur_char-1;

          if(row_comma_count > global_header_comma_count)
          {
            return -1;
          }

          quote_status = check_quotes(buf,last_char,first_char);
          //printf("row comma count is %d, char is %d\n", row_comma_count,cur_char);
          //printf("row count: %d\n", row_count);

          if(quote_status == -1 || (quote_status != GLOBAL_FIELD_QUOTED[field_count])){
            printf("row count: %d\n", row_count);
            printf("Invalid quotes or does not match header\n");
            return -1;
          }

          field_count +=1;    // increment for every field that we see
          first_char = last_char + 2;
          cur_char += 1;

        }
        //TODO this error case no longer works but changing to strlen(buf)-1 causes program to hang
        else if(cur_char == 1023)
        {
          if(row_comma_count < global_header_comma_count)
          {
            return -1;
          }
        }
        else
        {
          cur_char += 1;
        }

      }
    }
  }

  return 0;
}

//function searches array of structs for a particular username, returns index if existing
int struct_match_search(char* username){

  for(int index = 0; index < unique_user_count; index++){
    if(strcmp(GLOBAL_USERS_ARR[index].twitter_username, username) == 0){
      return index;
    }
  }
  return -1;
}

//function adds username to struct if not existing or creates new entry
void add_to_struct(char* username, int index){

  if(index == -1){
    for(int i = 0; i < strlen(username);i++)
    {
      GLOBAL_USERS_ARR[unique_user_count].twitter_username[i] = username[i];
    }

    GLOBAL_USERS_ARR[unique_user_count].count_of_tweets = 1;
    GLOBAL_USERS_ARR[unique_user_count].unique_id = unique_user_count;

    unique_user_count++;
  }
  else
  {
    GLOBAL_USERS_ARR[index].count_of_tweets +=1;
  }
}

//function to store usernames and counts in array of structs
void find_names(char *file_csv){

  FILE *csv_file = fopen(file_csv, "r"); // "r" for read

  char buf[2048];
  int row_count = 0;

  while (fgets(buf, 2048, csv_file))
  {
    row_count++;
    int row_comma_count = 0;

    int cur_char = 0;

    int field_count = 0;
    int first_char = 0;
    int last_char = 0;
    int ret_val = 0;

    //char temp_field[1023]; //TODO check whether size of 1023 is okay

    while (cur_char < strlen(buf))
    {
      char temp_field[1023] = {"\0"};
      //** case of header , will also retrieve global comma count
      if (row_count > 1)
      {
        char cpy = buf[cur_char];
        //see comma
        if (cpy == ',')
        {
          last_char = cur_char-1;

          if(row_comma_count == global_name_pos)
          {
            if(GLOBAL_FIELD_QUOTED[field_count] == 1)
            {
              int j = 0;
              for(int i = first_char+1; i < last_char-1; i++){
                temp_field[i] = buf[i];
                j++;
              }
              add_to_struct(temp_field, struct_match_search(temp_field)); //call add function
            }
            else //case where there are not quotes
            {
              //printf("name: ");
              int j = 0;
              for(int i = first_char; i <= last_char; i++){
                temp_field[j] = buf[i];
                //printf("%c",temp_field[j]);
                j++;
              }
              //printf("\n");
              ret_val = struct_match_search(temp_field);
              add_to_struct(temp_field, ret_val); //call add function
            }
          }

          first_char = last_char + 2;
          cur_char += 1;
          row_comma_count++;
          field_count ++;
        }
        //when we don't see comma
        else
        {
          cur_char += 1;
        }
      }
      else{
        break;
      }
    }
  }
}

void print_top_ten(){

  int min = GLOBAL_USERS_ARR[0].count_of_tweets;
  int index_of_min = 0;

  //put first ten users in array
  for(int i = 0; i < 10; i++){

    GLOBAL_TOP_TEN[i].count_of_tweets = GLOBAL_USERS_ARR[i].count_of_tweets;
    GLOBAL_TOP_TEN[i].unique_id = GLOBAL_USERS_ARR[i].unique_id;

    if(GLOBAL_USERS_ARR[i].count_of_tweets < min){
      min = GLOBAL_USERS_ARR[i].count_of_tweets;
      index_of_min = i;
    }
  }

  for(int i = 10; i < unique_user_count; i++){
    if(GLOBAL_USERS_ARR[i].count_of_tweets > min){

      GLOBAL_TOP_TEN[index_of_min].count_of_tweets = GLOBAL_USERS_ARR[i].count_of_tweets;
      GLOBAL_TOP_TEN[index_of_min].unique_id = GLOBAL_USERS_ARR[i].unique_id;
      min = GLOBAL_USERS_ARR[i].count_of_tweets;
    }
    for(int j = 0; j < 10; j++){
      if(GLOBAL_USERS_ARR[i].count_of_tweets < min){
        min = GLOBAL_USERS_ARR[i].count_of_tweets;
        index_of_min = i;
      }
    }
  }

  //sort top ten
  for (int i = 0; i < 10; i++)
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (GLOBAL_TOP_TEN[i].count_of_tweets < GLOBAL_TOP_TEN[j].count_of_tweets)
      {
        int temp_count = GLOBAL_TOP_TEN[i].count_of_tweets;
        GLOBAL_TOP_TEN[i].count_of_tweets = GLOBAL_TOP_TEN[j].count_of_tweets;
        GLOBAL_TOP_TEN[j].count_of_tweets = temp_count;

        int temp_id = GLOBAL_TOP_TEN[i].unique_id;
        GLOBAL_TOP_TEN[i].unique_id = GLOBAL_TOP_TEN[j].unique_id;
        GLOBAL_TOP_TEN[j].unique_id = temp_id;
      }
    }
  }

//print each entry
  for(int i = 0; i < 10; i++){
    int original_index = GLOBAL_TOP_TEN[i].unique_id;

    for(int j = 0; j < strlen(GLOBAL_USERS_ARR[original_index].twitter_username); j++){
      printf("%c",GLOBAL_USERS_ARR[original_index].twitter_username[j]);
    }
    printf(": %d\n",GLOBAL_USERS_ARR[original_index].count_of_tweets);
  }

}


int main(int argc, char *argv[])
{
  int res_find_row = find_name_pos(argv[1]);

  int process_rest_file = processing_file(argv[1]);

  //invalid file check
  if (res_find_row == -1 || process_rest_file == -1)
  {
    printf("Invalid Input Format");
    return 0;
  }

  find_names(argv[1]);

  print_top_ten();

  // for(int j=0; j < unique_user_count;j++){
  //   printf("twitter username: ");
  //   for(int i=0; i < strlen(GLOBAL_USERS_ARR[j].twitter_username); i++){
  //     printf("%c",GLOBAL_USERS_ARR[j].twitter_username[i]);
  //   }
  //   printf("\n");
  // }

  return 0;
}
