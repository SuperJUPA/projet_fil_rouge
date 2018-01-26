#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../SEARCH/by_keyword.h"
#include "../INDEXATION/index_txt.h"
#include "../INDEXATION/index_sound.h"
#include "../INDEXATION/index_img.h"
#include "menu.h"


void viderBuffer()
{
    int c = 0;
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
}

int lire(char *chaine, int longueur)
{
    char *positionEntree = NULL;

    if (fgets(chaine, longueur, stdin) != NULL)
    {
        positionEntree = strchr(chaine, '\n');
        if (positionEntree != NULL)
        {
            *positionEntree = '\0';
        }
        else
        {
            viderBuffer();
        }
        return 1;
    }
    else
    {
        viderBuffer();
        return 0;
    }
}

void modifConfig(int value,char* label)
{
  char cmd[10000];
  FILE * ptr_fic;
  char mot_fic[100];
  char nouvellesValeurs[10000];
  char conversion[10000];

  strcpy(nouvellesValeurs,"");

  ptr_fic = fopen("../EXTERN_FILES/configuration.config", "r");

  if( ptr_fic != NULL)
  {
      while ( fscanf(ptr_fic, "%s", mot_fic)==1 )
      {
            if(strcmp(mot_fic,label)==0)
            {
              strcat(nouvellesValeurs,mot_fic);
              fscanf(ptr_fic, "%s", mot_fic);
              strcat(nouvellesValeurs," ");
              snprintf(conversion,sizeof(conversion),"%d",value);
              strcat(nouvellesValeurs,conversion);

            }
            else
            {
              strcat(nouvellesValeurs,mot_fic);
            }
            strcat(nouvellesValeurs," ");
      }
      if(ptr_fic!=NULL)
      {
        fclose(ptr_fic);
      }

      ptr_fic = fopen("../EXTERN_FILES/configuration.config", "w");
      if(ptr_fic!=NULL)
      {
        fprintf(ptr_fic,"%s",nouvellesValeurs);
        if(ptr_fic!=NULL)
        {
          fclose(ptr_fic);
        }
      }
      else
      {
         fprintf(stderr, "ERREUR :  PB fonction modifConfig\n");
      }
  }
  else
  {
     fprintf(stderr, "ERREUR :  PB fonction modifConfig\n");
  }


}


/* menu de base quand on arrive dans l'api */
void menu_base() {

   printf("\nMenu principal : \n1 - Administrateur\n2 - Utilisateur\n3 - Quitter\n" );
   char choix[1];
   do {
     lire(choix,2);

     if ( (strcmp(choix,"1")!= 0) && (strcmp(choix,"2")!= 0) && (strcmp(choix,"3")!= 0) ) {
       printf("Probleme de saisie, veuillez recommencer.\n" );
     }

   } while( (strcmp(choix,"1")!= 0) && (strcmp(choix,"2")!= 0 ) && (strcmp(choix,"3")!= 0 ) );


   if (strcmp(choix,"1") == 0 ) {
     menu_administrateur(0);
   }else if (strcmp(choix,"2" )== 0 ) {
      menu_utilisateur();
   }else if(strcmp(choix,"3" )== 0 ){
      exit ;
   }
}

void menu_administrateur(int verif) {


char mdp[500];
char choix[1];

if (verif == 0) {
  printf("\nVeuillez entrer le mot de passe :\n" );
  do {
    lire(mdp,5);

    if (strcmp(mdp,"test") != 0 ) {
      printf("Mot de passe erroné, réessayez :\n" );
    }
  } while(strcmp(mdp,"test") != 0 );
}



printf("\nMenu administrateur :\n1 - Lancer indexation\n2 - Configuration\n3 - Visualiser descripteurs\n4 - Retour au menu principal\n" );

do {

  lire(choix,2);

  if ( (strcmp(choix,"1")!= 0) && (strcmp(choix,"2")!= 0) && (strcmp(choix,"3")!= 0) && (strcmp(choix,"4")!=0) ) {
    printf("Probleme de saisie, veuillez recommencer.\n" );
  }

} while( (strcmp(choix,"1")!= 0) && (strcmp(choix,"2")!= 0 )  && (strcmp(choix,"3")!= 0) && (strcmp(choix,"4")!=0) );

//indexation
   if (strcmp(choix,"1") == 0 ) {
      printf("Lancement de l'indexation !\n" );
      rmThenStartIndexation();
      indexImage();
      resetAndIndex();

      menu_administrateur(1);
//menu Configuration
   }else if (strcmp(choix,"2" )== 0 ) {
      menu_configuration();

//menu visualiser descripteurs
   }else if (strcmp(choix,"3" )== 0 ) {
     menu_visualiserdesc();

     //retour au menu principal
   } else if(strcmp(choix,"4" )== 0 ){
     menu_base();
   }



}

void menu_comparaison(){
  printf("\nMenu recherche par comparaison : \n1 - Texte\n2 - Son\n3 - Image\n4 - Retour\n" );
  char choix[1];
  do {

    lire(choix,2);

    if ( (strcmp(choix,"1")!= 0) && (strcmp(choix,"2")!= 0) && (strcmp(choix,"3")!= 0) && (strcmp(choix,"4")!=0) ) {
      printf("Probleme de saisie, veuillez recommencer.\n" );
    }

  } while( (strcmp(choix,"1")!= 0) && (strcmp(choix,"2")!= 0 )  && (strcmp(choix,"3")!= 0) && (strcmp(choix,"4")!=0) );


    char requete[2000];
  //Comparaison texte
     if (strcmp(choix,"1") == 0 ) {
        printf("Saisissez le chemin du fichier à comparer (.xml)\n" );
    		scanf("%s",requete);
    		indexFileToCompare(requete);

  //Comparaison son
     }else if (strcmp(choix,"2" )== 0 ) {
        printf("Saisissez le chemin du fichier à comparer (.bin)\n" );
  			scanf("%s", requete );
  			compare(requete);

  //menu comparaison image
     }else if (strcmp(choix,"3" )== 0 ) {
      printf("Saisissez le chemin du fichier à comparer (.txt)\n" );
  		scanf("%s",requete);
      searchImage(requete);

       //retour au menu utilisateur
     } else if(strcmp(choix,"4" )== 0 ){
       menu_utilisateur();
     }

   viderBuffer();
   menu_utilisateur();

}

void menu_recherche_motcle(){
    printf("\nRecherche par mot-clé.\nVeuillez entrer un mot clé : \n" );
    char mot[2000];
    scanf("%s", mot);
  	byword(mot);

    viderBuffer();
    menu_utilisateur();
}

void menu_visualiserdesc() {
    printf("\nMenu visualiser descripteurs : \n1 - Texte\n2 - Son\n3 - Image\n4 - Retour\n" );
    char choix[1];
    do {

      lire(choix,2);

      if ( (strcmp(choix,"1")!= 0) && (strcmp(choix,"2")!= 0) && (strcmp(choix,"3")!= 0) && (strcmp(choix,"4")!=0) ) {
        printf("Probleme de saisie, veuillez recommencer.\n" );
      }

    } while( (strcmp(choix,"1")!= 0) && (strcmp(choix,"2")!= 0 )  && (strcmp(choix,"3")!= 0) && (strcmp(choix,"4")!=0) );

    //Visualisation texte
       if (strcmp(choix,"1") == 0 ) {
          printf("Visualisation descripteurs texte\n" );
          system("cat ../EXTERN_FILES/database/base_texte/base_descripteur_texte.txt");
    //visualisation son
       }else if (strcmp(choix,"2" )== 0 ) {
          printf("Visualisation descripteurs son\n" );
          system("cat ../EXTERN_FILES/database/base_son/base_descripteur_son.txt");
    //visualisation image
       }else if (strcmp(choix,"3" )== 0 ) {
        printf("Visualisation descripteurs image\n" );
        system("cat ../EXTERN_FILES/database/base_image/base_descripteur_image.txt");
         //retour au menu admin
       } else if(strcmp(choix,"4" )== 0 ){
         menu_administrateur(1);
       }

       menu_administrateur(1);
}

void menu_configuration() {
  printf("\nMenu configuration : \n1 - Texte\n2 - Son\n3 - Image\n4 - Retour\n" );
  char choix[1];
  do {

    lire(choix,2);

    if ( (strcmp(choix,"1")!= 0) && (strcmp(choix,"2")!= 0) && (strcmp(choix,"3")!= 0) && (strcmp(choix,"4")!=0) ) {
      printf("Probleme de saisie, veuillez recommencer.\n" );
    }

  } while( (strcmp(choix,"1")!= 0) && (strcmp(choix,"2")!= 0 )  && (strcmp(choix,"3")!= 0) && (strcmp(choix,"4")!=0) );

  //configuration texte
     if (strcmp(choix,"1") == 0 ) {
        printf("Configuration texte\n" );
        printf("Saisissez le nombre d'occurences : \n" );
        int nbocc ;
        scanf("%d", &nbocc );
        modifConfig(nbocc, "txt_nboccurrences:");

  //configuration son
     }else if (strcmp(choix,"2" )== 0 ) {
        printf("Configuration son\n" );
        menu_configuration_son();

  //configuration image
     }else if (strcmp(choix,"3" )== 0 ) {
      printf("Configuration image\n" );
        menu_configuration_image();

       //retour au menu admin
     } else if(strcmp(choix,"4" )== 0 ){
       menu_administrateur(1);
     }
    viderBuffer();
     menu_administrateur(1);
}

void menu_configuration_son() {
printf("\nMenu configuration son\n1 - Nombre d'échantillons\n2 - Nombre de fenetres\n3 - Retour\n" );

char choix[1];

do {
  lire(choix,2);

  if ( (strcmp(choix,"1")!= 0) && (strcmp(choix,"2")!= 0) && (strcmp(choix,"3")!= 0) ) {
    printf("Probleme de saisie, veuillez recommencer.\n" );
  }

} while((strcmp(choix,"1")!= 0) && (strcmp(choix,"2")!= 0 ) && (strcmp(choix,"3")!= 0));

int valeur ;

if (strcmp(choix,"1") == 0 ) {
   printf("Saisissez le nombre d'échantillons : \n" );
   scanf("%d", &valeur );
   modifConfig(valeur, "echantillions:");
//menu comparaison
}else if (strcmp(choix,"2" )== 0 ) {
   printf("Saisissez le nombre d'intervalles : \n" );
   scanf("%d", &valeur );
   modifConfig(valeur, "nbintervalles:");
//Retour
}else if(strcmp(choix,"3" )== 0 ){
  menu_configuration();
}

viderBuffer();
menu_administrateur(1);

}

void menu_configuration_image() {
printf("\nMenu configuration image\n1 - Nombre de bits de quantification NB\n2 - Nombre de bits de quantification RVB\n3 - Retour\n" );

char choix[1];

do {
  lire(choix,2);

  if ( (strcmp(choix,"1")!= 0) && (strcmp(choix,"2")!= 0) && (strcmp(choix,"3")!= 0) ) {
    printf("Probleme de saisie, veuillez recommencer.\n" );
  }

} while((strcmp(choix,"1")!= 0) && (strcmp(choix,"2")!= 0 ) && (strcmp(choix,"3")!= 0));

  int valeur ;

if (strcmp(choix,"1") == 0 ) {
   printf("Saisissez le nombre de bits de quantification NB : \n" );
   scanf("%d", &valeur );
   modifConfig(valeur, "NbBit_NB:");
//menu comparaison
}else if (strcmp(choix,"2" )== 0 ) {
   printf("Saisissez le nombre de quantification RVB : \n" );
   scanf("%d", &valeur );
   modifConfig(valeur, "NbBit_RGB:");
//Retour
}else if(strcmp(choix,"3" )== 0 ){
  menu_configuration();
}
viderBuffer();
menu_administrateur(1);
}

void menu_utilisateur() {

   printf("\nMenu utilisateur : \n1 - Recherche par mot-clé\n2 - Recherche par comparaison\n3 - Retour au menu principal\n" );
char choix[1];
   //entrée du menu
   do {

     lire(choix,2);

     if ( (strcmp(choix,"1")!= 0) && (strcmp(choix,"2")!= 0) && (strcmp(choix,"3")!= 0) ) {
       printf("Probleme de saisie, veuillez recommencer.\n" );
     }

   } while( (strcmp(choix,"1")!= 0) && (strcmp(choix,"2")!= 0 )  && (strcmp(choix,"3")!= 0) );

//après premier choix

//recherche par mot clé
   if (strcmp(choix,"1") == 0 ) {
      menu_recherche_motcle();

//menu comparaison
   }else if (strcmp(choix,"2" )== 0 ) {
      menu_comparaison();

//Retour
   }else if (strcmp(choix,"3" )== 0 ) {

     menu_base();
   }


}


/*
int main(int argc, char const *argv[]) {

menu_base();


  return 0;
}*/
