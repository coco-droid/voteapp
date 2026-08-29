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

/* Verifie qu'un ID personne n'est membre d'aucun bureau, en ignorant
 * eventuellement un bureau (id_bv_exclu, utile en modification : un bureau
 * ne doit pas entrer en conflit avec lui-meme). Retourne 1 si occupe. */
static int bv_membre_occupe(const char *id_personne, const char *id_bv_exclu) {
    FILE *fichier = core_fichier_ouvrir(FICHIER_BUREAUX, "rb");
    bureau_vote temp;
    int occupe = 0;

    if (fichier == NULL) {
        return 0;
    }
    while (fread(&temp, sizeof(bureau_vote), 1, fichier) == 1) {
        if (id_bv_exclu != NULL &&
            strcasecmp(temp.Id_BV, id_bv_exclu) == 0) {
            continue;
        }
        if (strcasecmp(temp.Id_President, id_personne) == 0 ||
            strcasecmp(temp.Id_VPresident, id_personne) == 0 ||
            strcasecmp(temp.Id_Secret, id_personne) == 0) {
            occupe = 1;
            break;
        }
    }
    fclose(fichier);
    return occupe;
}

/* Controle de coherence commun a l'ajout et a la modification.
 * id_bv_exclu : bureau a ignorer pour le controle des conflits (NULL = aucun) */
static int bv_verifier_membres(const bureau_vote *bv, const char *id_bv_exclu,
                               char *err, size_t err_len) {
    if (bv->Adresse_BV[0] == '\0') {
        core_erreur(err, err_len, "L'adresse du bureau de vote est obligatoire.");
        return EL_ERR_INVALIDE;
    }
    if (strchr(bv->Adresse_BV, ',') == NULL) {
        core_erreur(err, err_len,
                    "L'adresse doit etre au format 'No Rue, Rue, Departement'.");
        return EL_ERR_INVALIDE;
    }
    if (bv->Id_President[0] == '\0' || bv->Id_VPresident[0] == '\0' ||
        bv->Id_Secret[0] == '\0') {
        core_erreur(err, err_len,
                    "Les trois membres du bureau (president, vice-president, secretaire) sont obligatoires.");
        return EL_ERR_INVALIDE;
    }
    if (bv_membre_occupe(bv->Id_President, id_bv_exclu)) {
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
    if (bv_membre_occupe(bv->Id_VPresident, id_bv_exclu)) {
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
    if (bv_membre_occupe(bv->Id_Secret, id_bv_exclu)) {
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
    rc = bv_verifier_membres(bv, NULL, err, err_len);
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
    int r, rc;

    if (bv == NULL || bv->Id_BV[0] == '\0') {
        core_erreur(err, err_len, "L'identifiant du bureau de vote est vide.");
        return EL_ERR_INVALIDE;
    }
    /* En modification, le bureau courant est exclu du controle de conflit */
    rc = bv_verifier_membres(bv, bv->Id_BV, err, err_len);
    if (rc != EL_OK) {
        return rc;
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

int core_bv_supprimer(const char *id, char *err, size_t err_len) {
    electeurs *elect = NULL;
    votes *vts = NULL;
    int n = 0, i, r;

    if (id == NULL || id[0] == '\0') {
        core_erreur(err, err_len, "L'identifiant du bureau de vote est vide.");
        return EL_ERR_INVALIDE;
    }
    /* Interdit si des electeurs sont affectes a ce bureau */
    if (core_electeur_liste(&elect, &n) == 0 && elect != NULL) {
        for (i = 0; i < n; i++) {
            if (strcasecmp(elect[i].BV, id) == 0) {
                core_erreur(err, err_len,
                            "Impossible de supprimer : des electeurs sont affectes a ce bureau.");
                free(elect);
                return EL_ERR_CONFLIT;
            }
        }
        free(elect);
    }
    /* Interdit si des votes y ont ete enregistres */
    if (core_vote_liste(&vts, &n) == 0 && vts != NULL) {
        for (i = 0; i < n; i++) {
            if (strcasecmp(vts[i].BV, id) == 0) {
                core_erreur(err, err_len,
                            "Impossible de supprimer : des votes ont ete enregistres dans ce bureau.");
                free(vts);
                return EL_ERR_CONFLIT;
            }
        }
        free(vts);
    }
    r = core_supprimer_enreg(FICHIER_BUREAUX, sizeof(bureau_vote),
                             bv_correspond, id);
    if (r < 0) {
        core_erreur(err, err_len, "Erreur d'ouverture du fichier %s", FICHIER_BUREAUX);
        return r;
    }
    if (r == 0) {
        core_erreur(err, err_len, "Aucun bureau de vote trouve avec l'ID : %s", id);
        return EL_ERR_INTROUVABLE;
    }
    return EL_OK;
}

int core_bv_prochain_id(char *out, size_t len) {
    return core_prochain_id(FICHIER_BUREAUX, sizeof(bureau_vote),
                            offsetof(bureau_vote, Id_BV),
                            sizeof(((bureau_vote *)0)->Id_BV),
                            "BV-", 2, out, len);
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
