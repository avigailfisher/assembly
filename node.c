#include "mac.h"

/* the names of the function in this file --->
 * 1. add2list
 * 2. printlist
 * 3. searchnode
 * 4. freelist
 * 5. freelist_label
 * 6. add2list_label
 * 7. printlist_label
 * 8. return_line_num
 * 9. add2list_entry_or_externy
 * 10. print_entry_or_externy_list1
 * 11. print_entry_or_externy_list
 */


void add2list(ptr *hptr, char *name, char *content, int line_num)
{

    ptr p1, t;



    t = (ptr)malloc(sizeof(macro_list));
    if (!t) {
        printf("\n cannot build list \n");
        exit(0);
    }
/* Allocate memory and copy the name */
    t->name_macro = malloc(strlen(name) + 1);
    if (!t->name_macro) {
        printf("\n cannot allocate memory for name \n");
        exit(0);
    }
strcpy(t->name_macro, name);

/* Allocate memory and copy the content */
    t->content = malloc(strlen(content) + 1);


    if (!t->content) {
        printf("\n cannot allocate memory for content \n");
        exit(0);
    }

 strcpy(t->content, content);

    t->next = NULL;
   
 t->line_num = line_num; /* Store the line number as an int */



    if (*hptr == NULL) {
        *hptr = t;
    } else {
        p1 = *hptr;

        while (p1->next != NULL) {
            p1 = p1->next;
        }
        p1->next = t;
    }

}



char *searchnode(ptr h, char *name_macro) {
    while (h) {
        if (strcmp(h->name_macro, name_macro) == 0) {
            return h->content;
        }
        h = h->next;
    }
    return NULL;
}

void freelist(ptr *hptr) {
    ptr p;
    while (*hptr) {
        p = *hptr;
        *hptr = (*hptr)->next;
        free(p->name_macro);
        free(p->content);
        free(p);
    }
}

void freelist_label(label_ptr *hptr) {
    label_ptr p;
    while (*hptr) {
        p = *hptr;
        *hptr = (*hptr)->next;
        free(p->name);
        free(p);
    }
}

void add2list_label(label_ptr *hptr, char *name_label, ptr h) {
    label_ptr p;
    label_ptr t;
ptr macro_p = h;
    p = *hptr;

    while (p) {
        if (strcmp(p->name, name_label) == 0) {
            printf("ERROR:  A node with this label already exists.\n");
            return;
        }
        p = p->next;
    }

 
    while (macro_p) {
        if (strcmp(macro_p->name_macro, name_label) == 0) {
            printf("Error: Label name '%s' is the same as an existing macro name.\n", name_label);
            break;
        }
        macro_p = macro_p->next;
    }
    t = malloc(sizeof(list_label));
    if (t == NULL) {
        perror("Failed to allocate memory");
        return;
    }
    t->name = malloc(strlen(name_label) + 1);
    if (t->name == NULL) {
        perror("Failed to allocate memory for name");
        free(t);
        return;
    }
    strcpy(t->name, name_label);
    t->adress = 0;
    t->next = NULL;

    if (*hptr == NULL) {
        *hptr = t;
    } else {
        p = *hptr;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = t;
    }

}




int return_line_num(ptr h, char *name_macro) {
    while (h) {
        if (strcmp(h->name_macro, name_macro) == 0) {

            return h->line_num;
        }
        h = h->next;
    }
    return 0;
}


void add2list_entry(entry_ptr *hptr, char* name){
    entry_ptr p;
    entry_ptr t;

    p = *hptr;
while (p) {
        if (strcmp(p->name, name) == 0) {
            printf(" A node with this entry already exists.\n");
            return;
        }
        p = p->next;
    }  
    t = malloc(sizeof(entry_node));
    if (t == NULL) {
        perror("Failed to allocate memory");
        return;
    }
    t->name = malloc(strlen(name) + 1);
    if (t->name == NULL) {
        perror("Failed to allocate memory for name");
        free(t);
        return;
    }
    strcpy(t->name, name);

    t->next = NULL;

    if (*hptr == NULL) {
        *hptr = t;
    } else {
        p = *hptr;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = t;
    }

}


void freelist_entry(entry_ptr *hptr) {
    entry_ptr p;
    while (*hptr) {
        p = *hptr;
        *hptr = (*hptr)->next;
        free(p->name);
        free(p);
    }
}



void add2list_extern(extern_ptr *hptr, char* name){
    extern_ptr p;
    extern_ptr t;

    p = *hptr;   
    t = malloc(sizeof(extern_node));
    if (t == NULL) {
        perror("Failed to allocate memory");
        return;
    }
    t->name = malloc(strlen(name) + 1);
    if (t->name == NULL) {
        perror("Failed to allocate memory for name");
        free(t);
        return;
    }
    strcpy(t->name, name);

    t->next = NULL;

    if (*hptr == NULL) {
        *hptr = t;
    } else {
        p = *hptr;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = t;
}
}

void freelist_extern(extern_ptr *hptr) {
 extern_ptr p;
    while (*hptr) {
        p = *hptr;
        *hptr = (*hptr)->next;
        free(p->name);
        free(p);
    }
}

