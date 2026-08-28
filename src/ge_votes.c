/*
 * ge_votes.c — Interface console des votes.
 * Toute la logique metier est deleguee a core/core_vote.c.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "election.h"
#include "election_core.h"

static void ecrire_votes(void) {
    votes vote_candi;
    char err[200];
    char bv[15];

    vider_buffer();
    memset(&vote_candi, 0, sizeof(vote_candi));

    // Saisie de l'ID du vote
    do {
        printf("\nEntrer l'ID du vote : ");
        lire_chaine(vote_candi.Id_vote, sizeof(vote_candi.Id_vote));
        if (core_existe_vote(vote_candi.Id_vote)) {
            printf("\n>> ERREUR : Un vote avec l'id '%s' est deja enregistre ! Veuillez reessayer.\n", vote_candi.Id_vote);
        } else {
            break;
        }
    } while (1);

    // Saisie du NINU avec verifications
    do {
        int existe_ninu, deja_vote;

        printf("\nEntrer le NINU de l'electeur : ");
        lire_chaine(vote_candi.NINU, sizeof(vote_candi.NINU));

        // 1. Verifie si l'electeur existe et recupere son BV
        existe_ninu = core_existe_NINU(vote_candi.NINU, bv);
        // 2. Verifie si le NINU a deja vote
        deja_vote = core_ninu_a_deja_vote(vote_candi.NINU);

        if (!existe_ninu) {
            printf("\n>> ERREUR : L'electeur avec le NINU '%s' n'existe pas ! Veuillez reessayer.\n", vote_candi.NINU);
        }
        else if (deja_vote) {
            printf("\n>> ERREUR : L'electeur avec le NINU '%s' a DEJA VOTE ! Veuillez reessayer.\n", vote_candi.NINU);
        }
        else {
            printf("\n>> Bureau de vote associe : %s\n", bv);
            break;
        }
    } while (1);

    // Saisie de l'ID du candidat (avec gestion du vote blanc "0")
    do {
        printf("\nEntrer l'ID du candidat (ou '0' pour vote blanc) : ");
        lire_chaine(vote_candi.id_candid, sizeof(vote_candi.id_candid));

        if (strcmp(vote_candi.id_candid, "0") == 0) {
            break; // Vote blanc valide
        }
        if (!core_existe_candidat(vote_candi.id_candid)) {
            printf("\n>> ERREUR : Le candidat avec l'id '%s' n'existe pas ! Veuillez reessayer.\n", vote_candi.id_candid);
        } else {
            break;
        }
    } while (1);

    // Enregistrement : le core re-verifie tout, remplit Date_vote et BV
    if (core_vote_ajouter(&vote_candi, err, sizeof(err)) != EL_OK) {
        printf("\n>> ERREUR : %s\n", err);
        return;
    }
    printf("\nLa date du vote generee automatiquement : %s\n", vote_candi.Date_vote);
    printf("\n>> Vote enregistre avec succes !\n");
}

static void afficher_votes(void) {
    votes *liste = NULL;
    int n = 0, i;

    core_vote_liste(&liste, &n);
    if (n == 0) {
        printf("\nAucun vote enregistre (ou ouverture de votes.dat impossible)\n\n");
        free(liste);
        return;
    }

    printf("\n===============================================================\n");
    printf(" %-7s | %-13s | %-7s | %-11s | %-7s\n", "ID-VOTE", "DATE VOTE", "B-V", "NINU", "ID-CANDID");
    printf("===============================================================\n");

    for (i = 0; i < n; i++) {
        printf(" %-7s | %-13s | %-7s | %-11s | %-7s\n",
               liste[i].Id_vote, liste[i].Date_vote, liste[i].BV, liste[i].NINU, liste[i].id_candid);
    }
    printf("===============================================================\n");
    free(liste);
}

int ge_vote(void) {
    int choix = 0;

    printf("\n\n================== GESTION VOTES ==================\n\n");
    do {
        printf("\n1. AJOUT VOTES\n2. AFFICHAGE\n3. QUITTER MENU VOTES\n\nChoix : ");

        if (scanf("%d", &choix) != 1) {
            vider_buffer();
            continue;
        }

        switch (choix) {
            case 1:
                ecrire_votes();
                break;

            case 2:
                afficher_votes();
                break;

            case 3:
                printf("\nRetour au menu principal...\n");
                break;

            default:
                printf("\nOption invalide !\n");
                break;
        }
    } while (choix != 3);

    return 0;
}
