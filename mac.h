#ifndef MAC_H
#define MAC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MIN_IMMIDIATE_VALUE -2048
#define MAX_IMMIDIATE_VALUE 2047
#define MAX_INTEGER_VALUE 16383
#define MIN_INTEGER_VALUE -16384

/*----------------struct---------------------*/

typedef struct {
    char* cut_word;
    char* p;
} word_and_p;



typedef struct macro_node {
    char* name_macro;
    char* content;
    int line_num;
    struct macro_node* next;
} macro_list;




typedef struct label_node {
    char* name;
    int adress;
    struct label_node* next;
} list_label;


typedef struct {
    int code;
    char* func_name;
} func_name_and_code;


typedef struct {
    int code;
    char* reg;
} reg_name_and_code;


typedef struct entry_node {
    char* name;
    struct entry_node* next;
} entry_node;


typedef struct extern_node {
    char* name;
    struct extern_node* next;
} extern_node;


typedef entry_node* entry_ptr;
typedef extern_node* extern_ptr;
typedef macro_list* ptr;
typedef list_label* label_ptr;


typedef struct data_image {
    int dc;
    int val;
    struct data_image* next;
} data_image;

typedef data_image* ptr_data;
typedef struct {
    char operand_name[32];
    int type_operand;
} string_and_int;

/* Typedef for line_1 structure */
typedef struct {
    int func;
    string_and_int op_source;
    string_and_int op_dest;
    int ARE;
} line_1;

/* Typedef for line_immediate structure */
typedef struct {
    int num;
    int ARE;
} line_immediate;

/* Typedef for line_register structure */
typedef struct {
    int num;
    int num2;
    int ARE;
    int start;
} line_register;

/* Typedef for line_label structure */
typedef struct {
    char first;
    char s_or_d;
    char label_n[31];
} line_label;






/*--------------------
try.c
---------------------*/
/**
 * remove_space - Removes leading spaces and tabs from a string.
 * @line: The string from which leading spaces and tabs will be removed.
 *
 * This function shifts the characters of the string to remove any leading
 * spaces and tabs, ensuring the string starts with the first non-space,
 * non-tab character.
 */
void remove_space(char *line);

/**
 * cut_word - Extracts a word from a line up to a specified symbol.
 * @line: A pointer to the string from which the word will be extracted.
 * @symbol: The character that marks the end of the word to extract.
 *
 * This function scans the line for the specified symbol and extracts
 * the word preceding it. It returns a structure containing the extracted
 * word and a pointer to the remainder of the line after the symbol.
 *
 * Return: A structure containing the extracted word and a pointer to
 * the remaining line.
 */
word_and_p cut_word(char **line, char symbol);

/**
 * generate_dest_filename - Generates a destination filename with a given extension.
 * @source_file: The original filename.
 * @extension: The file extension to append to the source filename.
 *
 * This function creates a new filename by appending the specified extension
 * to the source filename. The function allocates memory for the new string,
 * which the caller is responsible for freeing.
 *
 * Return: A pointer to the newly created filename string.
 */
char* generate_dest_filename(const char *source_file, const char *extension);

/**
 * copy_to_file - Appends a line to a specified file with a line number prefix.
 * @line: The line of text to be appended to the file.
 * @filename: The name of the file to which the line will be appended.
 * @line_number: The line number to prefix to the line in the file.
 *
 * This function opens the specified file in append mode, writes the line
 * with the prefixed line number, and then closes the file. If the file
 * cannot be opened, the program will print an error message and exit.
 */
void copy_to_file(const char *line, const char *filename, int *line_number);

/**
 * concatenate_strings - Concatenates multiple strings into a single string.
 * @num_strings: The number of strings to concatenate.
 * @strings: An array of strings to be concatenated.
 *
 * This function calculates the total length of the resulting string,
 * allocates memory, and then concatenates each string in the array
 * into a new string. The caller is responsible for freeing the memory
 * allocated for the concatenated string.
 *
 * Return: A pointer to the newly created concatenated string.
 */
char* concatenate_strings(int num_strings, char** strings);

/*--------------------
macro.c
---------------------*/

/**
 * process_line - Processes a single line of input.
 * @line: The line to be processed.
 * @h: Pointer to the head of the macro list.
 * @source: File pointer to the source file.
 * @filename: The name of the source file.
 * @dest_file: The name of the destination file.
 * @line_number: Pointer to the current line number.
 * @mistake: Pointer to a flag indicating if an error occurred.
 *
 * This function processes a line, checks if it's a macro definition or 
 * if it matches an existing macro. If it defines a macro, the macro is 
 * inserted into the list. If it matches an existing macro, the macro's 
 * content is copied to the destination file. If it's a normal line, 
 * it is copied to the destination file.
 */
void process_line(char *line, ptr *h, FILE *source, const char *filename, const char *dest_file, int * line_number, int *mistake);

/**
 * Differen_from_a_command_name - Checks if a macro name is different from command names.
 * @name_macro: The name of the macro.
 * @line_number: Pointer to the current line number.
 * @mistake: Pointer to a flag indicating if an error occurred.
 *
 * This function checks if the given macro name conflicts with existing 
 * instruction, action, or register names. If a conflict is found, it 
 * sets the mistake flag and returns 1. Otherwise, it returns 0.
 */
int Differen_from_a_command_name(char *name_macro, int *line_number, int *mistake);

/**
 * check_end_line - Checks if there are extraneous characters at the end of a line.
 * @end_command: Pointer to the end of the command in the line.
 * @index: The starting index for checking.
 * @flag: Flag indicating if there is no continuation of the line.
 * @line_number: Pointer to the current line number.
 * @mistake: Pointer to a flag indicating if an error occurred.
 *
 * This function checks if there are characters other than whitespace at the 
 * end of the line. If extraneous characters are found, it sets the mistake 
 * flag and returns 0. Otherwise, it returns 1.
 */
int check_end_line(char *end_command , int index, int flag, int * line_number, int *mistake);

/**
 * Insert_macro - Inserts a macro definition into the macro list.
 * @line: The line containing the macro definition.
 * @source: File pointer to the source file.
 * @h: Pointer to the head of the macro list.
 * @line_number: Pointer to the current line number.
 * @mistake: Pointer to a flag indicating if an error occurred.
 *
 * This function captures the content of a macro until the "endmacr" keyword 
 * is reached and inserts the macro into the list. It checks for errors such 
 * as duplicate macro names and invalid macro names.
 */
void Insert_macro(char* line, FILE* source, ptr* h, int *line_number, int * mistake);

/**
 * empty_line - Checks if a line is empty.
 * @line: The line to check.
 *
 * This function checks if the given line is empty or consists only of 
 * whitespace characters. It returns 1 if the line is empty, and 0 otherwise.
 */
int empty_line(char *line);
/*--------------------------
label.c
---------------------------*/
/**
 * process_line_label - Processes a line to extract and validate a label.
 * @line: The line to be processed.
 * @source: Pointer to the source file.
 * @line_number: The current line number being processed.
 * @first_word: Contains the first word in the line, which may be a label.
 * @hptr: Pointer to the label list.
 * @h: Pointer to the macro list.
 * @mistake2: Pointer to a variable indicating if an error occurred.
 *
 * This function extracts the label from the line, validates it by
 * checking its length, first character, and other characters, and
 * adds it to the label list if it is valid.
 *
 * Return: Returns 0 if the label is valid, otherwise returns 1.
 */
int process_line_label(char *line, FILE *source, int line_number, word_and_p first_word, label_ptr *hptr, ptr h, int *mistake2);

/**
 * remove_last_char - Removes the trailing colon character from the label.
 * @name_label: The label from which the colon should be removed.
 * @len_label: The length of the label.
 *
 * This function removes the trailing colon character ':' from the
 * end of the label, if present.
 *
 * Return: Returns 0 after processing.
 */
int remove_last_char(char *name_label, int len_label);

/**
 * check_length_label - Checks if the label length is within the allowed limit.
 * @name_label: The label to be checked.
 * @len_label: The length of the label.
 * @mistake2: Pointer to a variable indicating if an error occurred.
 *
 * This function checks if the length of the label is within the
 * allowed maximum length. If the length is invalid, it sets an
 * error indicator.
 *
 * Return: Returns 0 if the label length is valid, otherwise returns 1.
 */
int check_length_label(char *name_label, int len_label, int *mistake2, int line_number);

/**
 * check_first_char - Checks if the first character of the label is alphabetic.
 * @name_label: The label to be checked.
 * @mistake2: Pointer to a variable indicating if an error occurred.
 *
 * This function checks if the first character of the label is an
 * alphabetic character. If not, it sets an error indicator.
 *
 * Return: Returns 0 if the first character is alphabetic, otherwise returns 1.
 */
int check_first_char(char *name_label, int *mistake2, int line_number);

/**
 * check_other_characters - Checks if the remaining characters in the label are alphanumeric.
 * @name_label: The label to be checked.
 * @len_label: The length of the label.
 * @mistake2: Pointer to a variable indicating if an error occurred.
 *
 * This function checks if the characters after the first character
 * in the label are alphanumeric. If any non-alphanumeric characters
 * are found, it sets an error indicator.
 *
 * Return: Returns 0 if all characters are valid, otherwise returns 1.
 */
int check_other_characters(char *name_label, int len_label, int *mistake2, int line_number);



/*---------------------------------------------------------------------
node.c
--------------------------------------*/
/**
 * add2list - Adds a new macro to the list.
 * @hptr: Pointer to the head of the macro list.
 * @name: The name of the macro.
 * @content: The content of the macro.
 * @line_num: The line number where the macro is defined.
 *
 * This function creates a new node for the macro, allocates memory
 * for its name and content, and appends it to the end of the list.
 */
void add2list(ptr *hptr, char *name, char *content, int line_num);



/**
 * searchnode - Searches for a macro by name and returns its content.
 * @h: Pointer to the head of the macro list.
 * @name_macro: The name of the macro to search for.
 *
 * This function searches the macro list for a macro with the given
 * name and returns its content if found.
 *
 * Return: The content of the macro if found, otherwise NULL.
 */
char *searchnode(ptr h, char *name_macro);

/**
 * freelist - Frees the memory allocated for the macro list.
 * @hptr: Pointer to the head of the macro list.
 *
 * This function iterates through the macro list, freeing the memory
 * allocated for each macro's name, content, and the macro node itself.
 */
void freelist(ptr *hptr);

/**
 * freelist_label - Frees the memory allocated for the label list.
 * @hptr: Pointer to the head of the label list.
 *
 * This function iterates through the label list, freeing the memory
 * allocated for each label's name and the label node itself.
 */
void freelist_label(label_ptr *hptr);

/**
 * add2list_label - Adds a new label to the label list.
 * @hptr: Pointer to the head of the label list.
 * @name_label: The name of the label.
 * @h: Pointer to the macro list for validation.
 *
 * This function creates a new node for the label, checks if the label
 * already exists in the list or conflicts with existing macro names,
 * and appends it to the end of the label list.
 */
void add2list_label(label_ptr *hptr, char *name_label, ptr h);


/**
 * return_line_num - Returns the line number where a macro is defined.
 * @h: Pointer to the head of the macro list.
 * @name_macro: The name of the macro.
 *
 * This function searches the macro list for a macro with the given
 * name and returns its line number if found.
 *
 * Return: The line number of the macro if found, otherwise 0.
 */
int return_line_num(ptr h, char *name_macro);

/**
 * Adds a new entry to the end of an entry list.
 *
 * @param hptr Pointer to the head pointer of the entry list. The list is modified to include the new entry.
 * @param name The name to be added to the list. The name is duplicated and stored in the new node.
 *
 * @note Allocates memory for a new entry node and the name. It is the caller's responsibility to free this memory when it is no longer needed.
 */
void add2list_entry(entry_ptr *hptr, char* name);




/**
 * Frees all nodes in the entry list and their associated memory.
 *
 * @param hptr Pointer to the head pointer of the entry list. The list is freed, and the head pointer is set to NULL.
 *
 * @note This function frees the memory allocated for both the nodes and their names. It is important to call this function to avoid memory leaks.
 */
void freelist_entry(entry_ptr *hptr);



/**
 * Adds a new entry to the end of an extern list.
 *
 * @param hptr Pointer to the head pointer of the extern list. The list is modified to include the new entry.
 * @param name The name to be added to the list. The name is duplicated and stored in the new node.
 *
 * @note Allocates memory for a new extern node and the name. It is the caller's responsibility to free this memory when it is no longer needed.
 */
void add2list_extern(extern_ptr *hptr, char* name);

/**
 * Frees all nodes in the extern list and their associated memory.
 *
 * @param hptr Pointer to the head pointer of the extern list. The list is freed, and the head pointer is set to NULL.
 *
 * @note This function frees the memory allocated for both the nodes and their names. It is important to call this function to avoid memory leaks.
 */
void freelist_extern(extern_ptr *hptr);

/*------------------------------------------------------
valid_operand.c
---------------------------------------------------*/
/**
 * check_name_act - Checks and processes a line of assembly code based on its action or directive.
 * @line: The line of assembly code to check.
 * @line_number: The line number in the source file.
 * @hptr: Pointer to the head of the label list.
 * @flag_label: A flag indicating if label processing is required.
 * @label_name: The name of the label.
 * @ic: Pointer to the instruction counter.
 * @object: The file to write the object code to.
 * @mistake2: Pointer to a flag indicating if an error was encountered.
 *
 * This function processes a line of assembly code to determine if it matches
 * known instructions or directives. It handles two, one, and zero operand instructions.
 */
void check_name_act(char* line, int line_number, label_ptr* hptr, int flag_label, char* label_name, int* ic, char* object, int *mistake2);

/**
 * replace_tabs_with_spaces - Replaces tabs with spaces in a line of text.
 * @line: The line of text where tabs are replaced with spaces.
 *
 * This function iterates through the line and replaces each tab character with four spaces.
 */
void replace_tabs_with_spaces(char* line);

/**
 * check_valid_param_reg - Checks if a given parameter is a valid register.
 * @line: The parameter to check.
 * @line_number: The line number for error reporting.
 *
 * This function verifies if the parameter matches any of the known register names.
 *
 * Return: 0 if the parameter is a valid register, 1 otherwise.
 */
int check_valid_param_reg(char* line, int line_number);

/**
 * check_valid_param_num - Checks if a given parameter is a valid immediate number.
 * @line: The parameter to check.
 * @line_number: The line number for error reporting.
 * @mistake2: Pointer to a flag indicating if an error was encountered.
 *
 * This function verifies if the parameter is a valid number within the allowed range.
 *
 * Return: 0 if the parameter is a valid number, 1 otherwise.
 */
int check_valid_param_num(char* line, int line_number, int *mistake2);

/**
 * check_valid_param - Validates a parameter based on its type and updates counters accordingly.
 * @line: The line of code being processed.
 * @line_number: The line number for error reporting.
 * @p: A structure containing the parameter to check.
 * @counter: A pointer to a counter indicating the number of parameters.
 * @parameter1: A pointer to the first parameter value.
 * @parameter2: A pointer to the second parameter value.
 * @word1: A pointer to the word counter for the first parameter.
 * @word2: A pointer to the word counter for the second parameter.
 * @mistake2: Pointer to a flag indicating if an error was encountered.
 *
 * This function validates the parameter and adjusts the relevant counters based on
 * the parameter type (register, immediate number, or other).
 *
 * Return: 0 if the parameter is valid, 1 otherwise.
 */
int check_valid_param(char* line, int line_number, word_and_p p, int* counter, int* parameter1, int* parameter2, int* word1, int* word2, int *mistake2);

/**
 * handle_leading_special_char - Handles special characters like '+' or '-' in a parameter.
 * @line: The line of code being processed.
 * @line_number: The line number for error reporting.
 * @p: A structure containing the parameter to check.
 * @mistake2: Pointer to a flag indicating if an error was encountered.
 *
 * This function processes parameters that start with special characters, and
 * verifies if they are valid registers or numbers.
 *
 * Return: 0 if the parameter is valid, 1 otherwise.
 */
int handle_leading_special_char(char* line, int line_number, word_and_p p, int *mistake2);

/*-------------------------------------
line.c
----------------------------------*/
/**
 * Processes a line of input to handle an instruction with two operands.
 *
 * @param line         The line of input containing the instruction.
 * @param result       A structure containing the result of a previous operation.
 * @param number_operand A structure containing function name and code.
 * @param counter      A pointer to an integer counter to be updated.
 * @param hptr         A pointer to a label table.
 * @param ic           A pointer to the instruction counter.
 * @param label        An integer flag indicating the presence of a label.
 * @param label_name   A string representing the label name.
 * @param object       A string representing the object file.
 * @param word1        A pointer to the first operand.
 * @param word2        A pointer to the second operand.
 * @param mistake2     A pointer to an integer flag indicating errors.
 */
void two_operands(char* line, word_and_p result, func_name_and_code number_operand, int* counter, label_ptr* hptr, int* ic, int label, char* label_name, char* object, int* word1, int* word2, int * mistake2, int line_number);

/**
 * Processes a line of input to handle an instruction with one operand.
 *
 * @param line         The line of input containing the instruction.
 * @param result       A structure containing the result of a previous operation.
 * @param number_operand A structure containing function name and code.
 * @param counter      A pointer to an integer counter to be updated.
 * @param hptr         A pointer to a label table.
 * @param ic           A pointer to the instruction counter.
 * @param label        An integer flag indicating the presence of a label.
 * @param label_name   A string representing the label name.
 * @param object       A string representing the object file.
 * @param word1        A pointer to the first operand.
 * @param word2        A pointer to the second operand.
 * @param mistake2     A pointer to an integer flag indicating errors.
 */
void one_operands(char* line, word_and_p result, func_name_and_code number_operand, int* counter, label_ptr* hptr, int* ic, int label, char* label_name, char* object, int* word1, int* word2, int * mistake2, int line_number);

/**
 * Processes a line of input to handle an instruction with zero operands.
 *
 * @param line         The line of input containing the instruction.
 * @param result       A structure containing the result of a previous operation.
 * @param number_operand A structure containing function name and code.
 * @param counter      A pointer to an integer counter to be updated.
 * @param hptr         A pointer to a label table.
 * @param ic           A pointer to the instruction counter.
 * @param label        An integer flag indicating the presence of a label.
 * @param label_name   A string representing the label name.
 * @param object       A string representing the object file.
 * @param word1        A pointer to the first operand.
 * @param word2        A pointer to the second operand.
 * @param mistake2     A pointer to an integer flag indicating errors.
 */
void zero_operands(char* line, word_and_p result, func_name_and_code number_operand, int* counter, label_ptr* hptr, int* ic, int label, char* label_name, char* object, int* word1, int* word2, int * mistake2, int line_number);


/**
 * Handles the addressing method for operands and writes the line code to the output.
 *
 * @param func         The function code for the instruction.
 * @param p1           The first operand as a string.
 * @param p2           The second operand as a string.
 * @param parameter1   The type of the first operand.
 * @param parameter2   The type of the second operand.
 * @param hptr         A pointer to a label table.
 * @param ic           A pointer to the instruction counter.
 * @param label        An integer flag indicating the presence of a label.
 * @param label_name   A string representing the label name.
 * @param object       A string representing the object file.
 * @param word1        A pointer to the first operand.
 * @param word2        A pointer to the second operand.
 */
void addressing_method(int func, char* p1, char* p2, int parameter1, int parameter2, label_ptr* hptr, int* ic, int label, char* label_name, char* object, int* word1, int* word2);

/*----------------------------
add_data.c
--------------------------*/

/**
 * Removes trailing spaces from a string and returns the last non-space character.
 *
 * @param p   A pointer to the string to be processed.
 * @return    The last non-space character in the string. If the string is empty or contains only spaces, returns '\0'.
 */
char remove_space_from_end(char* p);

/**
 * Checks if a character is printable.
 *
 * @param ch  The character to be checked.
 * @return    1 if the character is printable, otherwise 0.
 */
int isPrintable(char ch);

/**
 * Updates_data - Processes a line of code and updates data based on specific directives.
 *
 * This function analyzes a given line of assembly code and performs actions based on the
 * directives found in the line. Specifically, it handles `.string` and `.data` directives,
 * updating the relevant data structures and counters.
 *
 * @param data        A pointer to the data structure where the processed information will be stored.
 * @param line        The line of code to be processed.
 * @param line_number The current line number in the source file, used for error reporting.
 * @param dc          A pointer to the data counter, which tracks the amount of data processed.
 * @param mistake     A pointer to an integer tracking general mistakes found during processing.
 * @param ic          A pointer to the instruction counter, which tracks the number of instructions processed.
 * @param object      The name of the object file where the processed data will be stored.
 * @param hp          A pointer to the structure managing entries.
 * @param hpt          A pointer to the structure managing externals.
 * @param label_flag  An integer indicating whether the current line contains a label.
 * @param label_name  The name of the label, if present.
 * @param hptr        A pointer to the label structure used to manage labels in the code.
 * @param mistake2    A pointer to an integer tracking mistakes specifically related to the current line's directive.
 */
void Updates_data(ptr_data* data,char* line, int line_number, int* dc, int* mistake, int* ic, char* object, entry_ptr *hp, extern_ptr *hpt, int label_flag, char * label_name, label_ptr* hptr, int * mistake2);

/**
 * Validates and processes a string that should be enclosed in double quotes, adding its printable characters to a data list.
 *
 * @param p         A pointer to the string to be validated.
 * @param dc        A pointer to the data counter, which will be updated.
 * @param line_number The line number where the string is located.
 * @param h         A pointer to the head of the data list.
 * @param ic        A pointer to the instruction counter.
 * @param object    A pointer to the object file name.
 * @param label_flag An integer flag indicating the presence of a label.
 * @param label_name A string representing the label name.
 * @param hptr      A pointer to the label table.
 * @param mistake2  A pointer to an integer flag indicating errors.
 */
void valid_string(char* p, int* dc, int line_number, ptr_data* h, int* ic, char* object, int label_flag, char* label_name, label_ptr* hptr, int* mistake2);

/**
 * Validates and processes numbers in a data line, handling signs and commas, and adds them to a data list.
 *
 * @param p         A pointer to the string containing the numbers.
 * @param dc        A pointer to the data counter, which will be updated.
 * @param line_number The line number where the numbers are located.
 * @param h         A pointer to the head of the data list.
 * @param ic        A pointer to the instruction counter.
 * @param object    A pointer to the object file name.
 * @param label_flag An integer flag indicating the presence of a label.
 * @param label_name A string representing the label name.
 * @param hptr      A pointer to the label table.
 * @param mistake2  A pointer to an integer flag indicating errors.
 */
void valid_number(char* p, int* dc, int line_number, ptr_data* h, int* ic, char* object, int label_flag, char* label_name, label_ptr* hptr, int* mistake2);

/**
 * Adds a new data entry to a linked list.
 *
 * @param h         A pointer to the head of the data list.
 * @param temp_num  The data value to be added.
 * @param dc        A pointer to the data counter.
 * @param ic        A pointer to the instruction counter.
 * @param object    A pointer to the object file name.
 * @param label_flag An integer flag indicating the presence of a label.
 * @param label_name A string representing the label name.
 * @param hptr      A pointer to the label table.
 */
void addlist_data(ptr_data* h, int temp_num, int* dc, int* ic, char* object, int label_flag, char* label_name, label_ptr* hptr);

/**
 * Processes a line to handle extern or entry directives and updates the corresponding list.
 *
 * @param line      The line of input containing the extern or entry directive.
 * @param line_number The line number where the directive is located.
 * @param hp        A pointer to the entry list.
 * @param hpt        A pointer to the extern list.
 * @param mistake2  A pointer to an integer flag indicating errors.
 */
void extern_or_entry(char* line, int line_number, entry_ptr *hp, extern_ptr *hpt, int * mistake2);

/**
 * Saves and prints the binary representation of data lines and updates the instruction counter.
 *
 * @param h         A pointer to the head of the data list.
 * @param ic        A pointer to the instruction counter, which will be updated.
 * @param object    A string representing the object file.
 * @param label_flag An integer flag indicating the presence of a label.
 * @param label_name A string representing the label name.
 * @param hptr      A pointer to the label table.
 */
void save_and_print_binary_line_data(ptr_data h, int* ic, char* object, int label_flag, char* label_name, label_ptr* hptr);

/*--------------------------------
kidud.c
--------------------------------*/

/**
 * Writes the line code based on the provided operands and function, updating the instruction counter and handling labels.
 *
 * @param func           The function code to be written.
 * @param op_source      A structure containing the source operand and its type.
 * @param op_dest        A structure containing the destination operand and its type.
 * @param ARE            Addressing method or Relocation Entry, represented as an integer.
 * @param num_of_operands The number of operands in the instruction.
 * @param line           A pointer to the line_1 structure to be updated with the function code.
 * @param ic             A pointer to the instruction counter, which will be incremented.
 * @param label_flag     An integer flag indicating the presence of a label.
 * @param label_name     A string representing the label name.
 * @param hptr           A pointer to the label table.
 * @param object         A string representing the object file name.
 * @param param1_label   A string representing the first label parameter.
 * @param param2_label   A string representing the second label parameter.
 */
void write_line_code(int func, string_and_int op_source, string_and_int op_dest, int ARE, int num_of_operands, line_1* line, int* ic, int label_flag, char* label_name, label_ptr* hptr, char* object, char *param1_label, char* param2_label);

/**
 * Writes the second line of code based on the source operand type, handling immediate values, labels, or registers.
 *
 * @param func           The function code for the instruction.
 * @param op_source      A structure containing the source operand and its type.
 * @param op_dest        A structure containing the destination operand and its type.
 * @param ARE            Addressing method or Relocation Entry, represented as an integer.
 * @param num_of_operands The number of operands in the instruction.
 * @param ic             A pointer to the instruction counter, which will be incremented.
 * @param object         A string representing the object file name.
 * @param label          A string representing the label name.
 * @param param1_label   A string representing the first label parameter.
 * @param param2_label   A string representing the second label parameter.
 */
void write_line2_code(int func, string_and_int op_source, string_and_int op_dest, int ARE, int num_of_operands, int* ic, char* object, char *label,  char *param1_label, char* param2_label);

/**
 * Writes the third line of code based on the destination operand type, handling immediate values, labels, or registers.
 *
 * @param func           The function code for the instruction.
 * @param op_source      A structure containing the source operand and its type.
 * @param op_dest        A structure containing the destination operand and its type.
 * @param ARE            Addressing method or Relocation Entry, represented as an integer.
 * @param num_of_operands The number of operands in the instruction.
 * @param ic             A pointer to the instruction counter, which will be incremented.
 * @param object         A string representing the object file name.
 * @param label          A string representing the label name.
 * @param param2_label   A string representing the second label parameter.
 */
void write_line3_code(int func, string_and_int op_source, string_and_int op_dest, int ARE, int num_of_operands, int* ic, char* object, char * label, char* param2_label);

/**
 * Prints the binary representation of a line_1 structure for debugging purposes.
 *
 * @param line  The line_1 structure containing the data to be printed.
 */
void print_binary(line_1 line);

/**
 * Converts a number to its binary representation and stores it in a string with the specified length.
 *
 * @param num          The number to be converted to binary.
 * @param length       The length of the binary string.
 * @param binary_str   A string to store the binary representation.
 */
void print_binary_with_length(int num, int length, char* binary_str);

/**
 * Saves and prints the binary representation of a line_1 structure to a file.
 *
 * @param line     The line_1 structure to be converted to binary.
 * @param ic       A pointer to the instruction counter, used to update the file position.
 * @param object   A string representing the object file name.
 */
void save_and_print_binary_line1(line_1 line, int* ic, char* object);

/**
 * Saves and prints the binary representation of a line_immediate structure to a file.
 *
 * @param line     The line_immediate structure to be converted to binary.
 * @param ic       A pointer to the instruction counter, used to update the file position.
 * @param object   A string representing the object file name.
 */
void save_and_print_binary_line_immediate(line_immediate line, int* ic, char* object);

/**
 * Saves and prints the binary representation of a line_register structure to a file.
 *
 * @param line     The line_register structure to be converted to binary.
 * @param ic       A pointer to the instruction counter, used to update the file position.
 * @param object   A string representing the object file name.
 */
void save_and_print_binary_line_register(line_register line, int* ic, char* object);

/**
 * Saves and prints the binary representation of a line_label structure to a file.
 *
 * @param line     The line_label structure to be converted to binary.
 * @param ic       A pointer to the instruction counter, used to update the file position.
 * @param object   A string representing the object file name.
 */
void save_and_print_binary_line_label(line_label line, int* ic, char* object);

/*--------------------------------
second_pass.c
--------------------------------*/

/**
 * Convert a binary string to an octal number.
 *
 * @param binary_str Pointer to a null-terminated binary string.
 * @return Pointer to a null-terminated string containing the octal representation.
 *         The caller is responsible for freeing the allocated memory.
 */
char* binary_to_octal(const char* binary_str);

/**
 * Processes a file by converting binary strings to octal and handling labels.
 *
 * Reads each line of the given file, converts binary values to octal format, 
 * and processes labels (indicated by '?'). The results are written to a temporary file,
 * which then replaces the original file.
 *
 * @param filename Pointer to the name of the file to process.
 */
void process_file(const char *filename, label_ptr hptr, extern_ptr hpt, entry_ptr hp, int * mistake2, char * file_ext, int ic, int dc );

/**
 * Check for duplicate names between two linked lists (entry and extern).
 *
 * This function scans through the provided entry and extern linked lists to identify any duplicate names.
 * It checks for duplicates between the two lists as well as within each list individually.
 * If a duplicate is found, an error message is printed, and the `mistake2` variable is set to 1.
 *
 * @param entry_head Pointer to the head of the entry linked list.
 * @param extern_head Pointer to the head of the extern linked list.
 * @param mistake2 Pointer to an integer that will be set to 1 if a duplicate is found,
 *                 or 0 if no duplicates are found.
 */

void check_duplicates(entry_ptr hp, extern_ptr hpt, int *mistake2, const char *filename, label_ptr hptr);
/**
 * Find the address of a label in a linked list of labels.
 *
 * This function searches for a label with the specified name in a linked list of labels.
 * If the label is found, the function returns its address. If the label is not found,
 * the function sets the `mistake2` flag to 1 and returns 1 to indicate the label was not found.
 *
 * @param hptr Pointer to the head of the linked list of labels.
 * @param label Pointer to a null-terminated string containing the label name to search for.
 * @param mistake2 Pointer to an integer that will be set to 1 if the label is not found.
 * @return The address of the label if found, or 1 if the label is not found.
 * 
 * Example usage:
 * @code
 * int mistake = 0;
 * int address = find_label_address(label_head, "LABEL_NAME", &mistake);
 * if (mistake) {
 *     // Handle the error: label not found
 * }
 * @endcode
 */
int find_label_address(label_ptr hptr, extern_ptr hpt, entry_ptr hp, char* label, int *mistake2);

char* binary_to_octal(const char* binary);

int label_exists(label_ptr label_list, const char* label_name);
void validate_entry_labels(entry_ptr hp, label_ptr hptr);
void check_for_duplicate_labels(label_ptr hptr, extern_ptr hpt, int *mistake);

#endif

