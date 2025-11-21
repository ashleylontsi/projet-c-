#include <stdio.h>
#include <stdlib.h>
#include "fonctions.h"


int main() {
    printf("=== PROJET GRAPHES DE MARKOV - TEST COMPLET ===\n\n");

    // Étape 1 - Chargement et vérification
    printf("--- ETAPE 1: Chargement et verification ---\n");
    liste_d_adjacence G = readGraph("exemple1.txt");
    printf("Graphe charge avec %d sommets\n", G.nbr);

    verif(&G);
    export_mermaid(&G, "graphe.mmd");
    printf("Fichier Mermaid exporte: graphe.mmd\n\n");

    // Étape 2 - Algorithme de Tarjan
    printf("--- ETAPE 2: Algorithme de Tarjan ---\n");
    t_partition partition = tarjan(&G);

    // Affichage des composantes (format exact du sujet)
    for (int i = 0; i < partition.nbr; i++) {
        printf("Composante C%d: {", i+1);
        for (int j = 0; j < partition.classes[i]->nbr; j++) {
            printf("%d", partition.classes[i]->sommets[j]->id);
            if (j < partition.classes[i]->nbr - 1) printf(",");
        }
        printf("}\n");
    }
    printf("\n");

    // Étape 3 - Diagramme de Hasse
    printf("--- ETAPE 3: Diagramme de Hasse ---\n");

    // Création du tableau d'appartenance
    int *appartenance = malloc((G.nbr + 1) * sizeof(int));
    for (int i = 0; i <= G.nbr; i++) appartenance[i] = -1;

    for (int c = 0; c < partition.nbr; c++) {
        for (int s = 0; s < partition.classes[c]->nbr; s++) {
            int sommet_id = partition.classes[c]->sommets[s]->id;
            appartenance[sommet_id] = c;
        }
    }

    // Recherche des liens entre classes
    int liens_trouves = 0;
    int *liens_uniques = calloc(partition.nbr * partition.nbr, sizeof(int));

    for (int i = 1; i <= G.nbr; i++) {
        int classe_i = appartenance[i];
        cellule *voisin = G.adjacente[i-1].head;

        while (voisin != NULL) {
            int j = voisin->sommet;
            int classe_j = appartenance[j];

            if (classe_i != classe_j) {
                if (!liens_uniques[classe_i * partition.nbr + classe_j]) {
                    printf("Lien: C%d -> C%d\n", classe_i + 1, classe_j + 1);
                    liens_uniques[classe_i * partition.nbr + classe_j] = 1;
                    liens_trouves++;
                }
            }
            voisin = voisin->next;
        }
    }

    if (liens_trouves == 0) {
        printf("Aucun lien entre composantes\n");
    }
    printf("\n");

    // Étape 4 - Caractéristiques du graphe
    printf("--- ETAPE 4: Caracteristiques du graphe ---\n");

    // Déterminer les classes transitoires/persistantes
    int *classe_a_sorties = calloc(partition.nbr, sizeof(int));

    for (int i = 1; i <= G.nbr; i++) {
        int classe_i = appartenance[i];
        cellule *voisin = G.adjacente[i-1].head;

        while (voisin != NULL) {
            int j = voisin->sommet;
            if (appartenance[j] != classe_i) {
                classe_a_sorties[classe_i] = 1;
                break;
            }
            voisin = voisin->next;
        }
    }

    // Affichage des caractéristiques
    printf("Analyse des composantes:\n");
    for (int c = 0; c < partition.nbr; c++) {
        if (classe_a_sorties[c]) {
            printf("Composante C%d: TRANSITOIRE - {", c+1);
        } else {
            printf("Composante C%d: PERSISTANTE - {", c+1);
        }

        for (int s = 0; s < partition.classes[c]->nbr; s++) {
            printf("%d", partition.classes[c]->sommets[s]->id);
            if (s < partition.classes[c]->nbr - 1) printf(",");
        }
        printf("}\n");
    }

    // États absorbants
    printf("\nEtats absorbants:\n");
    int absorbants = 0;
    for (int c = 0; c < partition.nbr; c++) {
        if (!classe_a_sorties[c] && partition.classes[c]->nbr == 1) {
            printf("Etat %d est ABSORBANT\n", partition.classes[c]->sommets[0]->id);
            absorbants++;
        }
    }
    if (absorbants == 0) {
        printf("Aucun etat absorbant\n");
    }

    // Irréductibilité
    printf("\nIrreductibilite:\n");
    if (partition.nbr == 1) {
        printf("Le graphe de Markov est irreductible\n");
    } else {
        printf("Le graphe de Markov n'est pas irreductible\n");
    }

    // Libération mémoire
    free(appartenance);
    free(liens_uniques);
    free(classe_a_sorties);

    printf("\n=== TEST COMPLET TERMINE ===\n");
    return 0;
}