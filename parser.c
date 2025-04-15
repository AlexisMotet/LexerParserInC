#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "lexer.h"
#include "parser.h"

//https://github.com/holyboom/Clite/blob/master/Parser.java
//2. PARSER

void initialiser_noeud_litteral(struct noeud_litteral * noeud_litteral, enum type_litteral type, void * valeur){
    noeud_litteral->type = type;
    switch (type){
        case type_litteral_entier :
            noeud_litteral->valeur.entier = *((int *)valeur);
            break;
        case type_litteral_booleen :
            noeud_litteral->valeur.booleen = *((bool *)valeur);
            break;
        case type_litteral_chaine :
            noeud_litteral->valeur.chaine = *((char **)valeur);
            break;
        case type_litteral_flottant :
            noeud_litteral->valeur.flottant = *((struct flottant *)valeur);
            break;
    }
    noeud_litteral->ptr_accepter = &accepter_litteral;
}

void initialiser_noeud_variable(struct noeud_variable * noeud_variable, char* id){
    noeud_variable->id = id;
    noeud_variable->ptr_accepter = &accepter_variable;
}

void initialiser_noeud_unaire(struct noeud_unaire * noeud_unaire, enum operateur op, struct noeud * droite){
    noeud_unaire->op = op;
    noeud_unaire->droite = droite;
    noeud_unaire->ptr_accepter = &accepter_unaire;
}

void initialiser_noeud_binaire(struct noeud_binaire * noeud_binaire, enum operateur op, struct noeud * gauche, struct noeud * droite){
    noeud_binaire->op = op;
    noeud_binaire->droite = droite;
    noeud_binaire->gauche = gauche;
    noeud_binaire->ptr_accepter = &accepter_binaire;
}

void initialiser_noeud_vide(struct noeud_vide * noeud_vide, unsigned niveau_indentation){
    noeud_vide->niveau_indentation = niveau_indentation;
    noeud_vide->ptr_accepter = &accepter_vide;
}

void initialiser_noeud_while(struct noeud_while * noeud_while, struct noeud * condition, struct noeud * corps, unsigned niveau_indentation){
    noeud_while->condition = condition;
    noeud_while->corps = corps;
    noeud_while->niveau_indentation = niveau_indentation;
    noeud_while->ptr_accepter = &accepter_while;
}

void initialiser_noeud_if(struct noeud_if * noeud_if, struct noeud * condition, struct noeud * corps_if, unsigned niveau_indentation){
    noeud_if->condition = condition;
    noeud_if->corps_if = corps_if;
    noeud_if->niveau_indentation = niveau_indentation;
    noeud_if->ptr_accepter = &accepter_if;
}

void initialiser_noeud_if_else(struct noeud_if_else * noeud_if_else, struct noeud * condition, struct noeud * corps_if, struct noeud * corps_else, unsigned niveau_indentation){
    noeud_if_else->condition = condition;
    noeud_if_else->corps_if = corps_if;
    noeud_if_else->corps_else = corps_else;
    noeud_if_else->niveau_indentation = niveau_indentation;
    noeud_if_else->ptr_accepter = &accepter_if_else;
}

void initialiser_noeud_assignation(struct noeud_assignation * noeud_assignation, struct noeud * source, struct noeud * cible, unsigned niveau_indentation){
    noeud_assignation->source = source;
    noeud_assignation->cible = cible;
    noeud_assignation->niveau_indentation = niveau_indentation;
    noeud_assignation->ptr_accepter = &accepter_assignation;
}

void initialiser_noeud_declaration(struct noeud_declaration * noeud_declaration, enum type_variable type, struct noeud * variable, unsigned niveau_indentation){
    noeud_declaration->type = type;
    noeud_declaration->variable = variable;
    noeud_declaration->niveau_indentation = niveau_indentation;
    noeud_declaration->ptr_accepter = &accepter_declaration;
}

void initialiser_noeud_declarations(struct noeud_declarations * noeud_declarations, struct declarations * d){
    noeud_declarations->d = d;
    noeud_declarations->ptr_accepter = &accepter_declarations;
}

void initialiser_noeud_instructions(struct noeud_instructions * noeud_instructions,struct instructions * l){
    noeud_instructions->l = l;
    noeud_instructions->ptr_accepter = &accepter_instructions;
}

void initialiser_noeud_programme(struct noeud_programme * noeud_programme, struct noeud * declarations, struct noeud * corps){
    noeud_programme->declarations = declarations;
    noeud_programme->corps = corps;
    noeud_programme->ptr_accepter = &accepter_programme;
}

bool consommer_token(struct liste_tokens ** ptr_l_tokens, struct token * tok){
    if (!*ptr_l_tokens){
        return false;
    }
    *tok = (*ptr_l_tokens)->premier;
    *ptr_l_tokens = (*ptr_l_tokens)->suivant;
    return true;
}

bool voir_token_suivant(struct liste_tokens **ptr_l_tokens, struct token * tok){
    if (!*ptr_l_tokens){
        return false;
    }
    *tok = (*ptr_l_tokens)->premier;
    return true;
}

bool consommer_si_tok_svt_symbole(struct liste_tokens ** l_tokens, enum symbole symb){
    struct token tok_svt;
    if (!voir_token_suivant(l_tokens, &tok_svt)){
        return false;
    }
    if (tok_svt.type == type_token_symbole){
        if (tok_svt.valeur.symbole == symb){
            consommer_token(l_tokens, &tok_svt);
            return true;
        }
    }
    return false;
}

bool consommer_si_tok_svt_mot_cle(struct liste_tokens ** l_tokens, enum mot_cle mot_cle){
    struct token tok_svt;
    if (!voir_token_suivant(l_tokens, &tok_svt)){
        return false;
    }
    if (tok_svt.type == type_token_mot_cle){
        if (tok_svt.valeur.mot_cle == mot_cle){
            consommer_token(l_tokens, &tok_svt);
            return true;
        }
    }
    return false;
}

bool consommer_si_tok_svt_chaine(struct liste_tokens **l_tokens, char** chaine){
    struct token tok_svt;
    if (!voir_token_suivant(l_tokens, &tok_svt)){
        return false;
    }
    if (tok_svt.type == type_token_chaine){
        *chaine = tok_svt.valeur.chaine;
        consommer_token(l_tokens, &tok_svt);
        return true;
    }
    return false;
}

bool consommer_si_tok_svt_id(struct liste_tokens **l_tokens, char ** id){
    struct token tok_svt;
    if (!voir_token_suivant(l_tokens, &tok_svt)){
        return false;
    }
    if (tok_svt.type == type_token_identifiant){
        *id = tok_svt.valeur.identifiant;
        consommer_token(l_tokens, &tok_svt);
        return true;
    }
    return false;
}

bool consommer_si_tok_svt_entier(struct liste_tokens **l_tokens, long * entier){
    struct token tok_svt;
    if (!voir_token_suivant(l_tokens, &tok_svt)){
        return false;
    }
    if (tok_svt.type == type_token_entier){
        *entier = tok_svt.valeur.entier;
        consommer_token(l_tokens, &tok_svt);
        return true;
    }
    return false;
}

bool consommer_si_tok_svt_flottant(struct liste_tokens **l_tokens, struct flottant * flottant){
    struct token tok_svt;
    if (!voir_token_suivant(l_tokens, &tok_svt)){
        return false;
    }
    if (tok_svt.type == type_token_flottant){
        *flottant = tok_svt.valeur.flottant;
        consommer_token(l_tokens, &tok_svt);
        return true;
    }
    return false;
}

int recuperer_indentation(struct liste_tokens ** l_tokens){
    struct token tok_svt;
    if (!voir_token_suivant(l_tokens, &tok_svt)){
        return -1;
    }
    return tok_svt.niveau_indentation;
}

bool tester_si_tok_svt_instruction(struct liste_tokens **l_tokens){
    struct token tok_svt;
    if (!voir_token_suivant(l_tokens, &tok_svt)){
        return false;
    }
    if (tok_svt.type == type_token_identifiant){
        return true;
    }
    if (tok_svt.type == type_token_mot_cle){
        if (tok_svt.valeur.mot_cle == mot_cle_if){
            return true;
        }
        if (tok_svt.valeur.mot_cle == mot_cle_while){
            return true;
        }
    }
    if (tok_svt.type == type_token_symbole){
        if (tok_svt.valeur.symbole == symbole_aco_ouv){
            return true;
        }
        if (tok_svt.valeur.symbole == symbole_poi_vir){
            return true;
        }
    }
    return false;
}

bool tester_si_tok_svt_id(struct liste_tokens **l_tokens){
    struct token tok_svt;
    if (!voir_token_suivant(l_tokens, &tok_svt)){
        return false;
    }
    if (tok_svt.type == type_token_identifiant){
        return true;
    }
    return false;
}

bool tester_si_tok_svt_symbole(struct liste_tokens **l_tokens, enum symbole symb){
    struct token tok_svt;
    if (!voir_token_suivant(l_tokens, &tok_svt)){
        return false;
    }
    if (tok_svt.type == type_token_symbole){
        if (tok_svt.valeur.symbole == symb){
            return true;
        }
    }
    return false;
}

bool tester_si_tok_svt_mot_cle(struct liste_tokens **l_tokens, enum mot_cle mot_cle){
    struct token tok_svt;
    if (!voir_token_suivant(l_tokens, &tok_svt)){
        return false;
    }
    if (tok_svt.type == type_token_mot_cle){
        if (tok_svt.valeur.mot_cle == mot_cle){
            return true;
        }
    }
    return false;
}

bool tester_si_tok_svt_type(struct liste_tokens **l_tokens){
    struct token tok_svt;
    if (!voir_token_suivant(l_tokens, &tok_svt)){
        return false;
    }
    if (tok_svt.type == type_token_mot_cle){
        if (tok_svt.valeur.mot_cle == mot_cle_bool){
            return true;
        }
        if (tok_svt.valeur.mot_cle == mot_cle_int){
            return true;
        }
        if (tok_svt.valeur.mot_cle == mot_cle_char){
            return true;
        }
        if (tok_svt.valeur.mot_cle == mot_cle_float){
            return true;
        }
    }
    return false;

}

struct operateur_nom {
    enum operateur op;
    const char* nom;
};

const struct operateur_nom noms_operateurs [] = {
    {operateur_ass, " = "},
    {operateur_ega, " == "},
    {operateur_non_ega, " != "},
    {operateur_mns, " - "},
    {operateur_div, "/"},
    {operateur_eto, " * "},
    {operateur_pls, " + "},
    {operateur_ega_inf, " =< "},
    {operateur_ega_inf_stc, " < "},
    {operateur_ega_sup, " >= "},
    {operateur_ega_sup_stc, " > "},
    {operateur_ou, " || "},
    {operateur_et, " && "},
    {operateur_non, "!"},
    {operateur_FIN, NULL}
};

void afficher_operateur(enum operateur op){
    for (size_t i = 0; noms_operateurs[i].nom; i++){
        if (noms_operateurs[i].op == op){
            printf("%s", noms_operateurs[i].nom);
        }
    }
};

bool consommer_si_op_unaire(struct liste_tokens **l_tokens, enum operateur * op){
    struct token tok_svt;
    if (!voir_token_suivant(l_tokens, &tok_svt)){
        return false;
    }
    if (tok_svt.type == type_token_symbole){
        if (tok_svt.valeur.symbole == symbole_mns){
            consommer_token(l_tokens, &tok_svt);
            *op = operateur_mns;
            return true;
        }
        if (tok_svt.valeur.symbole == symbole_non){
            consommer_token(l_tokens, &tok_svt);
            *op = operateur_non;
            return true;
        }
    }
    return false;
}

bool consommer_si_op_multiplicateur(struct liste_tokens **l_tokens, enum operateur * op){
    struct token tok_svt;
    if (!voir_token_suivant(l_tokens, &tok_svt)){
        return false;
    }
    if (tok_svt.type == type_token_symbole){
        if (tok_svt.valeur.symbole == symbole_eto){
            consommer_token(l_tokens, &tok_svt);
            *op = operateur_eto;
            return true;
        }
        if (tok_svt.valeur.symbole == symbole_div){
            consommer_token(l_tokens, &tok_svt);
            *op = operateur_div;
            return true;
        }
    }
    return false;
}

bool consommer_si_op_additionneur(struct liste_tokens **l_tokens, enum operateur * op){
    struct token tok_svt;
    if (!voir_token_suivant(l_tokens, &tok_svt)){
        return false;
    }
    if (tok_svt.type == type_token_symbole){
        if (tok_svt.valeur.symbole == symbole_pls){
            consommer_token(l_tokens, &tok_svt);
            *op = operateur_pls;
            return true;
        }
        if (tok_svt.valeur.symbole == symbole_mns){
            consommer_token(l_tokens, &tok_svt);
            *op = operateur_mns;
            return true;
        }
    }
    return false;
}

bool consommer_si_op_inegalite(struct liste_tokens **l_tokens, enum operateur * op){
    struct token tok_svt;
    if (!voir_token_suivant(l_tokens, &tok_svt)){
        return false;
    }
    if (tok_svt.type == type_token_symbole){
        if (tok_svt.valeur.symbole == symbole_ega_sup){
            consommer_token(l_tokens, &tok_svt);
            *op = operateur_ega_sup;
            return true;
        }
        if (tok_svt.valeur.symbole == symbole_ega_sup_stc){
            consommer_token(l_tokens, &tok_svt);
            *op = operateur_ega_sup_stc;
            return true;
        }
        if (tok_svt.valeur.symbole == symbole_ega_inf){
            consommer_token(l_tokens, &tok_svt);
            *op = operateur_ega_inf;
            return true;
        }
        if (tok_svt.valeur.symbole == symbole_ega_inf_stc){
            consommer_token(l_tokens, &tok_svt);
            *op = operateur_ega_inf_stc;
            return true;
        }
    }
    return false;
}

bool consommer_si_op_egalite(struct liste_tokens **l_tokens, enum operateur * op){
    struct token tok_svt;
    if (!voir_token_suivant(l_tokens, &tok_svt)){
        return false;
    }
    if (tok_svt.type == type_token_symbole){
        if (tok_svt.valeur.symbole == symbole_ega){
            consommer_token(l_tokens, &tok_svt);
            *op = operateur_ega;
            return true;
        }
        if (tok_svt.valeur.symbole == symbole_non_ega){
            consommer_token(l_tokens, &tok_svt);
            *op = operateur_non_ega;
            return true;
        }
    }
    return false;
}

bool consommer_si_op_et(struct liste_tokens **l_tokens){
    struct token tok_svt;
    if (!voir_token_suivant(l_tokens, &tok_svt)){
        return false;
    }
    if (tok_svt.type == type_token_symbole){
        if (tok_svt.valeur.symbole == symbole_et){
            consommer_token(l_tokens, &tok_svt);
            return true;
        }
    }
    return false;
}

bool consommer_si_op_ou(struct liste_tokens **l_tokens){
    struct token tok_svt;
    if (!voir_token_suivant(l_tokens, &tok_svt)){
        return false;
    }
    if (tok_svt.type == type_token_symbole){
        if (tok_svt.valeur.symbole == symbole_ou){
            consommer_token(l_tokens, &tok_svt);
            return true;
        }
    }
    return false;
}

struct type_variable_nom {
    enum type_variable type_var;
    const char* nom;
};

const struct type_variable_nom noms_types_variables [] = {
    {type_variable_int, "int"},
    {type_variable_float, "float"},
    {type_variable_char, "char"},
    {type_variable_bool, "bool"},
    {type_variable_FIN, NULL}
};

void afficher_type_variable(enum type_variable type_var){
    for (size_t i = 0; noms_types_variables[i].nom; i++){
        if (noms_types_variables[i].type_var == type_var){
            printf("%s", noms_types_variables[i].nom);
        }
    }
};

void ajouter_instruction(struct instructions **l, struct noeud * instruction){
    if (*l==NULL){
        *l = malloc(sizeof(struct instructions));
        (*l)->instruction = instruction;
        (*l)->suivante = NULL;
    }
    else {
        ajouter_instruction(&((*l)->suivante), instruction);
    }
}

void ajouter_declaration(struct declarations **l, struct noeud * declaration){
    if (*l==NULL){
        *l = malloc(sizeof(struct declarations));
        (*l)->declaration = declaration;
        (*l)->suivante = NULL;
    }
    else {
        ajouter_declaration(&((*l)->suivante), declaration);
    }
}

bool prog_instructions(struct liste_tokens ** l_tokens, struct noeud ** noeud);
bool parser_declarations(struct liste_tokens ** l_tokens, struct noeud ** noeud);

bool programme(struct liste_tokens ** l_tokens, struct noeud ** noeud){
    if (!consommer_si_tok_svt_mot_cle(l_tokens, mot_cle_int)){
        abort();
    }
    if (!consommer_si_tok_svt_mot_cle(l_tokens, mot_cle_main)){
        abort();
    }
    if (!consommer_si_tok_svt_symbole(l_tokens, symbole_par_ouv)){
        abort();
    }
    if (!consommer_si_tok_svt_symbole(l_tokens, symbole_par_fer)){
        abort();
    }
    if (!consommer_si_tok_svt_symbole(l_tokens, symbole_aco_ouv)){
        abort();
    }
    struct noeud * declarations;
    parser_declarations(l_tokens, &declarations);
    struct noeud * corps;
    prog_instructions(l_tokens, &corps);
    struct noeud_programme noeud_programme;
    initialiser_noeud_programme(&noeud_programme, declarations, corps);
    *noeud = malloc(sizeof(struct noeud));
    (*noeud)->contenu.noeud_programme = noeud_programme;
    (*noeud)->type = type_noeud_programme;
    if (!consommer_si_tok_svt_symbole(l_tokens, symbole_aco_fer)){
         abort();
    }
    return true;
}

bool declaration(struct liste_tokens ** l_tokens, struct declarations ** d);
enum type_variable type(struct liste_tokens ** l_tokens);
bool instructions(struct liste_tokens ** l_tokens, struct noeud ** noeud);
bool instruction_while(struct liste_tokens ** l_tokens, struct noeud ** noeud);
bool instruction_if(struct liste_tokens ** l_tokens, struct noeud ** noeud);
bool assignation(struct liste_tokens ** l_tokens, struct noeud ** noeud);
bool expression(struct liste_tokens ** l_tokens, struct noeud ** noeud);
bool conjonction(struct liste_tokens ** l_tokens, struct noeud ** noeud); 
bool egalite(struct liste_tokens ** l_tokens, struct noeud ** noeud); 
bool addition(struct liste_tokens ** l_tokens, struct noeud ** noeud);
bool relation(struct liste_tokens ** l_tokens, struct noeud ** noeud);
bool terme(struct liste_tokens ** l_tokens, struct noeud ** noeud);
bool facteur(struct liste_tokens ** l_tokens, struct noeud ** noeud);
bool primaire(struct liste_tokens ** l_tokens, struct noeud ** noeud);
bool litteral(struct liste_tokens ** l_tokens, struct noeud ** noeud);
bool variable(struct liste_tokens ** l_tokens, struct noeud ** noeud);

bool parser_declarations(struct liste_tokens ** l_tokens, struct noeud ** noeud){
    struct declarations * d = NULL;
    while(tester_si_tok_svt_type(l_tokens)){
        declaration(l_tokens, &d);
    }
    struct noeud_declarations noeud_declarations;
    initialiser_noeud_declarations(&noeud_declarations, d);
    *noeud = malloc(sizeof(struct noeud));
    (*noeud)->contenu.noeud_declarations = noeud_declarations;
    (*noeud)->type = type_noeud_declarations;
    return true;
}

enum type_variable type(struct liste_tokens ** l_tokens){
    if (consommer_si_tok_svt_mot_cle(l_tokens, mot_cle_bool)){
        return type_variable_bool;
    }
    if (consommer_si_tok_svt_mot_cle(l_tokens, mot_cle_int)){
        return type_variable_int;
    }
    if (consommer_si_tok_svt_mot_cle(l_tokens, mot_cle_char)){
        return type_variable_char;
    }
    if (consommer_si_tok_svt_mot_cle(l_tokens, mot_cle_float)){
        return type_variable_float;
    }
    return -1;
}

bool declaration(struct liste_tokens ** l_tokens, struct declarations ** d){
    int niveau_indentation = recuperer_indentation(l_tokens);
    enum type_variable type_variable = type(l_tokens);
    struct noeud * var;
    variable(l_tokens, &var);
    struct noeud_declaration noeud_declaration;
    initialiser_noeud_declaration(&noeud_declaration, type_variable, var, niveau_indentation);
    struct noeud * declaration = malloc(sizeof(struct noeud));
    declaration->contenu.noeud_declaration = noeud_declaration;
    declaration->type = type_noeud_declaration;
    ajouter_declaration(d, declaration);
    while (consommer_si_tok_svt_symbole(l_tokens, symbole_vir)){
        variable(l_tokens, &var);
        initialiser_noeud_declaration(&noeud_declaration, type_variable, var, niveau_indentation);
        declaration = malloc(sizeof(struct noeud));
        declaration->contenu.noeud_declaration = noeud_declaration;
        declaration->type = type_noeud_declaration;
        ajouter_declaration(d, declaration);
    }
    if (consommer_si_tok_svt_symbole(l_tokens, symbole_poi_vir)){
        return true;
    }
    return false;
}

bool instruction(struct liste_tokens ** l_tokens, struct noeud ** noeud){
    int niveau_indentation = recuperer_indentation(l_tokens);
    if (consommer_si_tok_svt_symbole(l_tokens, symbole_poi_vir)){
        struct noeud_vide noeud_vide;
        initialiser_noeud_vide(&noeud_vide, niveau_indentation);
        *noeud = malloc(sizeof(struct noeud));
        (*noeud)->type = type_noeud_vide;
        (*noeud)->contenu.noeud_vide = noeud_vide;
        return true;
    }
    else if (tester_si_tok_svt_symbole(l_tokens, symbole_aco_ouv)){
        instructions(l_tokens, noeud);
        return true;
    }
    else if (tester_si_tok_svt_mot_cle(l_tokens, mot_cle_while)){
        instruction_while(l_tokens, noeud);
        return true;
    }
    else if (tester_si_tok_svt_mot_cle(l_tokens, mot_cle_if)){
        instruction_if(l_tokens, noeud);
        return true;
    }
    else if (tester_si_tok_svt_id(l_tokens)){
        assignation(l_tokens, noeud);
        return true;
    }
    abort();
}

bool instructions(struct liste_tokens ** l_tokens, struct noeud ** noeud){
    if (!consommer_si_tok_svt_symbole(l_tokens, symbole_aco_ouv)){
        return false;
    }
    struct instructions * l = NULL;
    while(tester_si_tok_svt_instruction(l_tokens)){
        struct noeud * noeud_instruction;
        instruction(l_tokens, &noeud_instruction);
        ajouter_instruction(&l, noeud_instruction);
    }
    if (!consommer_si_tok_svt_symbole(l_tokens, symbole_aco_fer)){
        return false;
    }
    struct noeud_instructions noeud_instructions;
    initialiser_noeud_instructions(&noeud_instructions, l);
    *noeud = malloc(sizeof(struct noeud));
    (*noeud)->contenu.noeud_instructions = noeud_instructions;
    (*noeud)->type = type_noeud_instructions;
    return true;
}

bool prog_instructions(struct liste_tokens ** l_tokens, struct noeud ** noeud){
    struct instructions * l = NULL;
    while(tester_si_tok_svt_instruction(l_tokens)){
        struct noeud * noeud_instruction;
        instruction(l_tokens, &noeud_instruction);
        ajouter_instruction(&l, noeud_instruction);
    }
    struct noeud_instructions noeud_instructions;
    initialiser_noeud_instructions(&noeud_instructions, l);
    *noeud = malloc(sizeof(struct noeud));
    (*noeud)->contenu.noeud_instructions = noeud_instructions;
    (*noeud)->type = type_noeud_instructions;
    return true;
}

bool assignation(struct liste_tokens ** l_tokens, struct noeud ** noeud){
    int niveau_indentation = recuperer_indentation(l_tokens);
    struct noeud * source;
    struct noeud * cible;
    if (!variable(l_tokens, &cible)){
        return false;
    }
    if (!consommer_si_tok_svt_symbole(l_tokens, symbole_ass)){
        abort();
    }
    expression(l_tokens, &source);
    if (!consommer_si_tok_svt_symbole(l_tokens, symbole_poi_vir)){
        abort();
    }
    struct noeud_assignation noeud_assignation;
    initialiser_noeud_assignation(&noeud_assignation, source, cible, niveau_indentation);
    *noeud = malloc(sizeof(struct noeud));
    (*noeud)->contenu.noeud_assignation = noeud_assignation;
    (*noeud)->type = type_noeud_assignation;
    return true;
}

bool instruction_if(struct liste_tokens ** l_tokens, struct noeud ** noeud){
    int niveau_indentation = recuperer_indentation(l_tokens);
    struct noeud * corps_if;
    struct noeud * condition;
    if (!consommer_si_tok_svt_mot_cle(l_tokens, mot_cle_if)){
        return false;
    }
    if (!consommer_si_tok_svt_symbole(l_tokens, symbole_par_ouv)){
        //par ouv
        abort();
    }
    expression(l_tokens, &condition);
    if (!consommer_si_tok_svt_symbole(l_tokens, symbole_par_fer)){
        //par fer
        abort();
    }
    instruction(l_tokens, &corps_if);
    if (consommer_si_tok_svt_mot_cle(l_tokens, mot_cle_else)){
        struct noeud * corps_else;
        instruction(l_tokens, &corps_else);
        struct noeud_if_else noeud_if_else;
        initialiser_noeud_if_else(&noeud_if_else, condition, corps_if, corps_else, niveau_indentation);
        *noeud = malloc(sizeof(struct noeud));
        (*noeud)->contenu.noeud_if_else = noeud_if_else;
        (*noeud)->type = type_noeud_if_else;
    }
    else {
        struct noeud_if noeud_if;
        initialiser_noeud_if(&noeud_if, condition, corps_if, niveau_indentation);
        *noeud = malloc(sizeof(struct noeud));
        (*noeud)->contenu.noeud_if =  noeud_if;
        (*noeud)->type = type_noeud_if;
    }
    return true;
}

bool instruction_while(struct liste_tokens ** l_tokens, struct noeud ** noeud){
    int niveau_indentation = recuperer_indentation(l_tokens);
    struct noeud * corps;
    struct noeud * condition;
    if (!consommer_si_tok_svt_mot_cle(l_tokens, mot_cle_while)){
        return false;
    }
    if (!consommer_si_tok_svt_symbole(l_tokens, symbole_par_ouv)){
        //par ouv
        abort();
    }
    expression(l_tokens, &condition);
    if (!consommer_si_tok_svt_symbole(l_tokens, symbole_par_fer)){
        //par fer
        abort();
    }
    instruction(l_tokens, &corps);
    struct noeud_while noeud_while;
    initialiser_noeud_while(&noeud_while, condition, corps, niveau_indentation);
    *noeud = malloc(sizeof(struct noeud));
    (*noeud)->contenu.noeud_while = noeud_while;
    (*noeud)->type = type_noeud_while;
    return true;
}

bool expression(struct liste_tokens ** l_tokens, struct noeud ** noeud){
    conjonction(l_tokens, noeud);
    struct noeud * droite;
    while (consommer_si_op_ou(l_tokens)){
        conjonction(l_tokens, &droite);
        struct noeud_binaire noeud_binaire;
        initialiser_noeud_binaire(&noeud_binaire, operateur_ou, *noeud, droite);
        *noeud = malloc(sizeof(struct noeud));
        (*noeud)->contenu.noeud_binaire = noeud_binaire;
        (*noeud)->type = type_noeud_binaire; 
    }
    return true;
}


bool conjonction(struct liste_tokens ** l_tokens, struct noeud ** noeud){
    egalite(l_tokens, noeud);
    struct noeud * droite;
    while (consommer_si_op_et(l_tokens)){
        egalite(l_tokens, &droite);
        struct noeud_binaire noeud_binaire;
        initialiser_noeud_binaire(&noeud_binaire, operateur_et, *noeud, droite);
        *noeud = malloc(sizeof(struct noeud));
        (*noeud)->contenu.noeud_binaire = noeud_binaire;
        (*noeud)->type = type_noeud_binaire;
    }  
    return true;
}

bool egalite(struct liste_tokens ** l_tokens, struct noeud ** noeud){
    relation(l_tokens, noeud);
    struct noeud * droite;
    enum operateur op;
    while (consommer_si_op_egalite(l_tokens, &op)){
        relation(l_tokens, &droite);
        struct noeud_binaire noeud_binaire;
        initialiser_noeud_binaire(&noeud_binaire, op, *noeud, droite);
        *noeud = malloc(sizeof(struct noeud));
        (*noeud)->contenu.noeud_binaire = noeud_binaire;
        (*noeud)->type = type_noeud_binaire;
    }
    return true;
}

bool relation(struct liste_tokens ** l_tokens, struct noeud ** noeud){
    addition(l_tokens, noeud);
    enum operateur op;
    struct noeud * droite;
    while (consommer_si_op_inegalite(l_tokens, &op)){
        addition(l_tokens, &droite);
        struct noeud_binaire noeud_binaire;
        initialiser_noeud_binaire(&noeud_binaire, op, *noeud, droite);
        *noeud = malloc(sizeof(struct noeud));
        (*noeud)->contenu.noeud_binaire = noeud_binaire;
        (*noeud)->type = type_noeud_binaire;
    }
    return true;
}

bool addition(struct liste_tokens ** l_tokens, struct noeud ** noeud){
    terme(l_tokens, noeud);
    struct noeud * droite;
    enum operateur op;
    while (consommer_si_op_additionneur(l_tokens, &op)){
        terme(l_tokens, &droite);
        struct noeud_binaire noeud_binaire;
        initialiser_noeud_binaire(&noeud_binaire, op, *noeud, droite);
        *noeud = malloc(sizeof(struct noeud));
        (*noeud)->contenu.noeud_binaire = noeud_binaire;
        (*noeud)->type = type_noeud_binaire;
    }
    return true;
}

bool terme(struct liste_tokens ** l_tokens, struct noeud ** noeud){
    facteur(l_tokens, noeud);
    struct noeud * droite;
    enum operateur op;
    while (consommer_si_op_multiplicateur(l_tokens, &op)){
        facteur(l_tokens, &droite);
        struct noeud_binaire noeud_binaire;
        initialiser_noeud_binaire(&noeud_binaire, op, *noeud, droite);
        *noeud = malloc(sizeof(struct noeud));
        (*noeud)->contenu.noeud_binaire = noeud_binaire;
        (*noeud)->type = type_noeud_binaire;
        
    }
    return true;
}

bool facteur(struct liste_tokens ** l_tokens, struct noeud ** noeud){
    enum operateur op;
    if (consommer_si_op_unaire(l_tokens, &op)){
        struct noeud * droite;
        primaire(l_tokens, &droite);
        struct noeud_unaire noeud_unaire;
        initialiser_noeud_unaire(&noeud_unaire, op, droite);
        *noeud = malloc(sizeof(struct noeud));
        (*noeud)->contenu.noeud_unaire = noeud_unaire;
        (*noeud)->type = type_noeud_unaire;
    }
    else {
        primaire(l_tokens, noeud);
    }
    return true;
}


bool primaire(struct liste_tokens ** l_tokens, struct noeud ** noeud){
    if (variable(l_tokens, noeud)){
        return true;
    }
    else if (litteral(l_tokens, noeud)){
        return true;
    }
    else if (consommer_si_tok_svt_symbole(l_tokens, symbole_par_ouv)){
        if (expression(l_tokens, noeud)){
            if (consommer_si_tok_svt_symbole(l_tokens, symbole_par_fer)){
                return true;
            }
        }
    }
    return false;
}

bool litteral(struct liste_tokens ** l_tokens, struct noeud ** noeud){
    *noeud = malloc(sizeof(struct noeud));
    (*noeud)->type = type_noeud_litteral;
    char* chaine;
    long entier;
    struct flottant flottant;
    bool booleen;
    struct noeud_litteral noeud_litteral;
    if (consommer_si_tok_svt_chaine(l_tokens, &chaine)){
        initialiser_noeud_litteral(&noeud_litteral, type_litteral_chaine, (void *) &chaine);
        (*noeud)->contenu.noeud_litteral = noeud_litteral;
        return true;
    }
    else if (consommer_si_tok_svt_entier(l_tokens, &entier)){
        initialiser_noeud_litteral(&noeud_litteral, type_litteral_entier, (void *) &entier);
        (*noeud)->contenu.noeud_litteral = noeud_litteral;
        return true;
    }
    else if (consommer_si_tok_svt_flottant(l_tokens, &flottant)){
        initialiser_noeud_litteral(&noeud_litteral, type_litteral_flottant, (void *) &flottant);
        (*noeud)->contenu.noeud_litteral = noeud_litteral;
        return true;
    }
    else if (consommer_si_tok_svt_mot_cle(l_tokens, mot_cle_false)){
        booleen = false;
        initialiser_noeud_litteral(&noeud_litteral, type_litteral_booleen, (void *) &booleen);
        (*noeud)->contenu.noeud_litteral = noeud_litteral;
        return true;
        
    }
    else if (consommer_si_tok_svt_mot_cle(l_tokens, mot_cle_true)){
        booleen = true;
        initialiser_noeud_litteral(&noeud_litteral, type_litteral_booleen, (void *) &booleen);
        (*noeud)->contenu.noeud_litteral = noeud_litteral;
        return true;
    }
    return false;
}

bool variable(struct liste_tokens ** l_tokens, struct noeud ** noeud){
    char * id;
    if (consommer_si_tok_svt_id(l_tokens, &id)){
        struct noeud_variable noeud_variable;
        initialiser_noeud_variable(&noeud_variable, id);
        *noeud = malloc(sizeof(struct noeud));
        (*noeud)->contenu.noeud_variable = noeud_variable;  
        (*noeud)->type = type_noeud_variable;
        return true;
    }
    return false;
}

void reindenter(unsigned nouveau_niveau_indentation){
    for (unsigned i = 0; i<nouveau_niveau_indentation; i++){
        printf(" ");
    }
}

void parcourir_arbre(struct noeud * noeud){ 
    switch (noeud->type){
        struct declarations * d;
        struct instructions * l;
        case type_noeud_declarations :
            d = noeud->contenu.noeud_declarations.d;
            while (d){
                parcourir_arbre(d->declaration);
                d = d->suivante; 
            }
            break;
        case type_noeud_declaration :
            reindenter(noeud->contenu.noeud_declaration.niveau_indentation);
            afficher_type_variable(noeud->contenu.noeud_declaration.type);
            printf(" ");
            parcourir_arbre(noeud->contenu.noeud_declaration.variable);
            printf(";\n");
            break;
        case type_noeud_litteral :
            if (noeud->contenu.noeud_litteral.type == type_litteral_booleen){
                if (noeud->contenu.noeud_litteral.valeur.booleen == false){
                    printf("false");
                }
                else {
                    printf("true");
                }
            }
            else if (noeud->contenu.noeud_litteral.type == type_litteral_chaine){
                printf("'%s'", noeud->contenu.noeud_litteral.valeur.chaine);
                
            }
            else if (noeud->contenu.noeud_litteral.type == type_litteral_entier){
                printf("%ld", noeud->contenu.noeud_litteral.valeur.entier);
            }
            else if (noeud->contenu.noeud_litteral.type == type_litteral_flottant){
                printf(
                    "%ld.%s", 
                    noeud->contenu.noeud_litteral.valeur.flottant.partie_entiere, 
                    noeud->contenu.noeud_litteral.valeur.flottant.partie_fractionnaire);
            }
            break;
        case type_noeud_binaire :
            parcourir_arbre(noeud->contenu.noeud_binaire.gauche);
            afficher_operateur(noeud->contenu.noeud_binaire.op);
            parcourir_arbre(noeud->contenu.noeud_binaire.droite);
            break;
        case type_noeud_unaire :
            printf("(");
            afficher_operateur(noeud->contenu.noeud_unaire.op);
            parcourir_arbre(noeud->contenu.noeud_unaire.droite);
            printf(")");
            break;
        case type_noeud_variable :
            printf("%s", noeud->contenu.noeud_variable.id);
            break;
        case type_noeud_vide : 
            reindenter(noeud->contenu.noeud_vide.niveau_indentation);
            printf(";\n");
            break;
        case type_noeud_while :
            reindenter(noeud->contenu.noeud_while.niveau_indentation);
            printf("while (");
            parcourir_arbre(
                noeud->contenu.noeud_while.condition);
            printf("){\n");
            parcourir_arbre(
                noeud->contenu.noeud_while.corps);
            reindenter(noeud->contenu.noeud_if.niveau_indentation);
            printf("}\n");
            break;
        case type_noeud_if :
            reindenter(noeud->contenu.noeud_if.niveau_indentation);
            printf("if (");
            parcourir_arbre(noeud->contenu.noeud_if.condition);
            printf("){\n");
            parcourir_arbre(noeud->contenu.noeud_if.corps_if);
            reindenter(noeud->contenu.noeud_if.niveau_indentation);
            printf("}\n");
            break;
        case type_noeud_if_else :
            reindenter(noeud->contenu.noeud_if_else.niveau_indentation);
            printf("if (");
            parcourir_arbre(noeud->contenu.noeud_if_else.condition);
            printf("){\n");
            parcourir_arbre(noeud->contenu.noeud_if_else.corps_if);
            printf(" \n}\nelse{\n");
            parcourir_arbre(noeud->contenu.noeud_if_else.corps_else);
            reindenter(noeud->contenu.noeud_if.niveau_indentation);
            printf("}\n");
            break;
        case type_noeud_assignation :
            reindenter(noeud->contenu.noeud_assignation.niveau_indentation);
            parcourir_arbre(noeud->contenu.noeud_assignation.cible);
            afficher_operateur(operateur_ass);
            parcourir_arbre(noeud->contenu.noeud_assignation.source);
            printf(";\n");
            break;
        case type_noeud_instructions:
            l = noeud->contenu.noeud_instructions.l;
            while (l){
                parcourir_arbre(l->instruction);
                l = l->suivante; 
            }
            break;
        case type_noeud_programme :
            printf("int main(){\n");
            parcourir_arbre(noeud->contenu.noeud_programme.declarations);
            parcourir_arbre(noeud->contenu.noeud_programme.corps);
            printf("}\n");
            break;
    }
}


