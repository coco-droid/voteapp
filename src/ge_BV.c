/*
 * ge_BV.c — Interface console des bureaux de vote.
 * Toute la logique metier est deleguee a core/core_bv.c.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "election.h"
#include "election_core.h"

static void ecrire_bureau(void) {
    bureau_vote e;
    char err[200];

    vider_buffer();
    memset(&e, 0, sizeof(e));

    // --- 1. ID BUREAU DE VOTE ---
    do {
        printf("\nEntrer l'Id du bureau de vote : ");
        lire_chaine(e.Id_BV, sizeof(e.Id_BV));
        if (core_existe_BV(e.Id_BV)) {
            printf("\n>> ERREUR : Un bureau de vote avec l'id '%s' existe deja ! Veuillez reessayer.\n", e.Id_BV);
        } else {
            break;
        }
    } while (1);

    // --- 2. ADRESSE ---
    saisir_adresse("L'adresse du bureau de vote", e.Adresse_BV, sizeof(e.Adresse_BV));

    // --- 3. PRESIDENT ---
    do {
        printf("\nEntrer l'Id du president du bureau de vote : ");
        lire_chaine(e.Id_President, sizeof(e.Id_President));
        if (core_existe_membre_bv(e.Id_President)) {
            printf("\n>> ERREUR : La personne '%s' est deja membre d'un autre bureau de vote !\n", e.Id_President);
        } else {
            break;
        }
    } while (1);

    // --- 4. VICE PRESIDENT ---
    do {
        printf("\nEntrer l'Id du Vice president du bureau de vote : ");
        lire_chaine(e.Id_VPresident, sizeof(e.Id_VPresident));

        if (strcasecmp(e.Id_VPresident, e.Id_President) == 0) {
            printf("\n>> ERREUR : Le Vice-President ne peut pas avoir le meme ID que le President !\n");
        } else if (core_existe_membre_bv(e.Id_VPresident)) {
            printf("\n>> ERREUR : La personne '%s' est deja membre d'un autre bureau de vote !\n", e.Id_VPresident);
        } else {
            break;
        }
    } while (1);

    // --- 5. SECRETAIRE ---
    do {
        printf("\nEntrer l'Id du Secretaire du bureau de vote : ");
        lire_chaine(e.Id_Secret, sizeof(e.Id_Secret));

        if (strcasecmp(e.Id_Secret, e.Id_President) == 0 ||
            strcasecmp(e.Id_Secret, e.Id_VPresident) == 0) {
            printf("\n>> ERREUR : Le Secretaire ne peut pas etre la meme personne que le President ou le Vice-President !\n");
        } else if (core_existe_membre_bv(e.Id_Secret)) {
            printf("\n>> ERREUR : La personne '%s' est deja membre d'un autre bureau de vote !\n", e.Id_Secret);
        } else {
            break;
        }
    } while (1);

    // --- SAUVEGARDE (le core re-verifie toutes les contraintes) ---
    if (core_bv_ajouter(&e, err, sizeof(err)) != EL_OK) {
        printf("\n>> ERREUR : %s\n", err);
        return;
    }
    printf("\nDonnees enregistrees avec succes !\n");
}

static void afficher_BV(void) {
    bureau_vote *liste = NULL;
    int n = 0, i;

    core_bv_liste(&liste, &n);
    if (n == 0) {
        printf("\nAucun bureau de vote enregistre (ou ouverture de B_votes.dat impossible)\n\n");
        free(liste);
        return;
    }

    printf("\n=================================================================================================================\n");
    printf(" %-15s | %-35s | %-15s | %-15s | %-15s\n", "ID BUREAU", "ADRESSE", "ID PRESIDENT", "ID V-PRESIDENT", "ID SECRETAIRE");
    printf("=================================================================================================================\n");

    for (i = 0; i < n; i++) {
        printf(" %-15s | %-35s | %-15s | %-15s | %-15s\n",
               liste[i].Id_BV, liste[i].Adresse_BV, liste[i].Id_President,
               liste[i].Id_VPresident, liste[i].Id_Secret);
    }
    printf("=================================================================================================================\n");
    free(liste);
}

static void modifier_BV(char id_modif[]) {
    bureau_vote e;
    char err[200];
    int choix_mod = 0;

    if (!core_bv_obtenir(id_modif, &e)) {
        printf("\nAucun bureau de vote trouve avec l'ID : %s\n", id_modif);
        return;
    }

    do {
        // Nous pourrons modifier seulement l'adresse du bureau de vote
        printf("\n\n========== MODIFICATION DU BUREAU DE VOTE : (ID: %s) ==========\n", e.Id_BV);
        printf("1. Adresse du bureau de vote\n");
        printf("2. Enregistrer les modifications et Quitter\n");
        printf("=============================================================\n");
        printf("Choix : ");

        if (scanf("%d", &choix_mod) != 1) {
            vider_buffer();
            continue;
        }
        vider_buffer();

        switch (choix_mod) {
            case 1:
                saisir_adresse("L'adresse du bureau de vote", e.Adresse_BV, sizeof(e.Adresse_BV));
                break;

            case 2:
                printf("\nSauvegarde des modifications en cours...\n");
                break;

            default:
                printf("\nChoix invalide !\n");
                break;
        }
    } while (choix_mod != 2);

    if (core_bv_modifier(&e, err, sizeof(err)) == EL_OK) {
        printf("\nModification effectuee avec succes !\n");
    } else {
        printf("\n>> ERREUR : %s\n", err);
    }
}

int ge_BV(void) {
    int choix = 0;
    char id[10];

    printf("\n\n================== GESTION BUREAU VOTE ==================\n\n");
    do {
        printf("\n1. AJOUT BUREAU VOTE\n2. AFFICHAGE\n3. MODIFIER\n4. QUITTER MENU BUREAU DE VOTE\n\nChoix : ");

        if (scanf("%d", &choix) != 1) {
            vider_buffer();
            continue;
        }

        switch (choix) {
            case 1:
                ecrire_bureau();
                break;

            case 2:
                afficher_BV();
                break;

            case 3:
                printf("Entrer l'ID a modifier : ");
                scanf("%9s", id);
                vider_buffer();
                modifier_BV(id);
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
