#include "parser.h"
#include "visiteur.h"
#include "visiteur_dummy.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

static void visiter_litteral(){
    printf("visiter litteral\n");
}

static void visiter_unaire(){
    printf("visiter unaire\n");
}

static void visiter_binaire(){
    printf("visiter binaire\n");
}

static void visiter_variable(){
    printf("visiter variable\n");
}

static void visiter_vide(){
    printf("visiter vide\n");
}

static void visiter_while(){
    printf("visiter while\n");
}

static void visiter_if(){
    printf("visiter if\n");
}

static void visiter_if_else(){
    printf("visiter if else\n");
}

static void visiter_assignation(){
    printf("visiter assignation\n");
}

static void visiter_declaration(){
    printf("visiter declaration\n");
}

static void visiter_declarations(){
    printf("visiter declarations\n");
}

static void visiter_instructions(){
    printf("visiter instructions\n");
}

static void visiter_programme(){
    printf("visiter programme\n");
}

void initialiser_visiteur_dummy(struct visiteur * visiteur){
    visiteur->ptr_visiter_litteral = &visiter_litteral;
    visiteur->ptr_visiter_unaire = &visiter_unaire;
    visiteur->ptr_visiter_variable = &visiter_variable;
    visiteur->ptr_visiter_binaire = &visiter_binaire;
    visiteur->ptr_visiter_vide = &visiter_vide;
    visiteur->ptr_visiter_while = &visiter_while;
    visiteur->ptr_visiter_if = &visiter_if;
    visiteur->ptr_visiter_if_else = &visiter_if_else;
    visiteur->ptr_visiter_assignation = &visiter_assignation;
    visiteur->ptr_visiter_declaration = &visiter_declaration;
    visiteur->ptr_visiter_declarations = &visiter_declarations;
    visiteur->ptr_visiter_instructions = &visiter_instructions;
    visiteur->ptr_visiter_programme = &visiter_programme;
}