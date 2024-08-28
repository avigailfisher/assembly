#include "mac.h"

#define MAX_LINE_LENGTH 81
#define MAX_LABEL_LENGTH 31

/*
 * Function Declarations:
 *
 * process_line_label - Processes a line of text to validate and add a label.
 * remove_last_char - Removes the trailing colon character from the label.
 * check_length_label - Checks if the label length is within the allowed limit.
 * check_first_char - Checks if the first character of the label is alphabetic.
 * check_other_characters - Checks if the rest of the characters in the label are alphanumeric.
 */

/* Process the line to validate and add a label */
int process_line_label(char *line, FILE *source, int line_number, word_and_p first_word, label_ptr *hptr, ptr h, int *mistake2)
{
    /* Local variables */
    char name_label[MAX_LINE_LENGTH];
    int len_first_word = strlen(first_word.cut_word);
    int len_label = 0;

    /* If the first word in the line is not empty */
    if (first_word.cut_word != NULL)
    {
        /* Copy the first word into name_label */
        strncpy(name_label, first_word.cut_word, len_first_word);
        name_label[len_first_word] = '\0';
        len_label = len_first_word;

        /* Validate the label */
        if (check_length_label(name_label, len_first_word - 1, mistake2, line_number) == 0 &&
            remove_last_char(name_label, len_label) == 0 &&
            check_first_char(name_label, mistake2, line_number) == 0 &&
            check_other_characters(name_label, len_first_word - 1, mistake2, line_number) == 0 &&
            Differen_from_a_command_name(name_label, &line_number, mistake2) == 0)
        {
            /* If the label is valid, add it to the label list */
            add2list_label(hptr, name_label, h);
            return 0;  /* Valid label */
        }
    }

    *mistake2 = 1; /* Invalid label */
    return 1;
}

/* Remove the trailing colon character from the label */
int remove_last_char(char *name_label, int len_label)
{
    /* If the last character is a colon, remove it */
    if (len_label > 0 && name_label[len_label - 1] == ':')
    {
        name_label[len_label - 1] = '\0';
    }
    return 0;
}

/* Check if the label length is valid */
int check_length_label(char *name_label, int len_label, int *mistake2, int line_number)
{
    /* Validate label length */
    if (len_label <= MAX_LABEL_LENGTH)
    {
        return 0; /* Length is valid */
    }
    *mistake2 = 1;
    printf("Error: In line number %d, Label length is invalid.\n", line_number);
    return 1; /* Length is invalid */
}

/* Check if the first character of the label is alphabetic */
int check_first_char(char *name_label, int *mistake2, int line_number)
{
    /* Check if the label is NULL */
    if (name_label == NULL)
    {
        *mistake2 = 0;
        printf("Error: In line number %d, Label is NULL.\n", line_number);
        return 1;
    }

    /* Check if the label is an empty string */
    if (name_label[0] == '\0')
    {
        *mistake2 = 0;
        printf("Error: In line number %d, an empty string.\n", line_number);
        return 1;
    }

    /* Check if the first character is alphabetic */
    if (isalpha(name_label[0]))
    {
        return 0; /* First character is valid */
    }
    else
    {
        *mistake2 = 1;
        printf("Error: In line number %d, Label starts with a non-alphabetic character.\n", line_number);
        return 1; /* First character is invalid */
    }
}

/* Check if the remaining characters in the label are alphanumeric */
int check_other_characters(char *name_label, int len_label, int *mistake2, int line_number)
{
    int i;
    /* Check if all characters (except the first one) are alphanumeric */
    for (i = 1; i < len_label; i++)
    {
        if (!isalnum(name_label[i]))
        {
            *mistake2 = 1;
            printf("Error: In line number %d, Label contains an invalid character.\n", line_number);
            return 1; /* Invalid character found */
        }
    }
    return 0; /* All characters are valid */
}
