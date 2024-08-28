#include "mac.h"

#define MAX_LINE_LENGTH 80


int main(int argc, char* argv[]) {
    int i;
    int c;
    int line_number = 0;
    int mistake = 0;
    int mistake2=0;
    int size_word;
    size_t len;
    char* filename;
    char* dest_file;
    char copy_line_for[80];
    char* copy_line_for_name;
    char line[MAX_LINE_LENGTH + 2];
    char* object;
    FILE* source;
    FILE *ent ;
    char* result;
    char * file_ext;
    char * file_ent;
    word_and_p first_word;
    word_and_p start_word;
    word_and_p result_after_cut_label;
    int ic = 99;
    ptr h = NULL; 
    label_ptr hptr = NULL;
    entry_ptr hp = NULL;
    extern_ptr hpt = NULL;
    int dc;
    int label_flag;
    char* label_name;
    int length;
    char* strings[2];
    char* strings1[2];
    char* strings2[2];
    ptr_data data;
    label_ptr s;
    entry_ptr entry;
    label_ptr p;


    data=NULL;
    dc = 0;
    label_flag=0;
    
   
/* if missing files */
    if (argc < 2) {
        printf("missing files\n");
        return EXIT_FAILURE;
    }
/* for each file */
for (i = 1; i < argc; i++) {

/* Generates names for the files */       
       strings[0] = argv[i];
       strings[1] = ".as";
       
       strings1[0] = argv[i];
       strings1[1] = ".ext";
       
       strings2[0] = argv[i];
       strings2[1] = ".ent";

       result = concatenate_strings(2, strings); 
       file_ext = concatenate_strings(2, strings1); 
       file_ent = concatenate_strings(2, strings2); 

/* open the source file */
       source = fopen(result, "r");
       filename = result;
       
        if (source == NULL) {
            perror("Error opening source file");
            printf("Failed to open file: %s\n", result); 
            continue; /* Move to the next file */
        }
        
        /*opening file to am and ob*/
        dest_file = generate_dest_filename(argv[i], "am");
        
        

        /* Check if dest_file is valid and non-null */
        if (dest_file == NULL) { 
            printf("generate_dest_filename returned NULL for input: %s\n", argv[i]);
            fclose(source);
            continue;
        }
/* Processes each line in the file */
        while (fgets(line, sizeof(line), source) != NULL) {
            len = strlen(line);

            /* Check if the line is too long */
            if (len == MAX_LINE_LENGTH + 1 && line[MAX_LINE_LENGTH] != '\n') {
                printf("Line %d is too long: %s in file %s\n", line_number, line, filename);
                mistake = 1;
                /* Skip the rest of the long line */
                while ((c = fgetc(source)) != '\n' && c != EOF);
                continue;
            }

            /* Remove trailing newline character if it's there */
            if (len > 0 && line[len - 1] == '\n') {
                line[len - 1] = '\0';
            }

  
            if (empty_line(line)) {
                continue; /* Skip empty lines */
            }
            remove_space(line);
            /*Comment line*/
           if(*line== ';')
    {
        continue;
    }
            if (len <= MAX_LINE_LENGTH) {
                line_number++;
                process_line(line, &h, source, filename, dest_file, &line_number, &mistake);
            }
        }

        fclose(source);
/* If there is an error, delete the created file and move on to the next file */
        if (mistake == 1) {
            if (remove(dest_file) == 0) {
                continue;
            }
            else {
                perror("Error deleting file");
            }
        }

        object = generate_dest_filename(argv[i], "ob"); 
        /* we start the second maavar */
        source = fopen(dest_file, "r");
        if (source == NULL) {
            perror("Error opening source file\n");
            printf("\nFailed to open file: %s\n", dest_file); 
            continue; /* Move to the next file */
        }


        filename = dest_file;
        line_number = 1;

/* Goes through the entire file */
while (fgets(line, sizeof(line), source) != NULL) {
    if (empty_line(line)) {
        continue; /* Skip empty lines */
        }
            replace_tabs_with_spaces(line);
            remove_space(line);
            strcpy(copy_line_for, line);

            copy_line_for_name = copy_line_for;
            
            /* start word save the num of the line */
            start_word = cut_word(&copy_line_for_name, ' ');
            remove_space(start_word.p);
            /* start word.p point to all the line without the number */

            /* first word save the first word in the command */
            first_word = cut_word(&start_word.p, ' ');

            if (first_word.cut_word == NULL)
            {
                mistake2=1;
                first_word.cut_word = start_word.cut_word;
            }
            size_word = strlen(first_word.cut_word);


/*procces instuction*/
/*if it label --> */
if (first_word.cut_word[size_word - 1] == ':')
    {
        label_flag = 1;
        label_name = first_word.cut_word;
        length = strlen(label_name);
        label_name[length - 1] = '\0';
        remove_space(first_word.p);
        result_after_cut_label = cut_word(&first_word.p, ' ');
        /* procces the instruction after the label */
        if (process_line_label(line, source, line_number, first_word, &hptr, h, &mistake2) == 0) {
            /* check if it data entry or extern */
            if (result_after_cut_label.cut_word[0] == '.')
                    {
                        Updates_data(&data,first_word.p, line_number, &dc, &mistake, &ic, object, &hp, &hpt, label_flag, label_name,&hptr, &mistake2);
                    }
            /*check if it is name of act */   
            else {  
                  check_name_act(first_word.p, line_number, &hptr, label_flag, label_name, &ic, object, &mistake2);
                }

                }
            }

    /* check if it data entry or extern */
    else if (first_word.cut_word[0] == '.') {
            Updates_data(&data,start_word.p, line_number, &dc, &mistake, &ic, object, &hp, &hpt, label_flag, label_name,&hptr, &mistake2);
            line_number++;
            }
    /* check if it act name */
    else {
            check_name_act(start_word.p, line_number, &hptr, 0, NULL, &ic, object, &mistake2);
            line_number++;
        }
}

/* relaod the address label */
s=hptr;
while (s) {
    /*if the flag of data label is turn on*/
    if (s->adress>9000) {
        s->adress = s->adress/10000+1+ic;
        }
        s = s->next;
}
        
/*save the data*/
save_and_print_binary_line_data(data, &ic, object, label_flag, label_name, &hptr);

/* AddressSanitizer: SEGV */
 if(ic+dc+100 > 4096)
 {
 printf("ERROR: the number of unstruction is out of the range\n");
 mistake2=1;
 freelist(&h);
freelist_label(&hptr);
freelist_entry(&hp);
freelist_extern(&hpt);
 return EXIT_SUCCESS;
 }

 /*if there are error the file delet */
        
if (mistake2 == 1) {
    if (remove(object) == 0) {
        freelist(&h);
        freelist_label(&hptr);
        freelist_entry(&hp);
        freelist_extern(&hpt);
        return EXIT_SUCCESS;
        }
        else {
        perror("Error deleting file");
            }
        }
        
    check_duplicates(hp, hpt, &mistake2, filename, hptr);
    validate_entry_labels( hp, hptr);
    check_for_duplicate_labels(hptr, hpt, &mistake2);
    process_file(object, hptr, hpt, hp, &mistake2, file_ext, ic-100-dc, dc);
    
 /*----------------- entry file ---------------------*/
if(hp)
{
ent = fopen(file_ent, "a"); 
entry = hp;


while (entry) { 
    p = hptr; 
    while (p) { 
        if (strcmp(entry->name, p->name) == 0) { 
            fprintf(ent, "%s   %d\n", p->name, p->adress); 
            break; 
        }
        p = p->next;
    }
    if (!p) {
    }
    entry = entry->next;
}
 fclose(ent);
}

        fclose(source); 
}
freelist(&h);
freelist_label(&hptr);
freelist_entry(&hp);
freelist_extern(&hpt);
return EXIT_SUCCESS;
}



