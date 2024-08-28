

#include "mac.h"

/* Function declarations
void remove_space(char* p);
void valid_number(char* p, int* dc, int line_number, ptr_data *h);
void extern_or_entry(char* line, int line_number);
void addlist_data(ptr_data* hptr, int temp_num, int* dc);
void printlist_datalist(ptr_data h);
int isPrintable(char ch);
void valid_string(char* p, int* dc, int line_number, ptr_data *h);
char remove_space_from_end(char *p);
void Updates_data(char* line, int line_number, int* dc, int* mistake);
*/

/* Function to remove trailing spaces and return the last non-space character */
char remove_space_from_end(char* p) {
    char* end;
    char last_char = '\0';

    /* Set end to the last character of the string*/
    end = p + strlen(p) - 1;

    /* Find the last non-space character*/
    while (end >= p && isspace((unsigned char)*end)) {
        end--;
    }

    /* Store the last non-space character*/
    if (end >= p) {
        last_char = *end;
    }

    /* Null terminate the string after the last non-space character*/
    *(end + 1) = '\0';

    return last_char;
}



/*return if char is print */
int isPrintable(char ch) {
    return isprint(ch);
}


void Updates_data(ptr_data* data,char* line, int line_number, int* dc, int* mistake, int* ic, char* object, entry_ptr *hp, extern_ptr *hpt, int label_flag, char * label_name, label_ptr* hptr, int * mistake2) {
   
    word_and_p result;
    remove_space(line);
 
    result = cut_word(&line, ' ');
   
       

    if (result.cut_word == NULL) {
        *mistake2 = 1;
        return;
    }
   
/* if string send him to fuction that check validity*/
    if (strcmp(result.cut_word, ".string") == 0) {
        remove_space(result.p);
        remove_space_from_end(result.p);
        valid_string(result.p, dc, line_number, data, ic, object, label_flag, label_name, hptr, mistake2);

       
    }
    /* if data send him to fuction that check validity*/
    else if (strcmp(result.cut_word, ".data") == 0) {
        remove_space(result.p);

        valid_number(result.p, dc, line_number, data, ic, object, label_flag, label_name, hptr, mistake2);
       

    }
    /* if extern or entry send him to fuction that check validity*/
    else {
        extern_or_entry(line, line_number, hp, hpt, mistake2);
    }

    free(result.cut_word);
}


void valid_string(char* p, int* dc, int line_number, ptr_data* h, int* ic, char* object, int label_flag, char * label_name, label_ptr* hptr, int* mistake2)
{

    list_label *t;
    char last_char = remove_space_from_end(p);
    int prev_dc=*dc;
    
        /*relaod the address label */
        t = *hptr;
        if (label_flag == 1) {
            while (t) {
                if (strcmp(t->name, label_name) == 0) {
                    if(prev_dc==0)
                    {t->adress=9999;}
                    else{
                    t->adress = prev_dc*10000;}
                    break;
                }
                t = t->next;
            }
        }
        
    /*Checks if the first and last characters are a '"' */
    if (*p != '"' || last_char != '"')
    {
        *mistake2=1;
        printf("Error: line number %d The syntax is incorrect String must begin and end with \"\n", line_number);
        return;
    }
   
    p = p + 1;
    while (*p != '"' || *p == '\0')
    {
        if (isPrintable(*p)) {
            addlist_data(h, (int)*p, dc, ic, object, label_flag, label_name,hptr);
            p++;
        }
        else {
            *mistake2=1;
            printf("Error: in line %d the character number is invalid\n", line_number);
            return;
        }
    }

    addlist_data(h, '\0', dc, ic, object, label_flag, label_name,hptr);

}

/* Function to validate and process numbers in the data */
void valid_number(char* p, int* dc, int line_number, ptr_data* h, int* ic, char* object, int label_flag, char* label_name, label_ptr* hptr, int * mistake2) {

    int buffer = 0;
    int temp_num;
    int condition_met;
    int sign = 1;
    list_label *t;
    int prev_dc;
    prev_dc=*dc;
   
/*relaod the address label */
t = *hptr;
if (label_flag == 1) {
while (t) {
if (strcmp(t->name, label_name) == 0) {
if(prev_dc==0)
{t->adress=9999;}
else{
t->adress = prev_dc*10000;}
break;
}
t = t->next;
}
}
        
        
        
    while (*p != '\0' && *p != '\n') {
        condition_met = 0;

        remove_space(p);


        if (*p == '\0' || *p == '\n')
        {
            return;
        }

        if (*p == ',') {
            if (buffer == 1) {
                buffer = 0;
                condition_met = 1;
                p++;
            }
            else {
                printf("Error: line number %d has an unnecessary comma\n", line_number);
                *mistake2=1;
                return;
            }
        }
        else if ((*p >= '0' && *p <= '9') || *p == '-' || *p == '+') {

            if (*p == '-') {
                sign = -1;
                p++;
            }
            if (*p == '+') {
                sign = 1;
                p++;
            }
            temp_num = *p - '0';
            buffer = 1;


            p++;
            while (*p >= '0' && *p <= '9') {
                temp_num = temp_num * 10 + (*p - '0');
                p++;
            }
            temp_num *= sign;

            if (temp_num > MAX_INTEGER_VALUE || temp_num < MIN_INTEGER_VALUE)
            {
                printf("Error: In line %d number the number %d is out of range\n", line_number, temp_num);
                *mistake2=1;
                return;
            }
            addlist_data(h, temp_num, dc, ic, object, label_flag, label_name,hptr); /* בפונקציה valid_number*/

            condition_met = 1;
        }

        if (!condition_met) {
            printf("Error: Line number %d has an invalid character\n", line_number);
            *mistake2=1;
            return;
        }
    }
}

void addlist_data(ptr_data* h, int temp_num, int* dc, int* ic, char* object, int label_flag, char* label_name, label_ptr* hptr)
{
   
    ptr_data p1 = *h;

    ptr_data t = (ptr_data)malloc(sizeof(data_image));


   
    if (!t) {
        printf("\n cannot build list \n");
        exit(0);
    }

    t->dc = *dc;
    t->val = temp_num;
    t->next = NULL;
    (*dc)++;
    if (*h == NULL) {
        *h = t;
    } else {
        while (p1->next != NULL) {
            p1 = p1->next;
        }
        p1->next = t;
    }
   
}

void extern_or_entry(char* line, int line_number, entry_ptr *hp, extern_ptr *hpt, int * mistake2)
{
    word_and_p result;
   
    char name[81];
    result = cut_word(&line, ' ');
    if (result.cut_word == NULL)
    {
        printf("Line %d is wrong, entry/extern\n", line_number);
        *mistake2=1;
        return;
    }
    if (empty_line(result.p) == 1)
    {
        printf("Error: In line number %d there is an error in entry or extern\n", line_number);
        *mistake2=1;
        return;
    }
    remove_space(result.p);
    remove_space_from_end(result.p);
    if (strcmp(result.cut_word, ".entry") == 0) {

        strcpy(name, result.p);
        add2list_entry(hp, name);
    }
    else if (strcmp(result.cut_word, ".extern") == 0) {
         strcpy(name, result.p);

        add2list_extern(hpt,  name);
    }
    else
    {
    *mistake2=1;
    printf("ERROR: in line number %d the name of the instuction is unvalid", line_number);
    return;
    }
    return;
}




/* Function to save and print binary representation of data line */
void save_and_print_binary_line_data(ptr_data h, int* ic, char* object, int label_flag, char * label_name, label_ptr* hptr) {
    char num_binary[16];  /* 15 bits for the number + 1 for null terminator */
    if (!object) {
        printf("Error opening the file for writing.\n");
        return;
    }
    (*ic)++;
    while (h) {
       
        print_binary_with_length(h->val, 15, num_binary);  /* Convert number to 15-bit binary */

        /* copy the formatted line to the file */
        copy_to_file(num_binary, object, ic);

        /* Move to the next node in the list */
        h = h->next;
        (*ic)++;
    }
}


