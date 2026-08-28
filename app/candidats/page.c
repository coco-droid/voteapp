#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <LCUI/widgets.h>

#include "page.tsx.h"
#include "page.h"
#include "../backend.h"

typedef struct {
        candidats_page_react_t base;
} candidats_page_t;

/* Recharge la table des candidats, filtre optionnel (ID, nom ou prenom). */
static void cand_refresh(ui_widget_t *w)
{
        candidats_page_react_t *_that =
                ui_widget_get_data(w, candidats_page_proto);
        candidats *liste = NULL;
        int count = 0, i, affiches = 0;
        char filtre[64];
        char tampon[64];

        backend_input_text(_that->refs.recherche, filtre, sizeof(filtre));
        ui_widget_empty(_that->refs.table_body);

        if (core_candidat_liste(&liste, &count) == 0 && liste != NULL) {
                for (i = 0; i < count; i++) {
                        ui_widget_t *ligne;
                        ui_widget_t *cell;
                        char nom_complet[48];
                        char contact[56];
                        partie_politic parti;
                        const char *libelle_parti;

                        if (filtre[0] != '\0' &&
                            strcasestr(liste[i].ID_candid, filtre) == NULL &&
                            strcasestr(liste[i].Nom, filtre) == NULL &&
                            strcasestr(liste[i].Prenom, filtre) == NULL) {
                                continue;
                        }
                        snprintf(nom_complet, sizeof(nom_complet), "%s, %s",
                                 liste[i].Nom, liste[i].Prenom);
                        if (liste[i].tel[0] != '\0') {
                                snprintf(contact, sizeof(contact), "%s",
                                         liste[i].tel);
                        } else {
                                snprintf(contact, sizeof(contact), "%s",
                                         liste[i].email);
                        }
                        /* type = "0" => independant, sinon ID d'un parti */
                        if (strcmp(liste[i].type, "0") == 0) {
                                libelle_parti = "Indépendant";
                        } else if (core_parti_obtenir(liste[i].type, &parti)) {
                                libelle_parti = parti.Nom;
                        } else {
                                libelle_parti = liste[i].type;
                        }

                        ligne = backend_row();
                        ui_widget_append(ligne,
                                backend_cell("w-24", liste[i].ID_candid,
                                             "text-body-md font-medium text-on-surface"));
                        ui_widget_append(ligne,
                                backend_cell("flex-1", nom_complet,
                                             "text-body-md text-on-surface"));
                        ui_widget_append(ligne,
                                backend_cell("w-28",
                                             liste[i].Dat_Nais[0]
                                                     ? liste[i].Dat_Nais
                                                     : "—",
                                             "text-body-md text-secondary"));
                        cell = ui_create_widget(NULL);
                        ui_widget_add_class(cell, "w-32 flex flex-row");
                        if (strcmp(liste[i].type, "0") == 0) {
                                ui_widget_append(cell,
                                        backend_badge("Indép.", "blanc"));
                        } else {
                                ui_widget_append(cell,
                                        backend_text("text-body-md text-secondary",
                                                     libelle_parti));
                        }
                        ui_widget_append(ligne, cell);
                        ui_widget_append(ligne,
                                backend_cell("w-36",
                                             contact[0] ? contact : "—",
                                             "text-body-sm text-secondary"));
                        ui_widget_append(_that->refs.table_body, ligne);
                        affiches++;
                }
                free(liste);
        }
        if (affiches == 0) {
                ui_widget_append(_that->refs.table_body,
                        backend_text("text-body-sm text-secondary px-4 py-4",
                                     filtre[0] ? "Aucun candidat ne correspond."
                                               : "Aucun candidat enregistré."));
        }
        snprintf(tampon, sizeof(tampon), "Affichage de %d sur %d candidats",
                 affiches, count);
        ui_widget_set_text(_that->refs.table_count, tampon);
}

/* --- Handlers (references dans page.tsx) --- */

static void cand_search_change(ui_widget_t *w, ui_event_t *e, void *arg)
{
        (void)w;
        (void)arg;
        cand_refresh(e->data);
}

static void cand_toggle_form(ui_widget_t *w, ui_event_t *e, void *arg)
{
        candidats_page_react_t *_that =
                ui_widget_get_data(e->data, candidats_page_proto);
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

static void cand_save_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        candidats_page_react_t *_that =
                ui_widget_get_data(e->data, candidats_page_proto);
        candidats cand;
        char err[128] = "";
        int rc;
        (void)w;
        (void)arg;

        memset(&cand, 0, sizeof(cand));
        backend_input_text(_that->refs.f_id, cand.ID_candid,
                           sizeof(cand.ID_candid));
        backend_input_text(_that->refs.f_nom, cand.Nom, sizeof(cand.Nom));
        backend_input_text(_that->refs.f_prenom, cand.Prenom,
                           sizeof(cand.Prenom));
        backend_input_text(_that->refs.f_naissance, cand.Dat_Nais,
                           sizeof(cand.Dat_Nais));
        backend_input_text(_that->refs.f_tel, cand.tel, sizeof(cand.tel));
        backend_input_text(_that->refs.f_email, cand.email,
                           sizeof(cand.email));
        backend_input_text(_that->refs.f_adresse, cand.Adresse,
                           sizeof(cand.Adresse));
        backend_input_text(_that->refs.f_type, cand.type, sizeof(cand.type));
        backend_input_text(_that->refs.f_date_ins, cand.Date_ins,
                           sizeof(cand.Date_ins));

        /* Valeurs par defaut : independant + date du jour */
        if (cand.type[0] == '\0') {
                strcpy(cand.type, "0");
        }
        if (cand.Date_ins[0] == '\0') {
                core_obtenir_date(cand.Date_ins);
        }

        rc = core_candidat_ajouter(&cand, err, sizeof(err));
        if (rc != EL_OK) {
                backend_message(_that->refs.form_msg,
                                err[0] ? err
                                       : "Erreur lors de l'enregistrement.",
                                1);
                return;
        }
        backend_message(_that->refs.form_msg, "Candidat enregistré.", 0);
        ui_textinput_clear_text(_that->refs.f_id);
        ui_textinput_clear_text(_that->refs.f_nom);
        ui_textinput_clear_text(_that->refs.f_prenom);
        ui_textinput_clear_text(_that->refs.f_naissance);
        ui_textinput_clear_text(_that->refs.f_tel);
        ui_textinput_clear_text(_that->refs.f_email);
        ui_textinput_clear_text(_that->refs.f_adresse);
        ui_textinput_clear_text(_that->refs.f_type);
        ui_textinput_clear_text(_that->refs.f_date_ins);
        cand_refresh(e->data);
}

/* --- Cycle de vie du composant --- */

static void candidats_page_init(ui_widget_t *w)
{
        ui_widget_add_data(w, candidats_page_proto, sizeof(candidats_page_t));
        candidats_page_react_init(w);
        cand_refresh(w);
}

static void candidats_page_destroy(ui_widget_t *w)
{
        candidats_page_react_destroy(w);
}

void candidats_page_update(ui_widget_t *w)
{
        candidats_page_react_update(w);
}

ui_widget_t *ui_create_candidats_page(void)
{
        return ui_create_widget_with_prototype(candidats_page_proto);
}

void ui_register_candidats_page(void)
{
        candidats_page_init_prototype();
        candidats_page_proto->init = candidats_page_init;
        candidats_page_proto->destroy = candidats_page_destroy;
}
