//
// Created by loanl on 04/11/2025.
//

#ifndef FONCTIONS_H
#define FONCTIONS_H

#endif //FONCTIONS_H

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


//partie 2:
