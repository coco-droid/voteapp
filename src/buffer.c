#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "election.h"

// Fonction auxiliaire pour vider le buffer stdin
void vider_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Fonction de lecture securisee pour chaines de caracteres
void lire_chaine(char *chaine, int taille) {
    if (fgets(chaine, taille, stdin) != NULL) {
        size_t len = strlen(chaine);
        if (len > 0 && chaine[len - 1] == '\n') {
            chaine[len - 1] = '\0'; // Supprime le saut de ligne
        } else {
            // Saisie trop longue : vider le surplus restant dans le flux stdin
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        }
    }
}

// Saisie guidee d'une adresse "No Rue, Rue, Departement"
void saisir_adresse(const char *libelle, char *dest, size_t taille) {
    char no_rue[6], rue[20], dept[30];

    printf("\n%s (No Rue, Rue, Departement) : ", libelle);
    printf("\nEntrer le No de rue (ex : 12) : ");
    lire_chaine(no_rue, sizeof(no_rue));
    printf("\nEntrer la Rue (ex : Rue Capois) : ");
    lire_chaine(rue, sizeof(rue));
    printf("\nEntrer le Departement (ex : Nord) : ");
    lire_chaine(dept, sizeof(dept));

    // Assemblage de l'adresse (borne, contrairement a l'ancien sprintf)
    snprintf(dest, taille, "%s, %s, %s", no_rue, rue, dept);
}
