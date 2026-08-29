#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LCUI/widgets.h>

#include "page.tsx.h"
#include "page.h"
#include "../backend.h"

typedef struct {
        vote_page_react_t base;
        int ninu_verifie;             /* 1 si le NINU saisi peut voter   */
        char ninu[15];                /* NINU verifie                    */
        char id_candid_sel[10];       /* candidat selectionne ("0"=blanc) */
        ui_widget_t *ligne_sel;       /* ligne visuellement selectionnee */
} vote_page_t;

static const char *CLASSE_LIGNE =
        "flex flex-row items-center justify-between w-full "
        "border border-outline-variant rounded-lg px-4 py-3 mb-2";
static const char *CLASSE_LIGNE_SEL =
        "flex flex-row items-center justify-between w-full "
        "border-primary border-2 rounded-lg px-4 py-3 mb-2 bg-primary-fixed";

/* Clic sur une ligne candidat (lie a chaque ligne a sa creation).
 * ATTENTION : le 3e parametre `arg` du handler est NULL pour les evenements
 * d'interface (dispatch par ui_widget_emit_event(..., NULL)) — la page est
 * transmise via le `data` de ui_widget_on(), lue ici dans e->data. */
static void candidat_click(ui_widget_t *w, ui_event_t *e, void *arg);

/* (Re)construit la liste cliquable des candidats + option vote blanc. */
static void vote_liste_candidats(ui_widget_t *w)
{
        vote_page_t *that = ui_widget_get_data(w, vote_page_proto);
        candidats *liste = NULL;
        int count = 0, i;

        ui_widget_empty(that->base.refs.candidats);
        that->ligne_sel = NULL;
        that->id_candid_sel[0] = '\0';

        if (core_candidat_liste(&liste, &count) == 0 && liste != NULL) {
                for (i = 0; i < count; i++) {
                        ui_widget_t *ligne = ui_create_widget(NULL);
                        char libelle[64];

                        snprintf(libelle, sizeof(libelle), "%s %s",
                                 liste[i].Prenom, liste[i].Nom);
                        ui_widget_add_class(ligne, CLASSE_LIGNE);
                        ui_widget_set_attr(ligne, "data-candid",
                                           liste[i].ID_candid);
                        ui_widget_append(ligne,
                                backend_text("text-body-md text-on-surface",
                                             libelle));
                        ui_widget_append(ligne,
                                backend_text("text-label-md text-secondary",
                                             liste[i].ID_candid));
                        ui_widget_on(ligne, "click", candidat_click, w);
                        ui_widget_append(that->base.refs.candidats, ligne);
                }
                free(liste);
        }
        /* Option vote blanc, toujours presente */
        {
                ui_widget_t *ligne = ui_create_widget(NULL);
                ui_widget_add_class(ligne, CLASSE_LIGNE);
                ui_widget_set_attr(ligne, "data-candid", "0");
                ui_widget_append(ligne,
                        backend_text("text-body-md font-medium text-secondary",
                                     "Vote Blanc / Nul"));
                ui_widget_append(ligne,
                        backend_text("text-label-md text-secondary", "—"));
                ui_widget_on(ligne, "click", candidat_click, w);
                ui_widget_append(that->base.refs.candidats, ligne);
        }
}

/* --- Handlers (references dans page.tsx) --- */

static void vote_verifier_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        vote_page_t *that = ui_widget_get_data(e->data, vote_page_proto);
        electeurs elec;
        char ninu[15];
        (void)w;
        (void)arg;

        backend_input_text(that->base.refs.ninu, ninu, sizeof(ninu));
        that->ninu_verifie = 0;
        if (ninu[0] == '\0') {
                backend_message(that->base.refs.verif_msg,
                                "Veuillez saisir un NINU.", 1);
                return;
        }
        if (!core_electeur_obtenir(ninu, &elec)) {
                backend_message(that->base.refs.verif_msg,
                                "NINU introuvable dans le registre.", 1);
                ui_widget_set_text(that->base.refs.electeur_nom, "—");
                ui_widget_set_text(that->base.refs.electeur_bv, "—");
                return;
        }
        if (core_ninu_a_deja_vote(ninu)) {
                backend_message(that->base.refs.verif_msg,
                                "Cet électeur a déjà voté.", 1);
                ui_widget_set_text(that->base.refs.electeur_nom, elec.nom);
                ui_widget_set_text(that->base.refs.electeur_bv, elec.BV);
                return;
        }
        that->ninu_verifie = 1;
        strcpy(that->ninu, ninu);
        backend_message(that->base.refs.verif_msg,
                        "Électeur vérifié — peut voter.", 0);
        {
                char nom_complet[56];
                snprintf(nom_complet, sizeof(nom_complet), "%s, %s",
                         elec.nom, elec.prenom);
                ui_widget_set_text(that->base.refs.electeur_nom, nom_complet);
        }
        ui_widget_set_text(that->base.refs.electeur_bv, elec.BV);
}

static void vote_annuler_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        vote_page_t *that = ui_widget_get_data(e->data, vote_page_proto);
        (void)w;
        (void)arg;

        that->ninu_verifie = 0;
        that->ninu[0] = '\0';
        that->id_candid_sel[0] = '\0';
        that->ligne_sel = NULL;
        ui_textinput_clear_text(that->base.refs.ninu);
        ui_widget_set_text(that->base.refs.electeur_nom, "—");
        ui_widget_set_text(that->base.refs.electeur_bv, "—");
        ui_widget_set_text(that->base.refs.verif_msg, "");
        ui_widget_set_text(that->base.refs.vote_msg, "");
        vote_liste_candidats(e->data);
}

static void vote_valider_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        vote_page_t *that = ui_widget_get_data(e->data, vote_page_proto);
        votes v;
        char err[128] = "";
        char msg[160];
        int rc;
        (void)w;
        (void)arg;

        if (!that->ninu_verifie) {
                backend_message(that->base.refs.vote_msg,
                                "Vérifiez d'abord le NINU de l'électeur.", 1);
                return;
        }
        if (that->id_candid_sel[0] == '\0') {
                backend_message(that->base.refs.vote_msg,
                                "Sélectionnez un candidat (ou vote blanc).",
                                1);
                return;
        }
        memset(&v, 0, sizeof(v));
        strcpy(v.NINU, that->ninu);
        strcpy(v.id_candid, that->id_candid_sel);
        /* Id_vote (auto-increment VO-XX), Date_vote (aujourd'hui) et BV
         * (depuis le NINU) sont remplis automatiquement par le core */

        rc = core_vote_ajouter(&v, err, sizeof(err));
        if (rc != EL_OK) {
                backend_message(that->base.refs.vote_msg,
                                err[0] ? err : "Erreur lors du vote.", 1);
                return;
        }
        snprintf(msg, sizeof(msg),
                 "Vote enregistré au bureau %s. Merci !", v.BV);
        backend_message(that->base.refs.vote_msg, msg, 0);
        that->ninu_verifie = 0;
        that->ninu[0] = '\0';
        that->id_candid_sel[0] = '\0';
        that->ligne_sel = NULL;
        ui_textinput_clear_text(that->base.refs.ninu);
        ui_widget_set_text(that->base.refs.electeur_nom, "—");
        ui_widget_set_text(that->base.refs.electeur_bv, "—");
        ui_widget_set_text(that->base.refs.verif_msg, "");
        vote_liste_candidats(e->data);
}

/* Clic sur une ligne candidat (lie dynamiquement a l'init) */
static void candidat_click(ui_widget_t *w, ui_event_t *e, void *arg)
{
        vote_page_t *that = ui_widget_get_data(e->data, vote_page_proto);
        const char *id = ui_widget_get_attr(w, "data-candid");
        (void)arg;

        if (that == NULL || id == NULL) {
                return;
        }
        if (that->ligne_sel != NULL) {
                ui_widget_remove_class(that->ligne_sel, CLASSE_LIGNE_SEL);
                ui_widget_add_class(that->ligne_sel, CLASSE_LIGNE);
        }
        ui_widget_remove_class(w, CLASSE_LIGNE);
        ui_widget_add_class(w, CLASSE_LIGNE_SEL);
        that->ligne_sel = w;
        snprintf(that->id_candid_sel, sizeof(that->id_candid_sel), "%s", id);
}

/* --- Cycle de vie du composant --- */

static void vote_page_init(ui_widget_t *w)
{
        vote_page_t *that;

        ui_widget_add_data(w, vote_page_proto, sizeof(vote_page_t));
        vote_page_react_init(w);
        that = ui_widget_get_data(w, vote_page_proto);
        that->ninu_verifie = 0;
        that->ninu[0] = '\0';
        that->id_candid_sel[0] = '\0';
        that->ligne_sel = NULL;
        vote_liste_candidats(w);
}

static void vote_page_destroy(ui_widget_t *w)
{
        vote_page_react_destroy(w);
}

void vote_page_update(ui_widget_t *w)
{
        vote_page_react_update(w);
}

ui_widget_t *ui_create_vote_page(void)
{
        return ui_create_widget_with_prototype(vote_page_proto);
}

void ui_register_vote_page(void)
{
        vote_page_init_prototype();
        vote_page_proto->init = vote_page_init;
        vote_page_proto->destroy = vote_page_destroy;
}
