#ifndef CORE_INTERNE_H
#define CORE_INTERNE_H

/*
 * core_interne.h — Helpers internes partages par les modules du dossier core/
 * Ne pas inclure depuis la couche UI (console ou GUI).
 */

#include "../election_core.h"

/* Noms des fichiers de donnees (dans le dossier de donnees) */
#define FICHIER_PARTIS    "p_pol.dat"
#define FICHIER_BUREAUX   "B_votes.dat"
#define FICHIER_ELECTEURS "elect.dat"
#define FICHIER_CANDIDATS "candid.dat"
#define FICHIER_VOTES     "votes.dat"

/* Ouvre un fichier .dat dans le dossier de donnees configure */
FILE *core_fichier_ouvrir(const char *nom_fichier, const char *mode);

/* Ecrit un message formate dans le tampon d'erreur (tolere err == NULL) */
void core_erreur(char *err, size_t err_len, const char *fmt, ...);

/* Lit tout un fichier .dat en memoire.
 * Retourne un tableau alloue par malloc (NULL si vide/inexistant/erreur)
 * et place le nombre d'enregistrements lus dans *count. */
void *core_lire_tout(const char *nom_fichier, size_t taille_enreg, int *count);

/* Ajoute un enregistrement a la fin du fichier (mode "ab") */
int core_ajouter_enreg(const char *nom_fichier, const void *enreg, size_t taille_enreg);

/* Predicat de correspondance pour core_remplacer_enreg */
typedef int (*core_predicat)(const void *enreg, const void *cle);

/* Remplace en place le premier enregistrement correspondant au predicat.
 * Retourne 1 si trouve+remplace, 0 si introuvable, EL_ERR_* sinon. */
int core_remplacer_enreg(const char *nom_fichier, size_t taille_enreg,
                         core_predicat correspond, const void *cle,
                         const void *nouvel);

/* Supprime le premier enregistrement correspondant au predicat (reecriture
 * complete du fichier sans l'enregistrement).
 * Retourne 1 si supprime, 0 si introuvable, EL_ERR_* sinon. */
int core_supprimer_enreg(const char *nom_fichier, size_t taille_enreg,
                         core_predicat correspond, const void *cle);

/* Calcule l'identifiant auto-incremente suivant : plus grand suffixe
 * numerique + 1 parmi les ids commencant par `prefixe` (insensible a la
 * casse), zero-complete a `largeur` chiffres. `offset_id` est la position
 * du champ id dans l'enregistrement, `taille_id` sa taille en octets. */
int core_prochain_id(const char *nom_fichier, size_t taille_enreg,
                     size_t offset_id, size_t taille_id,
                     const char *prefixe, int largeur,
                     char *out, size_t out_len);

#endif /* CORE_INTERNE_H */
