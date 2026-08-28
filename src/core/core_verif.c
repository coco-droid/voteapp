/*
 * core_verif.c — Verifications d'existence et d'unicite.
 * Reprend la logique de l'ancien ge_verification.c, sans aucune I/O console.
 */
#include <stdio.h>
#include <string.h>

#include "core_interne.h"

/* Verifie si un NINU existe dans elect.dat ; copie optionnellement son BV */
int core_existe_NINU(const char *ninu, char *bv_out) {
    FILE *fichier = core_fichier_ouvrir(FICHIER_ELECTEURS, "rb");
    electeurs temp;
    int trouve = 0;

    if (fichier == NULL) {
        return 0;
    }
    while (fread(&temp, sizeof(electeurs), 1, fichier) == 1) {
        if (strcasecmp(temp.NINU, ninu) == 0) {
            trouve = 1;
            if (bv_out != NULL) {
                strcpy(bv_out, temp.BV);
            }
            break;
        }
    }
    fclose(fichier);
    return trouve;
}

/* Verifie si un ID est deja membre (President, VP ou Secretaire) d'un BV */
int core_existe_membre_bv(const char *id_personne) {
    FILE *fichier = core_fichier_ouvrir(FICHIER_BUREAUX, "rb");
    bureau_vote temp;
    int trouve = 0;

    if (fichier == NULL) {
        return 0;
    }
    while (fread(&temp, sizeof(bureau_vote), 1, fichier) == 1) {
        if (strcasecmp(temp.Id_President, id_personne) == 0 ||
            strcasecmp(temp.Id_VPresident, id_personne) == 0 ||
            strcasecmp(temp.Id_Secret, id_personne) == 0) {
            trouve = 1;
            break;
        }
    }
    fclose(fichier);
    return trouve;
}

int core_existe_BV(const char *id_bv) {
    FILE *fichier = core_fichier_ouvrir(FICHIER_BUREAUX, "rb");
    bureau_vote temp;
    int trouve = 0;

    if (fichier == NULL) {
        return 0;
    }
    while (fread(&temp, sizeof(bureau_vote), 1, fichier) == 1) {
        if (strcasecmp(temp.Id_BV, id_bv) == 0) {
            trouve = 1;
            break;
        }
    }
    fclose(fichier);
    return trouve;
}

int core_existe_parti(const char *id_parti) {
    FILE *fichier = core_fichier_ouvrir(FICHIER_PARTIS, "rb");
    partie_politic temp;
    int trouve = 0;

    if (fichier == NULL) {
        return 0;
    }
    while (fread(&temp, sizeof(partie_politic), 1, fichier) == 1) {
        if (strcasecmp(temp.id_PP, id_parti) == 0) {
            trouve = 1;
            break;
        }
    }
    fclose(fichier);
    return trouve;
}

int core_existe_candidat(const char *id_candid) {
    FILE *fichier = core_fichier_ouvrir(FICHIER_CANDIDATS, "rb");
    candidats temp;
    int trouve = 0;

    if (fichier == NULL) {
        return 0;
    }
    while (fread(&temp, sizeof(candidats), 1, fichier) == 1) {
        if (strcasecmp(temp.ID_candid, id_candid) == 0) {
            trouve = 1;
            break;
        }
    }
    fclose(fichier);
    return trouve;
}

int core_existe_vote(const char *id_vote) {
    FILE *fichier = core_fichier_ouvrir(FICHIER_VOTES, "rb");
    votes temp;
    int trouve = 0;

    if (fichier == NULL) {
        return 0;
    }
    while (fread(&temp, sizeof(votes), 1, fichier) == 1) {
        if (strcasecmp(temp.Id_vote, id_vote) == 0) {
            trouve = 1;
            break;
        }
    }
    fclose(fichier);
    return trouve;
}

int core_ninu_a_deja_vote(const char *ninu) {
    FILE *fichier = core_fichier_ouvrir(FICHIER_VOTES, "rb");
    votes v;

    if (fichier == NULL) {
        return 0;
    }
    while (fread(&v, sizeof(votes), 1, fichier) == 1) {
        if (strcasecmp(v.NINU, ninu) == 0) {
            fclose(fichier);
            return 1;
        }
    }
    fclose(fichier);
    return 0;
}
