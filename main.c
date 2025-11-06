#include <stdio.h>
#include <stdlib.h>
#include "fonctions.h"

int main(void) {
    printf("=== TEST LECTURE GRAPHE ===\n\n");

    liste_d_adjacence G = readGraph("scratch.txt");

    printf("Contenu du graphe lu depuis le fichier :\n");
    afficher_liste_d_adjacence(&G);

    verif(&G);

    // Libération mémoire
    for (int i = 0; i < G.nbr; i++) {
        cellule *tmp = G.adjacente[i].head;
        while (tmp) {
            cellule *next = tmp->next;
            free(tmp);
            tmp = next;
        }
    }

    free(G.adjacente);

    printf("\nFin du test.\n");
    return 0;
}
