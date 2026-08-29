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

/* Controle de coherence commun a l'ajout et a la modification */
static int parti_valider(const partie_politic *p, char *err, size_t err_len) {
    if (p->id_PP[0] == '\0') {
        core_erreur(err, err_len, "L'identifiant du parti politique est vide.");
        return EL_ERR_INVALIDE;
    }
    if (p->Nom[0] == '\0') {
        core_erreur(err, err_len, "Le nom du parti politique est obligatoire.");
        return EL_ERR_INVALIDE;
    }
    if (p->date_creat[0] != '\0' && !core_date_valide(p->date_creat)) {
        core_erreur(err, err_len,
                    "La date de creation doit etre une date reelle au format JJ/MM/AAAA.");
        return EL_ERR_INVALIDE;
    }
    if (!core_email_valide(p->email)) {
        core_erreur(err, err_len,
                    "L'adresse email est invalide (format attendu : nom@domaine.ht).");
        return EL_ERR_INVALIDE;
    }
    if (!core_tel_valide(p->Telephone)) {
        core_erreur(err, err_len,
                    "Le numero de telephone est invalide (7 a 14 chiffres).");
        return EL_ERR_INVALIDE;
    }
    return EL_OK;
}

int core_parti_ajouter(const partie_politic *p, char *err, size_t err_len) {
    int rc;

    if (p == NULL) {
        core_erreur(err, err_len, "Donnees du parti politique manquantes.");
        return EL_ERR_INVALIDE;
    }
    rc = parti_valider(p, err, err_len);
    if (rc != EL_OK) {
        return rc;
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
    int r, rc;

    if (p == NULL) {
        core_erreur(err, err_len, "Donnees du parti politique manquantes.");
        return EL_ERR_INVALIDE;
    }
    rc = parti_valider(p, err, err_len);
    if (rc != EL_OK) {
        return rc;
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

int core_parti_supprimer(const char *id, char *err, size_t err_len) {
    candidats *liste = NULL;
    int count = 0, i, r;

    if (id == NULL || id[0] == '\0') {
        core_erreur(err, err_len, "L'identifiant du parti politique est vide.");
        return EL_ERR_INVALIDE;
    }
    /* Interdit si des candidats sont rattaches a ce parti */
    if (core_candidat_liste(&liste, &count) == 0 && liste != NULL) {
        for (i = 0; i < count; i++) {
            if (strcasecmp(liste[i].type, id) == 0) {
                core_erreur(err, err_len,
                            "Impossible de supprimer : le candidat '%s %s' est rattache a ce parti.",
                            liste[i].Prenom, liste[i].Nom);
                free(liste);
                return EL_ERR_CONFLIT;
            }
        }
        free(liste);
    }
    r = core_supprimer_enreg(FICHIER_PARTIS, sizeof(partie_politic),
                             parti_correspond, id);
    if (r < 0) {
        core_erreur(err, err_len, "Erreur d'ouverture du fichier %s", FICHIER_PARTIS);
        return r;
    }
    if (r == 0) {
        core_erreur(err, err_len, "Aucun parti politique trouve avec l'ID : %s", id);
        return EL_ERR_INTROUVABLE;
    }
    return EL_OK;
}

int core_parti_prochain_id(char *out, size_t len) {
    return core_prochain_id(FICHIER_PARTIS, sizeof(partie_politic),
                            offsetof(partie_politic, id_PP),
                            sizeof(((partie_politic *)0)->id_PP),
                            "PP-", 2, out, len);
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
