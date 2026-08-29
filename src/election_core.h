#ifndef ELECTION_CORE_H
#define ELECTION_CORE_H

/*
 * election_core.h — Couche logique pure (aucune entree/sortie console).
 *
 * Cette API peut etre appelee depuis :
 *   - l'application console (wrappers ge_*.c)
 *   - une interface graphique (LCUI, GTK, ...)
 *
 * Convention : les fonctions core_*_ajouter / core_*_modifier retournent
 * EL_OK en cas de succes, sinon un code d'erreur EL_ERR_* et remplissent
 * le tampon `err` (s'il n'est pas NULL) avec un message en francais.
 */

#include <stddef.h>
#include <stdio.h>
#include "election.h"

#ifdef _WIN32
#include <string.h>
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#else
#include <strings.h>
#endif

/* ---------- Codes de retour ---------- */
enum {
    EL_OK = 0,              /* Succes                                  */
    EL_ERR_FICHIER = -1,    /* Ouverture/lecture/ecriture .dat echouee */
    EL_ERR_MEMOIRE = -2,    /* Allocation memoire echouee              */
    EL_ERR_DOUBLON = -3,    /* Identifiant deja utilise                */
    EL_ERR_INTROUVABLE = -4,/* Entite referencee introuvable           */
    EL_ERR_DEJA_VOTE = -5,  /* L'electeur a deja vote                  */
    EL_ERR_CONFLIT = -6,    /* Meme personne sur plusieurs postes      */
    EL_ERR_INVALIDE = -7    /* Donnee invalide (champ vide, ...)       */
};

/* ---------- Configuration ---------- */
/* Dossier contenant les fichiers .dat (defaut : "." = repertoire courant).
 * A appeler au demarrage du GUI pour pointer vers les donnees. */
void core_set_dossier_donnees(const char *dossier);
const char *core_dossier_donnees(void);

/* ---------- Utilitaires ---------- */
/* Date du jour au format JJ/MM/AAAA (date_out doit faire >= 14 octets) */
void core_obtenir_date(char *date_out);
/* Extrait le departement (texte apres la derniere virgule de l'adresse) */
void core_extraire_departement(const char *adresse, char *dept_out, size_t taille);

/* ---------- Validateurs de champs (1 = valide, 0 = invalide) ---------- */
int core_est_nombre(const char *s);     /* non vide, chiffres uniquement  */
int core_date_valide(const char *s);    /* format JJ/MM/AAAA, date reelle */
int core_date_passee(const char *s);    /* date valide et avant aujourd'hui */
int core_email_valide(const char *s);   /* vide tolere, sinon x@y.z       */
int core_tel_valide(const char *s);     /* vide tolere, sinon 7 a 14 chiffres (+ - . espace) */

/* ---------- Identifiants auto-incrementes ---------- */
/* Scannent le fichier correspondant et ecrivent l'identifiant suivant
 * (plus grand suffixe numerique + 1). Formats : PP-01, BV-01, CA-001, VO-01. */
int core_parti_prochain_id(char *out, size_t len);
int core_bv_prochain_id(char *out, size_t len);
int core_candidat_prochain_id(char *out, size_t len);
int core_vote_prochain_id(char *out, size_t len);

/* ---------- Verifications (1 = existe, 0 = non) ---------- */
int core_existe_NINU(const char *ninu, char *bv_out /* optionnel, >= 15 octets */);
int core_existe_BV(const char *id_bv);
int core_existe_parti(const char *id_parti);
int core_existe_membre_bv(const char *id_personne); /* president / VP / secretaire */
int core_existe_candidat(const char *id_candid);
int core_existe_vote(const char *id_vote);
int core_ninu_a_deja_vote(const char *ninu);

/* ---------- Partis politiques ---------- */
int core_parti_ajouter(const partie_politic *p, char *err, size_t err_len);
int core_parti_modifier(const partie_politic *p, char *err, size_t err_len);
int core_parti_supprimer(const char *id, char *err, size_t err_len);
int core_parti_obtenir(const char *id, partie_politic *out); /* 1 = trouve */
/* Liste complete : *out est alloue par malloc, a liberer avec free() */
int core_parti_liste(partie_politic **out, int *count);

/* ---------- Bureaux de vote ---------- */
int core_bv_ajouter(const bureau_vote *bv, char *err, size_t err_len);
int core_bv_modifier(const bureau_vote *bv, char *err, size_t err_len);
int core_bv_supprimer(const char *id, char *err, size_t err_len);
int core_bv_obtenir(const char *id, bureau_vote *out);
int core_bv_liste(bureau_vote **out, int *count);

/* ---------- Electeurs ---------- */
int core_electeur_ajouter(const electeurs *e, char *err, size_t err_len);
int core_electeur_modifier(const electeurs *e, char *err, size_t err_len);
int core_electeur_supprimer(const char *ninu, char *err, size_t err_len);
int core_electeur_obtenir(const char *ninu, electeurs *out);
int core_electeur_liste(electeurs **out, int *count);

/* ---------- Candidats ---------- */
int core_candidat_ajouter(const candidats *c, char *err, size_t err_len);
int core_candidat_modifier(const candidats *c, char *err, size_t err_len);
int core_candidat_supprimer(const char *id, char *err, size_t err_len);
int core_candidat_obtenir(const char *id, candidats *out);
int core_candidat_liste(candidats **out, int *count);

/* ---------- Votes ---------- */
/* Remplit automatiquement Date_vote (date du jour) et BV (depuis le NINU).
 * Si Id_vote est vide, il est genere automatiquement (VO-XX).
 * id_candid = "0" => vote blanc.
 * Les votes ne sont ni modifiables ni supprimables (integrite du scrutin). */
int core_vote_ajouter(votes *v, char *err, size_t err_len);
int core_vote_liste(votes **out, int *count);

/* ---------- Rapports / resultats ---------- */
typedef struct {
    char id_candid[15];
    char nom[20];
    char prenom[20];
    int total_votes;
    float pourcentage;
} core_stat_candidat;

typedef struct {
    char departement[30];
    char id_bv[15];
    int votes_blancs;
    int votes_non_blancs;
    int nb_stats;
    core_stat_candidat *stats; /* tableau trie par pourcentage decroissant */
} core_resultat_bv;

typedef struct {
    core_resultat_bv *bureaux; /* resultats par bureau de vote */
    int nb_bureaux;
    core_stat_candidat *national; /* resume national, trie */
    int nb_candidats;
    int total_blancs;
    int total_non_blancs;
    int total_general;
} core_resultats;

/* Calcule tous les resultats (par BV + national). Appeler
 * core_resultats_liberer() apres usage. */
int core_resultats_calculer(core_resultats *res);
void core_resultats_liberer(core_resultats *res);

#endif /* ELECTION_CORE_H */
