#include "parser.h"
#include "lexer.h"
#include "visiteur.h"
#include "visiteur_dummy.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

char * lire_fichier(char* nom_fichier){
    char * buffer = 0;
    long length;
    FILE * f = fopen (nom_fichier, "rb");
    if (f){
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = malloc (length);
        if (buffer){
            fread (buffer, 1, length, f);
        }
    fclose (f);
    }
    return buffer;
}

int main(){
    char* nom_fichier = "tests/test5.c";
    printf("--------------------------- FICHIER TEST ---------------------------\n");
    printf("%s\n", lire_fichier(nom_fichier));
    printf("--------------------------- FICHIER TEST ---------------------------\n");
    struct liste_tokens * l_tokens = lexer(lire_fichier(nom_fichier));
    printf("--------------------------- LEXER ---------------------------\n");
    afficher_liste_tokens(l_tokens);
    printf("--------------------------- LEXER ---------------------------\n");
    struct noeud * arbre;
    programme(&l_tokens, &arbre);   

    printf("--------------------------- AST ---------------------------\n");
    parcourir_arbre(arbre); 
    printf("--------------------------- AST ---------------------------\n");

    struct visiteur visiteur;
    initialiser_visiteur_dummy(&visiteur);
    printf("--------------------------- VISITEUR ---------------------------\n");
    accepter_programme(arbre->contenu.noeud_programme, &visiteur);
    printf("--------------------------- VISITEUR ---------------------------\n");
    
}




