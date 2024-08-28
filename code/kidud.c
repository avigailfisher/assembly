#include "mac.h"

/* Function Names in the Provided File:
1. write_line_code
2. write_line2_code
3. write_line3_code
4. print_binary
5. print_binary_with_length
6. save_and_print_binary_line1
7. save_and_print_binary_line_immediate
8. save_and_print_binary_line_register
9. save_and_print_binary_line_label
*/



/* Function to write the line code */
void write_line_code(int func, string_and_int op_source, string_and_int op_dest, int ARE, int num_of_operands, line_1* line, int* ic, int label_flag, char* label_name, label_ptr* hptr, char* object, char *param1_label, char* param2_label)
{
   
   

    if (!object) {
        fprintf(stderr, "Error opening the file for writing.\n");
        return;
    }
   

    line->func = func;
    line->ARE = ARE;


    (*ic)++;
   



    if (num_of_operands == 0) {
        line->op_source.type_operand = 0;
        line->op_dest.type_operand = 0;
        save_and_print_binary_line1(*line, ic, object);
        return;
    }
    else if (num_of_operands == 1) {
        line->op_source.type_operand = 0;
        line->op_dest = op_dest;
        save_and_print_binary_line1(*line, ic, object);
        write_line3_code(func, op_source, op_dest, ARE, num_of_operands, ic, object, label_name, param2_label);
    }
    else {
        line->op_source = op_source;
        line->op_dest = op_dest;
        save_and_print_binary_line1(*line, ic, object);
        write_line2_code(func, op_source, op_dest, ARE, num_of_operands, ic, object, label_name,param1_label, param2_label);
    }
    return;
}


/* Function to write the second line code */
void write_line2_code(int func, string_and_int op_source, string_and_int op_dest, int ARE, int num_of_operands, int* ic, char* object, char *label,  char *param1_label, char* param2_label) {
   

    (*ic)++;


    if (op_source.type_operand == 1) {
        line_immediate line;
        line.ARE = ARE;
        line.num = atoi(op_source.operand_name); /* Convert string to integer */

        save_and_print_binary_line_immediate(line, ic, object);
    }
    else if (op_source.type_operand == 2) {
        line_label line;
        line.first = '?';
        strcpy(line.label_n, param1_label);

        save_and_print_binary_line_label(line, ic, object);
    }
    else if (op_source.type_operand == 8 || op_source.type_operand == 4) {
        line_register line;
        line.ARE = 4;
        line.num = atoi(op_source.operand_name); /* Convert string to integer */
        line.num2 = 0;
        line.start = 0;
       

        if (num_of_operands == 1) {
            save_and_print_binary_line_register(line, ic, object);
            return;
        }
        if (num_of_operands == 2 && (op_dest.type_operand == 4 || op_dest.type_operand == 8)) {
            line.num2 = atoi(op_dest.operand_name); /* Convert string to integer */
            save_and_print_binary_line_register(line, ic, object);
            return;
        } else {
            save_and_print_binary_line_register(line, ic, object);
        }
    }

    if (num_of_operands == 2) {
        write_line3_code(func, op_source, op_dest, ARE, num_of_operands, ic, object, label,param2_label);
    }
}

/* Function to write the third line code */
void write_line3_code(int func, string_and_int op_source, string_and_int op_dest, int ARE, int num_of_operands, int* ic, char* object, char * label, char* param2_label) {

    (*ic)++;


    if (op_dest.type_operand == 1) {
        line_immediate line;
        line.ARE = ARE;
        line.num = atoi(op_dest.operand_name); /* Convert string to integer */

        save_and_print_binary_line_immediate(line, ic, object);
    }
    else if (op_dest.type_operand == 2) {
        line_label line;
        line.first = '?';
        strcpy(line.label_n, param2_label);

        save_and_print_binary_line_label(line, ic, object);
    }
    else if (op_dest.type_operand == 4 || op_dest.type_operand == 8) {
        line_register line;
        line.ARE = 4;
        line.num = 0;
        line.num2 = atoi(op_dest.operand_name); /* Convert string to integer */
        line.start = 0;

        save_and_print_binary_line_register(line, ic, object);
    }
}

/* Function to print the line in binary format */
void print_binary(line_1 line) {
    printf("Printing binary for line:\n");
    printf("func = %d\n", line.func);
    printf("op_source.type_operand = %d\n", line.op_source.type_operand);
    printf("op_dest.type_operand = %d\n", line.op_dest.type_operand);
    printf("ARE = %d\n", line.ARE);
}

/* Function to print a number in binary with a given length */
void print_binary_with_length(int num, int length, char* binary_str) {
    int i;
    int bit;
    for (i = length - 1; i >= 0; i--) {
        bit = (num >> i) & 1;
        binary_str[length - 1 - i] = bit + '0';
    }
    binary_str[length] = '\0';
}

/* Function to save and print binary representation of line_1 */
void save_and_print_binary_line1(line_1 line, int* ic, char* object) {
    char func_binary[5];
    char op_source_binary[5];
    char op_dest_binary[5];
    char ARE_binary[4];
    char* combined;
    char* strings[4];

    print_binary_with_length(line.func, 4, func_binary);
    print_binary_with_length(line.op_source.type_operand, 4, op_source_binary);
    print_binary_with_length(line.op_dest.type_operand, 4, op_dest_binary);
    print_binary_with_length(line.ARE, 3, ARE_binary);

    strings[0] = func_binary;
    strings[1] = op_source_binary;
    strings[2] = op_dest_binary;
    strings[3] = ARE_binary;
    combined = concatenate_strings(4, strings);


    copy_to_file(combined, object, ic); /* Change "output.txt" to your desired filename */
}

/* Function to save and print binary representation of line_immediate */
void save_and_print_binary_line_immediate(line_immediate line, int* ic, char* object) {
    char num_binary[13];
    char ARE_binary[4];
    char* combined;
    char* strings[2];

    print_binary_with_length(line.num, 12, num_binary);
    print_binary_with_length(line.ARE, 3, ARE_binary);

    strings[0] = num_binary;
    strings[1] = ARE_binary;
    combined = concatenate_strings(2, strings);


    copy_to_file(combined, object, ic); /* Change "output.txt" to your desired filename */
}

 /* Function to save and print binary representation of line_register */
    void save_and_print_binary_line_register(line_register line, int* ic, char* object) {
        char num_binary[4];
        char num2_binary[4];
        char ARE_binary[4];
        char start_binary[7]; /* Create binary string for start */
        char* combined;
        char* strings[4];

        print_binary_with_length(line.start, 6, start_binary); /* Convert start */
        print_binary_with_length(line.num, 3, num_binary);     /* Convert num */
        print_binary_with_length(line.num2, 3, num2_binary);   /* Convert num2 */
        print_binary_with_length(line.ARE, 3, ARE_binary);     /* Convert ARE */

        strings[0] = start_binary;
        strings[1] = num_binary;
        strings[2] = num2_binary;
        strings[3] = ARE_binary;
        combined = concatenate_strings(4, strings);


        copy_to_file(combined, object, ic); /* Change "output.txt" to your desired filename */
    }




void save_and_print_binary_line_label(line_label line, int* ic, char* object) {
    char label_n_binary[32];  
    char first_char[2];
    char* combined;
    char* strings[2];

 
    if (strlen(line.label_n) < sizeof(label_n_binary)) {
        strncpy(label_n_binary, line.label_n, sizeof(label_n_binary) - 1);
        label_n_binary[sizeof(label_n_binary) - 1] = '\0';  
    } else {
        fprintf(stderr, "Error: label_n length exceeds buffer size.\n");
        return;  
    }
   

    first_char[0] = line.first;
    first_char[1] = '\0';

    strings[0] = first_char;
    strings[1] = label_n_binary;
    combined = concatenate_strings(2, strings);


    copy_to_file(combined, object, ic);  /* Change "output.txt" to your desired filename */
}

