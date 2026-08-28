#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <LCUI/widgets.h>

#include "page.tsx.h"
#include "page.h"
#include "../backend.h"

typedef struct {
        partis_page_react_t base;
} partis_page_t;

/* Recharge la table des partis, avec filtre optionnel sur le nom. */
static void partis_refresh(ui_widget_t *w)
{
        partis_page_react_t *_that = ui_widget_get_data(w, partis_page_proto);
        partie_politic *liste = NULL;
        int count = 0, i, affiches = 0;
        char filtre[64];
        char tampon[64];

        backend_input_text(_that->refs.recherche, filtre, sizeof(filtre));
        ui_widget_empty(_that->refs.table_body);

        if (core_parti_liste(&liste, &count) == 0 && liste != NULL) {
                for (i = 0; i < count; i++) {
                        ui_widget_t *ligne;
                        if (filtre[0] != '\0' &&
                            strcasestr(liste[i].Nom, filtre) == NULL) {
                                continue;
                        }
                        ligne = backend_row();
                        ui_widget_append(ligne,
                                backend_cell("w-24", liste[i].id_PP,
                                             "text-body-sm text-secondary"));
                        ui_widget_append(ligne,
                                backend_cell("flex-1", liste[i].Nom,
                                             "text-body-md font-medium text-on-surface"));
                        ui_widget_append(ligne,
                                backend_cell("w-32", liste[i].date_creat,
                                             "text-body-md text-secondary"));
                        ui_widget_append(ligne,
                                backend_cell("w-32", liste[i].Responsable,
                                             "text-body-md text-on-surface"));
                        ui_widget_append(ligne,
                                backend_cell("w-40", liste[i].Telephone,
                                             "text-body-md text-secondary"));
                        {
                                ui_widget_t *cell = ui_create_widget(NULL);
                                ui_widget_add_class(cell, "w-24 flex flex-row "
                                                          "justify-center");
                                ui_widget_append(cell,
                                        backend_badge("Actif", "actif"));
                                ui_widget_append(ligne, cell);
                        }
                        ui_widget_append(_that->refs.table_body, ligne);
                        affiches++;
                }
                free(liste);
        }
        if (affiches == 0) {
                ui_widget_append(_that->refs.table_body,
                        backend_text("text-body-sm text-secondary px-4 py-4",
                                     filtre[0] ? "Aucun parti ne correspond à la recherche."
                                               : "Aucun parti enregistré."));
        }
        snprintf(tampon, sizeof(tampon), "Affichage de %d sur %d partis",
                 affiches, count);
        ui_widget_set_text(_that->refs.table_count, tampon);
}

/* --- Handlers (references dans page.tsx) --- */

static void partis_search_change(ui_widget_t *w, ui_event_t *e, void *arg)
{
        (void)w;
        (void)arg;
        partis_refresh(e->data);
}

static void partis_toggle_form(ui_widget_t *w, ui_event_t *e, void *arg)
{
        partis_page_react_t *_that = ui_widget_get_data(e->data,
                                                        partis_page_proto);
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

static void partis_save_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        partis_page_react_t *_that = ui_widget_get_data(e->data,
                                                        partis_page_proto);
        partie_politic p;
        char err[128] = "";
        int rc;
        (void)w;
        (void)arg;

        memset(&p, 0, sizeof(p));
        backend_input_text(_that->refs.f_id, p.id_PP, sizeof(p.id_PP));
        backend_input_text(_that->refs.f_nom, p.Nom, sizeof(p.Nom));
        backend_input_text(_that->refs.f_responsable, p.Responsable,
                           sizeof(p.Responsable));
        backend_input_text(_that->refs.f_date_creat, p.date_creat,
                           sizeof(p.date_creat));
        backend_input_text(_that->refs.f_tel, p.Telephone,
                           sizeof(p.Telephone));
        backend_input_text(_that->refs.f_email, p.email, sizeof(p.email));
        backend_input_text(_that->refs.f_adresse, p.Adresse,
                           sizeof(p.Adresse));
        /* Date d'inscription = aujourd'hui */
        core_obtenir_date(p.date_ins);

        rc = core_parti_ajouter(&p, err, sizeof(err));
        if (rc != EL_OK) {
                backend_message(_that->refs.form_msg,
                                err[0] ? err : "Erreur lors de l'ajout.", 1);
                return;
        }
        backend_message(_that->refs.form_msg, "Parti enregistré.", 0);
        ui_textinput_clear_text(_that->refs.f_id);
        ui_textinput_clear_text(_that->refs.f_nom);
        ui_textinput_clear_text(_that->refs.f_responsable);
        ui_textinput_clear_text(_that->refs.f_date_creat);
        ui_textinput_clear_text(_that->refs.f_tel);
        ui_textinput_clear_text(_that->refs.f_email);
        ui_textinput_clear_text(_that->refs.f_adresse);
        partis_refresh(e->data);
}

/* --- Cycle de vie du composant --- */

static void partis_page_init(ui_widget_t *w)
{
        ui_widget_add_data(w, partis_page_proto, sizeof(partis_page_t));
        partis_page_react_init(w);
        partis_refresh(w);
}

static void partis_page_destroy(ui_widget_t *w)
{
        partis_page_react_destroy(w);
}

void partis_page_update(ui_widget_t *w)
{
        partis_page_react_update(w);
}

ui_widget_t *ui_create_partis_page(void)
{
        return ui_create_widget_with_prototype(partis_page_proto);
}

void ui_register_partis_page(void)
{
        partis_page_init_prototype();
        partis_page_proto->init = partis_page_init;
        partis_page_proto->destroy = partis_page_destroy;
}
