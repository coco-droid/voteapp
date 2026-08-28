/*
 * core_rapp.c — Calcul des resultats du scrutin (sans I/O console).
 *
 * Reprend la logique de l'ancienne fonction resultat() de rapp.c :
 * stats par bureau de vote (tries par pourcentage decroissant) puis
 * resume national. Les tableaux sont desormais dynamiques : plus de
 * limite fixe de 100 candidats / 1000 votes.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "core_interne.h"

static void stat_init(core_stat_candidat *s, const candidats *c) {
    snprintf(s->id_candid, sizeof(s->id_candid), "%s", c->ID_candid);
    snprintf(s->nom, sizeof(s->nom), "%s", c->Nom);
    snprintf(s->prenom, sizeof(s->prenom), "%s", c->Prenom);
    s->total_votes = 0;
    s->pourcentage = 0.0f;
}

/* Tri bulle par pourcentage decroissant (meme algorithme que l'original) */
static void stat_trier(core_stat_candidat *stats, int n) {
    int i, j;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (stats[j].pourcentage < stats[j + 1].pourcentage) {
                core_stat_candidat temp = stats[j];
                stats[j] = stats[j + 1];
                stats[j + 1] = temp;
            }
        }
    }
}

int core_resultats_calculer(core_resultats *res) {
    candidats *candis = NULL;
    votes *vts = NULL;
    bureau_vote *bvs = NULL;
    int nc = 0, nv = 0, nb = 0;
    int k, i, j;

    memset(res, 0, sizeof(*res));

    core_candidat_liste(&candis, &nc);
    core_vote_liste(&vts, &nv);
    core_bv_liste(&bvs, &nb);

    res->nb_candidats = nc;
    res->national = calloc((size_t)(nc > 0 ? nc : 1), sizeof(core_stat_candidat));
    res->bureaux = calloc((size_t)(nb > 0 ? nb : 1), sizeof(core_resultat_bv));
    if (res->national == NULL || res->bureaux == NULL) {
        free(candis);
        free(vts);
        free(bvs);
        core_resultats_liberer(res);
        return EL_ERR_MEMOIRE;
    }

    for (i = 0; i < nc; i++) {
        stat_init(&res->national[i], &candis[i]);
    }
    res->nb_bureaux = nb;

    /* --- Stats par bureau de vote --- */
    for (k = 0; k < nb; k++) {
        core_resultat_bv *rb = &res->bureaux[k];
        int total_bv;

        core_extraire_departement(bvs[k].Adresse_BV, rb->departement,
                                  sizeof(rb->departement));
        snprintf(rb->id_bv, sizeof(rb->id_bv), "%s", bvs[k].Id_BV);

        rb->stats = calloc((size_t)(nc > 0 ? nc : 1), sizeof(core_stat_candidat));
        if (rb->stats == NULL) {
            res->nb_bureaux = k; /* ne liberer que ce qui est initialise */
            free(candis);
            free(vts);
            free(bvs);
            core_resultats_liberer(res);
            return EL_ERR_MEMOIRE;
        }
        rb->nb_stats = nc;
        for (i = 0; i < nc; i++) {
            stat_init(&rb->stats[i], &candis[i]);
        }

        for (i = 0; i < nv; i++) {
            if (strcasecmp(vts[i].BV, bvs[k].Id_BV) == 0) {
                if (strcmp(vts[i].id_candid, "0") == 0) {
                    rb->votes_blancs++;
                } else {
                    rb->votes_non_blancs++;
                    for (j = 0; j < nc; j++) {
                        if (strcasecmp(vts[i].id_candid, rb->stats[j].id_candid) == 0) {
                            rb->stats[j].total_votes++;
                            res->national[j].total_votes++;
                            break;
                        }
                    }
                }
            }
        }

        total_bv = rb->votes_blancs + rb->votes_non_blancs;
        for (i = 0; i < nc; i++) {
            if (total_bv > 0) {
                rb->stats[i].pourcentage =
                    ((float)rb->stats[i].total_votes / (float)total_bv) * 100.0f;
            }
        }
        stat_trier(rb->stats, nc);

        res->total_blancs += rb->votes_blancs;
        res->total_non_blancs += rb->votes_non_blancs;
    }

    /* --- Resume national --- */
    res->total_general = res->total_blancs + res->total_non_blancs;
    for (i = 0; i < nc; i++) {
        if (res->total_general > 0) {
            res->national[i].pourcentage =
                ((float)res->national[i].total_votes / (float)res->total_general) * 100.0f;
        }
    }
    stat_trier(res->national, nc);

    free(candis);
    free(vts);
    free(bvs);
    return EL_OK;
}

void core_resultats_liberer(core_resultats *res) {
    int k;

    if (res == NULL) {
        return;
    }
    if (res->bureaux != NULL) {
        for (k = 0; k < res->nb_bureaux; k++) {
            free(res->bureaux[k].stats);
        }
        free(res->bureaux);
    }
    free(res->national);
    memset(res, 0, sizeof(*res));
}
