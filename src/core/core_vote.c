/*
 * core_vote.c — Logique metier des votes (sans I/O console).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core_interne.h"

int core_vote_ajouter(votes *v, char *err, size_t err_len) {
    if (v == NULL || v->Id_vote[0] == '\0') {
        core_erreur(err, err_len, "L'identifiant du vote est vide.");
        return EL_ERR_INVALIDE;
    }
    if (core_existe_vote(v->Id_vote)) {
        core_erreur(err, err_len,
                    "Un vote avec l'id '%s' est deja enregistre !", v->Id_vote);
        return EL_ERR_DOUBLON;
    }
    /* L'electeur doit exister ; on recupere au passage son bureau de vote */
    if (!core_existe_NINU(v->NINU, v->BV)) {
        core_erreur(err, err_len,
                    "L'electeur avec le NINU '%s' n'existe pas !", v->NINU);
        return EL_ERR_INTROUVABLE;
    }
    if (core_ninu_a_deja_vote(v->NINU)) {
        core_erreur(err, err_len,
                    "L'electeur avec le NINU '%s' a DEJA VOTE !", v->NINU);
        return EL_ERR_DEJA_VOTE;
    }
    /* id_candid "0" = vote blanc, sinon le candidat doit exister */
    if (strcmp(v->id_candid, "0") != 0 && !core_existe_candidat(v->id_candid)) {
        core_erreur(err, err_len,
                    "Le candidat avec l'id '%s' n'existe pas !", v->id_candid);
        return EL_ERR_INTROUVABLE;
    }

    /* Date du jour generee automatiquement */
    core_obtenir_date(v->Date_vote);

    if (core_ajouter_enreg(FICHIER_VOTES, v, sizeof(votes)) != EL_OK) {
        core_erreur(err, err_len, "Erreur d'ouverture du fichier %s", FICHIER_VOTES);
        return EL_ERR_FICHIER;
    }
    return EL_OK;
}

int core_vote_liste(votes **out, int *count) {
    *out = core_lire_tout(FICHIER_VOTES, sizeof(votes), count);
    return EL_OK;
}
