//
// Created by loanl on 04/11/2025.
//


#ifndef FONCTIONS_H
#define FONCTIONS_H

#endif //FONCTIONS_H

#include <stdbool.h>

typedef struct cellule {
    int sommet;
    float p;
    struct cellule *next;
}cellule;

typedef struct{
    cellule *head;
}liste;

typedef struct {
    int nbr;
    liste *adjacente;
}liste_d_adjacence;

cellule *creer_cellule(int sommet, float p, cellule *next);
liste creer_liste();
void ajouter_cellule(liste *l, cellule *c);
void afficher_liste(liste *l);
liste_d_adjacence creer_liste_d_adjacence(int n);
void afficher_liste_d_adjacence(liste_d_adjacence *l);
liste_d_adjacence readGraph(const char *filename);
void verif(liste_d_adjacence *l);

char *getId(int num);
void export_mermaid(liste_d_adjacence *G, const char *filename);

//partie 2:
typedef struct {
    int id, num, num_acces;
    bool indicateur;
}t_tarjan_vertex;

typedef struct {
    char nom[10];
    t_tarjan_vertex **sommets;
    int nbr;
}t_classe;

typedef struct {
    t_classe **classes;
    int nbr;
}t_partition;

t_tarjan_vertex *creer_tab_tarjan_vertex(liste_d_adjacence *G);

typedef struct stack
{
    struct stack *prev;
    struct stack *next;
    void *data;
} stack_s;