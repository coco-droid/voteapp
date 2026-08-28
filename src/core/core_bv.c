/*
 * core_bv.c — Logique metier des bureaux de vote (sans I/O console).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core_interne.h"

static int bv_correspond(const void *enreg, const void *cle) {
    return strcasecmp(((const bureau_vote *)enreg)->Id_BV,
                      (const char *)cle) == 0;
}

/* Verifie les conflits de personnes (president / vice-president / secretaire) */
static int bv_verifier_membres(const bureau_vote *bv, char *err, size_t err_len) {
    if (core_existe_membre_bv(bv->Id_President)) {
        core_erreur(err, err_len,
                    "La personne '%s' est deja membre d'un autre bureau de vote !",
                    bv->Id_President);
        return EL_ERR_CONFLIT;
    }
    if (strcasecmp(bv->Id_VPresident, bv->Id_President) == 0) {
        core_erreur(err, err_len,
                    "Le Vice-President ne peut pas avoir le meme ID que le President !");
        return EL_ERR_CONFLIT;
    }
    if (core_existe_membre_bv(bv->Id_VPresident)) {
        core_erreur(err, err_len,
                    "La personne '%s' est deja membre d'un autre bureau de vote !",
                    bv->Id_VPresident);
        return EL_ERR_CONFLIT;
    }
    if (strcasecmp(bv->Id_Secret, bv->Id_President) == 0 ||
        strcasecmp(bv->Id_Secret, bv->Id_VPresident) == 0) {
        core_erreur(err, err_len,
                    "Le Secretaire ne peut pas etre la meme personne que le President ou le Vice-President !");
        return EL_ERR_CONFLIT;
    }
    if (core_existe_membre_bv(bv->Id_Secret)) {
        core_erreur(err, err_len,
                    "La personne '%s' est deja membre d'un autre bureau de vote !",
                    bv->Id_Secret);
        return EL_ERR_CONFLIT;
    }
    return EL_OK;
}

int core_bv_ajouter(const bureau_vote *bv, char *err, size_t err_len) {
    int rc;

    if (bv == NULL || bv->Id_BV[0] == '\0') {
        core_erreur(err, err_len, "L'identifiant du bureau de vote est vide.");
        return EL_ERR_INVALIDE;
    }
    if (core_existe_BV(bv->Id_BV)) {
        core_erreur(err, err_len,
                    "Un bureau de vote avec l'id '%s' existe deja !", bv->Id_BV);
        return EL_ERR_DOUBLON;
    }
    rc = bv_verifier_membres(bv, err, err_len);
    if (rc != EL_OK) {
        return rc;
    }
    if (core_ajouter_enreg(FICHIER_BUREAUX, bv, sizeof(bureau_vote)) != EL_OK) {
        core_erreur(err, err_len, "Erreur d'ouverture du fichier %s", FICHIER_BUREAUX);
        return EL_ERR_FICHIER;
    }
    return EL_OK;
}

int core_bv_modifier(const bureau_vote *bv, char *err, size_t err_len) {
    int r;

    if (bv == NULL || bv->Id_BV[0] == '\0') {
        core_erreur(err, err_len, "L'identifiant du bureau de vote est vide.");
        return EL_ERR_INVALIDE;
    }
    r = core_remplacer_enreg(FICHIER_BUREAUX, sizeof(bureau_vote),
                             bv_correspond, bv->Id_BV, bv);
    if (r < 0) {
        core_erreur(err, err_len, "Erreur d'ouverture du fichier %s", FICHIER_BUREAUX);
        return r;
    }
    if (r == 0) {
        core_erreur(err, err_len, "Aucun bureau de vote trouve avec l'ID : %s", bv->Id_BV);
        return EL_ERR_INTROUVABLE;
    }
    return EL_OK;
}

int core_bv_obtenir(const char *id, bureau_vote *out) {
    FILE *fichier = core_fichier_ouvrir(FICHIER_BUREAUX, "rb");
    bureau_vote temp;
    int trouve = 0;

    if (fichier == NULL) {
        return 0;
    }
    while (fread(&temp, sizeof(bureau_vote), 1, fichier) == 1) {
        if (bv_correspond(&temp, id)) {
            *out = temp;
            trouve = 1;
            break;
        }
    }
    fclose(fichier);
    return trouve;
}

int core_bv_liste(bureau_vote **out, int *count) {
    *out = core_lire_tout(FICHIER_BUREAUX, sizeof(bureau_vote), count);
    return EL_OK;
}
