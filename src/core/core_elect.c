/*
 * core_elect.c — Logique metier des electeurs (sans I/O console).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core_interne.h"

static int electeur_correspond(const void *enreg, const void *cle) {
    return strcasecmp(((const electeurs *)enreg)->NINU,
                      (const char *)cle) == 0;
}

int core_electeur_ajouter(const electeurs *e, char *err, size_t err_len) {
    if (e == NULL || e->NINU[0] == '\0') {
        core_erreur(err, err_len, "Le NINU de l'electeur est vide.");
        return EL_ERR_INVALIDE;
    }
    if (core_existe_NINU(e->NINU, NULL)) {
        core_erreur(err, err_len,
                    "Un electeur avec le NINU '%s' existe deja !", e->NINU);
        return EL_ERR_DOUBLON;
    }
    if (!core_existe_BV(e->BV)) {
        core_erreur(err, err_len,
                    "Le bureau de vote '%s' n'existe pas !", e->BV);
        return EL_ERR_INTROUVABLE;
    }
    if (core_ajouter_enreg(FICHIER_ELECTEURS, e, sizeof(electeurs)) != EL_OK) {
        core_erreur(err, err_len, "Erreur d'ouverture du fichier %s", FICHIER_ELECTEURS);
        return EL_ERR_FICHIER;
    }
    return EL_OK;
}

int core_electeur_modifier(const electeurs *e, char *err, size_t err_len) {
    int r;

    if (e == NULL || e->NINU[0] == '\0') {
        core_erreur(err, err_len, "Le NINU de l'electeur est vide.");
        return EL_ERR_INVALIDE;
    }
    if (!core_existe_BV(e->BV)) {
        core_erreur(err, err_len,
                    "Le bureau de vote '%s' n'existe pas !", e->BV);
        return EL_ERR_INTROUVABLE;
    }
    r = core_remplacer_enreg(FICHIER_ELECTEURS, sizeof(electeurs),
                             electeur_correspond, e->NINU, e);
    if (r < 0) {
        core_erreur(err, err_len, "Erreur d'ouverture du fichier %s", FICHIER_ELECTEURS);
        return r;
    }
    if (r == 0) {
        core_erreur(err, err_len, "Aucun electeur trouve avec l'ID : %s", e->NINU);
        return EL_ERR_INTROUVABLE;
    }
    return EL_OK;
}

int core_electeur_obtenir(const char *ninu, electeurs *out) {
    FILE *fichier = core_fichier_ouvrir(FICHIER_ELECTEURS, "rb");
    electeurs temp;
    int trouve = 0;

    if (fichier == NULL) {
        return 0;
    }
    while (fread(&temp, sizeof(electeurs), 1, fichier) == 1) {
        if (electeur_correspond(&temp, ninu)) {
            *out = temp;
            trouve = 1;
            break;
        }
    }
    fclose(fichier);
    return trouve;
}

int core_electeur_liste(electeurs **out, int *count) {
    *out = core_lire_tout(FICHIER_ELECTEURS, sizeof(electeurs), count);
    return EL_OK;
}
