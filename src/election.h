#ifndef ELECTION_H
#define ELECTION_H

/*
 * election.h — Structures de donnees (format binaire des .dat) et
 * points d'entree de l'interface console.
 *
 * IMPORTANT : ne pas modifier les structures sans migrer les fichiers
 * .dat existants (elles sont ecrites/lues telles quelles en binaire).
 *
 * La logique metier reutilisable (console + GUI) se trouve dans
 * election_core.h.
 */

#include <stdio.h>
#include <stddef.h>

// --- STRUCTURES COMMUNES ---
typedef struct {
    char Id_BV[15];
    char Id_President[30];
    char Id_VPresident[30];
    char Id_Secret[30];
    char Adresse_BV[100];
} bureau_vote;

typedef struct {
    char NINU[15];
    char nom[25];
    char prenom[25];
    char Adresse[100];
    char BV[15];
} electeurs;

typedef struct {
    char id_PP[10];
    char Nom[20];
    char date_creat[14];
    char date_ins[14];
    char Adresse[70];
    char Responsable[20];
    char Telephone[13];
    char email[40];
} partie_politic;

typedef struct{
    char ID_candid[10];
    char Nom[20];
    char Prenom[20];
    char Dat_Nais[14];
    char Adresse[70];
    char email[40];
    char tel[13];
    char type[20];
    char Date_ins[14];
}candidats;

typedef struct{
    char Id_vote[10];
    char Date_vote[14];
    char BV[10];
    char NINU[15];
    char id_candid[10];
}votes;

//---- POINTS D'ENTREE DES MENUS CONSOLE ----
int ge_BV(void);
int p_pol(void);
int ge_elect(void);
int ge_candi(void);
int ge_vote(void);
int ge_rapp(void);

//---- HELPERS CONSOLE (buffer.c) ----
void vider_buffer(void);
void lire_chaine(char *chaine, int taille);
/* Saisie guidee d'une adresse "No Rue, Rue, Departement" assemblee dans dest */
void saisir_adresse(const char *libelle, char *dest, size_t taille);

#endif
