#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI/widgets.h>

#include "page.tsx.h"
#include "page.h"
#include "../backend.h"

typedef struct {
        resultats_page_react_t base;
} resultats_page_t;

/* Cree une ligne candidat avec barre de progression :
 * [Nom (w-1/4)] [====barre==== (flex-1)] [voix (w-20)] [% (w-20)] */
static ui_widget_t *res_ligne_stat(const char *nom, int voix, float pct)
{
        ui_widget_t *ligne = ui_create_widget(NULL);
        ui_widget_t *piste = ui_create_widget(NULL);
        ui_widget_t *barre = ui_create_widget(NULL);
        char tampon[32];
        char largeur[16];

        ui_widget_add_class(ligne, "flex flex-row items-center w-full mb-2");

        ui_widget_append(ligne,
                backend_cell("w-1/4", nom, "text-body-md text-on-surface"));

        /* Piste de la barre (fond clair) + barre coloree proportionnelle */
        ui_widget_add_class(piste, "flex-1 bg-surface-container-high "
                                   "rounded-full mr-2");
        ui_widget_set_style_string(piste, "height", "12px");
        ui_widget_add_class(barre, "bg-primary rounded-full");
        ui_widget_set_style_string(barre, "height", "12px");
        snprintf(largeur, sizeof(largeur), "%.1f%%",
                 pct > 100.0f ? 100.0f : pct);
        ui_widget_set_style_string(barre, "width", largeur);
        ui_widget_append(piste, barre);
        ui_widget_append(ligne, piste);

        snprintf(tampon, sizeof(tampon), "%d", voix);
        ui_widget_append(ligne,
                backend_cell("w-20 text-right", tampon,
                             "text-body-md text-on-surface"));
        snprintf(tampon, sizeof(tampon), "%.1f %%", pct);
        ui_widget_append(ligne,
                backend_cell("w-20 text-right", tampon,
                             "text-body-md font-semibold text-primary"));
        return ligne;
}

/* Recharge toutes les donnees de la page depuis la couche core. */
static void res_refresh(ui_widget_t *w)
{
        resultats_page_react_t *_that =
                ui_widget_get_data(w, resultats_page_proto);
        core_resultats res;
        electeurs *elect = NULL;
        int n_elect = 0;
        char tampon[64];
        int i, j;

        if (core_resultats_calculer(&res) != EL_OK) {
                backend_message(NULL, "", 0);
                return;
        }
        if (core_electeur_liste(&elect, &n_elect) != 0 || elect == NULL) {
                n_elect = 0;
        }

        /* --- Synthese --- */
        snprintf(tampon, sizeof(tampon), "%d", res.total_general);
        ui_widget_set_text(_that->refs.total_votes, tampon);
        snprintf(tampon, sizeof(tampon), "%d", res.total_non_blancs);
        ui_widget_set_text(_that->refs.total_exprimes, tampon);
        snprintf(tampon, sizeof(tampon), "%d", res.total_blancs);
        ui_widget_set_text(_that->refs.total_blancs, tampon);
        if (n_elect > 0) {
                snprintf(tampon, sizeof(tampon), "%.1f %%",
                         100.0 * res.total_general / n_elect);
        } else {
                snprintf(tampon, sizeof(tampon), "—");
        }
        ui_widget_set_text(_that->refs.participation, tampon);

        /* --- Classement national --- */
        ui_widget_empty(_that->refs.national);
        if (res.nb_candidats == 0 || res.total_general == 0) {
                ui_widget_append(_that->refs.national,
                        backend_text("text-body-sm text-secondary py-2",
                                     "Aucun vote enregistré pour le moment."));
        } else {
                for (i = 0; i < res.nb_candidats; i++) {
                        char nom_complet[48];
                        snprintf(nom_complet, sizeof(nom_complet), "%s %s",
                                 res.national[i].prenom, res.national[i].nom);
                        ui_widget_append(_that->refs.national,
                                res_ligne_stat(nom_complet,
                                               res.national[i].total_votes,
                                               res.national[i].pourcentage));
                }
                if (res.total_blancs > 0) {
                        float pct_blancs =
                                100.0f * res.total_blancs / res.total_general;
                        ui_widget_append(_that->refs.national,
                                res_ligne_stat("Votes blancs",
                                               res.total_blancs, pct_blancs));
                }
        }

        /* --- Detail par bureau de vote --- */
        ui_widget_empty(_that->refs.bureaux);
        if (res.nb_bureaux == 0) {
                ui_widget_append(_that->refs.bureaux,
                        backend_text("text-body-sm text-secondary py-2",
                                     "Aucun bureau de vote enregistré."));
        }
        for (i = 0; i < res.nb_bureaux; i++) {
                core_resultat_bv *rb = &res.bureaux[i];
                ui_widget_t *carte = ui_create_widget(NULL);
                int total_bv = rb->votes_blancs + rb->votes_non_blancs;

                ui_widget_add_class(carte, "border border-outline-variant "
                                           "rounded-xl p-4 mb-3 flex "
                                           "flex-col");
                snprintf(tampon, sizeof(tampon), "%s — %s", rb->id_bv,
                         rb->departement[0] ? rb->departement
                                            : "Département inconnu");
                ui_widget_append(carte,
                        backend_text("text-headline-sm text-on-surface",
                                     tampon));
                snprintf(tampon, sizeof(tampon),
                         "%d bulletins (%d exprimés, %d blancs)", total_bv,
                         rb->votes_non_blancs, rb->votes_blancs);
                ui_widget_append(carte,
                        backend_text("text-label-sm text-secondary mb-3 mt-1",
                                     tampon));
                if (total_bv == 0) {
                        ui_widget_append(carte,
                                backend_text("text-body-sm text-secondary",
                                             "Aucun vote dans ce bureau."));
                }
                for (j = 0; j < rb->nb_stats && j < 5; j++) {
                        char nom_complet[48];
                        snprintf(nom_complet, sizeof(nom_complet), "%s %s",
                                 rb->stats[j].prenom, rb->stats[j].nom);
                        ui_widget_append(carte,
                                res_ligne_stat(nom_complet,
                                               rb->stats[j].total_votes,
                                               rb->stats[j].pourcentage));
                }
                ui_widget_append(_that->refs.bureaux, carte);
        }

        core_resultats_liberer(&res);
        free(elect);
}

/* Handler declare dans page.tsx (onClick="res_refresh_click") */
static void res_refresh_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        (void)w;
        (void)arg;
        res_refresh(e->data);
}

/* --- Cycle de vie du composant --- */

static void resultats_page_init(ui_widget_t *w)
{
        ui_widget_add_data(w, resultats_page_proto, sizeof(resultats_page_t));
        resultats_page_react_init(w);
        res_refresh(w);
}

static void resultats_page_destroy(ui_widget_t *w)
{
        resultats_page_react_destroy(w);
}

void resultats_page_update(ui_widget_t *w)
{
        resultats_page_react_update(w);
}

ui_widget_t *ui_create_resultats_page(void)
{
        return ui_create_widget_with_prototype(resultats_page_proto);
}

void ui_register_resultats_page(void)
{
        resultats_page_init_prototype();
        resultats_page_proto->init = resultats_page_init;
        resultats_page_proto->destroy = resultats_page_destroy;
}
