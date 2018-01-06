#include <stdio.h>
#include <stdlib.h>
#include "index_txt.h"
#define TAILLE_MAX 10000000



int main(int argc, char const *argv[]) {

    rmThenStartIndexation();

  return 0;
}

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
  strcpy(cmd,"rm -r temp_txt");
  system(cmd);

  // création des répertoires cibles sur lesquels on va travailler
  strcpy(cmd,"mkdir temp_txt");
  system(cmd);

  strcpy(cmd,"mkdir ./temp_txt/cpy");
  system(cmd);

  strcpy(cmd,"mkdir ./temp_txt/pure_txts");
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

  startIndexation();
}

void startIndexation()
{
  char cmd[1000];
  FILE * ptr_fic;
  char nom_fic[100];


  strcpy(cmd, "ls ");
  strcat(cmd, PATHFILES);
  strcat(cmd, " > ./temp_txt/fic_tempLIST.xml");
  fflush(stdout);
  system(cmd);

  ptr_fic = fopen("./temp_txt/fic_tempLIST.xml", "r");

  if( ptr_fic != NULL)
  {
      fscanf(ptr_fic, "%s", nom_fic);  /* %*s INGNORE LA CHAINE LUE */
      while ( !feof(ptr_fic) )
      {
          checkForUpdates(nom_fic);
          fscanf( ptr_fic, "%s", nom_fic);
      }
      fclose(ptr_fic);

  }
  else
  {
     fprintf(stderr, "ERREUR :  PB avec fic_tempLIST.xml\n");
  }

  createFicTempTableIndexTexte();
  printf("tous les documents xml ont bien été indexés !\n");
}

void indexFile(char* name,int id) {
  char cmd[1000];
  int c;
  char car;
  FILE * ptr_fic;
  char ligne[100];

  strcpy(cmd, "cat ");
  strcat(cmd, PATHFILES);
  strcat(cmd, name);
  strcat(cmd," > ./temp_txt/cpy/" );
  strcat(cmd,name);
  system(cmd);

  sortMeBalises(name);
  searchOccurences(name,id);
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
  strcat(cmd, " > ./temp_txt/trouveOuPas.xml");
  system(cmd);

  ptr_fic = fopen("./temp_txt/trouveOuPas.xml", "r");

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
      fclose(ptr_fic);
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

  strcpy(filename,"./temp_txt/pure_txts/");
  strcat(filename,name);

  strcpy(sourceName,"./temp_txt/cpy/");
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
           if(car!='>')
           {
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

void searchOccurences(char * name,int id)
{
  FILE * fichier_a_traiter;
  FILE * database;
  char filename[100000];
  char mot[100000];
  int param;
  int cpt=0;
  int index=0;
  int occurences=0;
  char chaineDescripteur[100000];

  param=recupParamConfigTexte();

  strcpy(filename,"./temp_txt/pure_txts/");
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
    fprintf(stderr, "ERREUR :  PB avec fonction searchOccurences\n");
  }

  char * tabchaines[cpt];
  strcpy(filename,"./temp_txt/pure_txts/");
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
    fprintf(stderr, "ERREUR :  PB avec fonction searchOccurences\n");
  }

  index=0;
  int i=0;
  int verif;
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
  database=fopen(DATABASE,"a+");

  char conversion2[50];
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
    fprintf(stderr, "ERREUR :  PB avec fonction searchOccurences\n");
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
  config = fopen("../EXTERN_FILES/configuration.config", "r");
  if(config!=NULL)
  {
    while (!feof(config))
    {
      fscanf(config,"%s",chaine);
      if(strcmp(chaine,"txt_nboccurences")==1)
      {
        booltest=1;
      }
      if(booltest==1)
      {
        value=atol(chaine);
        if(value!=0)
        {
          return value;
        }
      }
    }
    fclose(config);
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
      fscanf(ptr_fic, "%s", nom_fic);  /* %*s INGNORE LA CHAINE LUE */
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
    return cpt/6;
  }
  else
  {
    return cpt;
  }
}

void createFicTempTableIndexTexte()
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
  ptr_ficToWrite = fopen("temp_txt/table_index_texte_temp","w");
  if( ptr_ficToRead != NULL && ptr_ficToWrite != NULL)
  {
      fscanf(ptr_ficToRead, "%s", motLu);  /* %*s INGNORE LA CHAINE LUE */
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
  FILE* check;
  check = popen("wc -l ./temp_txt/table_index_texte_temp", "r");
  fscanf(check, "%d", &verif);
  fclose(check);
  while (verif>0) {

    generateTableIndexTexte();
    check = popen("wc -l ./temp_txt/table_index_texte_temp", "r");
    fscanf(check, "%d", &verif);
    fclose(check);
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

  ptr_ficToRead = fopen("temp_txt/table_index_texte_temp", "r");
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
          strcat(chaineConstruite," ;");
          tab[index]=cpt;
          index++;
        }
        free(otherId);
        free(otherMot);
        free(otherNbOcc);
      }

      strcat(chaineConstruite," </key_nb> ");
      fprintf(ptr_ficToWrite,"%s\n",chaineConstruite);
      system("sed 1d ./temp_txt/table_index_texte_temp -i");

      int cpt2=0;
      int cpt3=1;
      while (cpt2<index)
      {
        strcpy(cmd,"sed ");
        char conversion[50];
        snprintf(conversion,sizeof(conversion),"%d",tab[cpt2]-cpt3);
        strcat(cmd,conversion);
        strcat(cmd,"d ./temp_txt/table_index_texte_temp -i");
        system(cmd);
        cpt2++;
        cpt3++;
      }
      fclose(ptr_ficToRead);
      fclose(ptr_ficToWrite);
  }


}



int verifyPertinence(char * mot)
{
  int conversion=(int)mot[0];
  //cas du "à"
  if(conversion==-32)
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
