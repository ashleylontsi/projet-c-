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
    return l;
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

char *getId(int num) {
  static char id[4];
  if (num <= 26) {
    id[0] = 'A' + num - 1;
    id[1] = '\0';
  } else {
    id[0] = 'A' + (num - 27) / 26;
    id[1] = 'A' + (num - 27) % 26;
    id[2] = '\0';
  }
  return id;
}

void export_mermaid(liste_d_adjacence *G, const char *filename) {
  FILE *f = fopen(filename, "wt");
  if (f == NULL) {
    perror("Erreur ouverture fichier Mermaid");
    exit(EXIT_FAILURE);
  }

  fprintf(f, "---\n");
  fprintf(f, "config:\n");
  fprintf(f, "  layout: elk\n");
  fprintf(f, "  theme: neo\n");
  fprintf(f, "  look: neo\n");
  fprintf(f, "---\n\n");
  fprintf(f, "flowchart LR\n");

  for (int i = 0; i < G->nbr; i++) {
    fprintf(f, "%s((%d))\n", getId(i+1), i+1);
  }

  for (int i = 0; i < G->nbr; i++) {
    cellule *c = G->adjacente[i].head;
    while (c != NULL) {
      fprintf(f, "%s-->|%.2f|", getId(i+1), c->p);
      fprintf(f, "%s\n", getId(c->sommet));
      c = c->next;
    }
  }
  fclose(f);
}

//partie 2:
t_tarjan_vertex *creer_tab_tarjan_vertex(liste_d_adjacence *G) {
  t_tarjan_vertex *tab = malloc(G->nbr * sizeof(t_tarjan_vertex));

  for (int i = 0; i < G->nbr; i++) {
    tab[i].id = i+1;
    tab[i].num = -1;
    tab[i].num_acces = -1;
    tab[i].indicateur = 0;
  }

  return tab;
}

void initStack(t_pile *pile) {
  pile->nbElts = 0;
}

int isEmptyStack(t_pile *pile) {
  return pile->nbElts == 0;
}

void push(t_pile *pile, int value) {
  if (pile->nbElts < NBMAX) {
    pile->values[pile->nbElts] = value;
    pile->nbElts++;
  } else {
    printf("Erreur : pile pleine !\n");
  }
}

void *pop(t_pile *stack) {
  if (stack->nbElts == 0) {
    printf("Erreur : pile vide !\n");
    return NULL;
  }
  stack->nbElts--;
  return stack->values[stack->nbElts];
}


int min(int a, int b) {
  if (a>b) return b;
  else return a;
}

void parcours(liste_d_adjacence *G, int num, int v, t_tarjan_vertex *tab, t_pile *pile, t_partition *partition) {
  tab[v].num = num;
  tab[v].num_acces = num;
  num++;
  push(pile, v);
  tab[v].indicateur = true;

  int w;
  cellule *temp = G->adjacente[v].head;
  while(temp != NULL){
    w = temp->sommet-1;
    if (tab[w].num == -1) {
      parcours(G, num, w, tab, pile, partition);
      tab[v].num_acces = min(tab[v].num_acces, tab[w].num_acces);
    }else {
      tab[v].num_acces = min(tab[v].num_acces, tab[w].num);
    }
    temp = temp->next;
  }

  if (tab[v].num_acces == tab[v].num) {
    t_classe *c = malloc(sizeof(t_classe));
    c->nbr = 0;
    c->sommets = malloc(G->nbr * sizeof(t_tarjan_vertex *));
    do {
      w = pop(pile);
      tab[w].indicateur = false;
      c->sommets[c->nbr] = &tab[w];
      c->nbr++;
    } while (w != v);
    partition->classes = realloc(partition->classes, (partition->nbr + 1) * sizeof(t_classe *));
    partition->classes[partition->nbr] = c;
    partition->nbr++;
  }
}

t_partition tarjan(liste_d_adjacence *G){
  t_tarjan_vertex *tab = creer_tab_tarjan_vertex(G);
  t_pile pile;
  initStack(&pile);

  t_partition partition;
  partition.nbr = 0;
  partition.classes = NULL;

  int num = 0;

  for (int i = 0; i < G->nbr; i++) {
    if (tab[i].num == -1) {
      parcours(G, &num, i, tab, &pile, &partition);
    }
  }
  return partition;
}