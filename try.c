#include "mac.h"

/*--------------------------------------------------------------------------------
The functions defined in this file:

void remove_space(char *line);

word_and_p cut_word(char **line, char symbol);

char* generate_dest_filename(const char *source_file, const char *extension);

void copy_to_file(const char *line, const char *filename, int *line_number);

char* concatenate_strings(int num_strings, char** strings);
-----------------------------------------------------------------------------------*/




void remove_space(char *line)
{
char *start = line;
if(line == NULL)
{
return;
}
    
    while (*start == ' ' || *start == '\t')
    {
        start++;
    }
    if (start != line) {
        memmove(line, start, strlen(start) + 1);
    }
}


word_and_p cut_word(char **line, char symbol) {
    word_and_p result;
    int i;
    int j;
    result.cut_word = (char *)malloc(80 * sizeof(char));
     if (line == NULL || *line == NULL) {
        perror("Input line is NULL");
        result.cut_word = NULL;
        result.p = NULL;
        return result;
    }
    
    if (result.cut_word == NULL) {
        perror("Failed to allocate memory");/*Allocate memory for the word to be extracted*/
        result.p = NULL;
        return result;
    }

    i = 0;
    j = 0;
/*  Loop through the input line character by character */
    while ((*line)[i] != '\0' && (*line)[i] != '\n' && i < 80) {
    if (j >= 80) {  
            fprintf(stderr, "Error: cut_word buffer overflow.\n");
            break;
        }
    /* Check if the current character is the specified symbol */
        if ((*line)[i] == symbol) {
            result.cut_word[j] = '\0';
            if ((*line)[i + 1] == '\0' || (*line)[i + 1] == '\n') {
                result.p = NULL;
            } else {
                result.p = *line + i + 1;
            }
            return result;
        } else {
            result.cut_word[j++] = (*line)[i++];
        }
    }

   
    result.cut_word[j] = '\0';
    result.p = NULL;
    return result;
}

          
char* generate_dest_filename(const char *source_file, const char *extension) {
    size_t len = strlen(source_file); /* Calculate the length of the original filename string */
    size_t ext_len = strlen(extension); /* Calculate the length of the new extension string */
    char *dest_file = malloc(len + ext_len + 2); /* Allocate memory for the new filename */ 

    if (dest_file == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }
    strcpy(dest_file, source_file);/*Copy the original filename into the new string*/
    strcat(dest_file, "."); /*Add a dot (.) to the new string*/
    strcat(dest_file, extension);/*Add the new extension to the new string*/
    return dest_file; 
}


void copy_to_file(const char *line, const char *filename, int * line_number) {
    FILE *file = fopen(filename, "a");
    
    if (file == NULL) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    fprintf(file, "%d. %s\n", * line_number, line);
    fclose(file);

   
}


char* concatenate_strings(int num_strings, char** strings) {
int i;
    int total_length = 0;
    char* result ;
    for (i = 0; i < num_strings; i++) {
        total_length += strlen(strings[i]);
    }

    /* Allocate memory for the concatenated string (+1 for the null terminator)*/
    result = (char*)malloc((total_length + 1) * sizeof(char));
    if (result == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    result[0] = '\0';  /*Initialize the result as an empty string*/

    /* Concatenate the strings*/
    for (i = 0; i < num_strings; i++) {
        strcat(result, strings[i]);
    }

    return result;
}




