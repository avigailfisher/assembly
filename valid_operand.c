#include "mac.h"  
/* The name of the function in this file:
 * 1. check_name_act
 * 2. replace_tabs_with_spaces
 * 3. check_valid_param_reg
 * 4. check_valid_param_num
 * 5. check_valid_param
 * 6. handle_leading_special_char
 */


/*  check_name_act - Checks if a code line matches known commands and updates the label address if necessary. */
void check_name_act(char* line, int line_number, label_ptr* hptr, int flag_label, char* label_name, int* ic, char* object, int * mistake2)
{
    int i;
    int counter = 0;
    word_and_p result;
    int word1 = 0;
    int word2 = 0;

    func_name_and_code two_opranda[5] = {
        {0, "mov"},
        {1, "cmp"},
        {2, "add"},
        {3, "sub"},
        {4, "lea"}
    };

    func_name_and_code one_opranda[9] = {
        {5, "clr"},
        {6, "not"},
        {7, "inc"},
        {8, "dec"},
        {9, "jmp"},
        {10, "bne"},
        {11, "red"},
        {12, "prn"},
        {13, "jsr"}
    };

    func_name_and_code zero_opranda[2] = {
        {14, "rts"},
        {15, "stop"}
    };
     label_ptr p = *hptr;

    /* Check if the object file pointer is valid */
    if (!object) {
        fprintf(stderr, "Error opening the file for writing.\n");
        return;
    }
   
    /* Update label address if label processing is enabled */
    if (flag_label == 1) {
        while (p) {

            if (strcmp(p->name, label_name) == 0) {

                p->adress = *ic+1;
                break;
            }
            p = p->next;
        }
    }
    /* Cut the line into words and process the first word */
    result = cut_word(&line, ' ');

    /* If the cut word is null or empty, allocate memory and copy the line */
    if (result.cut_word == NULL || strlen(result.cut_word) == 0) {

        result.cut_word = malloc(strlen(line) + 1);
        if (result.cut_word != NULL) {
            strcpy(result.cut_word, line);
        }

    }

    remove_space(result.cut_word);
    remove_space_from_end(result.cut_word);

    /* Check if the first word matches known operations with two operands */
    for (i = 0; i < 5; i++) {
        if (strcmp(result.cut_word, two_opranda[i].func_name) == 0) {
            if (empty_line(result.p) == 1 || result.p == NULL)
            {
                printf("ERROR: Line %d is wrong, Invalid line of code.\n", line_number);
                *mistake2=1;
                return;
            }
            two_operands(line, result, two_opranda[i], &counter, hptr, ic, flag_label, label_name, object, &word1, &word2, mistake2, line_number);
            free(result.cut_word);
            return;
        }
    }

    /* Check if the first word matches known operations with one operand */
    for (i = 0; i < 9; i++) {
        if (strcmp(result.cut_word, one_opranda[i].func_name) == 0) {
            if (empty_line(result.p) == 1 || result.p == NULL)
            {
                printf("ERROE: Line %d is wrong, Invalid line of code.\n", line_number);
                *mistake2=1;
                return;
            }
            one_operands(line, result, one_opranda[i], &counter, hptr, ic, flag_label, label_name, object, &word1, &word2, mistake2, line_number);
            free(result.cut_word);
            return;
        }
    }
    /* Check if the first word matches known operations with zero operands */
    for (i = 0; i < 2; i++) {
        if (strcmp(result.cut_word, zero_opranda[i].func_name) == 0) {
            zero_operands(line, result, zero_opranda[i], &counter, hptr, ic, flag_label, label_name, object, &word1, &word2, mistake2, line_number);
            free(result.cut_word);
            return;
        }
    }
    *mistake2=1;
    printf("ERROR: Line %d is wrong, the sentence is neither a directive nor an action nor an instruction\n", line_number);

    free(result.cut_word);

}


/*replace_tabs_with_spaces - Replaces tabs with spaces in a code line. */
void replace_tabs_with_spaces(char* line) {
    char temp[82];

    int i, j = 0;

    /* Replace each tab character with four spaces */
    for (i = 0; line[i] != '\0'; i++) {
        if (line[i] == '\t') {

            temp[j++] = ' ';
            temp[j++] = ' ';
            temp[j++] = ' ';
            temp[j++] = ' ';
        }
        else {
            temp[j++] = line[i];
        }
    }
    temp[j] = '\0';
    strcpy(line, temp);
}


/* check_valid_param_reg - Checks if a string is a valid register name. */
int check_valid_param_reg(char* line, int line_number) {
    int i;
    reg_name_and_code list_register[8] = {
        {0, "r0"},
        {1, "r1"},
        {2, "r2"},
        {3, "r3"},
        {4, "r4"},
        {5, "r5"},
        {6, "r6"},
        {7, "r7"}
    };

     /* Check if the line matches any register name */
   for (i = 0; i < 8; i++) {
        if (strcmp(line, list_register[i].reg) == 0) {
            return 0;
        }
    }

    return 1;
}


/* check_valid_param_num - Checks if a string represents a valid numeric parameter. */
int check_valid_param_num(char* line, int line_number, int * mistake2) {
    int num;
    char* original_line = line;
    /* Check if the line consists of digits only */
    while (*line) {
        if (!isdigit((unsigned char)*line)) {
        *mistake2=1;
            return 1;
        }
        line++;
    }

    num = atoi(original_line);

    /* Check if the number is within the acceptable range */
    if (num > 4096 || num < -4096)
    {
    *mistake2=1;
        printf("ERROR: Line %d is wrong, The immediate number value is invalid.\n", line_number);
        return 1;
    }
    return 0;
}

/* check_valid_param - Validates parameters and updates counters accordingly. */
int check_valid_param(char* line, int line_number, word_and_p p, int* counter, int* parameter1, int* parameter2, int* word1, int* word2, int * mistake2) {
    if (p.cut_word == NULL || *p.cut_word == '\0') {
        printf("Error: p.cut_word is NULL or empty!\n");
        *mistake2=1;
        return 1;
    }
 
    /* Validate if the parameter is a register */
    if (check_valid_param_reg(p.cut_word, 0) == 0) {
       
        if ((*counter) == 1) {

            (*parameter1) = (*parameter1) + 8;
        }
        else {
            (*parameter2) = (*parameter2) + 8;
        }
        return 0;
    }
    /* Validate if the parameter starts with '*' */
    else if (*p.cut_word == '*') {
        if (handle_leading_special_char(line, line_number, p, mistake2) == 0) {
            if ((*counter) == 1) {
                (*parameter1) = (*parameter1) + 4;
            }
            else {
                (*parameter2) = (*parameter2) + 4;
            }
            return 0;
        }
    }
     /* Validate if the parameter starts with '#' */
    else if (*p.cut_word == '#') {
        if (handle_leading_special_char(line, line_number, p, mistake2) == 0) {
            if ((*counter) == 1) {
                (*parameter1) = (*parameter1) + 1;
            }
            else {
                (*parameter2) = (*parameter2) + 1;
            }
            return 0;
        }
    }
    /* Handle direct parameters */
    else {

        if ((*counter) == 1) {

            (*parameter1) = (*parameter1) + 2;
            (*word1) = (*word1) + 1;

        }
        else {

            (*parameter2) = (*parameter2) + 2;
            (*word2) = (*word2) + 1;

        }
        return 0;
    }
    return 1;
}


/* handle_leading_special_char - Handles special characters at the beginning of a parameter. */
int handle_leading_special_char(char* line, int line_number, word_and_p p, int * mistake2) {
    char* temp;

    temp = (char*)malloc(80 * sizeof(char));
    if (temp == NULL) {
        perror("Memory allocation failed for temp");
        return 1;
    }
    /* Handle parameters starting with '#' */
if (*p.cut_word == '#') {
    if (p.cut_word[1] == '+' || p.cut_word[1] == '-') {
        strcpy(temp, p.cut_word + 2);

    }


    else {
        strcpy(temp, p.cut_word + 1);
    }
if (check_valid_param_num(temp, line_number, mistake2) == 0) {

        free(temp);
        return 0;
    }
}
    /* Handle parameters starting with '*' */
else if (*p.cut_word == '*'){
        strcpy(temp, p.cut_word + 1);
        if (check_valid_param_reg(temp, line_number) == 0) {

            free(temp);
            return 0;
        }
   }

    free(temp);
    return 1;
}

