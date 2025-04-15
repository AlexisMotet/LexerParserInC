#ifndef header_lexer_123
#define header_lexer_123

#include <stddef.h>

enum mot_cle {
    mot_cle_main,

    mot_cle_int,
    mot_cle_char,
    mot_cle_bool,
    mot_cle_float,


    mot_cle_if,
    mot_cle_else,
    mot_cle_while,

    mot_cle_true,
    mot_cle_false,

    mot_cle_FIN,
};

struct mot_cle_nom {
    enum mot_cle s;
    const char* nom;
};

extern const struct mot_cle_nom noms_mots_cles [];

//SYMBOLES
//ATTENTION

enum symbole {
    symbole_non,
    symbole_poi_vir,
    symbole_et,
    symbole_ou,
    symbole_div,
    symbole_eto,
    symbole_ass,
    symbole_ega, 
    symbole_non_ega, 
    symbole_ega_sup_stc,
    symbole_ega_inf_stc,
    symbole_ega_inf,
    symbole_ega_sup,
    symbole_pls,
    symbole_mns,
    symbole_aco_ouv,
    symbole_aco_fer,
    symbole_cro_ouv,
    symbole_cro_fer,
    symbole_par_ouv,
    symbole_par_fer,
    symbole_vir, 
    symbole_int,
    symbole_FIN,
};

struct symbole_nom {
    enum symbole s;
    const char* nom;
};

extern const struct symbole_nom noms_symboles [];

//TOKENS

enum type_token{
    type_token_symbole, // "+ - ( {"        // "espace" et "\n"
    type_token_identifiant, 
    type_token_mot_cle, //lecteur identifiant
    type_token_entier,  //1231231231231 "123123" -> int
    type_token_chaine, // "aeazea  aze" / ''
    type_token_flottant,
};

struct flottant {
    long partie_entiere;
    char * partie_fractionnaire; //Pour garder les leading zeros    
};

struct token{
    union {
        long entier;    //valide si type == type_token_entier
        struct flottant flottant;
        char *identifiant;
        enum mot_cle mot_cle;
        enum symbole symbole;
        char* chaine;
    } valeur;
    unsigned numero_ligne;
    unsigned niveau_indentation;
    enum type_token type;
};

void afficher_token(struct token token);

//Liste tokens 

struct liste_tokens{      
    struct token premier;
    struct liste_tokens *suivant;    //pas de suivant si NULL
};

void afficher_liste_tokens(struct liste_tokens* l_tokens);

struct liste_tokens* lexer(const char* source);

#endif