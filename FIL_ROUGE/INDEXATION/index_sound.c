#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define desc "../EXTERN_FILES/database/base_son/base_descripteur_son.txt"
#define new_min(x,y) ((x) <= (y)) ? (x) : (y)
#define new_max(x, y) (((x) >= (y)) ? (x) : (y))
int ID = 0 ;
int echantillons  = 1024 ;
int nbintervalles = 40 ;

void init_valuesw(){

FILE * config = fopen("../EXTERN_FILES/configuration.config", "r");
char val[200] ;

if(config != NULL){
  while (fscanf(config,"%s",val) == 1) {

    if(strcmp(val, "echantillons:") == 0){
      fscanf(config,"%s",val);
      echantillons = atol(val);
    }

    if(strcmp(val, "nbintervalles:") == 0){
      fscanf(config,"%s",val);
      nbintervalles = atol(val);
    }

  }
  fclose(config);
}else{
  printf("Problème avec le fichier de configuration, valeurs par défaut utilisées. \n" );
}


}


/* Retourne intervalle pour chaque valeur du fichier son lue  */
int checkInterval(double val, float* tabGap){
 int i ;
 for (i = 0; i < nbintervalles+1; i++) {
   if (val>=tabGap[i] && val < tabGap[i+1]) {
     return i  ;
   }
 }

return -1 ;
}

/*Permet de savoir le nombre de fenetres dans un fichier binaire*/
int getNbFen(char * fichier, char * mode){

FILE * fic ;
int cpt = 0, ret=0 ;
double val ;
fic = fopen(fichier,mode);

  if (fic != NULL) {
      while(fread(&val,sizeof(double),1,fic)==1){
       cpt ++ ;
         if((cpt%echantillons) == 0){
           ret ++ ;
         }

      }
    fclose(fic);
  }

  return ret ;
}


/* Malloc sur matrice et intialisation des valeurs a 0  */
int** init_matrice(int size) {

  int ** matrice = (int **)malloc(size*sizeof(int));


  for(int i = 0 ; i < size ; i++){
    matrice[i]= (int *)malloc((nbintervalles+1) * sizeof(int));
  }

  //init de la matrice
  for (int a = 0; a < size ; a++) {
    for (int b = 0; b <nbintervalles+1 ; b++) {
      matrice[a][b] = 0 ;
    }
  }

  return matrice ;

}

void free_matrice(int ** matrice, int size){
  for(int i = 0 ; i < size ; i++){
    free(matrice[i]);
  }
  free(matrice);
}


/* Affiche contenu d'une matrice */
void affiche_matrice(int ** matrice, int size) {
  for (int y = 0; y < size ; y++) {
    for (int z = 0; z <nbintervalles+1 ; z++) {
      printf("%d  ",matrice[y][z]  );
    }
    printf("\n" );
  }
}


/* Ecrit contenu d'une matrice dans descripteur */
void write_in_desc(int** matrice, int size) {
  FILE* ecrire ;
  ecrire = fopen(desc,"a");
  fprintf(ecrire, "%s","<id> " );
  fprintf(ecrire, "%d", ID );
  fprintf(ecrire, "%s"," </id>\n" );

  for (int i = 0; i <size ; i++) {
    fprintf(ecrire, "%s","<fenetre> " );
    for (int j = 0; j < nbintervalles+1; j++) {
      fprintf(ecrire,"%d" ,matrice[i][j] );
      fprintf(ecrire,"%c" ,' ' );

    }

          fprintf(ecrire, "%s"," </fenetre>" );
          fprintf(ecrire,"%s" ,"\n" );
  }

fclose(ecrire);
}

/*Lis liste base son et regarde si un fichier a déja été indexé 1 si déja 0 si pas déja */
int read_desc(char * fichier) {

  int check = 0 ;
  FILE* read = fopen("../EXTERN_FILES/database/base_son/Liste_Base_Son.txt", "r");
  char val[200] ;

if(read != NULL){
    while (fscanf(read,"%s",val) == 1) {
      if(strcmp(val, fichier) == 0){
        return 1 ;
      }
    }


    fclose(read);
}

return 0 ;
}

int ** getMatriceFromFile(float * tabGap, char* fichier){


double ch ;
int ech = 0 ;
int nbfenlive = 0 ;
int interval ;
int nbfen = getNbFen(fichier, "rb");
int ** matrice = init_matrice(nbfen+1);
FILE* lire = fopen(fichier,"rb") ;

if(lire != NULL){
    while(fread(&ch ,sizeof(double),1,lire) == 1){
        ech ++ ;
        if(ech%echantillons == 0){
          nbfenlive++ ;
        }
        interval = checkInterval(ch, tabGap);
        matrice[nbfenlive][interval] += 1 ;
    }
    fclose(lire) ;
}

return matrice ;
}


//Lit le fichier bin et créé l'histogramme'
void setDescriptor(float * tabGap, char* fichier ){

  char fileToRead[500] = "../EXTERN_FILES/base_files/DATA_FIL_ROUGE_DEV/TEST_SON/" ;
  strcat(fileToRead,fichier);

  int nbfentotal = getNbFen(fileToRead, "rb");
  int ** matrice = getMatriceFromFile(tabGap,fileToRead);

//affiche_matrice(matriceDesc, nbfentotal+1);

write_in_desc(matrice, nbfentotal+1);

//read_desc();

}


//créé les intervalles pour l'histogramme
void createGap(float* tabGap) {
  int i ;
  float step = (2.0 / nbintervalles) ;
  float start = -1 - step;

  for ( i = 0 ;i <nbintervalles+1; i++) {
    tabGap[i] = start + step ;
    start = start + step ;

  }

}

/* ajoute un fichier à liste base son  */
void addListeBaseSon(char* fichier) {

FILE* check = popen("wc -l ../EXTERN_FILES/database/base_son/Liste_Base_Son.txt", "r");
fscanf(check, "%d", &ID);
fclose(check);

FILE * son = fopen("../EXTERN_FILES/database/base_son/Liste_Base_Son.txt", "a");
fprintf(son, "%s", "<id> ");
fprintf(son, "%d", ID);
fprintf(son, "%s", " </id>");
fprintf(son, "%s", "<fichier> " );
fprintf(son, "%s", fichier );
fprintf(son, "%s\n", " </fichier>" );
fclose(son);
}



/* lance l'indexation */
void indexFiles(){

  init_valuesw();
  float * tabGap = malloc((nbintervalles+1)*sizeof(float)) ;
  createGap(tabGap);

  FILE * conf = fopen(desc,"a");
  fprintf(conf, "%d ", echantillons );
  fprintf(conf, "%d\n", nbintervalles );
  fclose(conf);

FILE * p ;
char res[1000] ;
p = popen("ls ../EXTERN_FILES/base_files/DATA_FIL_ROUGE_DEV/TEST_SON/ | grep .bin", "r");

if(p != NULL){
  while (fscanf(p,"%s",res) == 1) {

    if(!read_desc(res)){
      addListeBaseSon(res);
      setDescriptor(tabGap, res);
      printf("Le fichier %s à été indexé !\n\n",res );
    }else{
      printf("Le fichier %s à dejà été indexé !\n",res );
    }
  }
    fclose(p);
}

}

void resetAndIndex() {

  fclose(fopen(desc, "w"));
  fclose(fopen("../EXTERN_FILES/database/base_son/Liste_Base_Son.txt", "w"));
  indexFiles();

}


/* Récupère le nombre de fenetre pour chaque descripteur à partir du fichier contenant les descripteurs */
int* getNbFenFromDesc(int * tabret){
FILE * base = fopen(desc, "r");
char current[500];
int nbid=0 ;
int nbfen =0;

  while (fscanf(base,"%s",current) == 1) {
      if(strcmp(current,"<id>") == 0){
        nbid++ ;
      }
  }

/*tableau contenant à l'id correspondant(de chaque descripteur) le nombre de fenetres*/
tabret = malloc(nbid*sizeof(int));
for (int i = 0; i < nbid; i++) {
  tabret[i] = 0 ;
}
/*reset le pointeur au début du fichier */
rewind(base) ;
nbid = -1 ;
while (fscanf(base,"%s",current) == 1) {
    if(strcmp(current,"<id>") == 0){
      nbid++ ;
    }

    if(strcmp(current,"<fenetre>") == 0){
        tabret[nbid]++ ;
    }

}

fclose(base);
return tabret ;

}



int compare_matrices(int ** matrice1, int taille1, int ** matrice2, int taille2, int id ){
// 70s corpusfi 3s

/* Récupère le nom depuis l'id */
FILE* read = fopen("../EXTERN_FILES/database/base_son/Liste_Base_Son.txt", "r");
char nomFichier[500] ;
if(read != NULL){
    while (fscanf(read,"%s",nomFichier) == 1) {
      if(strcmp(nomFichier,"<id>") == 0){

      }else if( atoi(nomFichier) == id ){
          /* permet de récupérer le nom du fichier */
          fscanf(read,"%s",nomFichier);
          fscanf(read,"%s",nomFichier);
          break;
      }
    }
    fclose(read);
}

/* tableau des distances */
int tabDistance[taille2-taille1];

/* index pour tabDistance */
int cptDistance = 0 ;

/* booleen pour savoir si fichier existe */
int fichierExiste = 0 ;

/* tableau contenant distance et suivante */
int tabExiste[2];

/* index pour tabExiste */
int cptExiste = 0 ;

/* Variable gardant la ditance max */
int distancemax = 0 ;

/* contient la position de la distance max (plus haute probabilité de match avec le jingle)*/
float distancemaxpos = 0 ;
/* echelle de temps pour identifier l'endroit ou le jingle est trouvé */
float echelle = echantillons / 16000.0 ;

/* variable temporaire pour chaque distance */
int distance = 0 ;

/* indice deuxieme matrice */
int k =0 ;

int borneinf = 0 ;

/* parcours de tout le fichier a comparer */
while (borneinf <= (taille2 - taille1)) {

/* parcours du du fichier a comparer */
  for (int i = 0; i < taille1 ; i++) {
    for (int j = 0; j < nbintervalles+1 ; j++) {
      distance += new_min(matrice1[i][j], matrice2[i+borneinf][j]);

    }
  }



  /* récupère la distance max  */
  distancemax = new_max(distance, distancemax);

 /* permet de récupérer l'endroit ou se situe la distance max */
  if(distancemax == distance){
    distancemaxpos = borneinf ;
  }

//  printf("endroit : %f %f Distance : %d\n", (borneinf*echelle), (borneinf+taille1)*echelle,distance );

/* si on trouve que le fichier n'existe pas */
  if(!fichierExiste){
    tabExiste[cptExiste] = distance ;
    cptExiste++ ;
    /* si le tableau tabExiste a les deux cases pleines */
    if(cptExiste == 2){
      if( (tabExiste[0] - tabExiste[1]) >1000 ||  (tabExiste[0] - tabExiste[1]) < -1000 ){
          fichierExiste = 1 ;
      }
      cptExiste = 0 ;
    }else if(taille1 == taille2){
        fichierExiste = 1 ;
    }
  }
  tabDistance[cptDistance] = distance ;
  cptDistance++;
  //printf("%d\n", fichierExiste );
  borneinf++;
  distance = 0 ;

}




if(fichierExiste){
  char aff_prev[2000] = "Une ressemblance à été trouvée dans ";
  strcat(aff_prev,nomFichier);
  strcat(aff_prev," à l'instant : %fs  \n\n");
  for(int ind = 0 ; ind <= taille2-taille1 ; ind++){

    if(tabDistance[ind] > (distancemax-1000) ){
    printf(aff_prev, (ind*echelle) );

    }
  }

  char affichage[2000] = "Trouvé dans ";
  strcat(affichage, nomFichier);
  strcat(affichage, " postiton : ");

  printf("%s %fs  %fs\n\n" , affichage, (distancemaxpos*echelle),((distancemaxpos+taille1)*echelle));

  /* concaténer pour que ça utilise nomFichier */
  //system("play ../EXTERN_FILES/base_files/DATA_FIL_ROUGE_DEV/TEST_SON/corpus_fi.wav");
}


return 0 ;
}

/* prend fichier à comparer en parametre et revoie le nom des fichiers qui correspondent */
void compare( char* fichier ){


char req[1000] = "Requête : " ;
strcat(req,fichier);
printf("%s\n", req );

  FILE * base = fopen(desc, "r");

    char valconf[50];
    fscanf(base,"%s",valconf);
    echantillons = atol(valconf);
    fscanf(base,"%s",valconf);
    nbintervalles = atol(valconf);

    float *tabGap = malloc((nbintervalles+1)*sizeof(float)) ;
    createGap(tabGap);

  char current[500];
  int idlive = -1 ;
  int fenetrelive = 0 ;
  int echantillonlive = 0 ;
  int ** matriceLue  ;
  int indicetabIdFen = -1 ;
  int * tabIdFen = getNbFenFromDesc(tabIdFen) ;

  /* Matrice à comparer */
  int ** matriceToCompare = getMatriceFromFile(tabGap, fichier);
  int nbFenToCompare = getNbFen(fichier, "rb");


  while (fscanf(base,"%s",current) == 1) {

    /* Permet de savoir à quel id on est et reset la valeur de fenetre live*/
    if(strcmp(current,"<id>") == 0){
      /*initialise la matrice à la taille correspondante (nieme valeur de tabIdFen) */
      if (indicetabIdFen>=0) {
        compare_matrices(matriceToCompare,nbFenToCompare+1,matriceLue,tabIdFen[indicetabIdFen], idlive);
        free_matrice(matriceLue, tabIdFen[indicetabIdFen]);
      }
        indicetabIdFen++ ;
        matriceLue = init_matrice(tabIdFen[indicetabIdFen]) ;
        idlive ++ ;
        fenetrelive = 0 ;
    }
    /* Pour éviter les problèmes quand on lit l'id et qu'il le prend pour une valeur*/
    if (strcmp(current,"</id>") == 0 ) {
      echantillonlive = 0 ;
    }
    /* Permet de savoir le nb de la fenetre et reset la valeur de l'échantillon */
    if(strcmp(current,"</fenetre>") == 0){
      fenetrelive++;
      echantillonlive = 0 ;
    }
    /* ajoute valeur dans la matrice et update à quel échantillon on est (41 max) */
    if(strcmp(current,"<id>") != 0 && strcmp(current,"</id>") != 0 && strcmp(current,"<fenetre>") != 0 && strcmp(current,"</fenetre>") != 0 ){
      matriceLue[fenetrelive][echantillonlive] = atol(current) ;
      echantillonlive ++ ;
    }
   /*printf("id :  %d\n",idlive );
    printf("echantillon : %d\n", echantillonlive );
    printf("fenetre : %d\n", fenetrelive );
    printf("mot :  %s\n",current);
    printf("------------------------\n" );*/
  }
  compare_matrices(matriceToCompare,nbFenToCompare+1,matriceLue,tabIdFen[indicetabIdFen], idlive);

free_matrice(matriceLue, tabIdFen[indicetabIdFen]);
free(tabIdFen);
free(tabGap);
fclose(base);

}



/*
int main(int argc, char const *argv[]) {

  //indexFiles();
  //resetAndIndex();
  compare("../EXTERN_FILES/base_files/DATA_FIL_ROUGE_DEV/TEST_SON/jingle_fi.bin");

  return 0;
}*/
