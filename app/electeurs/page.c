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
        int mode_edition;           /* 1 = modification d'un electeur       */
        char cle_edition[15];       /* NINU en cours de modification         */
        char cle_suppression[15];   /* NINU en attente de confirmation       */
} electeurs_page_t;

static void elect_edit_click(ui_widget_t *w, ui_event_t *e, void *arg);
static void elect_del_click(ui_widget_t *w, ui_event_t *e, void *arg);

/* Remet le formulaire en mode "nouvel electeur". */
static void elect_form_nouveau(ui_widget_t *w)
{
        electeurs_page_t *that = ui_widget_get_data(w, electeurs_page_proto);

        that->mode_edition = 0;
        that->cle_edition[0] = '\0';
        ui_widget_set_text(that->base.refs.form_title, "Inscrire un électeur");
        backend_input_readonly(that->base.refs.f_ninu, 0);
        ui_textinput_clear_text(that->base.refs.f_ninu);
        ui_textinput_clear_text(that->base.refs.f_nom);
        ui_textinput_clear_text(that->base.refs.f_prenom);
        ui_textinput_clear_text(that->base.refs.f_adresse);
        ui_textinput_clear_text(that->base.refs.f_bv);
        ui_widget_set_text(that->base.refs.form_msg, "");
}

static void elect_form_ouvrir(ui_widget_t *w)
{
        electeurs_page_react_t *_that =
                ui_widget_get_data(w, electeurs_page_proto);
        ui_widget_remove_class(_that->refs.form_panel, "hidden");
        ui_widget_add_class(_that->refs.form_panel, "flex");
}

static void elect_form_fermer(ui_widget_t *w)
{
        electeurs_page_react_t *_that =
                ui_widget_get_data(w, electeurs_page_proto);
        ui_widget_remove_class(_that->refs.form_panel, "flex");
        ui_widget_add_class(_that->refs.form_panel, "hidden");
}

static void elect_confirm_cacher(ui_widget_t *w)
{
        electeurs_page_t *that = ui_widget_get_data(w, electeurs_page_proto);
        ui_widget_remove_class(that->base.refs.confirm_panel, "flex");
        ui_widget_add_class(that->base.refs.confirm_panel, "hidden");
        that->cle_suppression[0] = '\0';
}

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
                        ui_widget_t *btn;
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

                        /* Colonne Actions : Modifier / Supprimer */
                        cell = ui_create_widget(NULL);
                        ui_widget_add_class(cell,
                                "w-36 flex flex-row justify-center");
                        btn = backend_action_btn("Modifier", 0);
                        ui_widget_set_attr(btn, "data-id", liste[i].NINU);
                        ui_widget_on(btn, "click", elect_edit_click, w);
                        ui_widget_append(cell, btn);
                        btn = backend_action_btn("Supprimer", 1);
                        ui_widget_set_attr(btn, "data-id", liste[i].NINU);
                        ui_widget_on(btn, "click", elect_del_click, w);
                        ui_widget_append(cell, btn);
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
        electeurs_page_t *that = ui_widget_get_data(e->data,
                                                    electeurs_page_proto);
        (void)w;
        (void)arg;
        if (ui_widget_has_class(that->base.refs.form_panel, "hidden")) {
                /* Ouvrir AVANT de remplir (cf. partis) */
                elect_form_ouvrir(e->data);
                elect_form_nouveau(e->data);
        } else {
                elect_form_fermer(e->data);
                elect_form_nouveau(e->data);
        }
}

static void elect_save_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        electeurs_page_t *that = ui_widget_get_data(e->data,
                                                    electeurs_page_proto);
        electeurs elec;
        char err[160] = "";
        int rc;
        (void)w;
        (void)arg;

        memset(&elec, 0, sizeof(elec));
        backend_input_text(that->base.refs.f_ninu, elec.NINU,
                           sizeof(elec.NINU));
        backend_input_text(that->base.refs.f_nom, elec.nom, sizeof(elec.nom));
        backend_input_text(that->base.refs.f_prenom, elec.prenom,
                           sizeof(elec.prenom));
        backend_input_text(that->base.refs.f_adresse, elec.Adresse,
                           sizeof(elec.Adresse));
        backend_input_text(that->base.refs.f_bv, elec.BV, sizeof(elec.BV));

        if (that->mode_edition) {
                /* Le NINU n'est pas modifiable : on reprend la cle */
                snprintf(elec.NINU, sizeof(elec.NINU), "%s",
                         that->cle_edition);
                rc = core_electeur_modifier(&elec, err, sizeof(err));
                if (rc != EL_OK) {
                        backend_message(that->base.refs.form_msg,
                                        err[0] ? err : "Erreur lors de la modification.",
                                        1);
                        return;
                }
                backend_message(that->base.refs.table_msg,
                                "Électeur modifié.", 0);
        } else {
                rc = core_electeur_ajouter(&elec, err, sizeof(err));
                if (rc != EL_OK) {
                        backend_message(that->base.refs.form_msg,
                                        err[0] ? err : "Erreur lors de l'inscription.",
                                        1);
                        return;
                }
                backend_message(that->base.refs.table_msg,
                                "Électeur inscrit.", 0);
        }
        elect_form_fermer(e->data);
        elect_form_nouveau(e->data);
        elect_refresh(e->data);
}

/* Clic sur "Modifier" d'une ligne : ouvre le formulaire prerempli. */
static void elect_edit_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        electeurs_page_t *that = ui_widget_get_data(e->data,
                                                    electeurs_page_proto);
        const char *id = ui_widget_get_attr(w, "data-id");
        electeurs elec;
        (void)arg;

        if (that == NULL || id == NULL) {
                return;
        }
        if (core_electeur_obtenir(id, &elec) == 0) {
                backend_message(that->base.refs.table_msg,
                                "Électeur introuvable.", 1);
                return;
        }
        that->mode_edition = 1;
        snprintf(that->cle_edition, sizeof(that->cle_edition), "%s", id);
        elect_confirm_cacher(e->data);
        elect_form_ouvrir(e->data);

        ui_widget_set_text(that->base.refs.form_title,
                           "Modifier l'électeur");
        backend_input_readonly(that->base.refs.f_ninu, 1);
        backend_input_set_text(that->base.refs.f_ninu, elec.NINU);

        backend_input_set_text(that->base.refs.f_nom, elec.nom);
        backend_input_set_text(that->base.refs.f_prenom, elec.prenom);
        backend_input_set_text(that->base.refs.f_adresse, elec.Adresse);
        backend_input_set_text(that->base.refs.f_bv, elec.BV);
        ui_widget_set_text(that->base.refs.form_msg, "");
}

/* Clic sur "Supprimer" d'une ligne : affiche la barre de confirmation. */
static void elect_del_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        electeurs_page_t *that = ui_widget_get_data(e->data,
                                                    electeurs_page_proto);
        const char *id = ui_widget_get_attr(w, "data-id");
        char msg[112];
        (void)arg;

        if (that == NULL || id == NULL) {
                return;
        }
        snprintf(that->cle_suppression, sizeof(that->cle_suppression),
                 "%s", id);
        snprintf(msg, sizeof(msg),
                 "Supprimer définitivement l'électeur NINU « %s » ?", id);
        ui_widget_set_text(that->base.refs.confirm_msg, msg);
        ui_widget_remove_class(that->base.refs.confirm_panel, "hidden");
        ui_widget_add_class(that->base.refs.confirm_panel, "flex");
}

static void elect_del_confirm(ui_widget_t *w, ui_event_t *e, void *arg)
{
        electeurs_page_t *that = ui_widget_get_data(e->data,
                                                    electeurs_page_proto);
        char err[160] = "";
        int rc;
        (void)w;
        (void)arg;

        if (that->cle_suppression[0] == '\0') {
                elect_confirm_cacher(e->data);
                return;
        }
        rc = core_electeur_supprimer(that->cle_suppression, err, sizeof(err));
        if (rc != EL_OK) {
                backend_message(that->base.refs.table_msg,
                                err[0] ? err : "Suppression impossible.", 1);
        } else {
                backend_message(that->base.refs.table_msg,
                                "Électeur supprimé.", 0);
        }
        elect_confirm_cacher(e->data);
        elect_form_nouveau(e->data);
        elect_refresh(e->data);
}

static void elect_del_cancel(ui_widget_t *w, ui_event_t *e, void *arg)
{
        (void)w;
        (void)arg;
        elect_confirm_cacher(e->data);
}

/* --- Cycle de vie du composant --- */

static void electeurs_page_init(ui_widget_t *w)
{
        electeurs_page_t *that;

        ui_widget_add_data(w, electeurs_page_proto, sizeof(electeurs_page_t));
        electeurs_page_react_init(w);
        that = ui_widget_get_data(w, electeurs_page_proto);
        that->mode_edition = 0;
        that->cle_edition[0] = '\0';
        that->cle_suppression[0] = '\0';
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
