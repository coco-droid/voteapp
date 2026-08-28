#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <limits.h>

#include "core_interne.h"

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

/* Dossier contenant les fichiers .dat (par defaut : repertoire courant,
 * ce qui preserve le comportement historique de l'application console) */
static char g_dossier[PATH_MAX] = ".";

void core_set_dossier_donnees(const char *dossier) {
    if (dossier != NULL && dossier[0] != '\0') {
        size_t len = strlen(dossier);
        if (len >= sizeof(g_dossier)) {
            len = sizeof(g_dossier) - 1;
        }
        memcpy(g_dossier, dossier, len);
        g_dossier[len] = '\0';
    } else {
        strcpy(g_dossier, ".");
    }
}

const char *core_dossier_donnees(void) {
    return g_dossier;
}

FILE *core_fichier_ouvrir(const char *nom_fichier, const char *mode) {
    char chemin[PATH_MAX];
    int n = snprintf(chemin, sizeof(chemin), "%s/%s", g_dossier, nom_fichier);
    if (n < 0 || (size_t)n >= sizeof(chemin)) {
        return NULL; /* Chemin trop long */
    }
    return fopen(chemin, mode);
}

void core_erreur(char *err, size_t err_len, const char *fmt, ...) {
    va_list ap;
    if (err == NULL || err_len == 0) {
        return;
    }
    va_start(ap, fmt);
    vsnprintf(err, err_len, fmt, ap);
    va_end(ap);
}

void *core_lire_tout(const char *nom_fichier, size_t taille_enreg, int *count) {
    FILE *f;
    long taille;
    int n, lus;
    void *tab;

    *count = 0;
    f = core_fichier_ouvrir(nom_fichier, "rb");
    if (f == NULL) {
        return NULL; /* Fichier inexistant => liste vide */
    }

    fseek(f, 0, SEEK_END);
    taille = ftell(f);
    rewind(f);

    n = (int)(taille / (long)taille_enreg);
    if (n <= 0) {
        fclose(f);
        return NULL;
    }

    tab = malloc((size_t)n * taille_enreg);
    if (tab == NULL) {
        fclose(f);
        return NULL;
    }

    lus = (int)fread(tab, taille_enreg, (size_t)n, f);
    fclose(f);

    if (lus <= 0) {
        free(tab);
        return NULL;
    }
    *count = lus;
    return tab;
}

int core_ajouter_enreg(const char *nom_fichier, const void *enreg, size_t taille_enreg) {
    FILE *f = core_fichier_ouvrir(nom_fichier, "ab");
    size_t ecrit;

    if (f == NULL) {
        return EL_ERR_FICHIER;
    }
    ecrit = fwrite(enreg, taille_enreg, 1, f);
    fclose(f);
    return (ecrit == 1) ? EL_OK : EL_ERR_FICHIER;
}

int core_remplacer_enreg(const char *nom_fichier, size_t taille_enreg,
                         core_predicat correspond, const void *cle,
                         const void *nouvel) {
    FILE *f = core_fichier_ouvrir(nom_fichier, "r+b");
    void *temp;
    int trouve = 0;

    if (f == NULL) {
        return EL_ERR_FICHIER;
    }
    temp = malloc(taille_enreg);
    if (temp == NULL) {
        fclose(f);
        return EL_ERR_MEMOIRE;
    }

    while (fread(temp, taille_enreg, 1, f) == 1) {
        if (correspond(temp, cle)) {
            fseek(f, -((long)taille_enreg), SEEK_CUR);
            fwrite(nouvel, taille_enreg, 1, f);
            fflush(f);
            trouve = 1;
            break;
        }
    }

    free(temp);
    fclose(f);
    return trouve;
}

void core_obtenir_date(char *date_out) {
    time_t temps = time(NULL);
    struct tm tm_info = *localtime(&temps);
    sprintf(date_out, "%02d/%02d/%04d",
            tm_info.tm_mday, tm_info.tm_mon + 1, tm_info.tm_year + 1900);
}

void core_extraire_departement(const char *adresse, char *dept_out, size_t taille) {
    const char *virgule = strrchr(adresse, ',');
    const char *src;

    if (virgule != NULL) {
        src = virgule + 1;
        while (*src == ' ') src++;
    } else {
        src = adresse;
    }
    snprintf(dept_out, taille, "%s", src);
}
