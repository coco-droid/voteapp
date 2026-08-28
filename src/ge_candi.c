/*
 * ge_candi.c — Interface console des candidats.
 * Toute la logique metier est deleguee a core/core_candid.c.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "election.h"
#include "election_core.h"

// Saisie du type de candidat : "0" = independant, sinon ID d'un parti existant
static void saisir_type_candidat(char *type, size_t taille) {
    int choix = 0;

    do {
        printf("\nEntrer le type de candidat :\n\n1. Independant\n2. Sous banniere d'un parti politique\nChoix : ");
        if (scanf("%d", &choix) != 1) {
            vider_buffer();
            continue;
        }
        vider_buffer();

        if (choix == 1) {
            strcpy(type, "0");
            printf("\n>> Candidature enregistree sous le statut : INDEPENDANT (Type 0)\n");
        }
        else if (choix == 2) {
            do {
                printf("\nEntrer l'ID du Parti Politique : ");
                lire_chaine(type, (int)taille);
                if (!core_existe_parti(type)) {
                    printf(">> ERREUR : Le parti avec l'ID '%s' n'existe pas ! Veuillez reessayer.\n", type);
                } else {
                    break;
                }
            } while (1);
        }
        else {
            printf(">> Choix invalide ! Entrez 1 pour Independant ou 2 pour Parti.\n");
        }
    } while (choix != 1 && choix != 2);
}

static void ecrire_candidat(void) {
    candidats candi;
    char err[200];

    vider_buffer();
    memset(&candi, 0, sizeof(candi));

    // 1. Saisie de l'ID Candidat
    do {
        printf("\nEntrer l'ID du candidat : ");
        lire_chaine(candi.ID_candid, sizeof(candi.ID_candid));
        if (core_existe_candidat(candi.ID_candid)) {
            printf("\n>> ERREUR : Un candidat avec l'id '%s' est deja enregistre ! Veuillez reessayer.\n", candi.ID_candid);
        } else {
            break;
        }
    } while (1);

    // 2. Nom et Prenom
    printf("\nEntrer le nom du candidat : ");
    lire_chaine(candi.Nom, sizeof(candi.Nom));

    printf("\nEntrer le prenom du candidat : ");
    lire_chaine(candi.Prenom, sizeof(candi.Prenom));

    // 3. Date de Naissance
    printf("\nEntrer la date de naissance (JJ/MM/AAAA) : ");
    lire_chaine(candi.Dat_Nais, sizeof(candi.Dat_Nais));

    // 4. Adresse, Email, Telephone
    saisir_adresse("L'adresse du candidat", candi.Adresse, sizeof(candi.Adresse));

    printf("\nEntrer l'email : ");
    lire_chaine(candi.email, sizeof(candi.email));

    printf("\nEntrer le telephone : ");
    lire_chaine(candi.tel, sizeof(candi.tel));

    // 5. Type (independant ou parti)
    saisir_type_candidat(candi.type, sizeof(candi.type));

    // 6. Date d'inscription
    printf("\nEntrer la date d'inscription (JJ/MM/AAAA) : ");
    lire_chaine(candi.Date_ins, sizeof(candi.Date_ins));

    // 7. Enregistrement
    if (core_candidat_ajouter(&candi, err, sizeof(err)) != EL_OK) {
        printf("\n>> ERREUR : %s\n", err);
        return;
    }
    printf("\nCandidat enregistre avec succes !\n");
}

static void afficher_candidat(void) {
    candidats *liste = NULL;
    int n = 0, i;

    core_candidat_liste(&liste, &n);
    if (n == 0) {
        printf("\nAucun candidat enregistre (ou ouverture de candid.dat impossible)\n\n");
        free(liste);
        return;
    }

    printf("\n========================================================================================================================================================================\n");
    printf(" %-7s | %-17s | %-20s | %-13s | %-30s | %-23s | %-12s | %-7s | %-13s\n", "ID", "NOM", "PRENOM", "DATE_NAIS", "ADRESSE", "EMAIL", "TELEPHONE", "TYPE", "DATE_INSC");
    printf("========================================================================================================================================================================\n");

    for (i = 0; i < n; i++) {
        printf(" %-7s | %-17s | %-20s | %-13s | %-30s | %-23s | %-12s | %-7s | %-13s\n",
               liste[i].ID_candid, liste[i].Nom, liste[i].Prenom, liste[i].Dat_Nais,
               liste[i].Adresse, liste[i].email, liste[i].tel, liste[i].type, liste[i].Date_ins);
    }
    printf("========================================================================================================================================================================\n");
    free(liste);
}

static void modifier_candidat(char id_modif[]) {
    candidats candi;
    char err[200];
    int choix_mod = 0;

    if (!core_candidat_obtenir(id_modif, &candi)) {
        printf("\nAucun candidat trouve avec l'ID : %s\n", id_modif);
        return;
    }

    do {
        printf("\n\n========== MODIFICATION DES DONNEES DU CANDIDAT : (ID: %s) ==========\n", candi.ID_candid);
        printf("1. Date de naissance du candidat\n");
        printf("2. Adresse du candidat\n");
        printf("3. Email du candidat\n");
        printf("4. Telephone du candidat\n");
        printf("5. Type du candidat\n");
        printf("6. Date d'inscription du candidat\n");
        printf("7. Enregistrer les modifications et Quitter\n");
        printf("=============================================================\n");
        printf("Choix : ");

        if (scanf("%d", &choix_mod) != 1) {
            vider_buffer();
            continue;
        }
        vider_buffer();

        switch (choix_mod) {
            case 1:
                printf("\nEntrer la nouvelle date de naissance du candidat (JJ/MM/AAAA) : ");
                lire_chaine(candi.Dat_Nais, sizeof(candi.Dat_Nais));
                break;

            case 2:
                saisir_adresse("La nouvelle adresse du candidat", candi.Adresse, sizeof(candi.Adresse));
                break;

            case 3:
                printf("\nEntrer le nouvel email du candidat : ");
                lire_chaine(candi.email, sizeof(candi.email));
                break;

            case 4:
                printf("\nEntrer le nouveau numero de telephone du candidat : ");
                lire_chaine(candi.tel, sizeof(candi.tel));
                break;

            case 5:
                saisir_type_candidat(candi.type, sizeof(candi.type));
                break;

            case 6:
                printf("\nEntrer la nouvelle date d'inscription du candidat : ");
                lire_chaine(candi.Date_ins, sizeof(candi.Date_ins));
                break;

            case 7:
                printf("\nSauvegarde des modifications en cours...\n");
                break;

            default:
                printf("\nChoix invalide !\n");
                break;
        }
    } while (choix_mod != 7);

    if (core_candidat_modifier(&candi, err, sizeof(err)) == EL_OK) {
        printf("\nModification effectuee avec succes !\n");
    } else {
        printf("\n>> ERREUR : %s\n", err);
    }
}

int ge_candi(void) {
    int choix = 0;
    char id[10];

    printf("\n\n================== GESTION CANDIDATS ==================\n\n");
    do {
        printf("\n1. AJOUT CANDIDATS\n2. AFFICHAGE\n3. MODIFIER\n4. QUITTER MENU CANDIDAT\n\nChoix : ");

        if (scanf("%d", &choix) != 1) {
            vider_buffer();
            continue;
        }

        switch (choix) {
            case 1:
                ecrire_candidat();
                break;

            case 2:
                afficher_candidat();
                break;

            case 3:
                printf("Entrer l'ID a modifier : ");
                scanf("%9s", id);
                vider_buffer();
                modifier_candidat(id);
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
