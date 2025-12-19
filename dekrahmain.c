#include <stdio.h>
#include <string.h>
#include "dekrah.h"



int main(void) {
File f;
initFile(&f);


Client c;
for (int i = 1; i <= 5; ++i) {
c.id = 100 + i;
snprintf(c.nom, sizeof(c.nom), "Client%d", i);
enfiler(&f, c);
}


// chercher récursivement
char target[30] = "Client3";
int id = trouverClientRec(&f, 0, target);
if (id != -1) printf("Client '%s' trouvé: id=%d\n", target, id);
else printf("Client '%s' non trouvé\n", target);


afficherFile(&f);
return 0;
}