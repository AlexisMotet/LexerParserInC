#include "parser.h"
#include "visiteur.h"
#include <stdbool.h>
#include <stdlib.h>

void accepter_noeud(struct noeud * noeud, struct visiteur * visiteur);

void accepter_litteral(struct noeud_litteral noeud_litteral, struct visiteur * visiteur){
    (*visiteur->ptr_visiter_litteral)(noeud_litteral);
}

void accepter_variable(struct noeud_variable noeud_variable, struct visiteur * visiteur){
    (*visiteur->ptr_visiter_variable)(noeud_variable);
}

void accepter_unaire(struct noeud_unaire noeud_unaire, struct visiteur * visiteur){
    accepter_noeud(noeud_unaire.droite, visiteur);
    (*visiteur->ptr_visiter_unaire)(noeud_unaire);
}

void accepter_binaire(struct noeud_binaire noeud_binaire, struct visiteur * visiteur){
    accepter_noeud(noeud_binaire.droite, visiteur);
    accepter_noeud(noeud_binaire.gauche, visiteur);
    (*visiteur->ptr_visiter_binaire)(noeud_binaire);
}

void accepter_vide(struct noeud_vide noeud_vide, struct visiteur * visiteur){
    (*visiteur->ptr_visiter_vide)(noeud_vide);
}

void accepter_while(struct noeud_while noeud_while, struct visiteur * visiteur){
    accepter_noeud(noeud_while.condition, visiteur);
    accepter_noeud(noeud_while.corps, visiteur);
    (*visiteur->ptr_visiter_while)(noeud_while);
}

void accepter_if(struct noeud_if noeud_if, struct visiteur * visiteur){
    accepter_noeud(noeud_if.condition, visiteur);
    accepter_noeud(noeud_if.corps_if, visiteur);
    (*visiteur->ptr_visiter_if)(noeud_if);
}

void accepter_if_else(struct noeud_if_else noeud_if_else, struct visiteur * visiteur){
    accepter_noeud(noeud_if_else.condition, visiteur);
    accepter_noeud(noeud_if_else.corps_if, visiteur);
    accepter_noeud(noeud_if_else.corps_else, visiteur);
    (*visiteur->ptr_visiter_if_else)(noeud_if_else);
}

void accepter_assignation(struct noeud_assignation noeud_assignation, struct visiteur * visiteur){
    accepter_noeud(noeud_assignation.source, visiteur);
    accepter_noeud(noeud_assignation.cible, visiteur);
    (*visiteur->ptr_visiter_assignation)(noeud_assignation);
}

void accepter_declaration(struct noeud_declaration noeud_declaration, struct visiteur * visiteur){
    accepter_noeud(noeud_declaration.variable, visiteur);
    (*visiteur->ptr_visiter_declaration)(noeud_declaration);
}

void accepter_declarations(struct noeud_declarations noeud_declarations, struct visiteur * visiteur){
    struct declarations * d;
    d = noeud_declarations.d;
    while (d){
        accepter_noeud(d->declaration, visiteur);
        d = d->suivante; 
    }
    (*visiteur->ptr_visiter_declarations)(noeud_declarations);
}

void accepter_instructions(struct noeud_instructions noeud_instructions, struct visiteur * visiteur){
    struct instructions * l;
    l = noeud_instructions.l;
    while (l){
        accepter_noeud(l->instruction, visiteur);
        l = l->suivante; 
    }
    (*visiteur->ptr_visiter_instructions)(noeud_instructions);
}

void accepter_programme(struct noeud_programme noeud_programme, struct visiteur * visiteur){
    accepter_noeud(noeud_programme.declarations, visiteur);
    accepter_noeud(noeud_programme.corps, visiteur);
    (*visiteur->ptr_visiter_programme)(noeud_programme);
}

void accepter_noeud(struct noeud * noeud, struct visiteur * visiteur){
    switch (noeud->type){
        case type_noeud_vide :
            accepter_vide(noeud->contenu.noeud_vide, visiteur);
            break;
        case type_noeud_declarations :
            accepter_declarations(noeud->contenu.noeud_declarations, visiteur);
            break;
        case type_noeud_declaration :
            accepter_declaration(noeud->contenu.noeud_declaration, visiteur);
            break;
        case type_noeud_litteral :
            accepter_litteral(noeud->contenu.noeud_litteral, visiteur);
            break;
        case type_noeud_binaire :
            accepter_binaire(noeud->contenu.noeud_binaire, visiteur);
            break;
        case type_noeud_unaire :
            accepter_unaire(noeud->contenu.noeud_unaire, visiteur);
            break;
        case type_noeud_variable :
            accepter_variable(noeud->contenu.noeud_variable, visiteur);
            break;
        case type_noeud_while :
            accepter_while(noeud->contenu.noeud_while, visiteur);
            break;
        case type_noeud_if :
            accepter_if(noeud->contenu.noeud_if, visiteur);
            break;
        case type_noeud_if_else :
            accepter_if_else(noeud->contenu.noeud_if_else, visiteur);
            break;
        case type_noeud_assignation :
            accepter_assignation(noeud->contenu.noeud_assignation, visiteur);
            break;
        case type_noeud_instructions:
            accepter_instructions(noeud->contenu.noeud_instructions, visiteur);
            break;
        case type_noeud_programme :
            accepter_programme(noeud->contenu.noeud_programme, visiteur);
            break;
    }
}

