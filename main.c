#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define SIZE_X 9
#define SIZE_Y 1
#define PATTERNS 5
#define IAnb 2



int actual_pattern=0;
int map[SIZE_X][SIZE_Y];

int pattern[2][PATTERNS];

void remplir_carte_vide()///REMPLISSAGE DE LA CARTE PAR DES 0
{
    int i, j;
    for (i = 0; i < SIZE_Y; i++)
    {
        for (j = 0; j < SIZE_X; j++)
        {
            map[i][j] = 0;
        }
    }

}

void cursor(int x,int y)///PERMET DE REFRESH L'AFFICHAGE
{
#ifdef _WIN32
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord;
    coord.X = (SHORT)x;
    coord.Y = (SHORT)y;
    SetConsoleCursorPosition(Handle,coord);
#endif
}

typedef struct Joueur Joueur; ///Creation de "fiches", pour chaque joueurs, qui contient leurs paramètres
struct Joueur
{
    int Coups;
    int Vie;
    int ID;
    int x;
    int colonne;
    int y;
    int num;
    int stamina;
    int wait;
    int *(*fonction)(int**,int,int);

};

Joueur liste_player[IAnb];///on stocke la fiche de chaque joueur

int *my_ai_play1(int **map, int height, int width)///exemple d'IA qui fonctionne de manière random
{
    return (rand()%4);
}

int *my_ai_play2(int **map, int height, int width)///exemple 2
{
    return (rand()%4);
}

int *(*listeFonctions[IAnb])(int**,int,int) = {my_ai_play1,my_ai_play2};///on stock les fonctions des IA correspondantes



void joueur_init()///on initialise les joueurs
{
    int i;
    int colonne=1;
    int y=0;
    for (i=0; i<IAnb; i++)
    {

        Joueur j;
        j.Vie=15;///la vie de chaque joueur commence à 10
        j.Coups=0;///nbr de coups qui vont déterminer le vaincqueur
        j.stamina=6;///stamina
        j.wait=0;
        j.ID=(rand()%1000)+1000;///ID du joueurs random mais vaste choix donc peut de probabilité de récurrence
        if(colonne ==1)///joueur du côté gauche -> colonne 1
        {
            j.x=1;
            j.y=y;
            j.colonne=1;
            colonne=2;
        }
        else if (colonne==2)///joueur du côté droit -> colonne 2
        {
            j.x=7;
            j.y=y;
            y++;
            j.colonne=2;
            colonne=1;
        }

        map[j.x][j.y]=j.ID;
        j.fonction=(*listeFonctions[i]);
        j.num=i+1;
        liste_player[i]=j;

    }
}

void afficher_map()
{
    cursor(0,40); ///refresh de la map
    int i,j;
    printf("------------------------------ARENA HUNTER------------------------------\n");///titre
    printf("X ");///bordure
    for (i=3; i>=1; i--)///affichage pattern du monstre
    {
        if (pattern[0][actual_pattern]==i)
        {
            printf("V ");
        }
        else if (pattern[1][actual_pattern]==i)
        {
            printf("V ");
        }
        else
        {
            printf("  ");
        }
    }
    printf("XXXXXXXXX ");///différenciation attaque monstre
    for (i=1; i<=3; i++)///affichage parallèle pour le côté droit
    {
        if (pattern[0][actual_pattern]==i)
        {
            printf("V ");
        }
        else if (pattern[1][actual_pattern]==i)
        {
            printf("V ");
        }
        else
        {
            printf("  ");
        }
    }
    printf("X\n");

    for (i=0; i<(SIZE_X)*2 +7; i++) printf("X"); ///Première ligne de X
    printf("\n");
    for (i=0; i<SIZE_Y; i++)
    {
        printf("X ");
        for (j=0; j<SIZE_X; j++)
        {
            if(j==3 || j==6)
                printf("| ");
            if (map [j][i]>=1000)
            {
                printf("Y ");
            }

            switch (map[j][i])///affichage map
            {
                case 0:
                    printf(". ");
                    break;
                case 1:
                    printf(" ");
                    break;
                case 10:
                    printf("KO");
                default:
                    break;
            }
        }
        printf("X\n");
    }
    for (i=0; i<(SIZE_X)*2 +7; i++)printf("X");
    for (i=0; i<13; i++)printf("\n"); ///espace entre le bas et l'affichage

    printf("\n");
}

void pattern_m()///pattern du monstre
{
    int j;
    for(j=0; j<PATTERNS; j++)
    {
        int nb_attack=rand()%2;
        if (nb_attack==0)/// attaque sur une colonne
        {
            int colonne=(rand()%3)+1;
            pattern [1][j]=-1; ///pattern = tableau coordonne l'attaque
            pattern [0][j]=colonne;
        }
        else ///attaque sur deux colonne
        {
            int colonne=(rand()%3)+1;
            int colonne1=(rand()%3)+1;
            pattern[1][j]=colonne;///pattern pour deux attaque
            pattern[0][j]=colonne1;

        }
    }



}


void score()/// système de points à terminer
{
    int i;
    int j;
    int keep;
    int keep_num;
    int tableau_score[i];
    int tableau_num[i];
    for(i=0; i<IAnb;i++)
    {
        printf("Le joueur %d a score %d\n", liste_player[i].num,liste_player[i].Coups);
        tableau_score[i]=liste_player[i].Coups;
        tableau_num[i]=liste_player[i].num;

    }
    for(i=0; i<IAnb;i++)
    {
        for(j=1; j<IAnb;j++)
        {
            if (tableau_score[i]>tableau_score[j])
            {
                keep=tableau_score[j];
                tableau_score[j]=tableau_score[i];
                tableau_score[i]=keep;
                keep_num=tableau_num[j];
                tableau_num[j]=tableau_num[i];
                tableau_num[i]=keep_num;

            }
        }
    }
    printf("Le gagnant est le joueur %d, avec %d points \n!", tableau_num[IAnb-1], tableau_score[IAnb-1]);
}


void vitality(int i)///permet de faire mourrir un joueur
{
    if (liste_player[i].Vie==0)
    {
        map[liste_player[i].x][liste_player[i].y]=10;///voir case 10 du switch de l'afficher map
    }
}


void degats(int i,int action)/// système de dégâts
{
    if((liste_player[i].stamina>=1))
    {
        if (liste_player[i].colonne==2)
        {
            if(liste_player[i].x==(pattern[0][actual_pattern]+5) || liste_player[i].x==(pattern[1][actual_pattern]+5) )///attaque du côté gauche
            {
                liste_player[i].Vie=liste_player[i].Vie-1;
                vitality(i);

            }

        }
        if (liste_player[i].colonne==1)///attaque droite
        {
            if(liste_player[i].x==0 && (pattern[0][actual_pattern]==3 || pattern[1][actual_pattern]==3))
            {
                liste_player[i].Vie=liste_player[i].Vie-1;
                vitality(i);
            }
            if(liste_player[i].x==1 && (pattern[0][actual_pattern]==2 || pattern[1][actual_pattern]==2))
            {
                liste_player[i].Vie=liste_player[i].Vie-1;
                vitality(i);
            }
            if(liste_player[i].x==2 && (pattern[0][actual_pattern]==1 || pattern[1][actual_pattern]==1))
            {
                liste_player[i].Vie=liste_player[i].Vie-1;
                vitality(i);
            }
        }
    }
    else if (action==0 && liste_player[i].stamina>=0)
    {

        printf("WOW le joueur %d c'est defendu\n", liste_player[i].num);
        liste_player[i].stamina=liste_player[i].stamina-2;
    }
    else if (action==0 && liste_player[i].stamina==0)
    {
        printf("Oh no le joueur %d est fatigue\n", liste_player[i].num);
    }

}



void gauche(int i)///fonction de déplacement gauche
{
    if (liste_player[i].stamina>=1)
    {


        if(liste_player[i].Vie>0)
        {
            if(liste_player[i].colonne==1)
            {
                if(liste_player[i].x>0)
                {
                    map[liste_player[i].x][liste_player[i].y]=0;///case position avant déplacement devient nul -> pour afficher points
                    liste_player[i].x=liste_player[i].x-1;///changement de position en abcisse
                    map[liste_player[i].x][liste_player[i].y]= liste_player[i].ID;///joueur dans map coordonnée
                    liste_player[i].stamina=liste_player[i].stamina-1;///baisse de stamina qui revient pour chaque actions
                }
            }
            else if(liste_player[i].colonne==2)
            {
                if(liste_player[i].x>6)
                {
                    map[liste_player[i].x][liste_player[i].y]=0;
                    liste_player[i].x=liste_player[i].x-1;
                    map[liste_player[i].x][liste_player[i].y]= liste_player[i].ID;
                    liste_player[i].stamina=liste_player[i].stamina-1;
                }

            }

        }
    }
    else if (liste_player[i].stamina<=0)
    {
        printf("Oh no le joueur %d est fatigue\n",liste_player[i].num);
    }


}

void droite (int i)/// fonction pour déplacement droit = presque pareil que celle du gauche
{
    if (liste_player[i].stamina>=1)
    {
        if(liste_player[i].Vie>0)
        {
            if(liste_player[i].colonne==1)
            {
                if(liste_player[i].x<2)
                {
                    map[liste_player[i].x][liste_player[i].y]=0;
                    liste_player[i].x=liste_player[i].x+1;
                    map[liste_player[i].x][liste_player[i].y]= liste_player[i].ID;
                    liste_player[i].stamina=liste_player[i].stamina-1;
                }
            }
            else if(liste_player[i].colonne==2)
            {
                if(liste_player[i].x<8)
                {
                    map[liste_player[i].x][liste_player[i].y]=0;
                    liste_player[i].x=liste_player[i].x+1;
                    map[liste_player[i].x][liste_player[i].y]= liste_player[i].ID;
                    liste_player[i].stamina=liste_player[i].stamina-1;
                }

            }
        }
        else
        {
            printf("le joueur %d est mort", liste_player[i].num);
        }


    }
    else if(liste_player[i].stamina<=0)
    printf("Oh no le joueur %d est fatigue\n",liste_player[i].num);



}
void attack(int i) ///ATTACK PROXIMITE MAX
{
    if (liste_player[i].stamina>=1)///joueur attaque que si il a du stamina
    {

        if(liste_player[i].colonne==1)/// si joueur de gauche
        {
            if (liste_player[i].x==2)
            {
                liste_player[i].Coups=+10;
                printf("le joueur %d attaque +10pts\n", liste_player[i].num);

            }

        }
        if(liste_player[i].colonne==2)/// si joueur de droite
        {
            if (liste_player[i].x==6)
            {
                liste_player[i].Coups=+10;
                printf("le joueur %d attaque +10pts\n", liste_player[i].num);
            }

        }
    }
    else if (liste_player[i].stamina<=0)///fatigue
    printf("Oh no le joueur %d est fatigue\n",liste_player[i].num);



}

void cooldown(int i)///gestion de la fatigue, non mise au points
{

    if(liste_player[i].stamina<=0 && liste_player[i].wait!=2)
    {
        liste_player[i].wait+=1;///wait de chaque joueur incrémenté de 1

    }
    else if (liste_player[i].wait==2)/// joueur rejoue lorsque son stamina est revenu, donc après deux tours
    {
        liste_player[i].stamina=6;
        ("Le joueur %d est pret a attaquer de nouveaux\n", liste_player[i].num);
        liste_player[i].wait=0;
    }
}





int main()
{

    srand(time(NULL));
    remplir_carte_vide();
    int game=1;
    pattern_m();
    joueur_init();
    int tours=0;
    while (game==1)
    {
        int i;
        for (i=0; i<IAnb; i++)
        {



            if ((*listeFonctions[i])!=NULL)
            {



                int action = liste_player[i].fonction(map,SIZE_Y,SIZE_X);
                degats(i, action);
                cooldown(i);///on appelle la fonction cooldown

                switch (action)/// switch pour chaque action
                {
                    case 1:
                        gauche(i);
                        break; ///move gauche
                    case 2:
                        droite(i);
                        break; ///move droite
                    case 3:
                        attack(i);
                        break; ///attack
                    default:
                        break;



                }


            }




        }

        afficher_map();/// on affiche
        actual_pattern+=1;///on incrémente actual pattern de 1

        if (actual_pattern>=PATTERNS) actual_pattern=0;///lorsque pattern dépasse le nombre de récurence limité par define PATTERNS on remet à 5, le monstre repète ainsi ses action en 5 tours

        if (tours==30)
        {
            score();///appel pour savoir qui gagne
            return 0;///délimitation
        }
        tours+=1;
        //usleep(500*1000);///<=> 200ms
        Sleep(1000);
    }

}









