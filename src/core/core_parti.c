/*
 * core_parti.c — Logique metier des partis politiques (sans I/O console).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core_interne.h"

static int parti_correspond(const void *enreg, const void *cle) {
    return strcasecmp(((const partie_politic *)enreg)->id_PP,
                      (const char *)cle) == 0;
}

int core_parti_ajouter(const partie_politic *p, char *err, size_t err_len) {
    if (p == NULL || p->id_PP[0] == '\0') {
        core_erreur(err, err_len, "L'identifiant du parti politique est vide.");
        return EL_ERR_INVALIDE;
    }
    if (core_existe_parti(p->id_PP)) {
        core_erreur(err, err_len,
                    "Un parti politique avec l'id '%s' existe deja !", p->id_PP);
        return EL_ERR_DOUBLON;
    }
    if (core_ajouter_enreg(FICHIER_PARTIS, p, sizeof(partie_politic)) != EL_OK) {
        core_erreur(err, err_len, "Erreur d'ouverture du fichier %s", FICHIER_PARTIS);
        return EL_ERR_FICHIER;
    }
    return EL_OK;
}

int core_parti_modifier(const partie_politic *p, char *err, size_t err_len) {
    int r;

    if (p == NULL || p->id_PP[0] == '\0') {
        core_erreur(err, err_len, "L'identifiant du parti politique est vide.");
        return EL_ERR_INVALIDE;
    }
    r = core_remplacer_enreg(FICHIER_PARTIS, sizeof(partie_politic),
                             parti_correspond, p->id_PP, p);
    if (r < 0) {
        core_erreur(err, err_len, "Erreur d'ouverture du fichier %s", FICHIER_PARTIS);
        return r;
    }
    if (r == 0) {
        core_erreur(err, err_len, "Aucun parti politique trouve avec l'ID : %s", p->id_PP);
        return EL_ERR_INTROUVABLE;
    }
    return EL_OK;
}

int core_parti_obtenir(const char *id, partie_politic *out) {
    FILE *fichier = core_fichier_ouvrir(FICHIER_PARTIS, "rb");
    partie_politic temp;
    int trouve = 0;

    if (fichier == NULL) {
        return 0;
    }
    while (fread(&temp, sizeof(partie_politic), 1, fichier) == 1) {
        if (parti_correspond(&temp, id)) {
            *out = temp;
            trouve = 1;
            break;
        }
    }
    fclose(fichier);
    return trouve;
}

int core_parti_liste(partie_politic **out, int *count) {
    *out = core_lire_tout(FICHIER_PARTIS, sizeof(partie_politic), count);
    return EL_OK;
}
