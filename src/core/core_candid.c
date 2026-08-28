/*
 * core_candid.c — Logique metier des candidats (sans I/O console).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core_interne.h"

static int candidat_correspond(const void *enreg, const void *cle) {
    return strcasecmp(((const candidats *)enreg)->ID_candid,
                      (const char *)cle) == 0;
}

/* type doit etre "0" (independant) ou l'ID d'un parti existant */
static int candidat_verifier_type(const char *type, char *err, size_t err_len) {
    if (strcmp(type, "0") == 0) {
        return EL_OK;
    }
    if (!core_existe_parti(type)) {
        core_erreur(err, err_len, "Le parti avec l'ID '%s' n'existe pas !", type);
        return EL_ERR_INTROUVABLE;
    }
    return EL_OK;
}

int core_candidat_ajouter(const candidats *c, char *err, size_t err_len) {
    int rc;

    if (c == NULL || c->ID_candid[0] == '\0') {
        core_erreur(err, err_len, "L'identifiant du candidat est vide.");
        return EL_ERR_INVALIDE;
    }
    if (core_existe_candidat(c->ID_candid)) {
        core_erreur(err, err_len,
                    "Un candidat avec l'id '%s' est deja enregistre !", c->ID_candid);
        return EL_ERR_DOUBLON;
    }
    rc = candidat_verifier_type(c->type, err, err_len);
    if (rc != EL_OK) {
        return rc;
    }
    if (core_ajouter_enreg(FICHIER_CANDIDATS, c, sizeof(candidats)) != EL_OK) {
        core_erreur(err, err_len, "Erreur d'ouverture du fichier %s", FICHIER_CANDIDATS);
        return EL_ERR_FICHIER;
    }
    return EL_OK;
}

int core_candidat_modifier(const candidats *c, char *err, size_t err_len) {
    int rc, r;

    if (c == NULL || c->ID_candid[0] == '\0') {
        core_erreur(err, err_len, "L'identifiant du candidat est vide.");
        return EL_ERR_INVALIDE;
    }
    rc = candidat_verifier_type(c->type, err, err_len);
    if (rc != EL_OK) {
        return rc;
    }
    r = core_remplacer_enreg(FICHIER_CANDIDATS, sizeof(candidats),
                             candidat_correspond, c->ID_candid, c);
    if (r < 0) {
        core_erreur(err, err_len, "Erreur d'ouverture du fichier %s", FICHIER_CANDIDATS);
        return r;
    }
    if (r == 0) {
        core_erreur(err, err_len, "Aucun candidat trouve avec l'ID : %s", c->ID_candid);
        return EL_ERR_INTROUVABLE;
    }
    return EL_OK;
}

int core_candidat_obtenir(const char *id, candidats *out) {
    FILE *fichier = core_fichier_ouvrir(FICHIER_CANDIDATS, "rb");
    candidats temp;
    int trouve = 0;

    if (fichier == NULL) {
        return 0;
    }
    while (fread(&temp, sizeof(candidats), 1, fichier) == 1) {
        if (candidat_correspond(&temp, id)) {
            *out = temp;
            trouve = 1;
            break;
        }
    }
    fclose(fichier);
    return trouve;
}

int core_candidat_liste(candidats **out, int *count) {
    *out = core_lire_tout(FICHIER_CANDIDATS, sizeof(candidats), count);
    return EL_OK;
}
