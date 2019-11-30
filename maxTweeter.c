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
};

int GLOBAL_FIELD_QUOTED[100] = {0};

//can declare this within main
struct USER_ARR *GLOBAL_USERS_ARR[MAX_FILE_SIZE];

int global_name_pos = 0;
int global_num_structs = 0;
int global_header_comma_count = 0;

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
  if ((buf[first_char] == '\"' && buf[last_char]  != '\"' )  || (buf[first_char] != '\"' && buf[last_char]  == '\"' ))
  {
    printf("invalid quotes\n");
    return -1;
  }

  else if((buf[first_char] == '\"' && buf[last_char]  == '\"' ))
  {
    printf("field has quotes\n");
    return 1;
  }

  else
  {
    printf("field does not have quotes\n");
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

    int buf_length = strlen(buf)-1;
    //printf("String length: %d\n", buf_length);

    row_count++;
    int comma_count = 0;
    int row_comma_count = 0;

    int cur_char = 0;

    int field_count = 0;
    int first_char = 0;
    int last_char = 0;
    bool first_comma = false;
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

//function to store usernames and counts in array of structs
void find_names(char *file_csv){

  FILE *csv_file = fopen(file_csv, "r"); // "r" for read

  char buf[2048];
  int row_count = 0;

  while (fgets(buf, 2048, csv_file))
  {
    char *field = strtok(buf, ",");
    char *temp_field[1024] = NULL;

    //if there are quotes, remove and then call add function
    if(GLOBAL_FIELD_QUOTED[field_count] == 1)
    {
      for(int i = 1; i < ((strlen(field[global_name_pos])-1); i++){
        temp_field[i] = field[global_name_pos][i];
      }
      //call add function
    }
    else //case where there are not quotes
    {
      //call add function
    }

  }
  return 0;

}


int main(int argc, char *argv[])
{

  // char* file_name = argv[1];

  int res_find_row = find_name_pos(argv[1]);

  int process_rest_file = processing_file(argv[1]);
  // printf("processing is taking a long time");
  //couldn't open file
  if (res_find_row == -1 || process_rest_file == -1)
  {
    printf("Invalid Input Format");
    return 0;
  }

  // printf("Name is at index %d\n", global_name_pos);

  return 0;
}
