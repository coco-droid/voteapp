/*
 * rapp.c — Rapports console : listes par BV/departement et resultats.
 * Les calculs sont delegues a core/core_rapp.c ; ce fichier ne fait
 * que de la mise en forme console.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "election.h"
#include "election_core.h"

// Affichage des electeurs par departement et par bureau de vote
static void liste_electeur(void) {
    bureau_vote *bureaux = NULL;
    electeurs *elecs = NULL;
    int nb_bv = 0, nb_el = 0, i, j;

    core_bv_liste(&bureaux, &nb_bv);
    core_electeur_liste(&elecs, &nb_el);

    if (nb_bv == 0) {
        printf("\n>> Aucun bureau de vote enregistre (ou ouverture des fichiers impossible)\n");
        free(bureaux);
        free(elecs);
        return;
    }

    printf("\n=========================================================================================================\n");
    printf("                                  LISTE DES ELECTEURS PAR DEPARTEMENT ET BV                              \n");
    printf("=========================================================================================================\n");

    for (i = 0; i < nb_bv; i++) {
        char depart[30];
        int trouve = 0;

        core_extraire_departement(bureaux[i].Adresse_BV, depart, sizeof(depart));

        printf("\n\n\nDEPARTEMENT : %-15s | ID-Bureau de vote : %-10s\n", depart, bureaux[i].Id_BV);
        printf("---------------------------------------------------------------------------------------------------------\n");
        printf(" %-15s | %-20s | %-20s | %-30s\n", "NINU", "NOM", "PRENOM", "ADRESSE");
        printf("---------------------------------------------------------------------------------------------------------\n");

        for (j = 0; j < nb_el; j++) {
            if (strcasecmp(elecs[j].BV, bureaux[i].Id_BV) == 0) {
                printf(" %-15s | %-20s | %-20s | %-30s\n",
                       elecs[j].NINU, elecs[j].nom, elecs[j].prenom, elecs[j].Adresse);
                trouve = 1;
            }
        }
        if (!trouve) printf(" (Aucun electeur trouve pour ce bureau de vote)\n");
    }

    printf("\n\n=========================================================================================================\n\n");
    free(bureaux);
    free(elecs);
}

static void liste_candidat(void) {
    candidats *liste = NULL;
    int n = 0, i;

    core_candidat_liste(&liste, &n);
    if (n == 0) {
        printf("\n>> Aucun candidat enregistre (ou ouverture de candid.dat impossible)\n\n");
        free(liste);
        return;
    }

    printf("\n===========================================================================================\n");
    printf("                           LISTE DES CANDIDATS A LA PRESIDENCE                         \n");
    printf("===========================================================================================\n");
    printf(" %-10s | %-20s | %-20s | %-25s\n", "ID CANDID", "NOM", "PRENOM", "TYPE / PARTI");
    printf("-------------------------------------------------------------------------------------------\n");

    for (i = 0; i < n; i++) {
        char type_str[30];
        if (strcmp(liste[i].type, "0") == 0) {
            strcpy(type_str, "INDEPENDANT");
        } else {
            sprintf(type_str, "PARTI ID: %s", liste[i].type);
        }
        printf(" %-10s | %-20s | %-20s | %-25s\n",
               liste[i].ID_candid, liste[i].Nom, liste[i].Prenom, type_str);
    }
    printf("===========================================================================================\n\n");
    free(liste);
}

static void liste_bureau_vote(void) {
    bureau_vote *liste = NULL;
    int n = 0, i;

    core_bv_liste(&liste, &n);
    if (n == 0) {
        printf("\n>> Aucun bureau de vote enregistre (fichier B_votes.dat introuvable)\n");
        free(liste);
        return;
    }

    printf("\n===========================================================================================================================\n");
    printf("                                                LISTE DES BUREAUX DE VOTE                                                  \n");
    printf("===========================================================================================================================\n");
    printf(" %-10s | %-35s | %-20s | %-20s | %-20s\n", "ID BV", "ADRESSE BV", "PRESIDENT (ID)", "VICE-PRES. (ID)", "SECRETAIRE (ID)");
    printf("---------------------------------------------------------------------------------------------------------------------------\n");

    for (i = 0; i < n; i++) {
        printf(" %-10s | %-35s | %-20s | %-20s | %-20s\n",
               liste[i].Id_BV, liste[i].Adresse_BV, liste[i].Id_President,
               liste[i].Id_VPresident, liste[i].Id_Secret);
    }
    printf("===========================================================================================================================\n\n");
    free(liste);
}

static void resultat(void) {
    core_resultats res;
    int k, i;

    if (core_resultats_calculer(&res) != EL_OK) {
        printf(">> ERREUR : Impossible de calculer les resultats\n");
        return;
    }

    printf("\n===============================================================================================\n");
    printf("                                    RESULTATS DU SCRUTIN                                       \n");
    printf("===============================================================================================\n");

    for (k = 0; k < res.nb_bureaux; k++) {
        core_resultat_bv *rb = &res.bureaux[k];
        int total_bv = rb->votes_blancs + rb->votes_non_blancs;

        printf("\n\nDEPARTEMENT : %-15s (BV: %s)\n", rb->departement, rb->id_bv);
        printf("-----------------------------------------------------------------------------------------------\n");
        printf(" %-20s | %-20s | %-12s | %-12s\n", "NOM CANDIDAT", "PRENOM", "VOTES OBTENUS", "POURCENTAGE");
        printf("-----------------------------------------------------------------------------------------------\n");

        for (i = 0; i < rb->nb_stats; i++) {
            printf(" %-20s | %-20s | %-13d | %-6.2f %%\n",
                   rb->stats[i].nom, rb->stats[i].prenom,
                   rb->stats[i].total_votes, rb->stats[i].pourcentage);
        }
        printf("-----------------------------------------------------------------------------------------------\n");
        printf(" Total votes non-blancs : %d | Total votes blancs : %d | Total General : %d\n",
               rb->votes_non_blancs, rb->votes_blancs, total_bv);
        printf("-----------------------------------------------------------------------------------------------\n");
    }

    printf("\n===============================================================================================\n");
    printf("                               RESUME GLOBAL POUR TOUT LE PAYS                                 \n");
    printf("===============================================================================================\n");
    printf(" %-20s | %-20s | %-12s | %-12s\n", "NOM CANDIDAT", "PRENOM", "VOTES NATIONAUX", "POURCENTAGE");
    printf("-----------------------------------------------------------------------------------------------\n");

    for (i = 0; i < res.nb_candidats; i++) {
        printf(" %-20s | %-20s | %-15d | %-6.2f %%\n",
               res.national[i].nom, res.national[i].prenom,
               res.national[i].total_votes, res.national[i].pourcentage);
    }

    printf("-----------------------------------------------------------------------------------------------\n");
    printf(" Total National Votes Non Blancs : %d\n", res.total_non_blancs);
    printf(" Total National Votes Blancs     : %d\n", res.total_blancs);
    printf(" Total National General          : %d\n", res.total_general);
    printf("===============================================================================================\n\n");

    core_resultats_liberer(&res);
}

int ge_rapp(void) {
    liste_electeur();
    printf("\n\n");
    liste_candidat();
    printf("\n\n");
    liste_bureau_vote();
    printf("\n\n");
    resultat();
    return 0;
}
