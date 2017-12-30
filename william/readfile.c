#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define fenetre 1024
#define histo 30
#define desc "fic.txt"
int ID = 0 ;

/* Retourne intervalle pour chaque valeur du fichier son lue  */
int checkInterval(double val, float* tabGap){
 int i ;

 for (i = 0; i < 41; i++) {
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
         if(cpt%fenetre == 0){
           ret ++ ;
         }

      }
  }
fclose(fic);
  return ret ;
}


/* Malloc sur matrice et intialisation des valeurs a 0  */
int** init_matrice(int** matrice, int size) {

  int i ;
  matrice  = (int **)malloc((size)*sizeof(int));

  for(i = 0 ; i < size ; i++){
    matrice[i]= (int *)malloc(41 * sizeof(int));
  }

  //init de la matrice
  for (int a = 0; a < size ; a++) {
    for (int b = 0; b <41 ; b++) {
      matrice[a][b] = 0 ;
    }
  }

  return matrice ;


}

/* Affiche contenu d'une matrice */
void affiche_matrice(int ** matrice, int size) {
  for (int y = 0; y < size ; y++) {
    for (int z = 0; z <41 ; z++) {
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
    for (int j = 0; j < 41; j++) {
      fprintf(ecrire,"%d" ,matrice[i][j] );
      fprintf(ecrire,"%c" ,' ' );

    }

          fprintf(ecrire, "%s"," </fenetre>" );
          fprintf(ecrire,"%s" ,"\n" );
  }


fclose(ecrire);
}

/*Lis descripteur d'entiers */
int read_desc(char * fichier) {

  int check = 0 ;
  FILE* read ;
  read = fopen("Liste_Base_Son.txt", "r");
  char* val = malloc(sizeof(char));

if(read != NULL){
    while (fscanf(read,"%s",val) == 1) {
      if(strcmp(val, fichier) == 0){
        check = 1 ;
      }
    }
}

//si fclose plante
//fclose(read);
return check ;
}



//Lit le fichier bin et créé l'histogramme'
void setDescriptor(float * tabGap, char* fichier ){

  char separateur[] = "\n<fenetre>\n";
  char fileToRead[100] = "TEST_SON/" ;
  strcat(fileToRead,fichier);
  FILE* lire;

  double ch ;
  int i ;
  int ech = 0 ;
  int nbfenlive = 0 ;
  int nbfentotal = getNbFen(fileToRead, "rb");
  int interval ;
  int ** matriceDesc ;
  char *tab = malloc(sizeof(char)) ;

  lire = fopen(fileToRead,"rb");  // r for read, b for binary


  //nblignes
 matriceDesc = init_matrice(matriceDesc, nbfentotal+1);


if (lire != NULL) {

    while(fread(&ch ,sizeof(double),1,lire) == 1){
      //increment compteur plage
      ech++ ;

      //fenetre tout les 1024
      if(ech%fenetre == 0){
        nbfenlive++ ;
      }

      interval = checkInterval(ch, tabGap);
      matriceDesc[nbfenlive][interval] += 1 ;
    //  printf("FENETRE :%d  INTERVAL : %d  VALEUR : %d \n",nbfenlive,interval,matriceDesc[nbfenlive][interval] );

   }

}

fclose(lire);

//affiche_matrice(matriceDesc, nbfentotal+1);

write_in_desc(matriceDesc, nbfentotal+1);

//read_desc();

}


//créé les intervalles pour l'histogramme
void createGap(float* tabGap) {
  int i ;
  float step = 0.05 ;
  float start = -1.05;

  for ( i = 0 ;i <41; i++) {
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
fclose(base);
return tabret ;

}

void compare(){
  FILE * base = fopen(desc, "r");
  char * current = malloc(sizeof(char));
  int idlive = -1 ;
  int fenetrelive = 0 ;
  int echantillonlive = 0 ;
  int ** matriceLue ;
  int indicetabIdFen = -1 ;
  int * tabIdFen = getNbFenFromDesc(tabIdFen) ;

  while (fscanf(base,"%s",current) == 1) {

    /* Permet de savoir à quel id on est et reset la valeur de fenetre live*/
    if(strcmp(current,"<id>") == 0){
      /*initialise la matrice à la taille correspondante (nieme valeur de tabIdFen) */
        indicetabIdFen++ ;
        matriceLue = init_matrice(matriceLue, tabIdFen[indicetabIdFen]) ;
        idlive ++ ;
        fenetrelive = 0 ;
    }
    /* Permet de savoir le nb de la fenetre et reset la valeur de l'échantillon */
    if(strcmp(current,"</fenetre>") == 0){
      fenetrelive++;
      echantillonlive = 0 ;
    }
    /* ajoute valeur dans la matrice et update à quel échantillon on est (41 max) */
    if(strcmp(current,"<id>") != 0 && strcmp(current,"</id>") != 0 && strcmp(current,"<fenetre>") != 0 && strcmp(current,"</fenetre>") != 0 ){
      echantillonlive ++ ;

      matriceLue[fenetrelive][echantillonlive] = atol(current) ;
    }

    printf("id :  %d\n",idlive );
    printf("echantillon : %d\n", echantillonlive );
    printf("fenetre : %d\n", fenetrelive );
    printf("mot :  %s\n",current);
  }

fclose(base);

}




int main(int argc, char const *argv[]) {


  float * tab = malloc(40*sizeof(float)) ;
  createGap(tab);
  IndexFiles(tab);
  compare();
//  read_desc();



  return 0;
}
