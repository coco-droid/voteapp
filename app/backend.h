#ifndef APP_BACKEND_H
#define APP_BACKEND_H

/*
 * backend.h — Pont entre l'interface LCUI et la couche metier
 * (src/election_core.h). Contient :
 *   - l'initialisation du dossier de donnees (.dat)
 *   - des helpers pour lire/remplir les widgets (textinput, text)
 *   - des constructeurs de lignes de table dynamiques
 */

#include <string.h>
#include <ui.h>
#include "../src/election_core.h"

#ifdef _WIN32
/* strcasestr() est une extension GNU absente de Windows/MinGW :
 * reimplementation portable pour la cross-compilation. */
static inline char *strcasestr(const char *haystack, const char *needle)
{
        size_t n = strlen(needle);

        if (n == 0)
                return (char *)haystack;
        for (; *haystack; haystack++) {
                if (strncasecmp(haystack, needle, n) == 0)
                        return (char *)haystack;
        }
        return NULL;
}
#endif

/* Pointe le core vers le dossier contenant les .dat.
 * Ordre de recherche : "data" (a cote de l'exe), puis "../src" (dev),
 * puis le repertoire courant. */
void backend_init(void);

/* Lit le texte UTF-8 d'un widget textinput dans out (tronque si besoin).
 * Les espaces de debut/fin sont supprimes. */
void backend_input_text(ui_widget_t *input, char *out, size_t len);

/* Remplit un widget textinput avec un texte UTF-8 (mode edition). */
void backend_input_set_text(ui_widget_t *input, const char *txt);

/* Passe un champ textinput en lecture seule (ou l'inverse) via
 * pointer-events:none + opacite reduite. NB : on n'utilise PAS
 * ui_widget_set_disabled() : la transition d'etat "disabled" fait perdre
 * le texte affecte au textinput dans LCUI. */
void backend_input_readonly(ui_widget_t *input, int lecture_seule);

/* Cree un petit bouton d'action de ligne de table ("Modifier"/"Supprimer").
 * danger != 0 => texte rouge (suppression). */
ui_widget_t *backend_action_btn(const char *txt, int danger);

/* Affiche un message dans un widget text, colore selon is_error. */
void backend_message(ui_widget_t *w, const char *msg, int is_error);

/* Cree un widget texte avec classe(s) et contenu. */
ui_widget_t *backend_text(const char *class_name, const char *txt);

/* Cree une cellule de table (div > text) de largeur classe_w. */
ui_widget_t *backend_cell(const char *classe_w, const char *txt,
                          const char *classe_txt);

/* Cree une ligne de table (div flex-row border-b) vide, a remplir
 * avec backend_cell()/backend_badge(). */
ui_widget_t *backend_row(void);

/* Cree un badge colore (texte dans un span arrondi).
 * kind : "ok" (vert), "no" (gris), "blanc" (jaune), "actif" (vert statut). */
ui_widget_t *backend_badge(const char *txt, const char *kind);

/* Extrait le departement (3e composante) d'une adresse
 * "No Rue, Rue, Departement" — reutilise le core. */
void backend_departement(const char *adresse, char *out, size_t len);

#endif /* APP_BACKEND_H */
