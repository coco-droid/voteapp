#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <LCUI/widgets.h>

#include "backend.h"

static int fichier_existe(const char *chemin)
{
        FILE *f = fopen(chemin, "rb");
        if (f) {
                fclose(f);
                return 1;
        }
        return 0;
}

void backend_init(void)
{
        /* rundir = dist/ : "../src" pointe vers les donnees du projet */
        if (fichier_existe("data/p_pol.dat")) {
                core_set_dossier_donnees("data");
        } else if (fichier_existe("../src/p_pol.dat")) {
                core_set_dossier_donnees("../src");
        } else {
                core_set_dossier_donnees(".");
        }
}

void backend_input_text(ui_widget_t *input, char *out, size_t len)
{
        size_t n, conv;
        wchar_t *wcs;
        char *debut, *fin;

        if (len == 0) {
                return;
        }
        out[0] = '\0';
        if (input == NULL) {
                return;
        }
        n = ui_textinput_get_text_length(input);
        wcs = malloc(sizeof(wchar_t) * (n + 4));
        if (wcs == NULL) {
                return;
        }
        ui_textinput_get_text_w(input, 0, n + 1, wcs);
        conv = wcstombs(out, wcs, len - 1);
        free(wcs);
        if (conv == (size_t)-1) {
                out[0] = '\0';
                return;
        }
        out[len - 1] = '\0';
        /* trim debut */
        debut = out;
        while (*debut == ' ' || *debut == '\t' || *debut == '\n') {
                debut++;
        }
        if (debut != out) {
                memmove(out, debut, strlen(debut) + 1);
        }
        /* trim fin */
        fin = out + strlen(out);
        while (fin > out && (fin[-1] == ' ' || fin[-1] == '\t' ||
                             fin[-1] == '\n' || fin[-1] == '\r')) {
                *--fin = '\0';
        }
}

void backend_input_set_text(ui_widget_t *input, const char *txt)
{
        if (input == NULL) {
                return;
        }
        ui_textinput_set_text(input, txt != NULL ? txt : "");
}

void backend_input_readonly(ui_widget_t *input, int lecture_seule)
{
        if (input == NULL) {
                return;
        }
        ui_widget_set_style_string(input, "pointer-events",
                                   lecture_seule ? "none" : "auto");
        ui_widget_set_style_string(input, "opacity",
                                   lecture_seule ? "0.55" : "1");
}

ui_widget_t *backend_action_btn(const char *txt, int danger)
{
        ui_widget_t *btn = ui_create_widget("button");
        ui_widget_t *t;

        ui_widget_add_class(btn, "border border-outline-variant rounded "
                                 "px-2 py-1 mr-1");
        t = backend_text(danger ? "text-label-sm text-error font-medium"
                                : "text-label-sm text-primary font-medium",
                         txt);
        ui_widget_append(btn, t);
        return btn;
}

void backend_message(ui_widget_t *w, const char *msg, int is_error)
{
        if (w == NULL) {
                return;
        }
        ui_widget_remove_class(w, "text-error");
        ui_widget_remove_class(w, "text-status-active-text");
        ui_widget_add_class(w, is_error ? "text-error"
                                        : "text-status-active-text");
        ui_widget_set_text(w, msg);
}

ui_widget_t *backend_text(const char *class_name, const char *txt)
{
        ui_widget_t *t = ui_create_widget("text");
        if (class_name != NULL) {
                ui_widget_add_class(t, class_name);
        }
        if (txt != NULL) {
                ui_widget_set_text(t, txt);
        }
        return t;
}

ui_widget_t *backend_cell(const char *classe_w, const char *txt,
                          const char *classe_txt)
{
        ui_widget_t *cell = ui_create_widget(NULL);
        ui_widget_t *t;
        if (classe_w != NULL) {
                ui_widget_add_class(cell, classe_w);
        }
        t = backend_text(classe_txt != NULL ? classe_txt
                                            : "text-body-md text-on-surface",
                         txt);
        ui_widget_append(cell, t);
        return cell;
}

ui_widget_t *backend_row(void)
{
        ui_widget_t *row = ui_create_widget(NULL);
        ui_widget_add_class(row, "flex flex-row items-center w-full "
                                 "border-b border-outline-variant px-4 py-3");
        return row;
}

ui_widget_t *backend_badge(const char *txt, const char *kind)
{
        ui_widget_t *badge = ui_create_widget(NULL);
        const char *couleurs = "bg-vote-no-bg text-vote-no-text";

        if (strcmp(kind, "ok") == 0) {
                couleurs = "bg-vote-ok-bg text-vote-ok-text";
        } else if (strcmp(kind, "blanc") == 0) {
                couleurs = "bg-vote-blanc-bg text-vote-blanc-text";
        } else if (strcmp(kind, "actif") == 0) {
                couleurs = "bg-status-active-bg text-status-active-text";
        }
        ui_widget_add_class(badge, "rounded-full px-3 py-1");
        ui_widget_add_class(badge, couleurs);
        ui_widget_append(badge, backend_text("text-label-sm font-medium", txt));
        return badge;
}

void backend_departement(const char *adresse, char *out, size_t len)
{
        core_extraire_departement(adresse, out, len);
}
