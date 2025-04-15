#ifndef header_visiteur_123
#define header_visiteur_123

#include "parser.h"
#include <stdbool.h>
#include <stdlib.h>


struct visiteur{
    void (*ptr_visiter_litteral)(struct noeud_litteral);
    void (*ptr_visiter_variable)(struct noeud_variable);
    void (*ptr_visiter_unaire)(struct noeud_unaire);
    void (*ptr_visiter_binaire)(struct noeud_binaire);
    void (*ptr_visiter_vide)(struct noeud_vide);
    void (*ptr_visiter_while)(struct noeud_while);
    void (*ptr_visiter_if)(struct noeud_if);
    void (*ptr_visiter_if_else)(struct noeud_if_else);
    void (*ptr_visiter_assignation)(struct noeud_assignation);
    void (*ptr_visiter_declaration)(struct noeud_declaration);
    void (*ptr_visiter_declarations)(struct noeud_declarations);
    void (*ptr_visiter_instructions)(struct noeud_instructions);
    void (*ptr_visiter_programme)(struct noeud_programme);
};


#endif