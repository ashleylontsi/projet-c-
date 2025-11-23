#include "fonctions.h"
#include <stdio.h>
#include <stdlib.h>

//Partie 1 : Etape 1

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

//Partie 1 : Etape 2

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

//Partie 1 : Etape 3

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

//Partie 2: Etape 1

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

//Partie 2 : Etape 2

int *creer_tab_classe_sommets(liste_d_adjacence *G, t_partition *partition) {
  int *classeSommet = malloc(G->nbr * sizeof(int));

  for (int c = 0; c < partition->nbr; c++) {
    t_classe *classe = partition->classes[c];

    for (int s = 0; s < classe->nbr; s++) {
      int idSommet = classe->sommets[s]->id;
      classeSommet[idSommet - 1] = c + 1;
    }
  }

  return classeSommet;
}

liens *hausse(liste_d_adjacence *G, t_partition *partition) {

  int *classeSommet = creer_tab_classe_sommets(G, partition);

  liens *liens = malloc(sizeof(liens));
  liens->nbr = 0;
  liens->capacite = 50;
  liens->tab = malloc(liens->capacite * sizeof(lien));

  int ci, cj;

  for (int i = 0; i < G->nbr; i++) {

    ci = classeSommet[i];
    cellule *temp = G->adjacente[i].head;

    while (temp != NULL) {

      cj = classeSommet[temp->sommet - 1];

      if (ci != cj) {

        bool existe = false;
        for (int s = 0; s < liens->nbr; s++) {
          if (liens->tab[s].depart == ci &&
              liens->tab[s].arrive == cj) {
            existe = true;
            break;
              }
        }

        if (!existe) {
          liens->tab[liens->nbr].depart = ci;
          liens->tab[liens->nbr].arrive = cj;
          liens->nbr++;
        }
      }

      temp = temp->next;
    }
  }

  return liens;
}


//Partie 2 : Etape 3

// Fonction pour créer un tableau de liens
t_link_array* creer_link_array() {
    t_link_array *link_array = malloc(sizeof(t_link_array));
    link_array->nbr_liens = 0;
    link_array->capacite = 10;
    link_array->liens = malloc(link_array->capacite * sizeof(t_lien));
    return link_array;
}

// Fonction pour ajouter un lien
void ajouter_lien(t_link_array *link_array, int classe_depart, int classe_arrivee) {
    // Vérifier si le lien existe déjà
    for (int i = 0; i < link_array->nbr_liens; i++) {
        if (link_array->liens[i].classe_depart == classe_depart &&
            link_array->liens[i].classe_arrivee == classe_arrivee) {
            return;
        }
    }

    // Redimensionner si nécessaire
    if (link_array->nbr_liens >= link_array->capacite) {
        link_array->capacite *= 2;
        link_array->liens = realloc(link_array->liens, link_array->capacite * sizeof(t_lien));
    }

    // Ajouter le nouveau lien
    link_array->liens[link_array->nbr_liens].classe_depart = classe_depart;
    link_array->liens[link_array->nbr_liens].classe_arrivee = classe_arrivee;
    link_array->nbr_liens++;
}

// Fonction pour trouver les liens entre classes
t_link_array* trouver_liens_classes(liste_d_adjacence *G, t_partition *partition) {
    t_link_array *link_array = creer_link_array();

    // Créer un tableau pour savoir à quelle classe appartient chaque sommet
    int *appartenance = malloc((G->nbr + 1) * sizeof(int));
    for (int i = 0; i <= G->nbr; i++) {
        appartenance[i] = -1;
    }

    // Remplir le tableau d'appartenance
    for (int c = 0; c < partition->nbr; c++) {
        for (int s = 0; s < partition->classes[c]->nbr; s++) {
            int sommet_id = partition->classes[c]->sommets[s]->id;
            appartenance[sommet_id] = c;
        }
    }

    // Parcourir tous les sommets du graphe
    for (int i = 1; i <= G->nbr; i++) {
        int classe_i = appartenance[i];

        // Parcourir tous les voisins du sommet i
        cellule *voisin = G->adjacente[i-1].head;
        while (voisin != NULL) {
            int j = voisin->sommet;
            int classe_j = appartenance[j];

            // Si les classes sont différentes, il y a un lien
            if (classe_i != classe_j) {
                ajouter_lien(link_array, classe_i, classe_j);
            }

            voisin = voisin->next;
        }
    }

    free(appartenance);
    return link_array;
}

// Fonction pour analyser les caractéristiques (étape 3)
void analyser_caracteristiques(t_partition *partition, t_link_array *link_array) {
    printf("\n=== ETAPE 3 - CARACTERISTIQUES DU GRAPHE ===\n");

    // Tableau pour savoir si une classe a des liens sortants
    int *classe_a_sorties = calloc(partition->nbr, sizeof(int));

    // Marquer les classes qui ont des liens sortants
    for (int i = 0; i < link_array->nbr_liens; i++) {
        int classe_depart = link_array->liens[i].classe_depart;
        classe_a_sorties[classe_depart] = 1;
    }

    // Afficher si les classes sont transitoires ou persistantes
    printf("\n--- Analyse des classes ---\n");
    for (int c = 0; c < partition->nbr; c++) {
        if (classe_a_sorties[c]) {
            printf("Classe %d: TRANSITOIRE - {", c);
        } else {
            printf("Classe %d: PERSISTANTE - {", c);
        }

        // Afficher les sommets de la classe
        for (int s = 0; s < partition->classes[c]->nbr; s++) {
            printf("%d", partition->classes[c]->sommets[s]->id);
            if (s < partition->classes[c]->nbr - 1) {
                printf(",");
            }
        }
        printf("}\n");
    }

    // Vérifier s'il y a des états absorbants
    printf("\n--- Etats absorbants ---\n");
    int etats_absorbants = 0;
    for (int c = 0; c < partition->nbr; c++) {
        if (!classe_a_sorties[c] && partition->classes[c]->nbr == 1) {
            etats_absorbants++;
            printf("Etat %d est ABSORBANT\n", partition->classes[c]->sommets[0]->id);
        }
    }
    if (etats_absorbants == 0) {
        printf("Aucun état absorbant\n");
    }

    // Vérifier si le graphe est irréductible
    printf("\n--- Irreductibilite ---\n");
    if (partition->nbr == 1) {
        printf("Le graphe de Markov est irreductible\n");
    } else {
        printf("Le graphe de Markov n'est pas irreductible\n");
    }

    free(classe_a_sorties);
}

//Partie 3 : Etape 1

float **matrix_vide(int n){
  float **matrix = malloc(n * sizeof(float*));
  for (int i= 0; i<n; i++){
    matrix[i] = malloc(n * sizeof(float));
    for (int j= 0; j<n; j++){
      matrix[i][j] = 0;
    }
  }
  return matrix;
}

void recopie_mat(float**N, float**M, int n){ // on va recopier N -> M
  for (int i=0; i<n;i++){
    for (int j=0; j<n; j++){
      M[i][j] = N[i][j];
    }
  }
}

float **multi_mat(float **M, float **N, int n){
  float **res = matrix_vide(n);
  for(int i=0; i<n; i++){
    for (int j=0; j<n; j++){
      for(int k=0; k<n; k++){
        res[i][j] += M[i][k]*N[k][j];
      }
    }
  }
  return res;
}

float diff_matrix(float **M, float **N, int n){
  float somme = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      somme += fabsf(M[i][j] - N[i][j]);
    }
  }
  return somme;
}

float **matrix_adj(liste_d_adjacence *liste_adj){
  int n = l->nbr;
  float **mat = matrix_vide(n);
  for (int i = 0; i < n; i++) {
    cellule *temp = l->adjacente[i].head;
    while (temp != NULL) {
      mat[i][temp->sommet] = temp->p;  //ici on attribut la probabilit� a sa position sans avoir a connaitre le sommet
      temp = temp->next;
    }
  }

  return mat;
}


t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index) {
  t_classe *classe = part.classes[compo_index]; // R�cup�ration de la classe voulue
  int n = classe->nbr;        // taille de la sous-matrice
  t_matrix sub_mat;
  sub_mat.n = n;  // Cr�ation d'une sous-matrice n�n
  sub_mat.data = matrix_vide(n);
  for (int a = 0; a < n; a++) {
    int i = classe->sommets[a]->id;   // num�ro global du sommet
    for (int b = 0; b < n; b++) {
      int j = classe->sommets[b]->id;  // num�ro global du sommet
      sub.data[a][b] = matrix.data[i][j];
    }
  }

  return sub;
}

