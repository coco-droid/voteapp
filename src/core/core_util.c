#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include <limits.h>
#include <ctype.h>

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

int core_supprimer_enreg(const char *nom_fichier, size_t taille_enreg,
                         core_predicat correspond, const void *cle) {
    void *tab;
    FILE *f;
    int count = 0, i, index_suppr = -1;

    tab = core_lire_tout(nom_fichier, taille_enreg, &count);
    if (tab == NULL) {
        return 0; /* Fichier vide ou inexistant => introuvable */
    }
    for (i = 0; i < count; i++) {
        if (correspond((const char *)tab + (size_t)i * taille_enreg, cle)) {
            index_suppr = i;
            break;
        }
    }
    if (index_suppr < 0) {
        free(tab);
        return 0;
    }
    f = core_fichier_ouvrir(nom_fichier, "wb");
    if (f == NULL) {
        free(tab);
        return EL_ERR_FICHIER;
    }
    for (i = 0; i < count; i++) {
        if (i == index_suppr) {
            continue;
        }
        if (fwrite((const char *)tab + (size_t)i * taille_enreg,
                   taille_enreg, 1, f) != 1) {
            fclose(f);
            free(tab);
            return EL_ERR_FICHIER;
        }
    }
    fclose(f);
    free(tab);
    return 1;
}

int core_prochain_id(const char *nom_fichier, size_t taille_enreg,
                     size_t offset_id, size_t taille_id,
                     const char *prefixe, int largeur,
                     char *out, size_t out_len) {
    void *tab;
    int count = 0, i;
    long max = 0;
    size_t plen = strlen(prefixe);

    tab = core_lire_tout(nom_fichier, taille_enreg, &count);
    if (tab != NULL) {
        for (i = 0; i < count; i++) {
            const char *champ =
                    (const char *)tab + (size_t)i * taille_enreg + offset_id;
            char id[64];
            const char *p;
            char *fin = NULL;
            long n;

            if (taille_id >= sizeof(id)) {
                continue;
            }
            memcpy(id, champ, taille_id);
            id[taille_id - 1] = '\0'; /* terminaison garantie */
            if (strncasecmp(id, prefixe, plen) != 0) {
                continue;
            }
            p = id + plen;
            if (*p == '\0') {
                continue;
            }
            n = strtol(p, &fin, 10);
            if (fin == p || *fin != '\0' || n < 0) {
                continue; /* suffixe pas purement numerique : ignore */
            }
            if (n > max) {
                max = n;
            }
        }
        free(tab);
    }
    snprintf(out, out_len, "%s%0*ld", prefixe, largeur, max + 1);
    return EL_OK;
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

/* ---------- Validateurs de champs ---------- */

int core_est_nombre(const char *s) {
    if (s == NULL || *s == '\0') {
        return 0;
    }
    for (; *s; s++) {
        if (!isdigit((unsigned char)*s)) {
            return 0;
        }
    }
    return 1;
}

static int annee_bissextile(int a) {
    return (a % 4 == 0 && a % 100 != 0) || (a % 400 == 0);
}

int core_date_valide(const char *s) {
    static const int jours_par_mois[] =
            {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int i, j, m, a, max_j;

    if (s == NULL || strlen(s) != 10) {
        return 0;
    }
    for (i = 0; i < 10; i++) {
        if (i == 2 || i == 5) {
            if (s[i] != '/') {
                return 0;
            }
        } else if (!isdigit((unsigned char)s[i])) {
            return 0;
        }
    }
    j = (s[0] - '0') * 10 + (s[1] - '0');
    m = (s[3] - '0') * 10 + (s[4] - '0');
    a = atoi(s + 6);
    if (a < 1900 || a > 2100 || m < 1 || m > 12) {
        return 0;
    }
    max_j = jours_par_mois[m - 1];
    if (m == 2 && annee_bissextile(a)) {
        max_j = 29;
    }
    return j >= 1 && j <= max_j;
}

int core_date_passee(const char *s) {
    char auj[14];
    int j, m, a, ja, ma, aa;

    if (!core_date_valide(s)) {
        return 0;
    }
    core_obtenir_date(auj);
    j = (s[0] - '0') * 10 + (s[1] - '0');
    m = (s[3] - '0') * 10 + (s[4] - '0');
    a = atoi(s + 6);
    ja = (auj[0] - '0') * 10 + (auj[1] - '0');
    ma = (auj[3] - '0') * 10 + (auj[4] - '0');
    aa = atoi(auj + 6);
    if (a != aa) {
        return a < aa;
    }
    if (m != ma) {
        return m < ma;
    }
    return j < ja;
}

int core_email_valide(const char *s) {
    const char *arobase, *point;

    if (s == NULL || *s == '\0') {
        return 1; /* champ optionnel : vide = valide */
    }
    arobase = strchr(s, '@');
    if (arobase == NULL || arobase == s) {
        return 0;
    }
    point = strrchr(arobase, '.');
    if (point == NULL || point == arobase + 1 || point[1] == '\0') {
        return 0;
    }
    return strchr(s, ' ') == NULL;
}

int core_tel_valide(const char *s) {
    int chiffres = 0;

    if (s == NULL || *s == '\0') {
        return 1; /* champ optionnel : vide = valide */
    }
    for (; *s; s++) {
        if (isdigit((unsigned char)*s)) {
            chiffres++;
        } else if (*s != '+' && *s != '-' && *s != ' ' && *s != '.') {
            return 0;
        }
    }
    return chiffres >= 7 && chiffres <= 14;
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
