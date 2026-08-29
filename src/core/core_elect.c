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

/* Controle de coherence commun a l'ajout et a la modification */
static int electeur_valider(const electeurs *e, char *err, size_t err_len) {
    if (e->NINU[0] == '\0') {
        core_erreur(err, err_len, "Le NINU de l'electeur est obligatoire.");
        return EL_ERR_INVALIDE;
    }
    if (!core_est_nombre(e->NINU)) {
        core_erreur(err, err_len,
                    "Le NINU ne doit contenir que des chiffres.");
        return EL_ERR_INVALIDE;
    }
    if (e->nom[0] == '\0') {
        core_erreur(err, err_len, "Le nom de l'electeur est obligatoire.");
        return EL_ERR_INVALIDE;
    }
    if (e->prenom[0] == '\0') {
        core_erreur(err, err_len, "Le prenom de l'electeur est obligatoire.");
        return EL_ERR_INVALIDE;
    }
    if (e->BV[0] == '\0') {
        core_erreur(err, err_len,
                    "Le bureau de vote de l'electeur est obligatoire.");
        return EL_ERR_INVALIDE;
    }
    if (!core_existe_BV(e->BV)) {
        core_erreur(err, err_len,
                    "Le bureau de vote '%s' n'existe pas !", e->BV);
        return EL_ERR_INTROUVABLE;
    }
    if (e->Adresse[0] != '\0' && strchr(e->Adresse, ',') == NULL) {
        core_erreur(err, err_len,
                    "L'adresse doit etre au format 'No Rue, Rue, Departement'.");
        return EL_ERR_INVALIDE;
    }
    return EL_OK;
}

int core_electeur_ajouter(const electeurs *e, char *err, size_t err_len) {
    int rc;

    if (e == NULL) {
        core_erreur(err, err_len, "Donnees de l'electeur manquantes.");
        return EL_ERR_INVALIDE;
    }
    rc = electeur_valider(e, err, err_len);
    if (rc != EL_OK) {
        return rc;
    }
    if (core_existe_NINU(e->NINU, NULL)) {
        core_erreur(err, err_len,
                    "Un electeur avec le NINU '%s' existe deja !", e->NINU);
        return EL_ERR_DOUBLON;
    }
    if (core_ajouter_enreg(FICHIER_ELECTEURS, e, sizeof(electeurs)) != EL_OK) {
        core_erreur(err, err_len, "Erreur d'ouverture du fichier %s", FICHIER_ELECTEURS);
        return EL_ERR_FICHIER;
    }
    return EL_OK;
}

int core_electeur_modifier(const electeurs *e, char *err, size_t err_len) {
    int r, rc;

    if (e == NULL) {
        core_erreur(err, err_len, "Donnees de l'electeur manquantes.");
        return EL_ERR_INVALIDE;
    }
    rc = electeur_valider(e, err, err_len);
    if (rc != EL_OK) {
        return rc;
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

int core_electeur_supprimer(const char *ninu, char *err, size_t err_len) {
    int r;

    if (ninu == NULL || ninu[0] == '\0') {
        core_erreur(err, err_len, "Le NINU de l'electeur est vide.");
        return EL_ERR_INVALIDE;
    }
    /* Un electeur qui a vote ne peut plus etre supprime : le vote
     * enregistre reference son NINU et les votes sont intangibles. */
    if (core_ninu_a_deja_vote(ninu)) {
        core_erreur(err, err_len,
                    "Impossible de supprimer : l'electeur '%s' a deja vote !",
                    ninu);
        return EL_ERR_CONFLIT;
    }
    r = core_supprimer_enreg(FICHIER_ELECTEURS, sizeof(electeurs),
                             electeur_correspond, ninu);
    if (r < 0) {
        core_erreur(err, err_len, "Erreur d'ouverture du fichier %s", FICHIER_ELECTEURS);
        return r;
    }
    if (r == 0) {
        core_erreur(err, err_len, "Aucun electeur trouve avec l'ID : %s", ninu);
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
