#include <stdio.h>
#include <stdlib.h>
#include "election.h"

//======================menu principal==========================
void menu_general(){
     int choix = 0;
    printf("\n=============================================================GESTION DES ELECTIONS DE 2026 EN HAITI ====================================================================\n\n");
    do{
        printf("\n=============================\n\n");
        printf("  1. PARTIES POLITIQUES\n  2. BUREAU DE VOTES\n  3. ELECTEURS\n  4. CANDIDATS\n  5. VOTES\n  6. RAPPORT ET RESULTAT\n  7. QUITTER L'APPLICATION\n\n=============================\n\nChoix : ");

        if (scanf("%d", &choix) != 1) {
            vider_buffer();
            printf("\nChoix invalide !\n");
            continue;
        }

        switch(choix){
        case 1:
          p_pol();
            break;

        case 2:
            ge_BV();
            break;

        case 3:
            ge_elect();
            break;

        case 4:
            ge_candi();
            break;

        case 5:
            ge_vote();
            break;

        case 6:
            ge_rapp();
            break;

        case 7:
            return;
            break;

        default:
            printf("\nChoix invalide !\n");
            break;
        }
    }while(choix!=7);

}
//================fonction principale=======================
int main()
{
   menu_general();
    return 0;
}
