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
        int mode_edition;           /* 1 = modification d'un bureau         */
        char cle_edition[15];       /* Id_BV en cours de modification        */
        char cle_suppression[15];   /* Id_BV en attente de confirmation      */
} bureaux_page_t;

static void bv_edit_click(ui_widget_t *w, ui_event_t *e, void *arg);
static void bv_del_click(ui_widget_t *w, ui_event_t *e, void *arg);

/* Remet le formulaire en mode "nouveau bureau" (ID auto-incremente). */
static void bv_form_nouveau(ui_widget_t *w)
{
        bureaux_page_t *that = ui_widget_get_data(w, bureaux_page_proto);
        char id[15];

        that->mode_edition = 0;
        that->cle_edition[0] = '\0';
        ui_widget_set_text(that->base.refs.form_title,
                           "Nouveau bureau de vote");
        core_bv_prochain_id(id, sizeof(id));
        backend_input_readonly(that->base.refs.f_id, 1);
        backend_input_set_text(that->base.refs.f_id, id);

        ui_textinput_clear_text(that->base.refs.f_adresse);
        ui_textinput_clear_text(that->base.refs.f_president);
        ui_textinput_clear_text(that->base.refs.f_vpresident);
        ui_textinput_clear_text(that->base.refs.f_secretaire);
        ui_widget_set_text(that->base.refs.form_msg, "");
}

static void bv_form_ouvrir(ui_widget_t *w)
{
        bureaux_page_react_t *_that =
                ui_widget_get_data(w, bureaux_page_proto);
        ui_widget_remove_class(_that->refs.form_panel, "hidden");
        ui_widget_add_class(_that->refs.form_panel, "flex");
}

static void bv_form_fermer(ui_widget_t *w)
{
        bureaux_page_react_t *_that =
                ui_widget_get_data(w, bureaux_page_proto);
        ui_widget_remove_class(_that->refs.form_panel, "flex");
        ui_widget_add_class(_that->refs.form_panel, "hidden");
}

static void bv_confirm_cacher(ui_widget_t *w)
{
        bureaux_page_t *that = ui_widget_get_data(w, bureaux_page_proto);
        ui_widget_remove_class(that->base.refs.confirm_panel, "flex");
        ui_widget_add_class(that->base.refs.confirm_panel, "hidden");
        that->cle_suppression[0] = '\0';
}

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
                        ui_widget_t *cell;
                        ui_widget_t *btn;

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

                        /* Colonne Actions : Modifier / Supprimer */
                        cell = ui_create_widget(NULL);
                        ui_widget_add_class(cell,
                                "w-36 flex flex-row justify-center");
                        btn = backend_action_btn("Modifier", 0);
                        ui_widget_set_attr(btn, "data-id", liste[i].Id_BV);
                        ui_widget_on(btn, "click", bv_edit_click, w);
                        ui_widget_append(cell, btn);
                        btn = backend_action_btn("Supprimer", 1);
                        ui_widget_set_attr(btn, "data-id", liste[i].Id_BV);
                        ui_widget_on(btn, "click", bv_del_click, w);
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
        bureaux_page_t *that = ui_widget_get_data(e->data, bureaux_page_proto);
        (void)w;
        (void)arg;
        if (ui_widget_has_class(that->base.refs.form_panel, "hidden")) {
                /* Ouvrir AVANT de remplir (cf. partis) */
                bv_form_ouvrir(e->data);
                bv_form_nouveau(e->data);
        } else {
                bv_form_fermer(e->data);
                bv_form_nouveau(e->data);
        }
}

static void bv_save_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        bureaux_page_t *that = ui_widget_get_data(e->data, bureaux_page_proto);
        bureau_vote bv;
        char err[160] = "";
        int rc;
        (void)w;
        (void)arg;

        memset(&bv, 0, sizeof(bv));
        backend_input_text(that->base.refs.f_id, bv.Id_BV, sizeof(bv.Id_BV));
        backend_input_text(that->base.refs.f_adresse, bv.Adresse_BV,
                           sizeof(bv.Adresse_BV));
        backend_input_text(that->base.refs.f_president, bv.Id_President,
                           sizeof(bv.Id_President));
        backend_input_text(that->base.refs.f_vpresident, bv.Id_VPresident,
                           sizeof(bv.Id_VPresident));
        backend_input_text(that->base.refs.f_secretaire, bv.Id_Secret,
                           sizeof(bv.Id_Secret));

        if (that->mode_edition) {
                /* L'identifiant n'est pas modifiable : on reprend la cle */
                snprintf(bv.Id_BV, sizeof(bv.Id_BV), "%s", that->cle_edition);
                rc = core_bv_modifier(&bv, err, sizeof(err));
                if (rc != EL_OK) {
                        backend_message(that->base.refs.form_msg,
                                        err[0] ? err : "Erreur lors de la modification.",
                                        1);
                        return;
                }
                backend_message(that->base.refs.table_msg,
                                "Bureau de vote modifié.", 0);
        } else {
                /* Ajout : ID auto-incremente */
                if (bv.Id_BV[0] == '\0') {
                        core_bv_prochain_id(bv.Id_BV, sizeof(bv.Id_BV));
                }
                rc = core_bv_ajouter(&bv, err, sizeof(err));
                if (rc != EL_OK) {
                        backend_message(that->base.refs.form_msg,
                                        err[0] ? err : "Erreur lors de l'enregistrement.",
                                        1);
                        return;
                }
                backend_message(that->base.refs.table_msg,
                                "Bureau de vote enregistré.", 0);
        }
        bv_form_fermer(e->data);
        bv_form_nouveau(e->data);
        bv_refresh(e->data);
}

/* Clic sur "Modifier" d'une ligne : ouvre le formulaire prerempli. */
static void bv_edit_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        bureaux_page_t *that = ui_widget_get_data(e->data, bureaux_page_proto);
        const char *id = ui_widget_get_attr(w, "data-id");
        bureau_vote bv;
        (void)arg;

        if (that == NULL || id == NULL) {
                return;
        }
        if (core_bv_obtenir(id, &bv) == 0) {
                backend_message(that->base.refs.table_msg,
                                "Bureau de vote introuvable.", 1);
                return;
        }
        that->mode_edition = 1;
        snprintf(that->cle_edition, sizeof(that->cle_edition), "%s", id);
        bv_confirm_cacher(e->data);
        bv_form_ouvrir(e->data);

        ui_widget_set_text(that->base.refs.form_title,
                           "Modifier le bureau de vote");
        backend_input_readonly(that->base.refs.f_id, 1);
        backend_input_set_text(that->base.refs.f_id, bv.Id_BV);
        backend_input_set_text(that->base.refs.f_adresse, bv.Adresse_BV);
        backend_input_set_text(that->base.refs.f_president, bv.Id_President);
        backend_input_set_text(that->base.refs.f_vpresident, bv.Id_VPresident);
        backend_input_set_text(that->base.refs.f_secretaire, bv.Id_Secret);
        ui_widget_set_text(that->base.refs.form_msg, "");
}

/* Clic sur "Supprimer" d'une ligne : affiche la barre de confirmation. */
static void bv_del_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        bureaux_page_t *that = ui_widget_get_data(e->data, bureaux_page_proto);
        const char *id = ui_widget_get_attr(w, "data-id");
        char msg[112];
        (void)arg;

        if (that == NULL || id == NULL) {
                return;
        }
        snprintf(that->cle_suppression, sizeof(that->cle_suppression),
                 "%s", id);
        snprintf(msg, sizeof(msg),
                 "Supprimer définitivement le bureau « %s » ?", id);
        ui_widget_set_text(that->base.refs.confirm_msg, msg);
        ui_widget_remove_class(that->base.refs.confirm_panel, "hidden");
        ui_widget_add_class(that->base.refs.confirm_panel, "flex");
}

static void bv_del_confirm(ui_widget_t *w, ui_event_t *e, void *arg)
{
        bureaux_page_t *that = ui_widget_get_data(e->data, bureaux_page_proto);
        char err[160] = "";
        int rc;
        (void)w;
        (void)arg;

        if (that->cle_suppression[0] == '\0') {
                bv_confirm_cacher(e->data);
                return;
        }
        rc = core_bv_supprimer(that->cle_suppression, err, sizeof(err));
        if (rc != EL_OK) {
                backend_message(that->base.refs.table_msg,
                                err[0] ? err : "Suppression impossible.", 1);
        } else {
                backend_message(that->base.refs.table_msg,
                                "Bureau de vote supprimé.", 0);
        }
        bv_confirm_cacher(e->data);
        bv_form_nouveau(e->data);
        bv_refresh(e->data);
}

static void bv_del_cancel(ui_widget_t *w, ui_event_t *e, void *arg)
{
        (void)w;
        (void)arg;
        bv_confirm_cacher(e->data);
}

/* --- Cycle de vie du composant --- */

static void bureaux_page_init(ui_widget_t *w)
{
        bureaux_page_t *that;

        ui_widget_add_data(w, bureaux_page_proto, sizeof(bureaux_page_t));
        bureaux_page_react_init(w);
        that = ui_widget_get_data(w, bureaux_page_proto);
        that->mode_edition = 0;
        that->cle_edition[0] = '\0';
        that->cle_suppression[0] = '\0';
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
