/* permet d'initialiser les valeurs contenues dans le fichier de config */
void init_valuesw();

/* permet de voir dans quel intervalle se situe l'échantillon */ 
int checkInterval(double val, float* tabGap);

/* Permet de récupérer le nombre de fenetres depuis fichier contenant les echantillons */
int getNbFen(char * fichier, char * mode);

/* permet d'initialiser les valeurs d'une matrice à 0 */
int** init_matrice(int size) ;

/* permet de free tous les mallocs de la matrice */
void free_matrice(int ** matrice, int size);

/* permet d'afficher le contenu d'une matrice */
void affiche_matrice(int ** matrice, int size);

/* permet d'écrire le descripteur a partir d'une matrice dans le fichier base_descripteur_son */
void write_in_desc(int** matrice, int size);

/* permet de vérifier si un fichier à déja été indexé */
int read_desc(char * fichier) ;

/* permet de récupérer une matrice à partir d'un fichier  */
int ** getMatriceFromFile(float * tabGap, char* fichier);

/*  */
void setDescriptor(float * tabGap, char* fichier );

/* créé les intervalles */
void createGap(float* tabGap) ;

/* ajoute un fichier dans liste base son */
void addListeBaseSon(char* fichier);

/* lance l'indexation des fichiers contenus dans le répertoire TEST_SON */
void indexFiles();

/* vide le contenu des fichiers de la base de données et réindexe les fichiers */
void resetAndIndex();

/* récupère le nombre de fenetres depuis un descripteur  */
int* getNbFenFromDesc(int * tabret);

/* effectue la comparaison entre les matrices */
int compare_matrices(int ** matrice1, int taille1, int ** matrice2, int taille2, int id );

/*  */
void compare( char* fichier );
