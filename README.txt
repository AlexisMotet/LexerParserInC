I - Lancer le projet
    1. "make" puis "./start_visiteur"

    ou

    2. "./compiler_lancer_visiteur.sh"

Si vous voulez essayer d'autres tests, il suffit d'aller dans le main (main.c) ligne 29 :

char* nom_fichier = "tests/[NOM_DU_FICHIER]"; 

II - Introduction
Le projet est constitué d'un lexer, d'un parser et d'un "dummy" visiteur, pour le langage CLite 
(la grammaire utilisée : http://myslu.stlawu.edu/~ehar/Spring10/364/clite_grammar.html).

III - Le lexer

Le lexer utilise une structure lecteur qu'on déplace dans le code à lexer. 

On essaye à chaque déplacement du lecteur de lexer un certain type de token 
(symbole/identifiant/mot-clé/entier/chaîne/le reste) en commançant par les mots-clés pour bien les 
distinguer des identifiants. 

Le reste est composé des caractères de nouvelles lignes et de tabulation notamment. On peut ainsi
mettre à jour la position du lecteur en les comptant, ce qui permet d'avoir pour chaque token son
numéro de ligne et son indentation. Cela peut être utile pour l'affichage du code à partir de l'AST 
ou pour retourner des erreurs de syntaxe avec le numéro de ligne.

Le type flottant est bien implanté conformément à la grammaire du CLite, grâce à une structure 
flottant composée d'une partie entière et d'une chaîne de caractère pour la partie fractionnaire
pour garder les n premiers zéros ("12.0003434").

Les tokens sont stockés dans une liste chaînée qu'on peut afficher avec une fonction de lexer.c.

IV - Le parser

Le parser est inspiré de https://github.com/holyboom/Clite/blob/master/Parser.java (en Java) qui suit 
lui-même la méthode décrite dans https://craftinginterpreters.com/parsing-expressions.html#design-note 
pour parser correctement des expressions. 

J'ai du cependant reproduire les classes de Java avec les structures du C pour créer l'AST. Pour 
cela, j'ai utilisé une structure que j'ai appellée "noeud" et qui est composée d'un discrimant 
(type) et d'une union (contenu). 

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

Cela équivaut à un héritage des structures de noeuds spécifiques. Une structure de noeud spécifique
est constituée ainsi :

struct noeud_binaire {
    struct noeud * droite,
    enum operateur operateur,
    struct noeud * gauche,
}

Pour pallier l'absence de listes à taille dynamique, j'ai du aussi utiliser des listes chaînées pour
contenir toutes les déclarations d'un noeud "déclarations" et les instructions d'un noeud 
"instructions".

Exemple :

struct noeud_instructions {
    struct instructions * l;
};

La liste chaînée "instructions"

struct instructions {
    struct noeud * instruction;
    struct instructions * suivante;
}; 

Les fonctions de parsing sont inspirées du parser en Java, et les structures sont créées comme les
classes en Java dans chaque fonction grâce une allocation de la mémoire.

Le parser en Java :

private Expression addition() {
        Expression e = term();
        while (isAddOp()) {
            Operator op = new Operator(match(token.type()));
            Expression term2 = term();
            e = new Binary(op, e, term2);
        }
        return e;
    }

Mon parser : 

bool addition(struct liste_tokens ** l_tokens, struct noeud ** noeud){
    terme(l_tokens, noeud); <- on essaye de parser un terme à gauche
    struct noeud * droite;
    enum operateur op;
    while (consommer_si_op_additionneur(l_tokens, &op)){
        terme(l_tokens, &droite);
        struct noeud_binaire noeud_binaire;
        initialiser_noeud_binaire(&noeud_binaire, op, *noeud, droite); <- création du noeud binaire
        *noeud = malloc(sizeof(struct noeud));  <- allocation de la mémoire 
        (*noeud)->contenu.noeud_binaire = noeud_binaire;    <- contenu : c'est un noeud binaire
        (*noeud)->type = type_noeud_binaire;    <- on indique son type
    }
    return true;
}

On voit que l'idée est la même.

V - Le visiteur

Il a fallu pour faire le visiteur ajouter des "méthodes" accept aux "classes" de noeuds spécifiques 
commela structure "noeud_binaire".

Pour ce faire, j'ai ajouté dans chaque structure de noeud spécifique un pointeur vers une fonction.

struct noeud_binaire {
    enum operateur op;
    struct noeud * droite;
    struct noeud * gauche;
    void (*ptr_accepter)(struct noeud_binaire, struct visiteur *); <- le pointeur vers la fonction
};

Il faut donc créer chaque noeud avec une fonction qui puisse initiliaser le pointeur et le faire
pointer vers la bonne méthode à savoir dans ce cas "accepter_noeud_binaire".

Exemple d'une fonction qui accepte le visiteur : 

void accepter_binaire(struct noeud_binaire noeud_binaire, struct visiteur * visiteur){
    accepter_noeud(noeud_binaire.droite, visiteur);
    accepter_noeud(noeud_binaire.gauche, visiteur);
    (*visiteur->ptr_visiter_binaire)(noeud_binaire);
}

La fonction "accepter_noeud" renvoie vers la fonction "accepter_noeud_specifique" grâce au
discriminant contenu dans chaque structure "noeud".

Le visiteur est lui aussi une structure, qui contient des "méthodes" de visite pour chaque noeud.

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

On doit aussi l'initialiser pour que les pointeurs pointent vers les bonnes fonctions de visite.

Je n'ai fait qu'un "dummy" visiteur car j'avais déjà une fonction d'affichage de l'AST grâce à un
parcours classique. 






