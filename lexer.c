#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "lexer.h"

//DOUGLAS THAIN
//crafting interpreter

const struct mot_cle_nom noms_mots_cles [] = {
    {mot_cle_main, "main"},

    {mot_cle_int, "int"},
    {mot_cle_float, "float"},
    {mot_cle_bool, "bool"},
    {mot_cle_char, "char"},

    {mot_cle_if, "if"},
    {mot_cle_else, "else"},
    {mot_cle_while, "while"},

    {mot_cle_true, "true"},
    {mot_cle_false, "false"},
    
    {mot_cle_FIN, NULL}   //signaler la fin
};

const struct symbole_nom noms_symboles [] = {
    {symbole_et, "&&"},
    {symbole_ou, "||"},
    {symbole_ega_inf, ">="},
    {symbole_ega_sup, "=<"},
    {symbole_ega, "=="},
    {symbole_non_ega, "!="},
    {symbole_ass, "="},
    {symbole_non, "!"},
    {symbole_ega_sup_stc, ">"},
    {symbole_ega_inf_stc, "<"},
    {symbole_div, "/"},
    {symbole_eto, "*"},
    {symbole_pls, "+"},
    {symbole_mns, "-"},
    {symbole_vir, ","},
    {symbole_poi_vir, ";"},
    {symbole_aco_ouv, "{"},
    {symbole_aco_fer, "}"},
    {symbole_cro_ouv, "["},
    {symbole_cro_fer, "]"},  
    {symbole_par_ouv, "("},
    {symbole_par_fer, ")"},  
    {symbole_FIN, NULL}
};

//1. LEXER

void afficher_token(struct token token){
    switch(token.type){
        case type_token_symbole: 
            for (size_t i = 0; noms_symboles[i].nom; i++){
                if (noms_symboles[i].s == token.valeur.symbole){
                    printf("token : '%s' | type : symbole | numero_ligne : %d | niveau_indentation : %d\n", 
                        noms_symboles[i].nom, token.numero_ligne, token.niveau_indentation);
                }
            }
            break;
        case type_token_identifiant: printf("token : '%s' | type : identifiant | numero_ligne : %d | niveau_indentation : %d\n", 
            token.valeur.identifiant, token.numero_ligne, token.niveau_indentation);
            break;
        case type_token_mot_cle: 
            for (size_t i = 0; noms_mots_cles[i].nom; i++){
                if (noms_mots_cles[i].s == token.valeur.mot_cle){
                    printf("token : '%s' | type : mot-cle | numero_ligne : %d | niveau_indentation : %d\n", 
                        noms_mots_cles[i].nom, token.numero_ligne, token.niveau_indentation);
                }
            }
            break;
        case type_token_entier: printf("token : '%ld' | type : entier | numero_ligne : %d | niveau_indentation : %d\n", 
            token.valeur.entier, token.numero_ligne, token.niveau_indentation);
            break;
        case type_token_flottant: printf("token : '%ld.%s' | type : flottant | numero_ligne : %d | niveau_indentation : %d\n", 
            token.valeur.flottant.partie_entiere, token.valeur.flottant.partie_fractionnaire, token.numero_ligne, token.niveau_indentation);
            break;
        case type_token_chaine: printf("token : '%s' | type : chaîne | numero_ligne : %d | niveau_indentation : %d\n", 
            token.valeur.chaine, token.numero_ligne, token.niveau_indentation);
            break;
        default: abort();
    }
}

//fin afficher tokens

void liste_tokens_ajouter(struct liste_tokens **l, const struct token *token){
    if (*l==NULL){
        *l = malloc(sizeof(struct liste_tokens));
        (*l)->premier = *token;
        (*l)->suivant = NULL;
    }
    else {
        liste_tokens_ajouter(&((*l)->suivant), token);
    }
}

void afficher_liste_tokens(struct liste_tokens* l_tokens){
    while (l_tokens){
        afficher_token(l_tokens->premier);
        l_tokens = l_tokens->suivant;
    }
}

//LECTEUR

struct lecteur{ //avancer - recuperer la position du lecteur - reinitialiser le lecteur
    const char* source;
    int numero_ligne;
    int niveau_indentation;
};

char avancer_lecteur(struct lecteur *l){
    char c = *(l->source);
    if (c == '\0'){
        return '\0';
    }
    if (c == ' '){
        (l->niveau_indentation)++;
    }
    if (c == '\t'){
        l->niveau_indentation = l->niveau_indentation + 5;
    }
    if (c =='\n'){
        (l->numero_ligne)++;
        l->niveau_indentation = 0;
    }
    (l->source)++;
    return c;
}

struct position_lecteur{
    const char* source;
    unsigned numero_ligne;
    unsigned niveau_indentation;
};

struct position_lecteur recuperer_position_lecteur(const struct lecteur *l){
    struct position_lecteur pos_lecteur = {
        .source = l->source, 
        .numero_ligne = l->numero_ligne,
        .niveau_indentation = l->niveau_indentation
    };
    return pos_lecteur;
}

void reinitialiser_lecteur(struct lecteur *l, struct position_lecteur pos){
    l->source = pos.source;
    l->numero_ligne = pos.numero_ligne;
    l->niveau_indentation = pos.niveau_indentation;
}

char* retourner_chaine(const struct lecteur *l, struct position_lecteur pos_precedente){
    assert(pos_precedente.source<l->source);
    unsigned n = l->source - pos_precedente.source;
    char* chaine = malloc(n+1);
    memcpy(chaine, pos_precedente.source, n);
    chaine[n] = '\0';
    return chaine;
}

//COMPARER CHAINES

bool comparer_chaines1(const char* a, const char* b){
    if (strlen(a) != strlen(b)){
        return false;
    }
    for (size_t i=0; i<strlen(a); i++){
        if (a[i] != b[i]){
            return false;
        }
    }
    return true;
}

bool comparer_chaines2(const char* a, const char* b){
    if (strlen(a) != strlen(b)){
        return false;
    }
    while (1){
        if (*a == '\0' || *b == '\0'){
            return true;
        }
        if (*a != *b){
            return false;
        }
        a++;
        b++;
    }
}

bool comparer_chaines3(const char* a, const char* b){
    if (*a == '\0' && *b == '\0'){
        return true;
    }
    if (*a == '\0' || *b == '\0'){
        return false;
    }
    if (*a != *b){
        return false;
    }
    return comparer_chaines3(a+1, b+1);
}

bool est_un_mot_cle(const char* chaine, enum mot_cle* ptr_mot_cle){
    for (size_t i=0; noms_mots_cles[i].nom; i++){
        const char* nom = noms_mots_cles[i].nom;
        if (comparer_chaines1(chaine, nom) && comparer_chaines2(chaine, nom) &&
            comparer_chaines3(chaine, nom)){
                *ptr_mot_cle = noms_mots_cles[i].s;
                return true;
            }
    }
    return false;
}

bool lexer_mot_cle(struct lecteur* l, struct token* token_ptr){
    struct position_lecteur debut = recuperer_position_lecteur(l);
    char c = avancer_lecteur(l);
    if (!isalpha(c)){
        reinitialiser_lecteur(l, debut);
        return false;
    }
    while (1){
        struct position_lecteur precedente = recuperer_position_lecteur(l);
        char c  = avancer_lecteur(l);
        if (!isalpha(c)){
            reinitialiser_lecteur(l, precedente);
            char* chaine = retourner_chaine(l, debut);
            enum mot_cle mot_cle; 
            if (est_un_mot_cle(chaine, &mot_cle)){
                *token_ptr = (struct token){
                    .valeur = {.mot_cle = mot_cle}, 
                    .numero_ligne = debut.numero_ligne,
                    .niveau_indentation = debut.niveau_indentation,
                    .type = type_token_mot_cle,
                };
                return true;
            }
            reinitialiser_lecteur(l, debut);
            return false;
        }
    }
}

bool lexer_identifiant(struct lecteur* l, struct token* token_ptr){
    struct position_lecteur debut = recuperer_position_lecteur(l);
    char c = avancer_lecteur(l);
    if (!isalpha(c) || (c >= 'A' && c <= 'Z')){
        reinitialiser_lecteur(l, debut);
        return false;
    }
    while (1){
        struct position_lecteur precedente = recuperer_position_lecteur(l);
        char c  = avancer_lecteur(l);
        if (!isalpha(c) && !isdigit(c) && c != '_') { 
            reinitialiser_lecteur(l, precedente);
            char* chaine = retourner_chaine(l, debut);
            *token_ptr = (struct token){
                .valeur = {.identifiant = chaine}, 
                .numero_ligne = debut.numero_ligne, 
                .niveau_indentation = debut.niveau_indentation,
                .type = type_token_identifiant,
            };
            return true;
        }
    }
}

int nombre_chiffres(int n){
    if (n == 0) return 1;
    return floor (log10 (abs (n))) + 1;
}

bool lexer_entier_ou_flottant(struct lecteur* l, struct token* token_ptr){
    struct position_lecteur debut = recuperer_position_lecteur(l);
    long entier = 0;
    unsigned nbr_chiffres = 0;
    while (1){
        struct position_lecteur precedente = recuperer_position_lecteur(l); 
        char c  = avancer_lecteur(l);
        if (c == '.'){
            if (nbr_chiffres == 0) {
                reinitialiser_lecteur(l, debut);
                return false;
            }
            //long partie_fractionnaire = 0;
            nbr_chiffres = 1;
            char * partie_fractionnaire = malloc(2*sizeof(char));
            while(1){
                precedente = recuperer_position_lecteur(l); 
                c  = avancer_lecteur(l);
                if (!isdigit(c)){
                    reinitialiser_lecteur(l, precedente);   //il faut revenir un coup en arrière
                    *token_ptr = (struct token){
                        .valeur = {.flottant = {.partie_entiere = entier, .partie_fractionnaire = partie_fractionnaire}}, 
                        .numero_ligne = debut.numero_ligne, 
                        .niveau_indentation = debut.niveau_indentation,
                        .type = type_token_flottant,
                    };
                    return true;
                }
                partie_fractionnaire = realloc(partie_fractionnaire, sizeof(char)*(nbr_chiffres + 1));
                partie_fractionnaire[strlen(partie_fractionnaire)] = c;
                partie_fractionnaire[strlen(partie_fractionnaire) + 1] = '\0';
                nbr_chiffres ++;
            }

        }
        else if (!isdigit(c)){
            if (nbr_chiffres == 0) {
                reinitialiser_lecteur(l, debut);
                return false;
            }   
            reinitialiser_lecteur(l, precedente);   //il faut revenir un coup en arrière
            *token_ptr = (struct token){
                .valeur = {.entier = entier}, 
                .numero_ligne = debut.numero_ligne, 
                .niveau_indentation = debut.niveau_indentation,
                .type = type_token_entier,
                };
            return true;
        }
        entier*= 10;
        entier+= c - '0';
        nbr_chiffres ++;
    }
}

bool lexer_symb_donne(struct lecteur* l, struct token* token_ptr, enum symbole symb){
    const char* nom_symb;
    for (size_t i = 0; noms_symboles[i].nom; i++){
        if (symb == noms_symboles[i].s){
            nom_symb = noms_symboles[i].nom;
            break;
        }
    }
    struct position_lecteur debut = recuperer_position_lecteur(l);
    while (*nom_symb){
        char c = avancer_lecteur(l);
        if (*nom_symb != c){
            reinitialiser_lecteur(l, debut);
            return false;
        }
        nom_symb++;
    }
    *token_ptr = (struct token){
        .valeur = {.symbole = symb}, 
        .numero_ligne = debut.numero_ligne, 
        .niveau_indentation = debut.niveau_indentation,
        .type = type_token_symbole,
    };
    return true;
}


bool lexer_symbole(struct lecteur* l, struct token* token_ptr){
    for (size_t i = 0; noms_symboles[i].nom; i++){
        if (lexer_symb_donne(l, token_ptr, noms_symboles[i].s)){
            return true;
        }
    }
    return false;   
}

bool lexer_car_chaine(struct lecteur* l, char *c){
    *c = avancer_lecteur(l);
    if (*c == '\\'){
        *c = avancer_lecteur(l);
        if (*c == 'n'){
            *c = '\n';
            return true;
        }
        else if (*c == 't'){
            *c = '\t';
            return true;
        }  
        else if (*c == '\\'){
            *c = '\\';
            return true;
        }  
        else if (*c == '\"'){
            *c = '\"';
            return true;
        } 
        else if (*c == '\''){
            *c = '\'';
            return true;
        } 
        else {
            return false;
        }
    }
    return true;
}

bool lexer_chaine1(struct lecteur* l, struct token* token_ptr){ 
    struct position_lecteur debut = recuperer_position_lecteur(l); 
    char* chaine_merde = malloc(0);
    char c = avancer_lecteur(l);
    if (c != '\'' && c != '\"'){
        reinitialiser_lecteur(l, debut);
        return false;
    }
    bool simples = (c == '\'');
    debut = recuperer_position_lecteur(l); 
    while (1){
        struct position_lecteur precedente = recuperer_position_lecteur(l);
        char c = avancer_lecteur(l);
        if ((c == '\'' && simples)|| (c == '\"' && !simples)){
            reinitialiser_lecteur(l, precedente);   //pour eviter de prendre le quote
            avancer_lecteur(l);
            *token_ptr = (struct token){
                    .valeur = {.chaine = chaine_merde}, 
                    .numero_ligne = debut.numero_ligne, 
                    .niveau_indentation = debut.niveau_indentation,
                    .type = type_token_chaine,
                };
            return true;
        }
        else if (c == '\0'){
            //guillemets pas fermes
            abort();
        }
        reinitialiser_lecteur(l, precedente);   
        if (!lexer_car_chaine(l, &c)){
            printf("lexer car chaine abort\n");
            abort();
        };
        size_t n = strlen(chaine_merde);
        chaine_merde = realloc(chaine_merde, n + 2);
        chaine_merde[n] = c;
        chaine_merde[n+1] = '\0';
        
    }
}
/*
bool lexer_chaine2(struct lecteur* l, struct token* token_ptr){ // :D
    struct position_lecteur debut = recuperer_position_lecteur(l); 
    char c = avancer_lecteur(l);
    if (c != '\'' && c != '\"'){
        reinitialiser_lecteur(l, debut);
        return false;
    }
    bool simples = (c == '\'');
    debut = recuperer_position_lecteur(l); 
    while (1){
        struct position_lecteur precedente = recuperer_position_lecteur(l);
        char c = avancer_lecteur(l);
        if ((c == '\'' && simples)|| (c == '\"' && !simples)){
            reinitialiser_lecteur(l, precedente);   //pour eviter de prendre le quote
            char* chaine = retourner_chaine(l, debut);
            avancer_lecteur(l);
            *token_ptr = (struct token){
                    .valeur = {.chaine = chaine}, 
                    .numero_ligne = debut.numero_ligne, 
                    .niveau_indentation = debut.niveau_indentation,
                    .type = type_token_chaine,
                };
            return true;
        }
        else if (c == '\0'){
            //guillemets pas fermes
            abort();
        }
    }
}
*/ 

bool lexer_car_merde(char c){
    if (c == '\n'){  //les \n
        return true;
    }
    else if (c == ' '){ //j'enleve les espaces
        return true;
    }
    else if (c == '\t'){  //les \t
        return true;
    }
    else if (c == '\r'){
        return true;
    }
    else if (c == '\0'){  //fin de chaine
        return false;
    }
    printf("Probleme lexer char %d\n", c);
    abort();
}

bool lexer_merde(struct lecteur* l){
    char c = avancer_lecteur(l);
    if (lexer_car_merde(c)){
        return true;
    }
    return false;
}

const char* type_token_str(enum type_token t){
    switch (t) {
        case type_token_symbole : return "symbole";
        case type_token_identifiant : return "identifiant";
        case type_token_mot_cle : return "mot_cle";
        case type_token_entier : return "entier";
        case type_token_chaine : return "chaine";
        default : 
            abort();
    }
}

bool lexer_token(struct lecteur* lec , struct token* ptr_tok){
    if (lexer_mot_cle(lec, ptr_tok)){
        return true;
    }
    else if (lexer_identifiant(lec, ptr_tok)){
        return true;
    }
    else if (lexer_symbole(lec, ptr_tok)){
        return true;
    }
    else if (lexer_entier_ou_flottant(lec, ptr_tok)){
        return true;
    }
    else if (lexer_chaine1(lec, ptr_tok)){
        return true;
    }
    else if (lexer_merde(lec)){
        lexer_token(lec, ptr_tok);   //On avance juste le ptr_tok
        return true;
    }
    else {
        return false;
    }
}

struct liste_tokens* lexer(const char* source){
    struct lecteur lec = {
        .source = source,
        .numero_ligne = 1,
        .niveau_indentation = 0,
    };
    struct liste_tokens * l_tokens = NULL;
    while (1){
        struct token tok;
        if (!lexer_token(&lec, &tok)) {
            break;
        }
        liste_tokens_ajouter(&l_tokens, &tok);
    }
    return l_tokens;
}






