//basic text processing file for .c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_FILE_SIZE 20000
#define MAX_LINE_LEN 1024

struct USER_ARR
{
  char twitter_username[MAX_LINE_LEN];
  int count_of_tweets;
  int unique_id;
};

int GLOBAL_FIELD_QUOTED[100] = {0};

struct USER_ARR GLOBAL_USERS_ARR[MAX_FILE_SIZE];
struct USER_ARR GLOBAL_TOP_TEN[10];

int global_name_pos = 0;
int global_header_comma_count = 0;
int unique_user_count = 0;

//finds position of name within the header
int find_name_pos(char *file_csv)
{

  FILE *csv_file = fopen(file_csv, "r"); // "r" for read

  if (!csv_file){return -1;}

  char buf[1024];
  int row_count = 0;
  int field_count = 0;
  int index_of_field = 0;

  while (fgets(buf, 1024, csv_file))
  {
    field_count = 0;
    row_count++;

    //in the case that name is found twice in the header
    if (row_count ==1)
    {
      char *ptr = strstr(buf, "name");
      if (ptr !=NULL)
      {
        char *ptr_to_second_name = strstr(ptr+1, "name");

        if (ptr_to_second_name!= NULL)
        {
          return -1;
        }
      }
    }

    char *field = strtok(buf, ",");
    //assuming that header is always row1
    while (field)
    {
      if (strcmp(field, "name") == 0)
      {
        global_name_pos = index_of_field;
        break;
      }
      index_of_field++;

      field = strtok(NULL, ",");

      field_count++;
    }
    break;
  }
  fclose(csv_file);
  return global_name_pos;
}

//used to check whether a field has quotes, no quotes, or are invalid
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


/* @arg file_csv : the file passed into the command line
*processing_file() checks
*consistent number of commas across the whole file (returns invalid if too few or too many)
*index out of bounds errors  (i.e if file length>20k, line length > 1024 )
*presence of new line character within fields
returns 0 if sucessful and if file is completely valid    */
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
    int buf_length = strlen(buf);

    if (row_count > MAX_FILE_SIZE){
      return-1;
    }

    row_count++;
    int row_comma_count = 0;

    int cur_char = 0;

    int field_count = 0;
    int first_char = 0;
    int last_char = 0;
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
          }
          else{
            valid = true;
          }

          field_count +=1;    // increment for every field that we see
          first_char = last_char + 2;
          cur_char += 1;
        }

        //when we don't see comma
        else
        {
          if (cpy =='\n' && (cur_char != (buf_length -1))){
            return -1;
          }

          cur_char += 1;
        }

      }

      //**for all other rows excluding header
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

          //check for validity of quotes
          quote_status = check_quotes(buf,last_char,first_char);

          if(quote_status == -1 || (quote_status != GLOBAL_FIELD_QUOTED[field_count])){
            return -1;
          }

          field_count +=1;    // increment for every field that we see
          first_char = last_char + 2;
          cur_char += 1;

        }

        else if(cur_char == (buf_length-1))
        {
          if(row_comma_count < global_header_comma_count)
          {
            return -1;
          }
          cur_char+=1;

        }
        else
        {
          if (cpy =='\n' && (cur_char != (buf_length -1))) {
            return -1;
          }
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

    while (cur_char < strlen(buf))
    {
      char temp_field[1023] = {"\0"};
      //** case of header , will also retrieve global comma count
      if (row_count > 1)
      {
        char cpy = buf[cur_char];
        //** see comma
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

              int j = 0;
              for(int i = first_char; i <= last_char; i++){
                temp_field[j] = buf[i];
                j++;
              }
              ret_val = struct_match_search(temp_field);
              add_to_struct(temp_field, ret_val); //call add function
            }
          }

          first_char = last_char + 2;
          cur_char += 1;
          row_comma_count++;
          field_count ++;
        }

        //** when we don't see comma
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



/* args:none, returns: none
function prints the array of structs with the top ten tweeters */
void print_top_ten(){

  int min = GLOBAL_USERS_ARR[0].count_of_tweets;
  int index_of_min = 0;
  int printing_index = 10;

  //** if there are <= 10 users, put all users in a temporary array, sort, and print
  if(unique_user_count <= 10){

    printing_index = unique_user_count;
    //** put all users into temporary array
    for(int i = 0; i < unique_user_count; i++){

      GLOBAL_TOP_TEN[i].count_of_tweets = GLOBAL_USERS_ARR[i].count_of_tweets;
      GLOBAL_TOP_TEN[i].unique_id = GLOBAL_USERS_ARR[i].unique_id;
    }

    //** sort users
    for (int i = 0; i < unique_user_count; i++)
    {
      for (int j = i + 1; j < unique_user_count; j++)
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

  }
  //** if user count is greater than 10
  else{

    //** put first ten users in temporary array
    for(int i = 0; i < 10; i++){

      GLOBAL_TOP_TEN[i].count_of_tweets = GLOBAL_USERS_ARR[i].count_of_tweets;
      GLOBAL_TOP_TEN[i].unique_id = GLOBAL_USERS_ARR[i].unique_id;

      if(GLOBAL_USERS_ARR[i].count_of_tweets < min){
        min = GLOBAL_USERS_ARR[i].count_of_tweets;
        index_of_min = i;
      }
    }


    //** check remaining usernames against the current minimum
    for(int i = 10; i < unique_user_count; i++){

      if(GLOBAL_USERS_ARR[i].count_of_tweets > min){

        GLOBAL_TOP_TEN[index_of_min].count_of_tweets = GLOBAL_USERS_ARR[i].count_of_tweets;
        GLOBAL_TOP_TEN[index_of_min].unique_id = GLOBAL_USERS_ARR[i].unique_id;
        min = GLOBAL_USERS_ARR[i].count_of_tweets;
      }

      //** update min if necessary
      for(int j = 0; j < 10; j++){

        if(GLOBAL_TOP_TEN[j].count_of_tweets < min){
          min = GLOBAL_TOP_TEN[j].count_of_tweets;
          index_of_min = j;

        }
      }

    }

    //** sort top ten in temp array
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
  }


  //** print each entry from USER_ARR with corresponding index from temp TOP_TEN array
  for(int i = 0; i < printing_index; i++){
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

  return 0;
}
