#include "fonctions.h"
#include <stdio.h>
#include <stdlib.h>

cellule *creer_cellule(int sommet, float p, cellule *next){
  cellule *c;
  c = malloc(sizeof(cellule));
  if(c == NULL){
    printf("Erreur\n");
    return NULL;
  }
  c->sommet = sommet;
  c->p = p;
  c->next = next;
  return c;
}

liste creer_liste(){
  liste l;
  l.head = NULL;
  return l;
}

void ajouter_cellule(liste *l, cellule *c){
  if(l->head == NULL){
    l->head = c;
    return;
  }
  cellule *temp = l->head;
  while(temp->next != NULL){
    temp = temp->next;
  }
  temp->next = c;
}

void afficher_liste(liste *l){
  cellule *temp = l->head;
  while(temp != NULL){
    printf("Sommet -> %d || proba : %.2f \n", temp->sommet, temp->p);
    temp = temp->next;
  }
}

liste_d_adjacence creer_liste_d_adjacence(int n) {
  liste_d_adjacence l;
  l.nbr = n;

  l.adjacente = malloc(n*sizeof(liste));
  for (int i = 0; i < n; i++) {
    l.adjacente[i] = creer_liste();
  }

  return l;
}

void afficher_liste_d_adjacence(liste_d_adjacence *l) {
  for (int i = 0; i < l->nbr; i++) {
    printf("Sommet %d :\n", i+1);
    afficher_liste(&(l->adjacente[i]));
    printf("\n");
  }
}

liste_d_adjacence readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    int nbvert, depart, arrivee;
    float proba;

    liste_d_adjacence l;

    if (file == NULL) {
        perror("Could not open file for reading");
        exit(EXIT_FAILURE);
    }

    if (fscanf(file, "%d", &nbvert) != 1) {
        perror("Could not read number of vertices");
        exit(EXIT_FAILURE);
    }

    l = creer_liste_d_adjacence(nbvert);

    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
      cellule *c = creer_cellule(arrivee, proba, NULL);
      ajouter_cellule(&(l.adjacente[depart - 1]), c);
    }

    fclose(file);
    return l; // retourner la liste d’adjacence remplie
}

void verif(liste_d_adjacence *l) {
  float a;

  for (int i = 0; i < l->nbr; i++) {
    a = 0.0;
    cellule *temp = l->adjacente[i].head;

    while(temp != NULL) {
      a+=temp->p;
      temp = temp->next;
    }

    if ((a<0.99 || a>1)) {
      printf("Le graphe n est pas un graphe de Markov\nLa somme des probabilites du sommet %d est %.2f \n", i+1, a);
      return;
    }
  }
  printf("Le graphe est un graphe de Markov\n");
  return;
}

