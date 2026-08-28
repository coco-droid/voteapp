#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <LCUI/widgets.h>

#include "page.tsx.h"
#include "page.h"
#include "../backend.h"

typedef struct {
        bureaux_page_react_t base;
} bureaux_page_t;

/* Compte le nombre d'electeurs affectes a un bureau de vote. */
static int bv_compter_electeurs(const char *id_bv)
{
        electeurs *liste = NULL;
        int count = 0, i, total = 0;

        if (core_electeur_liste(&liste, &count) == 0 && liste != NULL) {
                for (i = 0; i < count; i++) {
                        if (strcasecmp(liste[i].BV, id_bv) == 0) {
                                total++;
                        }
                }
                free(liste);
        }
        return total;
}

/* Recharge la table des bureaux, filtre optionnel (ID ou adresse). */
static void bv_refresh(ui_widget_t *w)
{
        bureaux_page_react_t *_that =
                ui_widget_get_data(w, bureaux_page_proto);
        bureau_vote *liste = NULL;
        int count = 0, i, affiches = 0;
        char filtre[64];
        char tampon[64];
        char dept[30];

        backend_input_text(_that->refs.recherche, filtre, sizeof(filtre));
        ui_widget_empty(_that->refs.table_body);

        if (core_bv_liste(&liste, &count) == 0 && liste != NULL) {
                for (i = 0; i < count; i++) {
                        ui_widget_t *ligne;

                        if (filtre[0] != '\0' &&
                            strcasestr(liste[i].Id_BV, filtre) == NULL &&
                            strcasestr(liste[i].Adresse_BV, filtre) == NULL &&
                            strcasestr(liste[i].Id_President, filtre) ==
                                NULL) {
                                continue;
                        }
                        backend_departement(liste[i].Adresse_BV, dept,
                                            sizeof(dept));
                        snprintf(tampon, sizeof(tampon), "%d",
                                 bv_compter_electeurs(liste[i].Id_BV));

                        ligne = backend_row();
                        ui_widget_append(ligne,
                                backend_cell("w-24", liste[i].Id_BV,
                                             "text-body-md font-medium text-on-surface"));
                        ui_widget_append(ligne,
                                backend_cell("flex-1",
                                             liste[i].Adresse_BV[0]
                                                     ? liste[i].Adresse_BV
                                                     : "—",
                                             "text-body-sm text-on-surface"));
                        ui_widget_append(ligne,
                                backend_cell("w-28",
                                             dept[0] ? dept : "—",
                                             "text-body-md text-secondary"));
                        ui_widget_append(ligne,
                                backend_cell("w-32",
                                             liste[i].Id_President[0]
                                                     ? liste[i].Id_President
                                                     : "—",
                                             "text-body-md text-secondary"));
                        ui_widget_append(ligne,
                                backend_cell("w-28 text-center", tampon,
                                             "text-body-md text-on-surface"));
                        ui_widget_append(_that->refs.table_body, ligne);
                        affiches++;
                }
                free(liste);
        }
        if (affiches == 0) {
                ui_widget_append(_that->refs.table_body,
                        backend_text("text-body-sm text-secondary px-4 py-4",
                                     filtre[0] ? "Aucun bureau ne correspond."
                                               : "Aucun bureau de vote enregistré."));
        }
        snprintf(tampon, sizeof(tampon), "Affichage de %d sur %d bureaux",
                 affiches, count);
        ui_widget_set_text(_that->refs.table_count, tampon);
}

/* --- Handlers (references dans page.tsx) --- */

static void bv_search_change(ui_widget_t *w, ui_event_t *e, void *arg)
{
        (void)w;
        (void)arg;
        bv_refresh(e->data);
}

static void bv_toggle_form(ui_widget_t *w, ui_event_t *e, void *arg)
{
        bureaux_page_react_t *_that =
                ui_widget_get_data(e->data, bureaux_page_proto);
        (void)w;
        (void)arg;
        if (ui_widget_has_class(_that->refs.form_panel, "hidden")) {
                ui_widget_remove_class(_that->refs.form_panel, "hidden");
                ui_widget_add_class(_that->refs.form_panel, "flex");
        } else {
                ui_widget_remove_class(_that->refs.form_panel, "flex");
                ui_widget_add_class(_that->refs.form_panel, "hidden");
        }
}

static void bv_save_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        bureaux_page_react_t *_that =
                ui_widget_get_data(e->data, bureaux_page_proto);
        bureau_vote bv;
        char err[128] = "";
        int rc;
        (void)w;
        (void)arg;

        memset(&bv, 0, sizeof(bv));
        backend_input_text(_that->refs.f_id, bv.Id_BV, sizeof(bv.Id_BV));
        backend_input_text(_that->refs.f_adresse, bv.Adresse_BV,
                           sizeof(bv.Adresse_BV));
        backend_input_text(_that->refs.f_president, bv.Id_President,
                           sizeof(bv.Id_President));
        backend_input_text(_that->refs.f_vpresident, bv.Id_VPresident,
                           sizeof(bv.Id_VPresident));
        backend_input_text(_that->refs.f_secretaire, bv.Id_Secret,
                           sizeof(bv.Id_Secret));

        if (bv.Id_President[0] == '\0' || bv.Id_VPresident[0] == '\0' ||
            bv.Id_Secret[0] == '\0') {
                backend_message(_that->refs.form_msg,
                                "Les trois membres du bureau sont requis.", 1);
                return;
        }
        rc = core_bv_ajouter(&bv, err, sizeof(err));
        if (rc != EL_OK) {
                backend_message(_that->refs.form_msg,
                                err[0] ? err
                                       : "Erreur lors de l'enregistrement.",
                                1);
                return;
        }
        backend_message(_that->refs.form_msg, "Bureau de vote enregistré.", 0);
        ui_textinput_clear_text(_that->refs.f_id);
        ui_textinput_clear_text(_that->refs.f_adresse);
        ui_textinput_clear_text(_that->refs.f_president);
        ui_textinput_clear_text(_that->refs.f_vpresident);
        ui_textinput_clear_text(_that->refs.f_secretaire);
        bv_refresh(e->data);
}

/* --- Cycle de vie du composant --- */

static void bureaux_page_init(ui_widget_t *w)
{
        ui_widget_add_data(w, bureaux_page_proto, sizeof(bureaux_page_t));
        bureaux_page_react_init(w);
        bv_refresh(w);
}

static void bureaux_page_destroy(ui_widget_t *w)
{
        bureaux_page_react_destroy(w);
}

void bureaux_page_update(ui_widget_t *w)
{
        bureaux_page_react_update(w);
}

ui_widget_t *ui_create_bureaux_page(void)
{
        return ui_create_widget_with_prototype(bureaux_page_proto);
}

void ui_register_bureaux_page(void)
{
        bureaux_page_init_prototype();
        bureaux_page_proto->init = bureaux_page_init;
        bureaux_page_proto->destroy = bureaux_page_destroy;
}
