#ifndef FONCTIONS_H
#define FONCTIONS_H

#include <stdbool.h>


//Partie 1

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

//Partie 2:

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

#define NBMAX 100

typedef struct s_pile {
    void *values[NBMAX];
    int nbElts;
} t_pile;

void initStack(t_pile *pile);
int isEmptyStack(t_pile *pile);
void push(t_pile *pile, int value);
void *pop(t_pile *pile);

void parcours(liste_d_adjacence *G, int num, int v, t_tarjan_vertex *tab, t_pile *pile, t_partition *partition);
t_partition tarjan(liste_d_adjacence *G);

void trouver_liens_classes_simple(liste_d_adjacence *G, t_partition *partition) ;

typedef struct {
    int depart;
    int arrive;
}lien;

typedef struct {
    lien *tab;
    int nbr;
    int capacite;
}liens;

int *creer_tab_classe_sommets(liste_d_adjacence *G, t_partition *partition);
liens *hausse(liste_d_adjacence *G, t_partition *partition);

// Structure pour les liens entre classes
typedef struct {
    int classe_depart;
    int classe_arrivee;
} t_lien;

typedef struct {
    t_lien *liens;
    int nbr_liens;
    int capacite;
} t_link_array;

// Prototypes des fonctions étape 3
t_link_array* trouver_liens_classes(liste_d_adjacence *G, t_partition *partition);
void analyser_caracteristiques(t_partition *partition, t_link_array *link_array);

typedef struct {
    float **data;   // matrice n�n
    int n;
} t_matrix;

/***
//Partie 3


float **matrix_vide(int);
/* On cr�er une matrice ayant 0 a chaque coef
   n (un entier) = taille de martice
   on renvoit le pointeur vers la matrice cr��e
*/

//void recopie_mat(float**, float**, int);
/*On recopie les valeurs d'une matrice a une autre de meme taille
  float**, float** sont les deux matrix que l'on va utiliser, sachant que ce sera les valeurs de la premi�re dans le deuxi�me
  int serait la taille de ces matrix
  on ne renvoie rien car les matrix directement modifi�es
*/

//float **multi_mat(float**, float**, int);
/*On veut faire une multiplication de matrices de meme taille
  int**, int** sont les deux matrices qu'on va utiliser
  int serait la taille de ces matrix
  on renvoie une matrix qui serait le resutalt de ces matrices
*/

//float diff_matrix(float**, float**, int);
/*On veut faire la difference de matrices de meme taille
  float**, float** sont les deux matrices qu'on va utiliser
  int serait la taille de ces matrix
  on renvoie la somme de cette difference
*/

//float **matrix_adj(liste_d_adjacence *);
/*On veut cr�er une matrice a partir de l'information d'une liste adjacente
  liste_d_adjacence * serait la liste adjacence
  on renvoie la matrix cr�er
*/

//Etape 2

/**
 * @brief Extracts a submatrix corresponding to a specific
component of a graph partition.
 *
 * @param matrix The original adjacency matrix of the graph.
 * @param part The partition of the graph into strongly
connected components.
 * @param compo_index The index of the component to extract.
 * @return t_matrix The submatrix corresponding to the
specified component.
 */
//t_matrix subMatrix(t_matrix , t_partition , int);

#endif
