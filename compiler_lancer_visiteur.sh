#!/bin/sh -ex
cc parser.c lexer.c visiteur.c visiteur_dummy.c main.c -o start_visiteur -Wall -Wextra -g
./start_visiteur