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
        int mode_edition;           /* 1 = modification d'un parti existant */
        char cle_edition[10];       /* id_PP en cours de modification        */
        char cle_suppression[10];   /* id_PP en attente de confirmation      */
} partis_page_t;

static void partis_edit_click(ui_widget_t *w, ui_event_t *e, void *arg);
static void partis_del_click(ui_widget_t *w, ui_event_t *e, void *arg);

/* Remet le formulaire en mode "nouveau parti" (ID auto-incremente). */
static void partis_form_nouveau(ui_widget_t *w)
{
        partis_page_t *that = ui_widget_get_data(w, partis_page_proto);
        char id[10];

        that->mode_edition = 0;
        that->cle_edition[0] = '\0';
        ui_widget_set_text(that->base.refs.form_title,
                           "Nouveau parti politique");
        core_parti_prochain_id(id, sizeof(id));
        backend_input_readonly(that->base.refs.f_id, 1);
        backend_input_set_text(that->base.refs.f_id, id);

        ui_textinput_clear_text(that->base.refs.f_nom);
        ui_textinput_clear_text(that->base.refs.f_responsable);
        ui_textinput_clear_text(that->base.refs.f_date_creat);
        ui_textinput_clear_text(that->base.refs.f_tel);
        ui_textinput_clear_text(that->base.refs.f_email);
        ui_textinput_clear_text(that->base.refs.f_adresse);
        ui_widget_set_text(that->base.refs.form_msg, "");
}

static void partis_form_ouvrir(ui_widget_t *w)
{
        partis_page_react_t *_that = ui_widget_get_data(w, partis_page_proto);
        ui_widget_remove_class(_that->refs.form_panel, "hidden");
        ui_widget_add_class(_that->refs.form_panel, "flex");
}

static void partis_form_fermer(ui_widget_t *w)
{
        partis_page_react_t *_that = ui_widget_get_data(w, partis_page_proto);
        ui_widget_remove_class(_that->refs.form_panel, "flex");
        ui_widget_add_class(_that->refs.form_panel, "hidden");
}

static void partis_confirm_cacher(ui_widget_t *w)
{
        partis_page_t *that = ui_widget_get_data(w, partis_page_proto);
        ui_widget_remove_class(that->base.refs.confirm_panel, "flex");
        ui_widget_add_class(that->base.refs.confirm_panel, "hidden");
        that->cle_suppression[0] = '\0';
}

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
                        ui_widget_t *cell;
                        ui_widget_t *btn;
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
                                backend_cell("w-32",
                                             liste[i].date_creat[0]
                                                     ? liste[i].date_creat
                                                     : "—",
                                             "text-body-md text-secondary"));
                        ui_widget_append(ligne,
                                backend_cell("w-32",
                                             liste[i].Responsable[0]
                                                     ? liste[i].Responsable
                                                     : "—",
                                             "text-body-md text-on-surface"));
                        ui_widget_append(ligne,
                                backend_cell("w-40",
                                             liste[i].Telephone[0]
                                                     ? liste[i].Telephone
                                                     : "—",
                                             "text-body-md text-secondary"));
                        {
                                ui_widget_t *cellule = ui_create_widget(NULL);
                                ui_widget_add_class(cellule, "w-24 flex flex-row "
                                                             "justify-center");
                                ui_widget_append(cellule,
                                        backend_badge("Actif", "actif"));
                                ui_widget_append(ligne, cellule);
                        }
                        /* Colonne Actions : Modifier / Supprimer */
                        cell = ui_create_widget(NULL);
                        ui_widget_add_class(cell,
                                "w-36 flex flex-row justify-center");
                        btn = backend_action_btn("Modifier", 0);
                        ui_widget_set_attr(btn, "data-id", liste[i].id_PP);
                        ui_widget_on(btn, "click", partis_edit_click, w);
                        ui_widget_append(cell, btn);
                        btn = backend_action_btn("Supprimer", 1);
                        ui_widget_set_attr(btn, "data-id", liste[i].id_PP);
                        ui_widget_on(btn, "click", partis_del_click, w);
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
        partis_page_t *that = ui_widget_get_data(e->data, partis_page_proto);
        (void)w;
        (void)arg;
        if (ui_widget_has_class(that->base.refs.form_panel, "hidden")) {
                /* Ouvrir AVANT de remplir : un textinput jamais rendu
                 * (display:none depuis sa creation) ignore le texte qu'on
                 * lui affecte lorsqu'il est desactive. */
                partis_form_ouvrir(e->data);
                partis_form_nouveau(e->data);
        } else {
                partis_form_fermer(e->data);
                partis_form_nouveau(e->data);
        }
}

static void partis_save_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        partis_page_t *that = ui_widget_get_data(e->data, partis_page_proto);
        partie_politic p;
        char err[128] = "";
        int rc;
        (void)w;
        (void)arg;

        memset(&p, 0, sizeof(p));
        backend_input_text(that->base.refs.f_id, p.id_PP, sizeof(p.id_PP));
        backend_input_text(that->base.refs.f_nom, p.Nom, sizeof(p.Nom));
        backend_input_text(that->base.refs.f_responsable, p.Responsable,
                           sizeof(p.Responsable));
        backend_input_text(that->base.refs.f_date_creat, p.date_creat,
                           sizeof(p.date_creat));
        backend_input_text(that->base.refs.f_tel, p.Telephone,
                           sizeof(p.Telephone));
        backend_input_text(that->base.refs.f_email, p.email, sizeof(p.email));
        backend_input_text(that->base.refs.f_adresse, p.Adresse,
                           sizeof(p.Adresse));

        if (that->mode_edition) {
                /* L'identifiant n'est pas modifiable : on reprend la cle */
                snprintf(p.id_PP, sizeof(p.id_PP), "%s", that->cle_edition);
                if (core_parti_obtenir(p.id_PP, &p) == 0) {
                        backend_message(that->base.refs.form_msg,
                                        "Parti introuvable.", 1);
                        return;
                }
                /* Relecture des champs modifiables saisis */
                backend_input_text(that->base.refs.f_nom, p.Nom,
                                   sizeof(p.Nom));
                backend_input_text(that->base.refs.f_responsable,
                                   p.Responsable, sizeof(p.Responsable));
                backend_input_text(that->base.refs.f_date_creat,
                                   p.date_creat, sizeof(p.date_creat));
                backend_input_text(that->base.refs.f_tel, p.Telephone,
                                   sizeof(p.Telephone));
                backend_input_text(that->base.refs.f_email, p.email,
                                   sizeof(p.email));
                backend_input_text(that->base.refs.f_adresse, p.Adresse,
                                   sizeof(p.Adresse));
                rc = core_parti_modifier(&p, err, sizeof(err));
                if (rc != EL_OK) {
                        backend_message(that->base.refs.form_msg,
                                        err[0] ? err : "Erreur lors de la modification.",
                                        1);
                        return;
                }
                backend_message(that->base.refs.table_msg,
                                "Parti modifié.", 0);
                partis_form_fermer(e->data);
                partis_form_nouveau(e->data);
                partis_refresh(e->data);
                return;
        }

        /* Ajout : l'ID est auto-incremente, la date d'inscription = aujourd'hui */
        if (p.id_PP[0] == '\0') {
                core_parti_prochain_id(p.id_PP, sizeof(p.id_PP));
        }
        core_obtenir_date(p.date_ins);

        rc = core_parti_ajouter(&p, err, sizeof(err));
        if (rc != EL_OK) {
                backend_message(that->base.refs.form_msg,
                                err[0] ? err : "Erreur lors de l'ajout.", 1);
                return;
        }
        backend_message(that->base.refs.table_msg,
                        "Parti enregistré.", 0);
        partis_form_fermer(e->data);
        partis_form_nouveau(e->data);
        partis_refresh(e->data);
}

/* Clic sur "Modifier" d'une ligne : ouvre le formulaire prerempli. */
static void partis_edit_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        partis_page_t *that = ui_widget_get_data(e->data, partis_page_proto);
        const char *id = ui_widget_get_attr(w, "data-id");
        partie_politic p;
        (void)arg;

        if (that == NULL || id == NULL) {
                return;
        }
        if (core_parti_obtenir(id, &p) == 0) {
                backend_message(that->base.refs.table_msg,
                                "Parti introuvable.", 1);
                return;
        }
        that->mode_edition = 1;
        snprintf(that->cle_edition, sizeof(that->cle_edition), "%s", id);
        partis_confirm_cacher(e->data);
        partis_form_ouvrir(e->data);

        ui_widget_set_text(that->base.refs.form_title,
                           "Modifier le parti politique");
        backend_input_readonly(that->base.refs.f_id, 1);
        backend_input_set_text(that->base.refs.f_id, p.id_PP);
        backend_input_set_text(that->base.refs.f_nom, p.Nom);
        backend_input_set_text(that->base.refs.f_responsable, p.Responsable);
        backend_input_set_text(that->base.refs.f_date_creat, p.date_creat);
        backend_input_set_text(that->base.refs.f_tel, p.Telephone);
        backend_input_set_text(that->base.refs.f_email, p.email);
        backend_input_set_text(that->base.refs.f_adresse, p.Adresse);
        ui_widget_set_text(that->base.refs.form_msg, "");
}

/* Clic sur "Supprimer" d'une ligne : affiche la barre de confirmation. */
static void partis_del_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        partis_page_t *that = ui_widget_get_data(e->data, partis_page_proto);
        const char *id = ui_widget_get_attr(w, "data-id");
        char msg[96];
        (void)arg;

        if (that == NULL || id == NULL) {
                return;
        }
        snprintf(that->cle_suppression, sizeof(that->cle_suppression),
                 "%s", id);
        snprintf(msg, sizeof(msg),
                 "Supprimer définitivement le parti « %s » ?", id);
        ui_widget_set_text(that->base.refs.confirm_msg, msg);
        ui_widget_remove_class(that->base.refs.confirm_panel, "hidden");
        ui_widget_add_class(that->base.refs.confirm_panel, "flex");
}

static void partis_del_confirm(ui_widget_t *w, ui_event_t *e, void *arg)
{
        partis_page_t *that = ui_widget_get_data(e->data, partis_page_proto);
        char err[160] = "";
        int rc;
        (void)w;
        (void)arg;

        if (that->cle_suppression[0] == '\0') {
                partis_confirm_cacher(e->data);
                return;
        }
        rc = core_parti_supprimer(that->cle_suppression, err, sizeof(err));
        if (rc != EL_OK) {
                backend_message(that->base.refs.table_msg,
                                err[0] ? err : "Suppression impossible.", 1);
        } else {
                backend_message(that->base.refs.table_msg,
                                "Parti supprimé.", 0);
        }
        partis_confirm_cacher(e->data);
        partis_form_nouveau(e->data);
        partis_refresh(e->data);
}

static void partis_del_cancel(ui_widget_t *w, ui_event_t *e, void *arg)
{
        (void)w;
        (void)arg;
        partis_confirm_cacher(e->data);
}

/* --- Cycle de vie du composant --- */

static void partis_page_init(ui_widget_t *w)
{
        partis_page_t *that;

        ui_widget_add_data(w, partis_page_proto, sizeof(partis_page_t));
        partis_page_react_init(w);
        that = ui_widget_get_data(w, partis_page_proto);
        that->mode_edition = 0;
        that->cle_edition[0] = '\0';
        that->cle_suppression[0] = '\0';
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
