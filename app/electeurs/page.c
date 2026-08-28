#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <LCUI/widgets.h>

#include "page.tsx.h"
#include "page.h"
#include "../backend.h"

typedef struct {
        electeurs_page_react_t base;
} electeurs_page_t;

/* Recharge la table des electeurs, filtre optionnel (NINU ou nom). */
static void elect_refresh(ui_widget_t *w)
{
        electeurs_page_react_t *_that =
                ui_widget_get_data(w, electeurs_page_proto);
        electeurs *liste = NULL;
        int count = 0, i, affiches = 0;
        char filtre[64];
        char tampon[64];
        char dept[30];

        backend_input_text(_that->refs.recherche, filtre, sizeof(filtre));
        ui_widget_empty(_that->refs.table_body);

        if (core_electeur_liste(&liste, &count) == 0 && liste != NULL) {
                for (i = 0; i < count; i++) {
                        ui_widget_t *ligne;
                        ui_widget_t *cell;
                        char nom_complet[56];
                        int a_vote;

                        if (filtre[0] != '\0' &&
                            strcasestr(liste[i].NINU, filtre) == NULL &&
                            strcasestr(liste[i].nom, filtre) == NULL &&
                            strcasestr(liste[i].prenom, filtre) == NULL) {
                                continue;
                        }
                        snprintf(nom_complet, sizeof(nom_complet), "%s, %s",
                                 liste[i].nom, liste[i].prenom);
                        backend_departement(liste[i].Adresse, dept,
                                            sizeof(dept));
                        a_vote = core_ninu_a_deja_vote(liste[i].NINU);

                        ligne = backend_row();
                        ui_widget_append(ligne,
                                backend_cell("w-40", liste[i].NINU,
                                             "text-body-md font-medium text-on-surface"));
                        ui_widget_append(ligne,
                                backend_cell("flex-1", nom_complet,
                                             "text-body-md text-on-surface"));
                        ui_widget_append(ligne,
                                backend_cell("w-32",
                                             dept[0] ? dept : "—",
                                             "text-body-md text-secondary"));
                        ui_widget_append(ligne,
                                backend_cell("w-32", liste[i].BV,
                                             "text-body-md text-secondary"));
                        cell = ui_create_widget(NULL);
                        ui_widget_add_class(cell,
                                "w-28 flex flex-row justify-center");
                        if (a_vote) {
                                ui_widget_append(cell,
                                        backend_badge("Voté", "ok"));
                        } else {
                                ui_widget_append(cell,
                                        backend_badge("Non Voté", "no"));
                        }
                        ui_widget_append(ligne, cell);
                        ui_widget_append(_that->refs.table_body, ligne);
                        affiches++;
                }
                free(liste);
        }
        if (affiches == 0) {
                ui_widget_append(_that->refs.table_body,
                        backend_text("text-body-sm text-secondary px-4 py-4",
                                     filtre[0] ? "Aucun électeur ne correspond."
                                               : "Aucun électeur enregistré."));
        }
        snprintf(tampon, sizeof(tampon), "Affichage de %d sur %d électeurs",
                 affiches, count);
        ui_widget_set_text(_that->refs.table_count, tampon);
}

/* --- Handlers (references dans page.tsx) --- */

static void elect_search_change(ui_widget_t *w, ui_event_t *e, void *arg)
{
        (void)w;
        (void)arg;
        elect_refresh(e->data);
}

static void elect_toggle_form(ui_widget_t *w, ui_event_t *e, void *arg)
{
        electeurs_page_react_t *_that =
                ui_widget_get_data(e->data, electeurs_page_proto);
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

static void elect_save_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        electeurs_page_react_t *_that =
                ui_widget_get_data(e->data, electeurs_page_proto);
        electeurs elec;
        char err[128] = "";
        int rc;
        (void)w;
        (void)arg;

        memset(&elec, 0, sizeof(elec));
        backend_input_text(_that->refs.f_ninu, elec.NINU, sizeof(elec.NINU));
        backend_input_text(_that->refs.f_nom, elec.nom, sizeof(elec.nom));
        backend_input_text(_that->refs.f_prenom, elec.prenom,
                           sizeof(elec.prenom));
        backend_input_text(_that->refs.f_adresse, elec.Adresse,
                           sizeof(elec.Adresse));
        backend_input_text(_that->refs.f_bv, elec.BV, sizeof(elec.BV));

        rc = core_electeur_ajouter(&elec, err, sizeof(err));
        if (rc != EL_OK) {
                backend_message(_that->refs.form_msg,
                                err[0] ? err : "Erreur lors de l'inscription.",
                                1);
                return;
        }
        backend_message(_that->refs.form_msg, "Électeur inscrit.", 0);
        ui_textinput_clear_text(_that->refs.f_ninu);
        ui_textinput_clear_text(_that->refs.f_nom);
        ui_textinput_clear_text(_that->refs.f_prenom);
        ui_textinput_clear_text(_that->refs.f_adresse);
        ui_textinput_clear_text(_that->refs.f_bv);
        elect_refresh(e->data);
}

/* --- Cycle de vie du composant --- */

static void electeurs_page_init(ui_widget_t *w)
{
        ui_widget_add_data(w, electeurs_page_proto, sizeof(electeurs_page_t));
        electeurs_page_react_init(w);
        elect_refresh(w);
}

static void electeurs_page_destroy(ui_widget_t *w)
{
        electeurs_page_react_destroy(w);
}

void electeurs_page_update(ui_widget_t *w)
{
        electeurs_page_react_update(w);
}

ui_widget_t *ui_create_electeurs_page(void)
{
        return ui_create_widget_with_prototype(electeurs_page_proto);
}

void ui_register_electeurs_page(void)
{
        electeurs_page_init_prototype();
        electeurs_page_proto->init = electeurs_page_init;
        electeurs_page_proto->destroy = electeurs_page_destroy;
}
