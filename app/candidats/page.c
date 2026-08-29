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
        int mode_edition;           /* 1 = modification d'un candidat       */
        char cle_edition[10];       /* ID_candid en cours de modification    */
        char cle_suppression[10];   /* ID_candid en attente de confirmation  */
} candidats_page_t;

static void cand_edit_click(ui_widget_t *w, ui_event_t *e, void *arg);
static void cand_del_click(ui_widget_t *w, ui_event_t *e, void *arg);

/* Remet le formulaire en mode "nouveau candidat" (ID auto-incremente). */
static void cand_form_nouveau(ui_widget_t *w)
{
        candidats_page_t *that = ui_widget_get_data(w, candidats_page_proto);
        char id[10];

        that->mode_edition = 0;
        that->cle_edition[0] = '\0';
        ui_widget_set_text(that->base.refs.form_title, "Nouveau candidat");
        core_candidat_prochain_id(id, sizeof(id));
        backend_input_readonly(that->base.refs.f_id, 1);
        backend_input_set_text(that->base.refs.f_id, id);

        ui_textinput_clear_text(that->base.refs.f_nom);
        ui_textinput_clear_text(that->base.refs.f_prenom);
        ui_textinput_clear_text(that->base.refs.f_naissance);
        ui_textinput_clear_text(that->base.refs.f_tel);
        ui_textinput_clear_text(that->base.refs.f_email);
        ui_textinput_clear_text(that->base.refs.f_adresse);
        ui_textinput_clear_text(that->base.refs.f_type);
        ui_widget_set_text(that->base.refs.form_msg, "");
}

static void cand_form_ouvrir(ui_widget_t *w)
{
        candidats_page_react_t *_that =
                ui_widget_get_data(w, candidats_page_proto);
        ui_widget_remove_class(_that->refs.form_panel, "hidden");
        ui_widget_add_class(_that->refs.form_panel, "flex");
}

static void cand_form_fermer(ui_widget_t *w)
{
        candidats_page_react_t *_that =
                ui_widget_get_data(w, candidats_page_proto);
        ui_widget_remove_class(_that->refs.form_panel, "flex");
        ui_widget_add_class(_that->refs.form_panel, "hidden");
}

static void cand_confirm_cacher(ui_widget_t *w)
{
        candidats_page_t *that = ui_widget_get_data(w, candidats_page_proto);
        ui_widget_remove_class(that->base.refs.confirm_panel, "flex");
        ui_widget_add_class(that->base.refs.confirm_panel, "hidden");
        that->cle_suppression[0] = '\0';
}

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
                        ui_widget_t *btn;
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

                        /* Colonne Actions : Modifier / Supprimer */
                        cell = ui_create_widget(NULL);
                        ui_widget_add_class(cell,
                                "w-36 flex flex-row justify-center");
                        btn = backend_action_btn("Modifier", 0);
                        ui_widget_set_attr(btn, "data-id",
                                           liste[i].ID_candid);
                        ui_widget_on(btn, "click", cand_edit_click, w);
                        ui_widget_append(cell, btn);
                        btn = backend_action_btn("Supprimer", 1);
                        ui_widget_set_attr(btn, "data-id",
                                           liste[i].ID_candid);
                        ui_widget_on(btn, "click", cand_del_click, w);
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
        candidats_page_t *that = ui_widget_get_data(e->data,
                                                    candidats_page_proto);
        (void)w;
        (void)arg;
        if (ui_widget_has_class(that->base.refs.form_panel, "hidden")) {
                /* Ouvrir AVANT de remplir (cf. partis) */
                cand_form_ouvrir(e->data);
                cand_form_nouveau(e->data);
        } else {
                cand_form_fermer(e->data);
                cand_form_nouveau(e->data);
        }
}

static void cand_save_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        candidats_page_t *that = ui_widget_get_data(e->data,
                                                    candidats_page_proto);
        candidats cand;
        char err[160] = "";
        int rc;
        (void)w;
        (void)arg;

        memset(&cand, 0, sizeof(cand));
        backend_input_text(that->base.refs.f_id, cand.ID_candid,
                           sizeof(cand.ID_candid));
        backend_input_text(that->base.refs.f_nom, cand.Nom, sizeof(cand.Nom));
        backend_input_text(that->base.refs.f_prenom, cand.Prenom,
                           sizeof(cand.Prenom));
        backend_input_text(that->base.refs.f_naissance, cand.Dat_Nais,
                           sizeof(cand.Dat_Nais));
        backend_input_text(that->base.refs.f_tel, cand.tel, sizeof(cand.tel));
        backend_input_text(that->base.refs.f_email, cand.email,
                           sizeof(cand.email));
        backend_input_text(that->base.refs.f_adresse, cand.Adresse,
                           sizeof(cand.Adresse));
        backend_input_text(that->base.refs.f_type, cand.type,
                           sizeof(cand.type));

        /* Valeur par defaut : independant si le champ parti est vide */
        if (cand.type[0] == '\0') {
                strcpy(cand.type, "0");
        }

        if (that->mode_edition) {
                /* L'identifiant n'est pas modifiable : on reprend la cle.
                 * La date d'inscription d'origine est conservee. */
                snprintf(cand.ID_candid, sizeof(cand.ID_candid), "%s",
                         that->cle_edition);
                {
                        candidats ancien;
                        if (core_candidat_obtenir(cand.ID_candid, &ancien)) {
                                snprintf(cand.Date_ins, sizeof(cand.Date_ins),
                                         "%s", ancien.Date_ins);
                        }
                }
                rc = core_candidat_modifier(&cand, err, sizeof(err));
                if (rc != EL_OK) {
                        backend_message(that->base.refs.form_msg,
                                        err[0] ? err : "Erreur lors de la modification.",
                                        1);
                        return;
                }
                backend_message(that->base.refs.table_msg,
                                "Candidat modifié.", 0);
        } else {
                /* Ajout : ID auto-incremente, date d'inscription = aujourd'hui */
                if (cand.ID_candid[0] == '\0') {
                        core_candidat_prochain_id(cand.ID_candid,
                                                  sizeof(cand.ID_candid));
                }
                core_obtenir_date(cand.Date_ins);
                rc = core_candidat_ajouter(&cand, err, sizeof(err));
                if (rc != EL_OK) {
                        backend_message(that->base.refs.form_msg,
                                        err[0] ? err : "Erreur lors de l'enregistrement.",
                                        1);
                        return;
                }
                backend_message(that->base.refs.table_msg,
                                "Candidat enregistré.", 0);
        }
        cand_form_fermer(e->data);
        cand_form_nouveau(e->data);
        cand_refresh(e->data);
}

/* Clic sur "Modifier" d'une ligne : ouvre le formulaire prerempli. */
static void cand_edit_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        candidats_page_t *that = ui_widget_get_data(e->data,
                                                    candidats_page_proto);
        const char *id = ui_widget_get_attr(w, "data-id");
        candidats cand;
        (void)arg;

        if (that == NULL || id == NULL) {
                return;
        }
        if (core_candidat_obtenir(id, &cand) == 0) {
                backend_message(that->base.refs.table_msg,
                                "Candidat introuvable.", 1);
                return;
        }
        that->mode_edition = 1;
        snprintf(that->cle_edition, sizeof(that->cle_edition), "%s", id);
        cand_confirm_cacher(e->data);
        cand_form_ouvrir(e->data);

        ui_widget_set_text(that->base.refs.form_title,
                           "Modifier le candidat");
        backend_input_readonly(that->base.refs.f_id, 1);
        backend_input_set_text(that->base.refs.f_id, cand.ID_candid);
        backend_input_set_text(that->base.refs.f_nom, cand.Nom);
        backend_input_set_text(that->base.refs.f_prenom, cand.Prenom);
        backend_input_set_text(that->base.refs.f_naissance, cand.Dat_Nais);
        backend_input_set_text(that->base.refs.f_tel, cand.tel);
        backend_input_set_text(that->base.refs.f_email, cand.email);
        backend_input_set_text(that->base.refs.f_adresse, cand.Adresse);
        backend_input_set_text(that->base.refs.f_type, cand.type);
        ui_widget_set_text(that->base.refs.form_msg, "");
}

/* Clic sur "Supprimer" d'une ligne : affiche la barre de confirmation. */
static void cand_del_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        candidats_page_t *that = ui_widget_get_data(e->data,
                                                    candidats_page_proto);
        const char *id = ui_widget_get_attr(w, "data-id");
        char msg[112];
        (void)arg;

        if (that == NULL || id == NULL) {
                return;
        }
        snprintf(that->cle_suppression, sizeof(that->cle_suppression),
                 "%s", id);
        snprintf(msg, sizeof(msg),
                 "Supprimer définitivement le candidat « %s » ?", id);
        ui_widget_set_text(that->base.refs.confirm_msg, msg);
        ui_widget_remove_class(that->base.refs.confirm_panel, "hidden");
        ui_widget_add_class(that->base.refs.confirm_panel, "flex");
}

static void cand_del_confirm(ui_widget_t *w, ui_event_t *e, void *arg)
{
        candidats_page_t *that = ui_widget_get_data(e->data,
                                                    candidats_page_proto);
        char err[160] = "";
        int rc;
        (void)w;
        (void)arg;

        if (that->cle_suppression[0] == '\0') {
                cand_confirm_cacher(e->data);
                return;
        }
        rc = core_candidat_supprimer(that->cle_suppression, err, sizeof(err));
        if (rc != EL_OK) {
                backend_message(that->base.refs.table_msg,
                                err[0] ? err : "Suppression impossible.", 1);
        } else {
                backend_message(that->base.refs.table_msg,
                                "Candidat supprimé.", 0);
        }
        cand_confirm_cacher(e->data);
        cand_form_nouveau(e->data);
        cand_refresh(e->data);
}

static void cand_del_cancel(ui_widget_t *w, ui_event_t *e, void *arg)
{
        (void)w;
        (void)arg;
        cand_confirm_cacher(e->data);
}

/* --- Cycle de vie du composant --- */

static void candidats_page_init(ui_widget_t *w)
{
        candidats_page_t *that;

        ui_widget_add_data(w, candidats_page_proto, sizeof(candidats_page_t));
        candidats_page_react_init(w);
        that = ui_widget_get_data(w, candidats_page_proto);
        that->mode_edition = 0;
        that->cle_edition[0] = '\0';
        that->cle_suppression[0] = '\0';
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
