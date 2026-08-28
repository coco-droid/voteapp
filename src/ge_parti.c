/*
 * ge_parti.c — Interface console des partis politiques.
 * Toute la logique metier est deleguee a core/core_parti.c.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "election.h"
#include "election_core.h"

static void ecrire_partie(void) {
    partie_politic partie;
    char err[200];
    int rc;

    vider_buffer(); // Nettoie le \n laisse par le scanf du menu
    memset(&partie, 0, sizeof(partie));

    // Verification interactive de l'unicite de l'ID (re-verifiee par le core)
    do {
        printf("\nEntrer l'Id du parti politique : ");
        lire_chaine(partie.id_PP, sizeof(partie.id_PP));
        if (core_existe_parti(partie.id_PP)) {
            printf("\n>> ERREUR : Un parti politique avec l'id '%s' existe deja ! Veuillez reessayer.\n", partie.id_PP);
        } else {
            break;
        }
    } while (1);

    printf("\nEntrer le nom du parti politique : ");
    lire_chaine(partie.Nom, sizeof(partie.Nom));

    printf("\nEntrer la date de creation du parti politique (JJ/MM/AAAA) : ");
    lire_chaine(partie.date_creat, sizeof(partie.date_creat));

    printf("\nEntrer la date d'inscription du parti politique (JJ/MM/AAAA) : ");
    lire_chaine(partie.date_ins, sizeof(partie.date_ins));

    saisir_adresse("L'adresse du parti politique", partie.Adresse, sizeof(partie.Adresse));

    printf("\nEntrer le nom du responsable du parti politique : ");
    lire_chaine(partie.Responsable, sizeof(partie.Responsable));

    printf("\nEntrer le telephone du parti politique : ");
    lire_chaine(partie.Telephone, sizeof(partie.Telephone));

    printf("\nEntrer l'email du parti politique : ");
    lire_chaine(partie.email, sizeof(partie.email));

    rc = core_parti_ajouter(&partie, err, sizeof(err));
    if (rc != EL_OK) {
        printf("\n>> ERREUR : %s\n", err);
        return;
    }
    printf("\nDonnees enregistrees avec succes !\n");
}

static void afficher_PP(void) {
    partie_politic *liste = NULL;
    int n = 0, i;

    core_parti_liste(&liste, &n);
    if (n == 0) {
        printf("\nAucune donnee trouvee ou erreur d'ouverture du fichier p_pol.dat\n\n");
        free(liste);
        return;
    }

    printf("\n========================================================================================================================================================================\n");
    printf(" %-10s | %-20s | %-13s | %-13s | %-30s | %-20s | %-12s | %-25s\n",
           "ID_PP", "NOM", "DATE CREATION", "DATE INSCRIT", "ADRESSE", "RESPONSABLE", "TELEPHONE", "EMAIL");
    printf("========================================================================================================================================================================\n");

    for (i = 0; i < n; i++) {
        printf(" %-10s | %-20s | %-13s | %-13s | %-30s | %-20s | %-12s | %-25s\n",
               liste[i].id_PP, liste[i].Nom, liste[i].date_creat, liste[i].date_ins,
               liste[i].Adresse, liste[i].Responsable, liste[i].Telephone, liste[i].email);
    }
    printf("========================================================================================================================================================================\n");
    free(liste);
}

static void modifier_PP(char id_modif[]) {
    partie_politic partie;
    char err[200];
    int choix_mod = 0;

    if (!core_parti_obtenir(id_modif, &partie)) {
        printf("\nAucun parti politique trouve avec l'ID : %s\n\n", id_modif);
        return;
    }

    do {
        printf("\n\n========== MODIFICATION DU PARTI POLITIQUE : %s (ID: %s) ==========\n", partie.Nom, partie.id_PP);
        printf("1. Nom\n");
        printf("2. Date de Creation\n");
        printf("3. Date d'Inscription\n");
        printf("4. Adresse\n");
        printf("5. Responsable\n");
        printf("6. Telephone\n");
        printf("7. Email\n");
        printf("8. Enregistrer les modifications et Quitter\n");
        printf("=============================================================\n");
        printf("Choix : ");

        if (scanf("%d", &choix_mod) != 1) {
            vider_buffer();
            continue;
        }
        vider_buffer();

        switch (choix_mod) {
            case 1:
                printf("Entrer le nouveau Nom : ");
                lire_chaine(partie.Nom, sizeof(partie.Nom));
                break;

            case 2:
                printf("Entrer la nouvelle Date de Creation (JJ/MM/AAAA) : ");
                lire_chaine(partie.date_creat, sizeof(partie.date_creat));
                break;

            case 3:
                printf("Entrer la nouvelle Date d'Inscription (JJ/MM/AAAA) : ");
                lire_chaine(partie.date_ins, sizeof(partie.date_ins));
                break;

            case 4:
                saisir_adresse("L'adresse du parti politique", partie.Adresse, sizeof(partie.Adresse));
                break;

            case 5:
                printf("Entrer le nouveau Responsable : ");
                lire_chaine(partie.Responsable, sizeof(partie.Responsable));
                break;

            case 6:
                printf("Entrer le nouveau Telephone : ");
                lire_chaine(partie.Telephone, sizeof(partie.Telephone));
                break;

            case 7:
                printf("Entrer le nouvel Email : ");
                lire_chaine(partie.email, sizeof(partie.email));
                break;

            case 8:
                printf("\nSauvegarde des modifications en cours...\n");
                break;

            default:
                printf("\nChoix invalide !\n");
                break;
        }
    } while (choix_mod != 8);

    if (core_parti_modifier(&partie, err, sizeof(err)) == EL_OK) {
        printf("\nModification effectuee avec succes !\n\n");
    } else {
        printf("\n>> ERREUR : %s\n", err);
    }
}

int p_pol(void) {
    int choix = 0;
    char id_PP[10];

    printf("\n================== GESTION PARTIS POLITIQUES ==================\n");
    do {
        printf("\n1. AJOUT PARTI POLITIQUE\n2. AFFICHAGE\n3. MODIFIER\n4. QUITTER LE MENU PARTIS POLITIQUE\n\nChoix : ");
        if (scanf("%d", &choix) != 1) {
            vider_buffer();
            continue;
        }

        switch (choix) {
            case 1:
                ecrire_partie();
                break;

            case 2:
                afficher_PP();
                break;

            case 3:
                printf("Entrer l'ID a modifier : ");
                scanf("%9s", id_PP);
                vider_buffer();
                modifier_PP(id_PP);
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
