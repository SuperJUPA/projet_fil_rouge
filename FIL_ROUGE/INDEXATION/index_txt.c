#include <stdio.h>
#include <stdlib.h>
#include "index_txt.h"
#define TAILLE_MAX 10000000

/*
Pour une meilleure compréhension du code se référer a index_txt.h qui explique le rôle de chaque fonction.

j'ai décidé de créer des fichiers temporaires a chaque fois que j'en ressentais le besoin, pour séparer
mon traitement des documents originaux.(../INDEXATION/temp_txt).

*/
/*
=========================================================================
Main de test
=========================================================================
*/
/*
int main(int argc, char const *argv[]) {

      rmThenStartIndexation();
      indexFileToCompare("05-L_apnéiste_français_Guillaume_Néry_plonge.xml");

  return 0;
}*/

/*
=========================================================================
Code associé au génération des documents liés à l'startIndexation
=========================================================================
*/

void rmThenStartIndexation()
{
  char cmd[1000];
  FILE * ptr_fic;
  char nom_fic[100];
  //supression de liste_base_texte base_descripteur_texte
  strcpy(cmd,"rm ");
  strcat(cmd,LISTE);
  system(cmd);

  strcpy(cmd,"rm ");
  strcat(cmd,DATABASE);
  system(cmd);

  strcpy(cmd,"rm ");
  strcat(cmd,TABLE);
  system(cmd);

  //supression du répertoire temporaire
  strcpy(cmd,"rm -r ../INDEXATION/temp_txt");
  system(cmd);

  // création des répertoires cibles sur lesquels on va travailler
  strcpy(cmd,"mkdir ../INDEXATION/temp_txt");
  system(cmd);

  strcpy(cmd,"mkdir ../INDEXATION/temp_txt/cpy");
  system(cmd);

  strcpy(cmd,"mkdir ../INDEXATION/temp_txt/pure_txts");
  system(cmd);


  // récréation des fichiers de BD
  strcpy(cmd,"touch ");
  strcat(cmd,LISTE);
  system(cmd);

  strcpy(cmd,"touch ");
  strcat(cmd,DATABASE);
  system(cmd);

  strcpy(cmd,"touch ");
  strcat(cmd,TABLE);
  system(cmd);

  strcpy(cmd,"touch ");
  strcat(cmd,"../INDEXATION/table_index_texte_temp");
  system(cmd);

  startIndexation();
}

void startIndexation()
{
  char cmd[1000];
  FILE * ptr_fic;
  char nom_fic[100];

  strcpy(cmd, "ls ");
  strcat(cmd, PATHFILES);
  strcat(cmd, " > ../INDEXATION/temp_txt/fic_tempLIST.xml");
  fflush(stdout);
  system(cmd);

  ptr_fic = fopen("../INDEXATION/temp_txt/fic_tempLIST.xml", "r");

  if( ptr_fic != NULL)
  {
      while ( fscanf(ptr_fic, "%s", nom_fic)==1 )
      {
          checkForUpdates(nom_fic);
      }
      if(ptr_fic!=NULL)
      {
        fclose(ptr_fic);
      }

  }
  else
  {
     fprintf(stderr, "ERREUR :  PB avec fic_tempLIST.xml\n");
  }

  createTableIndexTexte();
  printf("tous les documents xml ont bien été indexés !\n");
}

void indexFile(char* name,int id) {
  char cmd[1000];
  int c;
  char car;
  char* pch;
  FILE * ptr_fic;
  char ligne[100];
  char *realName;

  if(id!=-1)
  {
    strcpy(cmd, "cat ");
    strcat(cmd, PATHFILES);
    strcat(cmd, name);
    strcat(cmd," > ../INDEXATION/temp_txt/cpy/" );
    strcat(cmd,name);
  }

  else
  {
    strcpy(cmd, "cat ");
    strcat(cmd, name);
    const char s[2] = "/";
    char *nameabs=malloc(sizeof(char)*strlen(name));
    strcpy(nameabs,name);
    char *token;
    /* get the first token */
    token = strtok(nameabs, s);

    /* walk through other tokens */
    while( token != NULL )
    {
       realName=token;
       token = strtok(NULL, s);
    }
    strcat(cmd," > ../INDEXATION/temp_txt/cpy/" );
    strcat(cmd,realName);
  }
  system(cmd);

  if(id!=-1)
  {
    sortMeBalises(name);
    searchOccurencesAndInsertBD(name,id);
  }
  else if(id=-1)
  {
    sortMeBalises(realName);
    searchOccurencesAndInsertBD(realName,id);
  }
}

void checkForUpdates(char * name)
{
  char cmd[1000];
  FILE * ptr_fic;
  char ligne[100];

  strcpy(cmd, "grep ");
  strcat(cmd, name);
  strcat(cmd, " ");
  strcat(cmd, LISTE);
  strcat(cmd, " > ../INDEXATION/temp_txt/trouveOuPas.xml");
  system(cmd);

  ptr_fic = fopen("../INDEXATION/temp_txt/trouveOuPas.xml", "r");

  if( ptr_fic != NULL)
  {
      fscanf(ptr_fic, "%s", ligne);
      if(feof(ptr_fic))
      {
        int id;
        id=getNewID();
        char conversion[50];
        snprintf(conversion,sizeof(conversion),"%d",id);
        strcpy(cmd, "echo ");
        strcat(cmd, "'<'id'>' ");
        strcat(cmd,conversion);
        strcat(cmd," '<''/'id'>' ");
        strcat(cmd, "'<'filename'>' ");
        strcat(cmd, name);
        strcat(cmd, " '<''/'filename'>'");
        strcat(cmd, " >> ");
        strcat(cmd, LISTE);
        system(cmd);
        indexFile(name,id);
      }
      if(ptr_fic!=NULL)
      {
        fclose(ptr_fic);
      }
  }
  else
  {
     fprintf(stderr, "ERREUR :  PB avec trouveOuPas\n");
  }
}

void sortMeBalises(char * name)
{
  int c;
  char car;
  char filename[100000];
  char sourceName[100000];
  FILE * ptr_fic;
  FILE * ptr_fic2;
  int debut=0;
  int fin=0;
  char ponctuation[100]={',',';',',','?','!','%','.','(',')'};

  strcpy(filename,"../INDEXATION/temp_txt/pure_txts/");
  strcat(filename,name);

  strcpy(sourceName,"../INDEXATION/temp_txt/cpy/");
  strcat(sourceName,name);
  ptr_fic = fopen(sourceName, "r");
  ptr_fic2=fopen(filename, "w");
  if( ptr_fic != NULL)
  {
    while ((c = fgetc(ptr_fic)) != EOF)
       {
         char car = (char)c;
         if(car=='<')
         {
           debut=1;
           fin=0;
         }
         if(car=='>')
         {
           fin=1;
           debut=0;
         }
         if(debut==0 && fin==1)
         {
           if(car!='>' && testPonctuation_TEXTE(car))
           {
             if(car=='\'')
             {
               car=' ';
             }
             fputc(car,ptr_fic2);
           }
         }
       }
       fclose(ptr_fic);
       fclose(ptr_fic2);
  }
  else
  {
     fprintf(stderr, "ERREUR :  PB avec fonction sortMeBalises\n");
  }
}

int testPonctuation_TEXTE(char car)
{
  if(car=='.' || car==',' || car==';' || car=='?' || car=='!'||  car==')' || car=='(' || car==':' || car=='%')
  {
    return 0;
  }
  return 1;
}

void searchOccurencesAndInsertBD(char * name,int id)
{
  FILE * fichier_a_traiter;
  FILE * database;
  FILE * temporaire;
  char filename[100000];
  char mot[100000];
  int param;
  int cpt=0;
  int index=0;
  int occurences=0;
  char chaineDescripteur[100000];

  param=recupParamConfigTexte();

  strcpy(filename,"../INDEXATION/temp_txt/pure_txts/");
  strcat(filename,name);
  fichier_a_traiter=fopen(filename,"r");
  if(fichier_a_traiter!=NULL)
  {
    while (!feof(fichier_a_traiter)) {
      fscanf(fichier_a_traiter,"%s",mot);
      if(!feof(fichier_a_traiter))
      {
        cpt++;
      }
    }

    fclose(fichier_a_traiter);
  }
  else
  {
    fprintf(stderr, "ERREUR :  PB avec fonction searchOccurencesAndInsertBD\n");
  }

  char * tabchaines[cpt];
  strcpy(filename,"../INDEXATION/temp_txt/pure_txts/");
  strcat(filename,name);
  fichier_a_traiter=fopen(filename,"r");
  if(fichier_a_traiter!=NULL)
  {
    while (!feof(fichier_a_traiter)) {
      fscanf(fichier_a_traiter,"%s",mot);
      if(!feof(fichier_a_traiter))
      {
            tabchaines[index]=strdup(mot);
            index++;
      }
    }
    fclose(fichier_a_traiter);
  }
  else
  {
    fprintf(stderr, "ERREUR :  PB avec fonction searchOccurencesAndInsertBD\n");
  }

  index=0;
  int i=0;
  int verif=0;
  int pertinence;
  char motsretenus[10000];
  char conversion[50];
  strcpy(motsretenus,"");
  while (index<cpt)
  {
    for(i=0;i<cpt;i++)
    {
      if(index!=i)
      {
        verif=strcmp(tabchaines[index],tabchaines[i]);
      }
      if(verif==0)
      {
        occurences++;
      }
    }

    if(strstr(motsretenus,tabchaines[index])==NULL)
    {
      if(occurences>=param && verifyPertinence(tabchaines[index]))
      {
        snprintf(conversion,sizeof(conversion),"%d",occurences);
        strcat(motsretenus,tabchaines[index]);
        strcat(motsretenus," ");
        strcat(motsretenus,conversion);
        strcat(motsretenus," ; ");
      }
    }
    index++;
    occurences=0;
  }
  char conversion2[50];
  if(id!=-1)
  {
    database=fopen(DATABASE,"a+");
    if(database!=NULL)
    {
      snprintf(conversion2,sizeof(conversion2),"%d",id);
      strcpy(chaineDescripteur, "<debut> ");
      strcat(chaineDescripteur, "<id> ");
      strcat(chaineDescripteur, conversion2);
      strcat(chaineDescripteur, " </id> ");
      strcat(chaineDescripteur, "<mots> ");
      fprintf(database,"%s",chaineDescripteur);
      fprintf(database, "%s ",motsretenus);
      strcpy(chaineDescripteur, " </mots>");
      strcat(chaineDescripteur, " </fin>");
      fprintf(database, "%s\n",chaineDescripteur);
      fclose(database);
    }
    else
    {
      fprintf(stderr, "ERREUR :  PB avec fonction searchOccurencesAndInsertBD\n");
    }
  }
  else
  {
    temporaire=fopen(TEMPO,"w");
    if(temporaire!=NULL)
    {
      snprintf(conversion2,sizeof(conversion2),"%d",id);
      strcpy(chaineDescripteur, "<debut> ");
      strcat(chaineDescripteur, "<id> ");
      strcat(chaineDescripteur, conversion2);
      strcat(chaineDescripteur, " </id> ");
      strcat(chaineDescripteur, "<mots> ");
      fprintf(temporaire,"%s",chaineDescripteur);
      fprintf(temporaire, "%s ",motsretenus);
      strcpy(chaineDescripteur, " </mots>");
      strcat(chaineDescripteur, " </fin>");
      fprintf(temporaire, "%s\n",chaineDescripteur);
      fclose(temporaire);
    }
    else
    {
      fprintf(stderr, "ERREUR :  PB avec fonction searchOccurencesAndInsertBD2\n");
    }

  }

  index=0;
  while (index<cpt) {
    free(tabchaines[index]);
    index++;
  }
}


int recupParamConfigTexte()
{
  FILE * config;
  int value=0;
  int booltest=0;

  char chaine[100000];
  config = fopen("../EXTERN_FILES/configuration.config","r");
  if(config!=NULL)
  {
    while (!feof(config))
    {
      fscanf(config,"%s",chaine);
      if(strcmp(chaine,"txt_nboccurrences:")==0)
      {
        booltest=1;
      }
      if(booltest==1)
      {
        value=atol(chaine);
        if(value!=0)
        {
          fclose(config);
          return value;
        }
      }
    }

  }
  else
  {
    fprintf(stderr, "ERREUR :  PB avec fonction recupParamConfigTexte\n");
  }
}

int getNewID()
{
  FILE * ptr_fic;
  char nom_fic[1000];
  int cpt=0;
  ptr_fic = fopen(LISTE, "r");
  if( ptr_fic != NULL)
  {
      fscanf(ptr_fic, "%s", nom_fic);
      while ( !feof(ptr_fic) )
      {
        cpt++;
        fscanf( ptr_fic, "%s", nom_fic);
      }
        fclose(ptr_fic);
  }
  else
  {
     fprintf(stderr, "ERREUR :  PB avec liste_base_texte\n");
  }

  // il y a 6 fscanf entre chaque ligne réelle pourrait être traité autrement
  if(cpt!=0)
  {
    return (cpt/6)+1;
  }
  else
  {
    return cpt+1;
  }
}

void createTableIndexTexte()
{
  char cmd[1000];
  FILE * ptr_ficToRead;
  FILE * ptr_ficToWrite;
  char motLu[100];
  char id[100];
  int nextIsId=0;
  int nextIsMots=0;
  int nextIsOcc=0;
  int nextIsSeparator=0;


  ptr_ficToRead = fopen(DATABASE, "r");
  ptr_ficToWrite = fopen("../INDEXATION/temp_txt/table_index_texte_temp","w");
  if( ptr_ficToRead != NULL && ptr_ficToWrite != NULL)
  {
      fscanf(ptr_ficToRead, "%s", motLu);
      while ( !feof(ptr_ficToRead) )
      {
        if(nextIsId==1)
        {
          strcpy(id,motLu);
          nextIsId=0;
        }
        if(strcmp(motLu,"<id>")==0)
        {
          nextIsId=1;
        }
        if(strcmp(motLu,"</mots>")==0)
        {
          nextIsMots=0;
        }
        if(nextIsMots==1)
        {
          if(nextIsSeparator==1)
          {
            nextIsSeparator=0;
          }
          else
          {
            if(nextIsOcc==1)
            {
              fprintf(ptr_ficToWrite,"%s\n",motLu );
              nextIsOcc=0;
              nextIsSeparator=1;
            }
            else
            {
              fprintf(ptr_ficToWrite,"%s ",id );
              fprintf(ptr_ficToWrite,"%s ",motLu);
              nextIsOcc=1;
            }
          }
        }
        if(strcmp(motLu,"<mots>")==0)
        {
          nextIsMots=1;
        }

        fscanf( ptr_ficToRead, "%s", motLu);
      }
        fclose(ptr_ficToRead);
        fclose(ptr_ficToWrite);
  }
  else
  {
     fprintf(stderr, "ERREUR :  PB avec liste_base_texte\n");
  }
  int verif;
  int cpt=0;
  FILE* check;
  check = popen("wc -l ../INDEXATION/temp_txt/table_index_texte_temp", "r");
  fscanf(check, "%d", &verif);
  fclose(check);
  while (verif>0) {

    generateTableIndexTexte();
    check = popen("wc -l ../INDEXATION/temp_txt/table_index_texte_temp", "r");
    fscanf(check, "%d", &verif);
    fclose(check);
    cpt++;
  }
}

void generateTableIndexTexte()
{
  char cmd[1000];
  char motLu[100];
  FILE * ptr_ficToRead;
  FILE * ptr_ficToWrite;
  char* motToSearch;
  char* myId;
  char* myNbOcc;
  char* otherId;
  char* otherMot;
  char* otherNbOcc;
  int cpt=1;
  int index=0;
  int tab[1000];
  char chaineConstruite[100000];
  char chaineConstruiteFinale[100000];

  ptr_ficToRead = fopen("../INDEXATION/temp_txt/table_index_texte_temp", "r");
  ptr_ficToWrite = fopen(TABLE,"a+");

  if( ptr_ficToRead != NULL && ptr_ficToWrite != NULL)
  {
      fscanf(ptr_ficToRead, "%s", motLu);
      myId=strdup(motLu);
      fscanf(ptr_ficToRead, "%s", motLu);
      motToSearch=strdup(motLu);
      fscanf(ptr_ficToRead, "%s", motLu);
      myNbOcc=strdup(motLu);
      strcpy(chaineConstruite,"<mot> ");
      strcat(chaineConstruite,motToSearch);
      strcat(chaineConstruite," </mot> <key_nb> ");
      strcat(chaineConstruite,myId);
      strcat(chaineConstruite," ");
      strcat(chaineConstruite,myNbOcc);
      strcat(chaineConstruite," ; ");

      while ( !feof(ptr_ficToRead) )
      {
        fscanf(ptr_ficToRead, "%s", motLu);
        otherId=strdup(motLu);
        fscanf(ptr_ficToRead, "%s", motLu);
        otherMot=strdup(motLu);
        fscanf(ptr_ficToRead, "%s", motLu);
        otherNbOcc=strdup(motLu);

        cpt ++;
        if(strcasecmp(otherMot,motToSearch)==0)
        {
          strcat(chaineConstruite,otherId);
          strcat(chaineConstruite," ");
          strcat(chaineConstruite,otherNbOcc);
          strcat(chaineConstruite," ; ");
          tab[index]=cpt;
          index++;
        }
        free(otherId);
        free(otherMot);
        free(otherNbOcc);
      }
      free(motToSearch);
      free(myId);
      free(myNbOcc);
      chaineConstruite[strlen(chaineConstruite)-2]='\0';
      strcat(chaineConstruite," </key_nb> ");

      fprintf(ptr_ficToWrite,"%s\n",chaineConstruite);
      system("sed 1d ../INDEXATION/temp_txt/table_index_texte_temp -i");

      int cpt2=0;
      int cpt3=1;
      while (cpt2<index)
      {
        strcpy(cmd,"sed ");
        char conversion[50];
        snprintf(conversion,sizeof(conversion),"%d",tab[cpt2]-cpt3);
        strcat(cmd,conversion);
        strcat(cmd,"d ../INDEXATION/temp_txt/table_index_texte_temp -i");
        system(cmd);
        cpt2++;
        cpt3++;
      }
      fclose(ptr_ficToRead);
      fclose(ptr_ficToWrite);
  }
}

void indexFileToCompare(char* name)
{
  //-1 correspond à un fichier que l'on ne va pas conserver juste utilisé pour la comparaison
  indexFile(name,-1);
  compareFiles();
}

void compareFiles()
{
  char cmd[1000];
  FILE * ptr_fic;
  char motfic[100];
  char idDescTemp[100];
  int nbElem=0;
  int cptOcc=0;
  int cptNbelem=0;

  PILE_JULIEN pSize;
  pSize=empilerDescripteurs();
  while (!PILE_JULIEN_EST_VIDE(&pSize))
  {
    char* str=DEPILE_JULIEN(&pSize);
    if(str!=NULL)
    {
      if(str!=NULL)
      {
        free(str);
      }

    }
    nbElem++;
  }

  char* pch;
  int* tabID=(int*)malloc(sizeof(int)*nbElem);
  int* tabOccurences=(int*)malloc(sizeof(int)*nbElem);

  PILE_JULIEN p;
  p=empilerDescripteurs();
  while (!PILE_JULIEN_EST_VIDE(&p))
  {
    char* str=DEPILE_JULIEN(&p);
    ptr_fic = fopen(TEMPO,"r");
    if( ptr_fic != NULL)
    {
        while ( fscanf(ptr_fic,"%s",motfic)==1 )
        {
          if(strcmp(motfic,"<mots>")==0)
          {
            fscanf(ptr_fic,"%s",motfic);
            while (strcmp(motfic,"</mots>")!=0)
            {
              if(strstr(str,motfic))
              {
                cptOcc++;
              }
              fscanf(ptr_fic,"%s",motfic);
              fscanf(ptr_fic,"%s",motfic);
              fscanf(ptr_fic,"%s",motfic);
            }
          }
        }
        pch = strtok (str," ");
        tabID[cptNbelem]=atoi(pch);
        tabOccurences[cptNbelem]=cptOcc;

        cptOcc=0;
        if(ptr_fic!=NULL)
        {
          fclose(ptr_fic);
        }
    }
    else
    {
       fprintf(stderr, "ERREUR :  PB avec fonction compareFiles\n");
    }
    if(str!=NULL)
    {
      free(str);
    }
    cptNbelem++;
  }

  int cpt3=0;
  int cpt4=0;
  int cpt5=0;
  int k=0;


  int* tabID2=(int*)malloc(sizeof(int)*nbElem);
  int* tabOccurences2=(int*)malloc(sizeof(int)*nbElem);

  while (cpt3<cptNbelem) {
    if(tabOccurences[cpt3]!=0 && tabOccurences[cpt3]!=1)
    {
      tabID2[cpt4]=tabID[cpt3];
      tabOccurences2[cpt4]=tabOccurences[cpt3];
      cpt4++;
    }
    cpt3++;
  }

  while (k<cpt4) {
    printf("document d'id %d a %d mots en commun sur avec votre fichier\n",tabID2[k],tabOccurences2[k]);
    k++;
  }

  if(cpt4>0)
  {
    printf("\naffichage des documents dans l'ordre décroissant:\n");
  }


  sort(tabOccurences2,tabID2,cpt4);

  /*
  if(tabID!=NULL)
  {
    free(tabID);
  }
  if(tabOccurences!=NULL)
  {
    free(tabOccurences);
  }
  if(tabID2!=NULL)
  {
    free(tabID2);
  }
  if(tabOccurences2!=NULL)
  {
    free(tabOccurences2);
  }*/

  strcpy(cmd,"rm ");
  strcat(cmd,TEMPO);
  system(cmd);

}

void sort( int *tab,int *tab1, int tab_size)
{
  int i=0;
  int tmp=0;
  int tmp1=0;
  int j=0;

  for(i = 0; i < tab_size; i++)
    {
      for(j = 1; j < tab_size; j++)
        {
          if(tab[i] < tab[j])
            {
              tmp = tab[i];
              tab[i] = tab[j];
              tab[j] = tmp;

              tmp1= tab1[i];
              tab1[i] = tab1[j];
              tab1[j] = tmp1;
              j--;
            }
        }
    }
  tmp = tab[0];
    tmp1 = tab1[0];
  for(i = 0; i < tab_size; i++)
  {
    tab1[i] = tab1[i+1];
      tab[i] = tab[i+1];

  }

  tab[tab_size-1] = tmp;
  tab1[tab_size-1] = tmp1;
  i=0;

  for(i=tab_size-1;i>=0;i--)
  {
    appelerDesId(tab1[i]);
  }
}


PILE_JULIEN empilerDescripteurs()
{
  char cmd[1000];
  FILE * ptr_fic;
  char motfic[100];
  char chaineDescripteur[100000];
  char idDescripteur[100];
  PILE_JULIEN mapile;
  INIT_PILE_JULIEN(&mapile);

  ptr_fic = fopen(DATABASE, "r");
  if( ptr_fic != NULL)
  {
      while ( fscanf(ptr_fic,"%s",motfic)==1)
      {
        if(strcmp(motfic,"<id>")==0)
        {
          fscanf(ptr_fic,"%s",motfic);
          strcpy(chaineDescripteur,"");
          strcat(chaineDescripteur,motfic);
          strcat(chaineDescripteur," ");
        }
        if(strcmp(motfic,"<mots>")==0)
        {
          fscanf(ptr_fic,"%s ",motfic);
          while (strcmp(motfic,"</mots>")!=0)
          {
            strcat(chaineDescripteur,motfic);
            strcat(chaineDescripteur," ");
            fscanf(ptr_fic,"%s ",motfic);
          }
        }
        if(strcmp(motfic,"</fin>")==0)
        {
          EMPILE_JULIEN(&mapile,chaineDescripteur);
        }

      }
      fclose(ptr_fic);
  }
  else
  {
     fprintf(stderr, "ERREUR :  PB avec fonction empiler descripteurs\n");
  }
  return mapile;
}


int verifyPertinence(char * mot)
{
  int conversion=(int)mot[0];
  //cas du "à"
  if(conversion==-32)
  {
    return 0;
  }
  if(strlen(mot)<=3)
  {
    return 0;
  }
  //ponctuation
  if(strcasecmp(mot,":")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,",")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,".")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,";")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"?")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"!")==0)
  {
    return 0;
  }

  //articles
  if(strcasecmp(mot,"le")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"la")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"les")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"de")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"du")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"des")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"sur")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"dans")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"pour")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"un")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"une")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"a")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"et")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"est")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"ces")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"par")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"que")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"qui")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"au")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"en")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"son")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"sa")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"ses")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"se")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"sa")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"pas")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"ne")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"ou")==0)
  {
    return 0;
  }if(strcasecmp(mot,"avec")==0)
  {
    return 0;
  }

  // pronoms
  if(strcasecmp(mot,"je")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"tu")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"il")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"elle")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"on")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"nous")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"vous")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"ils")==0)
  {
    return 0;
  }

  //quelques verbes
  if(strcasecmp(mot,"ont")==0)
  {
    return 0;
  }
  if(strcasecmp(mot,"sont")==0)
  {
    return 0;
  }

  return 1;
}


/*
=========================================================================
Code associé a ma PILE_JULIEN dynamique étant donné que chacun utilise sa propre
structure on l'inclu dans notre code de gestion d'indexation.
=========================================================================
*/
void INIT_PILE_JULIEN(PILE_JULIEN* pilefournie)
{
  *pilefournie=NULL;
}

void AFFICHE_PILE_JULIEN(PILE_JULIEN* pilefournie)
{
  if (pilefournie == NULL)
  {
      exit(EXIT_FAILURE);
  }
  cellule *actuelle = (cellule*) *pilefournie;
  while (actuelle != NULL)
  {
    printf("%s\n",actuelle->chaine );
    actuelle = actuelle->suivant;
  }
}

int PILE_JULIEN_EST_VIDE(PILE_JULIEN* pilefournie)
{
  if(*pilefournie==NULL)
  {
    return 1;
  }
  return 0;
}

void EMPILE_JULIEN(PILE_JULIEN* pilefournie,char* chaine)
{
    cellule *nouvelle = malloc(sizeof(*nouvelle));
    char* machaine=strdup(chaine);
    if (pilefournie == NULL || nouvelle == NULL)
    {
        exit(EXIT_FAILURE);
    }
    if(*pilefournie==NULL)
    {
      *pilefournie=nouvelle;
      cellule *actuelle = (cellule*) *pilefournie;
      actuelle->chaine=machaine;
      actuelle->suivant=NULL;
    }
    else
    {
      nouvelle->suivant=(cellule*) *pilefournie;
      *pilefournie=nouvelle;
      nouvelle->chaine=machaine;
    }
}

char* DEPILE_JULIEN(PILE_JULIEN* pilefournie)
{
  if(!PILE_JULIEN_EST_VIDE(pilefournie))
  {
    char* chaine;
    cellule *actuelle = (cellule*) *pilefournie;
    chaine=actuelle->chaine;
    *pilefournie=actuelle->suivant;
    if(actuelle!=NULL)
    {
      free(actuelle);
    }

    return chaine;
  }
}
