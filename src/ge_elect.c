/*
 * ge_elect.c — Interface console des electeurs.
 * Toute la logique metier est deleguee a core/core_elect.c.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "election.h"
#include "election_core.h"

static void ecrire_electeur(void) {
    electeurs elec;
    char err[200];

    vider_buffer();
    memset(&elec, 0, sizeof(elec));

    // --- BOUCLE DE VERIFICATION DU NINU (UNICITE) ---
    do {
        printf("\nEntrer le NINU de l'electeur : ");
        lire_chaine(elec.NINU, sizeof(elec.NINU));
        if (core_existe_NINU(elec.NINU, NULL)) {
            printf("\n>> ERREUR : Un electeur avec le NINU '%s' existe deja ! Veuillez reessayer.\n", elec.NINU);
        } else {
            break;
        }
    } while (1);

    printf("\nEntrer le nom de l'electeur : ");
    lire_chaine(elec.nom, sizeof(elec.nom));

    printf("\nEntrer le prenom de l'electeur : ");
    lire_chaine(elec.prenom, sizeof(elec.prenom));

    saisir_adresse("L'adresse de l'electeur", elec.Adresse, sizeof(elec.Adresse));

    // --- BOUCLE DE VERIFICATION DU BUREAU DE VOTE ---
    do {
        printf("\nEntrer l'Id du bureau de votes assigne a l'electeur : ");
        lire_chaine(elec.BV, sizeof(elec.BV));
        if (!core_existe_BV(elec.BV)) {
            printf(">> ERREUR : Le bureau de vote '%s' n'existe pas ! Veuillez entrer un ID valide.\n", elec.BV);
        } else {
            break;
        }
    } while (1);

    if (core_electeur_ajouter(&elec, err, sizeof(err)) != EL_OK) {
        printf("\n>> ERREUR : %s\n", err);
        return;
    }
    printf("\nDonnees enregistrees avec succes !\n");
}

static void affichage_electeur(void) {
    electeurs *liste = NULL;
    int n = 0, i;

    core_electeur_liste(&liste, &n);
    if (n == 0) {
        printf("\nAucun electeur enregistre (ou ouverture de elect.dat impossible)\n\n");
        free(liste);
        return;
    }

    printf("\n==========================================================================================================\n");
    printf(" %-10s | %-20s | %-20s | %-30s | %-10s\n", "NINU", "NOM", "PRENOM", "ADRESSE", "ID BUREAU");
    printf("==========================================================================================================\n");

    for (i = 0; i < n; i++) {
        printf(" %-10s | %-20s | %-20s | %-30s | %-10s\n",
               liste[i].NINU, liste[i].nom, liste[i].prenom, liste[i].Adresse, liste[i].BV);
    }
    printf("==========================================================================================================\n");
    free(liste);
}

static void modifier_electeur(char id_modif[]) {
    electeurs elect;
    char err[200];
    int choix_mod = 0;

    if (!core_electeur_obtenir(id_modif, &elect)) {
        printf("\nAucun electeur trouve avec l'ID : %s\n", id_modif);
        return;
    }

    do {
        printf("\n\n========== MODIFICATION D'INFORMATION DE L'ELECTEUR : (ID: %s) ==========\n", elect.NINU);
        printf("1. Nom de l'electeur\n");
        printf("2. Prenom de l'electeur\n");
        printf("3. Adresse de l'electeur\n");
        printf("4. Id du Bureau de vote assigne a l'electeur\n");
        printf("5. Enregistrer les modifications et Quitter\n");
        printf("=============================================================\n");
        printf("Choix : ");

        if (scanf("%d", &choix_mod) != 1) {
            vider_buffer();
            continue;
        }
        vider_buffer();

        switch (choix_mod) {
            case 1:
                printf("Entrer le nouveau nom de l'electeur : ");
                lire_chaine(elect.nom, sizeof(elect.nom));
                break;

            case 2:
                printf("Entrer le nouveau prenom de l'electeur : ");
                lire_chaine(elect.prenom, sizeof(elect.prenom));
                break;

            case 3:
                saisir_adresse("L'adresse de l'electeur", elect.Adresse, sizeof(elect.Adresse));
                break;

            case 4:
                do {
                    printf("Entrer le nouveau Id assigne a l'electeur : ");
                    lire_chaine(elect.BV, sizeof(elect.BV));
                    if (!core_existe_BV(elect.BV)) {
                        printf(">> ERREUR : Le bureau de vote '%s' n'existe pas ! Veuillez entrer un ID valide.\n", elect.BV);
                    } else {
                        break;
                    }
                } while (1);
                break;

            case 5:
                printf("\nSauvegarde des modifications en cours...\n");
                break;

            default:
                printf("\nChoix invalide !\n");
                break;
        }
    } while (choix_mod != 5);

    if (core_electeur_modifier(&elect, err, sizeof(err)) == EL_OK) {
        printf("\nModification effectuee avec succes !\n");
    } else {
        printf("\n>> ERREUR : %s\n", err);
    }
}

int ge_elect(void) {
    int choix = 0;
    char id[15];

    printf("\n\n================== GESTION ELECTEUR ==================\n\n");
    do {
        printf("\n1. AJOUT ELECTEUR\n2. AFFICHAGE\n3. MODIFIER\n4. QUITTER MENU ELECTEURS\n\nChoix : ");

        if (scanf("%d", &choix) != 1) {
            vider_buffer();
            continue;
        }

        switch (choix) {
            case 1:
                ecrire_electeur();
                break;

            case 2:
                affichage_electeur();
                break;

            case 3:
                printf("Entrer l'ID a modifier : ");
                scanf("%14s", id);
                vider_buffer();
                modifier_electeur(id);
                break;

            case 4:
                printf("\nRetour au menu principal...\n");
                break;

            default:
                printf("\nOption invalide !\n");
                break;
        }
    } while (choix != 4);

    return 0;
}
