#include <stdio.h>
#include <stdlib.h>
#include "fonctions.h"

int main() {
    printf("=== TEST TARJAN : lecture du graphe + création du tableau ===\n\n");

    // Lecture du graphe depuis un fichier
    liste_d_adjacence G = readGraph("scratch.txt");
    export_mermaid(&G, "test");


    return 0;
}
