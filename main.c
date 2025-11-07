#include <stdio.h>
#include <stdlib.h>
#include "fonctions.h"

int main() {
    // Chargement du graphe depuis un fichier texte
    liste_d_adjacence G = readGraph("scratch.txt");
    t_partition partition = tarjan(&G);

    // Affiche la partition finale
    for (int i = 0; i < partition.nbr; i++) {
        printf("Classe %d : ", i + 1);
        for (int j = 0; j < partition.classes[i]->nbr; j++) {
            printf("%d ", partition.classes[i]->sommets[j]->id);
        }
        printf("\n");
    }


    return 0;
}
