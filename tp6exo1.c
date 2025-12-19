# include  <stdio.h>

typedef struct {
int id;
char nom[30];
} Client;
typedef struct {
Client tab[50];
int tete, queue, taille;
} File;
void initFile(File *f);
int estVide(File *f);
int estPleine(File *f);
void enfiler(File *f, Client c);
Client defiler(File *f);
void afficherFile(File *f);
int trouverClient(File *f, int index, char nomCherche[30]);
int trouverClientRec(File *f, int index, char nomCherche[30]);