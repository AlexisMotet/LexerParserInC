#ifndef header_parser_visiteur_123
#define header_parser_visiteur_123

#include "lexer.h"
#include <stdbool.h>
#include <stdlib.h>

struct noeud_litteral;
struct noeud_variable;
struct noeud_unaire;
struct noeud_binaire;
struct noeud_vide;
struct noeud_while;
struct noeud_assignation;
struct noeud_instructions;
struct noeud_programme;
struct noeud_if;
struct noeud_if_else;
struct noeud_declaration;
struct noeud_declarations;

struct visiteur;
struct noeud;

void accepter_noeud(struct noeud * noeud, struct visiteur * visiteur);

void accepter_litteral(struct noeud_litteral noeud_litteral, struct visiteur * visiteur);

void accepter_variable(struct noeud_variable noeud_variable, struct visiteur * visiteur);

void accepter_unaire(struct noeud_unaire noeud_unaire, struct visiteur * visiteur);

void accepter_binaire(struct noeud_binaire noeud_binaire, struct visiteur * visiteur);

void accepter_vide(struct noeud_vide noeud_vide, struct visiteur * visiteur);

void accepter_while(struct noeud_while noeud_while, struct visiteur * visiteur);

void accepter_if(struct noeud_if noeud_if, struct visiteur * visiteur);

void accepter_if_else(struct noeud_if_else noeud_if_else, struct visiteur * visiteur);

void accepter_assignation(struct noeud_assignation noeud_assignation, struct visiteur * visiteur);

void accepter_declaration(struct noeud_declaration noeud_declaration, struct visiteur * visiteur);

void accepter_declarations(struct noeud_declarations noeud_declarations, struct visiteur * visiteur);

void accepter_instructions(struct noeud_instructions noeud_instructions, struct visiteur * visiteur);

void accepter_programme(struct noeud_programme noeud_programme, struct visiteur * visiteur);

enum type_noeud {
    type_noeud_litteral,
    type_noeud_binaire,
    type_noeud_unaire,
    type_noeud_variable,
    type_noeud_vide,
    type_noeud_while,
    type_noeud_if,
    type_noeud_if_else,
    type_noeud_assignation,
    type_noeud_instructions,
    type_noeud_programme,
    type_noeud_declaration,
    type_noeud_declarations,
};

enum operateur{
    operateur_ass,
    operateur_ega,
    operateur_non_ega,
    operateur_mns,
    operateur_div,
    operateur_eto,
    operateur_pls,
    operateur_ega_inf,
    operateur_ega_inf_stc,
    operateur_ega_sup,
    operateur_ega_sup_stc,
    operateur_ou,
    operateur_et,
    operateur_non,
    operateur_FIN,
};

enum type_litteral{
    type_litteral_chaine,
    type_litteral_entier,
    type_litteral_booleen,
    type_litteral_flottant,
};

struct noeud_litteral {
    enum type_litteral type;
    union {
        struct flottant flottant;
        char* chaine;
        long entier;
        bool booleen;
    } valeur;
    void (*ptr_accepter)(struct noeud_litteral, struct visiteur *);
};

enum type_variable {
    type_variable_int,
    type_variable_bool,
    type_variable_char,
    type_variable_float,
    type_variable_FIN,
};

struct noeud_variable {
    char * id;
    enum type_variable type;
    void (*ptr_accepter)(struct noeud_variable, struct visiteur *);
};

struct noeud_unaire {
    enum operateur op;
    struct noeud * droite;
    void (*ptr_accepter)(struct noeud_unaire, struct visiteur *);
};

struct noeud_binaire {
    enum operateur op;
    struct noeud * droite;
    struct noeud * gauche;
    void (*ptr_accepter)(struct noeud_binaire, struct visiteur *);
};

struct noeud_vide {
    unsigned niveau_indentation;
    unsigned numero_ligne;
    void (*ptr_accepter)(struct noeud_vide, struct visiteur *);
};

struct noeud_while {
    struct noeud * condition;
    struct noeud * corps;
    unsigned niveau_indentation;
    unsigned numero_ligne;
    void (*ptr_accepter)(struct noeud_while, struct visiteur *);
};

struct noeud_if {
    struct noeud * condition;
    struct noeud * corps_if;
    unsigned niveau_indentation;
    unsigned numero_ligne;
    void (*ptr_accepter)(struct noeud_if, struct visiteur *);
};

struct noeud_if_else {
    struct noeud * condition;
    struct noeud * corps_if;
    struct noeud * corps_else;
    unsigned niveau_indentation;
    unsigned numero_ligne;
    void (*ptr_accepter)(struct noeud_if_else, struct visiteur *);
};

struct noeud_assignation {
    struct noeud * source;
    struct noeud * cible;
    unsigned niveau_indentation;
    unsigned numero_ligne;
    void (*ptr_accepter)(struct noeud_assignation, struct visiteur *);
};

struct noeud_declaration{
    enum type_variable type;
    struct noeud * variable;
    unsigned niveau_indentation;
    unsigned numero_ligne;
    void (*ptr_accepter)(struct noeud_declaration, struct visiteur *);
};

struct noeud_declarations {
    struct declarations *d;
    void (*ptr_accepter)(struct noeud_declarations, struct visiteur *);
};

struct noeud_instructions {
    struct instructions * l;
    void (*ptr_accepter)(struct noeud_instructions, struct visiteur *);
};

struct noeud_programme {
    struct noeud * declarations;
    struct noeud * corps;
    void (*ptr_accepter)(struct noeud_programme, struct visiteur *);
};

struct noeud {
    enum type_noeud type;
    union {
        struct noeud_litteral noeud_litteral;
        struct noeud_variable noeud_variable;
        struct noeud_unaire noeud_unaire;
        struct noeud_binaire noeud_binaire;
        struct noeud_vide noeud_vide;
        struct noeud_while noeud_while;
        struct noeud_assignation noeud_assignation;
        struct noeud_instructions noeud_instructions;
        struct noeud_programme noeud_programme;
        struct noeud_if noeud_if;
        struct noeud_if_else noeud_if_else;
        struct noeud_declaration noeud_declaration;
        struct noeud_declarations noeud_declarations;
    } contenu;
};

struct instructions {
    struct noeud * instruction;
    struct instructions * suivante;
};

struct declarations {
    struct noeud * declaration;
    struct declarations * suivante;
};

bool programme(struct liste_tokens ** l_tokens, struct noeud ** noeud);
void parcourir_arbre(struct noeud * noeud);

#endif