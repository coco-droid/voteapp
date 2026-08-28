#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI/widgets.h>

#include "page.tsx.h"
#include "page.h"
#include "backend.h"

typedef struct {
        root_page_react_t base;
        /* Etats supplementaires du composant ici */
} root_page_t;

/* Recharge toutes les donnees du tableau de bord depuis la couche core. */
static void dashboard_refresh(ui_widget_t *w)
{
        root_page_react_t *_that = ui_widget_get_data(w, root_page_proto);
        electeurs *elect = NULL;
        partie_politic *partis = NULL;
        bureau_vote *bvs = NULL;
        votes *vts = NULL;
        core_resultats res;
        int n_elect = 0, n_partis = 0, n_bv = 0, n_votes = 0;
        char tampon[128];
        char date[14];
        int i, debut;

        /* --- Compteurs --- */
        if (core_electeur_liste(&elect, &n_elect) != 0 || elect == NULL) {
                n_elect = 0;
        }
        if (core_parti_liste(&partis, &n_partis) != 0 || partis == NULL) {
                n_partis = 0;
        }
        if (core_bv_liste(&bvs, &n_bv) != 0 || bvs == NULL) {
                n_bv = 0;
        }
        if (core_vote_liste(&vts, &n_votes) != 0 || vts == NULL) {
                n_votes = 0;
        }

        snprintf(tampon, sizeof(tampon), "%d", n_elect);
        ui_widget_set_text(_that->refs.nb_electeurs, tampon);
        snprintf(tampon, sizeof(tampon), "%d", n_partis);
        ui_widget_set_text(_that->refs.nb_partis, tampon);
        snprintf(tampon, sizeof(tampon), "%d", n_bv);
        ui_widget_set_text(_that->refs.nb_bv, tampon);
        snprintf(tampon, sizeof(tampon), "%d", n_votes);
        ui_widget_set_text(_that->refs.nb_votes, tampon);

        if (n_elect > 0) {
                snprintf(tampon, sizeof(tampon),
                         "Participation : %.1f %%",
                         100.0 * n_votes / n_elect);
        } else {
                snprintf(tampon, sizeof(tampon), "Participation : —");
        }
        ui_widget_set_text(_that->refs.participation, tampon);

        core_obtenir_date(date);
        snprintf(tampon, sizeof(tampon), "Données au %s", date);
        ui_widget_set_text(_that->refs.date_maj, tampon);

        /* --- Resultats nationaux --- */
        ui_widget_empty(_that->refs.resultats);
        if (core_resultats_calculer(&res) == 0 && res.nb_candidats > 0) {
                for (i = 0; i < res.nb_candidats && i < 8; i++) {
                        ui_widget_t *ligne = backend_row();
                        char nom_complet[48];
                        char voix[16];
                        char pct[16];

                        snprintf(nom_complet, sizeof(nom_complet), "%s %s",
                                 res.national[i].prenom, res.national[i].nom);
                        snprintf(voix, sizeof(voix), "%d",
                                 res.national[i].total_votes);
                        snprintf(pct, sizeof(pct), "%.1f %%",
                                 res.national[i].pourcentage);
                        ui_widget_append(ligne,
                                backend_cell("w-1/3", nom_complet,
                                             "text-body-md text-on-surface"));
                        ui_widget_append(ligne,
                                backend_cell("w-1/4", res.national[i].id_candid,
                                             "text-body-sm text-secondary"));
                        ui_widget_append(ligne,
                                backend_cell("w-1/6 text-right", voix,
                                             "text-body-md text-on-surface"));
                        ui_widget_append(ligne,
                                backend_cell("w-1/4 text-right", pct,
                                             "text-body-md font-semibold text-primary"));
                        ui_widget_append(_that->refs.resultats, ligne);
                }
                if (res.total_blancs > 0) {
                        ui_widget_t *ligne = backend_row();
                        char voix[16];
                        snprintf(voix, sizeof(voix), "%d", res.total_blancs);
                        ui_widget_append(ligne,
                                backend_cell("w-1/3", "Votes blancs",
                                             "text-body-md text-secondary"));
                        ui_widget_append(ligne,
                                backend_cell("w-1/4", "—",
                                             "text-body-sm text-secondary"));
                        ui_widget_append(ligne,
                                backend_cell("w-1/6 text-right", voix,
                                             "text-body-md text-secondary"));
                        ui_widget_append(ligne,
                                backend_cell("w-1/4 text-right", "—",
                                             "text-body-md text-secondary"));
                        ui_widget_append(_that->refs.resultats, ligne);
                }
        } else {
                ui_widget_append(_that->refs.resultats,
                        backend_text("text-body-sm text-secondary py-2",
                                     "Aucun vote enregistré pour le moment."));
        }
        core_resultats_liberer(&res);

        /* --- Activite recente : 5 derniers votes --- */
        ui_widget_empty(_that->refs.activite);
        if (n_votes == 0) {
                ui_widget_append(_that->refs.activite,
                        backend_text("text-body-sm text-secondary py-2",
                                     "Aucune activité récente."));
        } else {
                debut = n_votes > 5 ? n_votes - 5 : 0;
                for (i = n_votes - 1; i >= debut; i--) {
                        ui_widget_t *ligne = backend_row();
                        char desc[96];
                        const char *cible =
                                strcmp(vts[i].id_candid, "0") == 0
                                        ? "Vote blanc"
                                        : vts[i].id_candid;
                        snprintf(desc, sizeof(desc),
                                 "Vote enregistré — BV %s — %s",
                                 vts[i].BV, cible);
                        ui_widget_append(ligne,
                                backend_cell("flex-1", desc,
                                             "text-body-sm text-on-surface"));
                        ui_widget_append(ligne,
                                backend_cell("w-32 text-right", vts[i].Date_vote,
                                             "text-label-sm text-secondary"));
                        ui_widget_append(_that->refs.activite, ligne);
                }
        }

        free(elect);
        free(partis);
        free(bvs);
        free(vts);
}

/* Handler declare dans page.tsx (onClick="dashboard_refresh_click") */
static void dashboard_refresh_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        (void)w;
        (void)arg;
        dashboard_refresh(e->data);
}

static void root_page_init(ui_widget_t *w)
{
        ui_widget_add_data(w, root_page_proto, sizeof(root_page_t));
        root_page_react_init(w);
        dashboard_refresh(w);
}

static void root_page_destroy(ui_widget_t *w)
{
        root_page_react_destroy(w);
}

void root_page_update(ui_widget_t *w)
{
        root_page_react_update(w);
}

ui_widget_t *ui_create_root_page(void)
{
        return ui_create_widget_with_prototype(root_page_proto);
}

void ui_register_root_page(void)
{
        root_page_init_prototype();
        root_page_proto->init = root_page_init;
        root_page_proto->destroy = root_page_destroy;
}
