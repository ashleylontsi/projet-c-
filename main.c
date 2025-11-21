#include <stdio.h>
#include <stdlib.h>
#include "fonctions.h"

int main() {
    // Chargement du graphe depuis un fichier texte
    liste_d_adjacence G = readGraph("scratch.txt");
    t_partition partition = tarjan(&G);

    // Affiche la partition finale
    printf("=== PARTITION DU GRAPHE ===\n");
    for (int i = 0; i < partition.nbr; i++) {
        printf("Classe %d : ", i + 1);
        for (int j = 0; j < partition.classes[i]->nbr; j++) {
            printf("%d ", partition.classes[i]->sommets[j]->id);
        }
        printf("\n");
    }

    // Étape 2 - Trouver les liens entre classes
    t_link_array *link_array = trouver_liens_classes(&G, &partition);

    // Après avoir trouvé les liens
    printf("=== LIENS TROUVES ===\n");
    for (int i = 0; i < link_array->nbr_liens; i++) {
        printf("Classe %d -> Classe %d\n",
               link_array->liens[i].classe_depart,
               link_array->liens[i].classe_arrivee);
    }

    // Étape 3 - Analyser les caractéristiques
    analyser_caracteristiques(&partition, link_array);

    // Libération mémoire
    free(link_array->liens);
    free(link_array);

    return 0;
}


