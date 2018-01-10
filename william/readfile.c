#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define desc "fic.txt"
int ID = 0 ;
int echantillons  = 1024 ;
int nbintervalles = 40 ;


void init_values(){

FILE * config = fopen("configuration.config", "r");
char * val = malloc(sizeof(char));

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
//  fclose(config);
}else{
  printf("Problème avec le fichier de configuration, valeurs par défaut utilisées. \n" );
}

//free(val);

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
         if(cpt%echantillons == 0){
           ret ++ ;
         }

      }
    fclose(fic);
  }

  return ret ;
}


/* Malloc sur matrice et intialisation des valeurs a 0  */
int** init_matrice( int size) {

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
  //  free(matrice[i]);
  }
//  free(matrice);
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
  char * tab = malloc(sizeof(char));

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

//free(tab);
fclose(ecrire);
}

/*Lis liste base son et regarde si un fichier a déja été indexé 1 si déja 0 si pas déja */
int read_desc(char * fichier) {

  int check = 0 ;
  FILE* read = fopen("Liste_Base_Son.txt", "r");
  char* val = malloc(sizeof(char));

if(read != NULL){
    while (fscanf(read,"%s",val) == 1) {
      if(strcmp(val, fichier) == 0){
        return 1 ;
      }
    }
  //  free(val);
    //si fclose plante
    //fclose(read);
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

  char fileToRead[100] = "TEST_SON/" ;
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

FILE* check = popen("wc -l Liste_Base_Son.txt", "r");
fscanf(check, "%d", &ID);
fclose(check);

FILE * son = fopen("Liste_Base_Son.txt", "a");
fprintf(son, "%s", "<id> ");
fprintf(son, "%d", ID);
fprintf(son, "%s", " </id>");
fprintf(son, "%s", "<fichier> " );
fprintf(son, "%s", fichier );
fprintf(son, "%s\n", " </fichier>" );
fclose(son);
}



/* lance l'indexation */
void IndexFiles(float * tabGap){
FILE * p ;
char * res = malloc(sizeof(char));
p = popen("ls TEST_SON/ | grep .bin", "r");

if(p != NULL){
  while (fscanf(p,"%s",res) == 1) {

    if(!read_desc(res)){
      addListeBaseSon(res);
      setDescriptor(tabGap, res);
      printf("Le fichier %s à été indexé !\n",res );
    }else{
      printf("Le fichier %s à dejà été indexé !\n",res );
    }
  }
}

free(res);
}

void resetAndIndex(float * tabGap) {
  system("echo > fic.txt ");
  system("echo > Liste_Base_Son.txt ");
//  IndexFiles(tabGap);
}


/* Récupère le nombre de fenetre pour chaque descripteur à partir du fichier contenant les descripteurs */
int* getNbFenFromDesc(int * tabret){
FILE * base = fopen(desc, "r");
char * current = malloc(sizeof(char));
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
//free(current);
fclose(base);
return tabret ;

}


int compare_matrices(int ** matrice1, int taille1, int ** matrice2, int taille2, int id ){
int k = 0 ;
  for (int i = 0; i < taille1; i++) {
    for (int j = 0; j < nbintervalles+1; j++) {
      if(matrice1[i][j] == matrice2[k][j] && matrice1[i][j] != 0){
        printf("ID : %d  [%d][%d] = %d \n",id, k,j, matrice1[k][j] );
      }
    }
    k++ ;
  }
return 0 ;
}

/* prend fichier à comparer en parametre et revoie le nom des fichiers qui correspondent */
void compare(float * tabGap, char* fichier ){


  FILE * base = fopen(desc, "r");
  char * current = malloc(sizeof(char));
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
        compare_matrices(matriceToCompare,nbFenToCompare,matriceLue,tabIdFen[indicetabIdFen], idlive);
      //  free_matrice(matriceLue, tabIdFen[indicetabIdFen]);
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
//free_matrice(matriceLue, tabIdFen[indicetabIdFen]);
//free(current);
fclose(base);

}




int main(int argc, char const *argv[]) {

init_values();
 float *tab = malloc((nbintervalles+1)*sizeof(float)) ;
  createGap(tab);
  IndexFiles(tab);
  //compare(tab, "TEST_SON/jingle_fi.bin");
//  resetAndIndex(tab);




  return 0;
}
