#include <stdio.h>
#include "election.h"
#define DUMP(f, s, fmt) { FILE *fp=fopen(f,"rb"); if(fp){ s x; int n=0; while(fread(&x,sizeof(x),1,fp)==1){ printf fmt; n++; } printf("  -> %d enregistrements (%s, %zu octets/rec)\n", n, f, sizeof(x)); fclose(fp);} else printf("!! %s introuvable\n", f); }
int main(void){
    DUMP("p_pol.dat", partie_politic, ("[PP] %s | %s | %s | %s\n", x.id_PP, x.Nom, x.date_creat, x.Responsable));
    DUMP("B_votes.dat", bureau_vote, ("[BV] %s | %s | P:%s VP:%s S:%s\n", x.Id_BV, x.Adresse_BV, x.Id_President, x.Id_VPresident, x.Id_Secret));
    DUMP("elect.dat", electeurs, ("[EL] %s | %s %s | BV:%s\n", x.NINU, x.nom, x.prenom, x.BV));
    DUMP("candid.dat", candidats, ("[CA] %s | %s %s | %s\n", x.ID_candid, x.Nom, x.Prenom, x.type));
    DUMP("votes.dat", votes, ("[VO] %s | %s | BV:%s NINU:%s C:%s\n", x.Id_vote, x.Date_vote, x.BV, x.NINU, x.id_candid));
    return 0;
}
