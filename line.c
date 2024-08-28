#include "mac.h"  

/*
 * Function List:
 * - two_operands: Processes instructions with two operands.
 * - one_operands: Processes instructions with one operand.
 * - zero_operands: Processes instructions with zero operands.
 * - addressing_method: Handles the addressing of operands and writes the appropriate machine code.
 */

/* two_operands - Processes instructions with two operands. */
void two_operands(char* line, word_and_p result, func_name_and_code number_operand, int* counter, label_ptr* hptr, int* ic, int label, char* label_name, char* object, int* word1, int* word2, int * mistake2, int line_number) {
    char func_name[7];
    char param1[32];
    char param2[32];
    int parameter1 = 0;
    int parameter2 = 0;
    word_and_p result1 = { NULL, NULL };
    word_and_p result2 = { NULL, NULL };
   
    /* Increment the counter */
    (*counter)++;
   
    /* Remove leading spaces and get the first operand */
    remove_space(result.cut_word);
    result1 = cut_word(&result.p, ',');

    /* Check if the first operand is valid */
    if (result1.cut_word == NULL || strlen(result1.cut_word) == 0) {
        result1.cut_word = result.p;
        if (result1.cut_word == NULL) {
            printf("Error: in line number %d paremters is NULL\n", line_number);
            return;
        }
    }

    /* Store the function name and validate the first operand */
    strcpy(func_name, number_operand.func_name);
    remove_space(result1.cut_word);
    remove_space_from_end(result1.cut_word);


    if (check_valid_param(line, line_number, result1, counter, &parameter1, &parameter2, word1, word2, mistake2) == 0) {
        strcpy(param1, result1.cut_word);
        (*counter)++;
    }
    else {
        *mistake2=1;
        printf("ERROR: in line number %d param1 is invalid \n", line_number);
        return;
    }

    if (empty_line(result1.p) == 1) {
    *mistake2=1;
        printf("Error: in line number %d ',' expected\n", line_number);
        return;
    }

    /* Check for the second operand after the comma */
    if (*counter == 2) {
        if (empty_line(result1.p) == 0 && result1.p != NULL) {
            result2.cut_word = result1.p;
            remove_space(result2.cut_word);
            remove_space_from_end(result2.cut_word);

            if (check_valid_param(line, line_number, result2, counter, &parameter1, &parameter2, word1, word2, mistake2) == 0) {
                strcpy(param2, result2.cut_word);
            }
            else {
            *mistake2=1;
                printf("ERROR: in line number %d the parameters are invalid\n", line_number);
                return;
            }

            if (result2.cut_word != NULL) {
                addressing_method(number_operand.code, param1, param2, parameter1, parameter2, hptr, ic, label, label_name, object, word1, word2);
            }
            else {
            *mistake2=1;
                printf("ERROR: in line number %d missing parameter\n", line_number);
                return;
            }
        }
    }

}


/* one_operands - Processes instructions with one operand. */
void one_operands(char* line, word_and_p result, func_name_and_code number_operand, int* counter, label_ptr* hptr, int* ic, int label, char* label_name, char* object, int* word1, int* word2, int * mistake2, int line_number) {
    char func_name[7];
    char* param1 = NULL;
    char param2[31];
    int parameter1 = 0;
    int parameter2 = 0;
    word_and_p result1;
    int temp = 0;
   
    /* Increment the counter */
    (*counter)++;

    /* Remove leading spaces and get the operand */
    remove_space(result.cut_word);
    result1 = cut_word(&result.p, ' ');

     /* Check if the operand is valid */
   if (result1.cut_word == NULL || strlen(result1.cut_word) == 0) {
        result1.cut_word = malloc(strlen(result.p) + 1);
        if (result1.cut_word != NULL) {
            strcpy(result1.cut_word, result.p);
            result.p = NULL;
        }
        else {
            fprintf(stderr, "Memory allocation failed for result1.cut_word\n");
            return;
        }
    }

     /* Store the function name and validate the operand */
    strcpy(func_name, number_operand.func_name);


    if (result1.cut_word != NULL) {
        remove_space(result1.cut_word);
        remove_space_from_end(result1.cut_word);
    }

    /* Check if the operand is valid */
    if (check_valid_param(line, line_number, result1, counter, &parameter1, &parameter2, word1, word2, mistake2) == 0) {
        strcpy(param2, result1.cut_word);
        (*counter)++;


        /* Swap word1 and word2 for single operand instructions */
        temp = (*word2);
        (*word2) = (*word1);
        (*word1) = temp;

    }
    else {
    *mistake2=1;
        printf("ERROR: in line number %d the parameter is invalid\n",line_number);
        free(result1.cut_word);
        return;
    }

    /* Check if there is any extra data in the line */
    if (empty_line(result1.p) == 1 || result.p == NULL) {

    }
    else {
        printf("ERROR: in line number %d the parameter is invalid", line_number);
        free(result1.cut_word);
        return;
    }

    addressing_method(number_operand.code, param1, param2, parameter1, parameter2, hptr, ic, label, label_name, object, word1, word2);

}

/* zero_operands - Processes instructions with zero operands. */
void zero_operands(char* line, word_and_p result, func_name_and_code number_operand, int* counter, label_ptr* hptr, int* ic, int label, char* label_name, char* object, int* word1, int* word2, int * mistake2, int line_number)
{
    char* param1 = NULL;
    char* param2 = NULL;
    int parameter1 = 0;
    int parameter2 = 0;
   
    /* Check if the line is empty or there are extra parameters */
    if (empty_line(result.p) == 1 || result.p == NULL)
    {
         addressing_method(number_operand.code, param1, param2, parameter1, parameter2, hptr, ic, label, label_name, object, word1, word2);
    }
    else {
    *mistake2=1;
        printf("ERROR:in line number %d the parameters are invalid",line_number);
        return;
    }
}

/* addressing_method - Handles the addressing of operands and writes the appropriate machine code. */
void addressing_method(int func, char* p1, char* p2, int parameter1, int parameter2, label_ptr* hptr, int* ic, int label, char* label_name, char* object, int* word1, int* word2)
{
    string_and_int parame1; /* Holds operand 1 details */
    line_1 line;  /* Stores the final machine code */
    string_and_int parame2;  /* Holds operand 2 details */
    char param1_label[32];  /* Label for operand 1 */
    char param2_label[32];   /* Label for operand 2 */
    int num_of_operands = 0;  /* Number of operands (1 or 2) */
    parame1.type_operand = 0;
    parame2.type_operand = 0;


    /* Process operand 1 */
  if (p1 != NULL)
    {
        if (*p1 == '*')
        {
            strcpy(parame1.operand_name, p1 + 2);  /* Handle pointer */
            num_of_operands++;
            parame1.type_operand = parameter1;
        }
        else if (*p1 == '#')
        {
            if (p1[1] == '+' || p1[1] == '-')
            {
                strcpy(parame1.operand_name, p1 + 2); /* Handle immediate value */
            }
            else
            {
                strcpy(parame1.operand_name, p1 + 1);
            }
            num_of_operands++;
            parame1.type_operand = parameter1;

        }
        else if (*word1 == 1)
        {
            strcpy(parame1.operand_name, p1);  /* Handle label */
            num_of_operands++;
            strcpy(param1_label, p1);


        }
        else
        {
            strcpy(parame1.operand_name, p1 + 1);  /* Handle variable */
            num_of_operands++;

            parame1.type_operand = parameter1;

        }

        /* Process operand 2 if exists */
        if (*p2 == '*')
        {
            strcpy(parame2.operand_name, p2 + 2);  /* Handle pointer */
            num_of_operands++;
        }
        else if (*p2 == '#')
        {
            if (p2[1] == '+')
            {
                strcpy(parame2.operand_name, p2 + 2);  /* Handle immediate value */
            }
            else
            {
                strcpy(parame2.operand_name, p2 + 1);
            }
            num_of_operands++;
        }
        else if (*word2 == 1)
        {
            strcpy(parame2.operand_name, p2);  /* Handle label */
            num_of_operands++;
            strcpy(param2_label, p2);

        }
        else
        {
            strcpy(parame2.operand_name, p2 + 1);  /* Handle variable */
            num_of_operands++;
        }
        parame1.type_operand = parameter1;
        parame2.type_operand = parameter2;
 
            /* Write the machine code for the instruction */
        write_line_code(func, parame1, parame2, 4, num_of_operands, &line, ic, label, label_name, hptr, object, param1_label, param2_label);

    }

    else if (p1 == NULL && p2 != NULL)
    {
        if (*p2 == '*')
        {
            strcpy(parame2.operand_name, p2 + 2);
            num_of_operands++;
        }
        else if (*p2 == '#')
        {
            if (p2[1] == '+')
            {
                strcpy(parame2.operand_name, p2 + 2);

            }
            else
            {
                strcpy(parame2.operand_name, p2 + 1);
            }
            num_of_operands++;
        }
        else if (*word2 == 1)
        {
            strcpy(parame2.operand_name, p2);
            num_of_operands++;
            strcpy(param2_label, p2);
        }
        else
        {
            strcpy(parame2.operand_name, p2 + 1);
            num_of_operands++;
        }
        parame2.type_operand = parameter1;

       
     

        write_line_code(func, parame1, parame2, 4, num_of_operands, &line, ic, label, label_name, hptr, object, param1_label, param2_label);
    }
    else
    {
       
        write_line_code(func, parame1, parame2, 4, num_of_operands, &line, ic, label, label_name, hptr, object, param1_label, param2_label);
    }
}


