#include "mac.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define MAX_LABEL_LENGTH 31

/* Function to convert a binary string to an octal number */
char* binary_to_octal(const char* binary_str) {
    int decimal_value = 0;
    int base = 1;
    int i;
    char* octal_str = NULL;

    /* Convert binary to decimal */
    for (i = strlen(binary_str) - 1; i >= 0; i--) {
        if (binary_str[i] == '1') {
            decimal_value += base;
        }
        base *= 2;
    }

    /* Allocate enough space for the octal string */
    octal_str = malloc(6);  /* 5 digits for octal + '\0' */
    if (!octal_str) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    /* Convert decimal to octal with leading zeros to width of 5 */
    sprintf(octal_str, "%05o", decimal_value);

    return octal_str;
}



void process_file(const char *filename, label_ptr hptr, extern_ptr hpt, entry_ptr hp, int *mistake2, char * file_ext, int ic, int dc)
{
    FILE *file = fopen(filename, "r");
    FILE *temp_file = fopen("tempfile.txt", "w");
    char line[MAX_LINE_LENGTH];
    char label[MAX_LABEL_LENGTH];
    int line_number;
    char *question_mark = NULL;
    int address;
    char* binary_label;
    binary_label = malloc(16); 
    if (!binary_label) {
    printf("Memory allocation failed!\n");
    exit(1);
    }
/* Adds a header of the IC and DC number*/
    fprintf(temp_file, " %d    %d\n", ic,dc);

    if (!file || !temp_file)
    {
        printf("Error opening file(s).\n");
        return;
    }

    while (fgets(line, sizeof(line), file))
    {
        /* Remove newline character if present */
        line[strcspn(line, "\n")] = 0;

        /* Extract line number */
        sscanf(line, "%d", &line_number);

        if (strstr(line, "?") != NULL)
        {
            /* Find the position of the '?' character */
            question_mark = strchr(line, '?');
            if (question_mark)
            {
                /* Store the label name */
                strcpy(label, question_mark + 1);

                /* Check the label length */
                if (strlen(label) > MAX_LABEL_LENGTH)
                {
                    printf("Error: Label length exceeds %d characters.\n", MAX_LABEL_LENGTH);
                }
                else
                {
                    /* Find label address and print octal representation */
                    address = find_label_address(hptr, hpt, hp, label, mistake2);

                    if (address == 1)
                    {
                    
                    FILE *ext = fopen(file_ext, "a"); 

                    if (ext == NULL) {
                    printf("Error opening file!\n");
                    exit(0);
                    }
                    /* Codes the extern label to 1 */
                    print_binary_with_length(address, 12, binary_label);
                    fprintf(temp_file, "%d. %s\n", line_number, binary_to_octal(binary_label));
                    /*Prints the location of the label to the EXT file*/  
                    fprintf(ext, "%s   %d\n",label, line_number);

                    }
                    else if (address == -1 && *mistake2 == 1)
                    {
                        printf("Error: Label '%s' not found in any list.\n", label);
                        continue;  /* Skip this line */
                    }

                    else
                    {
                    char* strings[2];
                    char *combined;
                    char ARE[4];               
                    print_binary_with_length(address, 12, binary_label);
           
                    strcpy(ARE, "010");
                    strings[0] = binary_label;
                    strings[1] = ARE;
                    combined = concatenate_strings(2, strings);
                    /* Prints the label address and adds ARE to it */
                    fprintf(temp_file, "%d. %s\n", line_number, binary_to_octal(combined));
                    }
                }
            }
        }
        else
        {
            /* Get the binary part after the line number and convert to octal */
            char *binary_str = strchr(line, ' ') + 1;
            fprintf(temp_file, "%d. %s\n", line_number, binary_to_octal(binary_str));
        }
    }

    fclose(file);
    fclose(temp_file);
   /* fclose(ext); */
/* remove temp file*/
if (rename("tempfile.txt", filename) != 0) {
    /* If rename fails, remove the temp file */
    if (remove("tempfile.txt") != 0) {
        perror("Error removing temp file");
    }
}

}

/* Function to check for duplicate names between two lists */
void check_duplicates(entry_ptr hp, extern_ptr hpt, int *mistake2, const char *filename, label_ptr hptr) {
    entry_ptr e_ptr;
    extern_ptr ex_ptr;


    /* Check for duplicates in entry list against extern list */
    e_ptr = hp;
    while (e_ptr != NULL) {
        ex_ptr = hpt;
        while (ex_ptr != NULL) {
            /* Compare names between entry and extern lists */
            if (strcmp(e_ptr->name, ex_ptr->name) == 0) {
                printf("Error: Duplicate name '%s' found in both entry and extern lists.\n", e_ptr->name);
                *mistake2 = 1;
            }
            ex_ptr = ex_ptr->next;
        }
        e_ptr = e_ptr->next;
    }

    /* Check for duplicates within the entry list */
    e_ptr = hp;
    while (e_ptr != NULL) {
        entry_ptr inner_ptr;
        inner_ptr = e_ptr->next;
        while (inner_ptr != NULL) {
            /* Compare names within the entry list */
            if (strcmp(e_ptr->name, inner_ptr->name) == 0) {
                printf("Error: Duplicate entry name '%s' found in the entry list.\n", e_ptr->name);

            }
            inner_ptr = inner_ptr->next;
        }
        e_ptr = e_ptr->next;
    }

    /* Check for duplicates within the extern list */
    ex_ptr = hpt;
    while (ex_ptr != NULL) {
        extern_ptr inner_ptr;
        inner_ptr = ex_ptr->next;
        while (inner_ptr != NULL) {
            /* Compare names within the extern list */
            if (strcmp(ex_ptr->name, inner_ptr->name) == 0) {
                printf("Error: Duplicate extern name '%s' found in the extern list.\n", ex_ptr->name);
            }
            inner_ptr = inner_ptr->next;
        }
        ex_ptr = ex_ptr->next;
    }
}

int find_label_address(label_ptr hptr, extern_ptr hpt, entry_ptr hp, char* label, int *mistake2)
{
    label_ptr current_label = hptr;
    entry_ptr current_entry = hp;
    extern_ptr current_extern = hpt;

    /* Check in the label list */
    while (current_label != NULL) {
        if (strcmp(current_label->name, label) == 0) {
            return current_label->adress;  /* Fixed 'adress' to 'address' */
        }
        current_label = current_label->next;
    }

    /* Check in the entry list */
    while (current_entry != NULL) {
        if (strcmp(current_entry->name, label) == 0) {
            /* Check again in the label list for the address */
            current_label = hptr;
            while (current_label != NULL) {
                if (strcmp(current_label->name, label) == 0) {
                    return current_label->adress;  /* Return the address found in the label list */
                }
                current_label = current_label->next;
            }

            *mistake2 = 1;
            return -1;  /* Indicate an error */
        }
        current_entry = current_entry->next;
    }

    /* Check in the extern list */
    while (current_extern != NULL) {
        if (strcmp(current_extern->name, label) == 0) {            
            return 1;  /* Indicate that the label is extern */
        }
        current_extern = current_extern->next;
    }

    *mistake2 = 1;
    return -1;  /* Indicate an error */
}

/* Function to check if a label exists in the label list */
int label_exists(label_ptr label_list, const char* label_name) {
    label_ptr current = label_list;
    while (current != NULL) {
        if (strcmp(current->name, label_name) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

/* Function to check if all entry labels exist in the label list */
void validate_entry_labels(entry_ptr hp, label_ptr hptr) {
    entry_ptr e_ptr = hp;

    while (e_ptr != NULL) {
        if (!label_exists(hptr, e_ptr->name)) {
            /* Print an error message if the entry label is not found in the label list */
            printf("Error: Entry label '%s' not found in the label list.\n", e_ptr->name);
        }
        e_ptr = e_ptr->next;
    }

}


/* function to check duplicate labels between the list of labels and the list of external labels (extern) */
void check_for_duplicate_labels(label_ptr hptr, extern_ptr hpt, int *mistake2) {
    label_ptr label_current = hptr;

    /* Go through all the labels in the list of labels */
    while (label_current != NULL) {
        extern_ptr extern_current = hpt;
       
        /* Go through all labels in the extern list */
        while (extern_current != NULL) {
            if (strcmp(label_current->name, extern_current->name) == 0) {
                /* If a duplicate label is found, print an error message and update mistake */
                printf("Error: Label '%s' is found in both the label list and the extern list.\n", label_current->name);
                *mistake2 = 1; 
            }
            extern_current = extern_current->next;
        }
        label_current = label_current->next;
    }
}


