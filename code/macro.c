#include "mac.h"
#define MAX_LINE_LENGTH 1024

/* Function declarations
size_t strnlen(const char *s, size_t maxlen);

int Differen_from_a_command_name(char *name_macro, int *line_number, int *mistake);

int empty_line(char *line);

int check_end_line(char *end_command , int index, int flag, int *line_number, int *mistake);

void Insert_macro(char *line, FILE *source, ptr *h, int *line_number, int *mistake);

void process_line(char *line, ptr *h, FILE *source, const char *filename, const char *dest_file, int *line_number, int *mistake);
*/

void process_line(char *line, ptr *h, FILE *source, const char *filename, const char *dest_file, int * line_number, int *mistake) {
    word_and_p result;
    char *func;
    char *end_line;
    char copy_line[82];
    int index;
    char * content;
    char * content_copy;
    char *ptr_copy_line = copy_line;
    result = cut_word(&ptr_copy_line, ' ');
   
    strcpy(copy_line, line);
    ptr_copy_line = copy_line;
    result=cut_word(&ptr_copy_line,' ');
   
    func = result.cut_word; /* Outputs the first word in the line */




    if (func == NULL) {
       
        return; /* Ensure func is not NULL */
    }

    end_line = result.p; /* Saves the continuation of the command */


    /* if start macro--- */
    if (strcmp(func, "macr") == 0) {

        /* output the name of the macro */
        (*line_number)--;
        Insert_macro(end_line, source, h, line_number, mistake);    
    }
   
    /* if he has different name from all of the macros name he copy the all line */
    else if (searchnode(*h, func) == NULL) {

        copy_to_file(line, dest_file, line_number);}
       
       
    /* copy the content of the macro to the new file */
    else {

        index = return_line_num(*h, func);

        content = searchnode(*h, func);


        /* Use a temporary copy of the content */
        content_copy = malloc(strlen(content) + 1);
        if (content_copy == NULL) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }
        strcpy(content_copy, content);

        while (index > 0) {
            char new_line[81];
            char *newline_pos = strchr(content_copy, '\n');
           
            if (newline_pos == NULL) {
                strcpy(new_line, content_copy);
                content_copy[0] = '\0';
            } else {
                int line_length = newline_pos - content_copy;
                strncpy(new_line, content_copy, line_length);
                new_line[line_length] = '\0';
                memmove(content_copy, newline_pos + 1, strlen(newline_pos));
            }
       
            copy_to_file(new_line, dest_file, line_number);
            (*line_number)++;
            index--;
           
        }
        free(content_copy); /* Free the temporary copy */
        (*line_number)--;}
       
       
        }
int Differen_from_a_command_name(char *name_macro, int *line_number, int *mistake) {
      int i;
    const char *symbols[4] = {".data", ".string", ".entry", ".extern"};
    const char *macro[2] = {"macr", "endmacr"};
    const char *instructions[16] = {
        "mov", "cmp", "add", "sub", "lea", "not", "clr", "inc", "dec",
        "jmp", "bne", "red", "prn", "jsr", "rts", "stop"
    };
    const char *registers[8] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};


    /* Checks if the macro name is the same as an instruction name */
    for (i = 0; i < sizeof(symbols) / sizeof(symbols[0]); i++) {
        if (strcmp(symbols[i], name_macro) == 0) {
            printf("ERROR: in line %d the macro/label name is the same as an instruction name\n", *line_number);
            *mistake = 1;
            return 1;
        }
    }

    /* Checks if the macro name is the same as a macro definition */
    for (i = 0; i < sizeof(macro) / sizeof(macro[0]); i++) {
        if (strcmp(macro[i], name_macro) == 0) {
            printf("ERROR: in line %d the macro/label name is the same as a macro definition\n", *line_number);
            *mistake = 1;
            return 1;
        }
    }

    /* Checks if the macro name is the same as an action name */
    for (i = 0; i < sizeof(instructions) / sizeof(instructions[0]); i++) {
        if (strcmp(instructions[i], name_macro) == 0) {
            printf("ERROR: in line %d the macro/label name is the same as an action name\n", *line_number);
            *mistake = 1;
            return 1;
        }
    }

    /* Checks if the macro name is the same as a register name */
    for (i = 0; i < sizeof(registers) / sizeof(registers[0]); i++) {
        if (strcmp(registers[i], name_macro) == 0) {
            printf("ERROR: in line %d the macro/label name is the same as a register name\n", *line_number);
            *mistake = 1;
            return 1;
        }
    }

    /* If the macro name is correct - returns 0 */
    return 0;
}



/* return 1 if the end of the line without redundant characters */
int check_end_line(char *end_command , int index, int flag, int  * line_number, int *mistake) {

if (flag == 1){
 return 1;}
 if (end_command == NULL) {
        return 1; /* Handle null pointer*/
    }

    if (end_command[index] == '\n' || end_command[index] == '\0') {
        return 1;
    }
    while (end_command[index] != '\n' && end_command[index] != '\0') {  /* loop until the end of the line  */
        if (end_command[index] != ' ' ) {  /* If the current character is not a space */
            printf("ERROR: in line %d Extraneous text after end of command %c\n", *line_number, end_command[index]);

            (*mistake)=1;
            return 0;
        } else {
            index++;  
        }
    }

    return 1;  /* Returns 1 if you have reached the end of the string without unwanted characters*/
}


void Insert_macro(char* line, FILE* source, ptr* h, int *line_number, int * mistake) {
    char* name_macro;
    char* end_line;
    char* content = NULL;  /* Buffer to store the content of the macro */
    int flag;
    char* first_word;
    char copy_line[80];
    char copy_line_for[80];
    char * copy_line_for_name;
    int line_num;
    int content_capacity;
    int new_capacity;
    char* temp;
    size_t content_length = 0;  
    word_and_p result;
    copy_line_for_name=copy_line_for;
    line_num = 0;
    flag = 0;


    remove_space(line);

    strcpy(copy_line_for, line);
    result = cut_word(&copy_line_for_name, ' ');

    name_macro = result.cut_word; /* Outputs the command name */
    end_line = result.p; /* Saves the continuation of the line */
   
   
    content_capacity = 100;
    content = (char*)malloc(content_capacity);
    if (content == NULL) {
        perror("Failed to allocate memory for content");
        exit(EXIT_FAILURE);
    }
    content[0] = '\0'; /* Initialize the content buffer*/
   
   
    /* check if the name of the macro is used */
    if ((searchnode(*h, name_macro)) != NULL) {
        printf("ERROR: in line %d the name of the macro is used\n", *line_number);
        (*mistake)=1;
        free(content);
        return;
    }

    /* check if the macro name is ligall */
    if (Differen_from_a_command_name(name_macro, line_number,mistake) != 0) {
   
        free(content);
        return;
    }

    if (end_line == NULL) {
        flag = 1;
    }

    /* check if the end of the line is empty or with white charachters */
    if (check_end_line(end_line, 0, flag, line_number, mistake) != 1) {
   
        printf("ERROR: in lune number %d there characters after the name of the macro\n", *line_number);
        *mistake=1;
        return;
    }

    /* A loop that captures the macro content as long as endmacr is not reached */
    while (fgets(line, MAX_LINE_LENGTH + 2, source) != NULL) {
        size_t len = strlen(line);
        /* check if the length of the line big then 80 characters */
         if (len > 80) {
            printf("ERROR: in line %d the line length exceeds 80 characters\n", *line_number);
            *mistake = 1;
            free(content);
            return;
        }
        if (content_length + len + 2 >= content_capacity) { /* +2 for the newline and null terminator */
            new_capacity = content_capacity * 2;
            temp = realloc(content, new_capacity);
            if (temp == NULL) {
                perror("Failed to reallocate memory for content");
                free(content);  
                exit(EXIT_FAILURE);
            }
            content = temp;
            content_capacity = new_capacity;
        }

        if (empty_line(line)) {

            continue; /* Skip empty lines */
        }

        /* Remove trailing newline character if it's there */
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }

        /* Check if the line starts with "endmacro" */
        strcpy(copy_line, line);
        first_word = strtok(copy_line, " ");

        if (strcmp(first_word, "endmacr") == 0) {
            break;  /* End of macro */
        }

        strncat(content, line, len);
        strncat(content, "\n", 1);
        content_length += len + 1;  

        line_num++;
    }

    /* Add the macro to the list if it doesn't already exist */
    if (check_end_line(end_line, 0, flag, line_number, mistake) != 0 && flag != 1) {
   
        *mistake=1;
        printf("ERROR: in line number %d there are Unnecessary chars", *line_number);
    }

    /* send it to list */

    add2list(h, name_macro, content, line_num);
    free(content);
}





/* return 1 if the line empty */
int empty_line(char *line)
{
    if (line == NULL) {
        return 1; /* Consider NULL line as empty*/
    }

    /* Loop through each character in the line*/
    while (*line != '\0') {
        if (!isspace((unsigned char)*line)) {
            return 0; /*Found a non-whitespace character*/
        }
        line++;
    }
   
    return 1; /* All characters are whitespace*/
}

