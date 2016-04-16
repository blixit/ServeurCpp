#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <winsock2.h>
#include <unistd.h>

#include "sockUtils.h"

/*int sizeOfChar2(void** tab){
    if(!tab)
        return 0;
    int i=0;
    while(*tab!=ENDOFCHAR2) // en tant qu'adresse, malloc ne mettra jamais 1 dans un char*. Cette valeur est donc d�fini par moi.
    {
        //printf("%s\n",*tab);
        tab++; i++;
    }
    return i;
}*/

char** explode(const char* delimiter, char* str, int* nbElements){
    int i=0,cpt=1,sz = strlen(str);
    char** dest;

    if(sz == 0){
        return NULL;
    }

    char source[sz];
    strcpy(source,str);

    while(i<sz && (cpt += source[i++]==delimiter[0] ? 1:0 ));
    //printf("i=%d  cpt=%d\n",i,cpt);

    //si on ne trouve pas le symbole ou bien s'il se trouve en fin de chaine
    if(cpt==1 || (cpt==2 && source[sz-1]==delimiter[0])){
        if(nbElements != NULL)
        *nbElements = 1;
        //si le delimiter est en fin de chaine, on le supprime
        if(source[sz-1]==delimiter[0])
            source[sz-1] = '\0';
        dest = (char**)malloc(cpt*sizeof(char*));
        *dest = (char*)malloc(sz);
        strcpy(*dest,source);
        return dest;
    }

    //si le param�tre 'nbElements', non initialis� avec -1, est fourni
    if(nbElements != NULL)
        *nbElements = cpt;
    dest = (char**)calloc((cpt+1),sizeof(char*)); //+1 pour mettre le dernier � NULL
    char * pch;
    pch = strtok (source,delimiter);
    i = 0;
    while (pch != NULL)
    {
        sz = strlen(pch);
        dest[i] = (char*)calloc(sz+1,1);
        strcpy(dest[i],pch);
        pch = strtok (NULL, delimiter);
        i++;
    }
    dest[i] = NULL;//ENDOFCHAR2;
    return dest;
}

int sendMessage(int csock, const char* message,int flags){
    return send(csock,message,strlen(message),flags);
}

/**
*   Lit le buffer et inscrit la ligne 'line' dans le tampon 'read'
*   @param buffer Cha�ne contenant le buffer � traiter
*   @param read La cha�ne o� on stocke les caract�res lus
*   @param line La ligne � partir de laquelle on lit le buffer
*/
int readBufferLine(const char* buffer, char** read, int line){
    int i = 0, cpt = 0, seek = 0, lastSeek = 0, length, sz = strlen(buffer);
    //si read pointe vers quelque chose, on lib�re son pointeur
    if(*read)
        free(*read);
    *read = NULL;

    //le buffer ne peut pas �tre null. La premi�re ligne est indic� par 1
    if(buffer==NULL || line<=0)
        return -1;

    //on parcourt le buffer ligne par ligne afin de trouver la position de la ligne 'line' (appel�e lastseek)
    while(i<sz && cpt!=line){
        if(buffer[i]=='\n'){
            cpt += 1;
            lastSeek = seek;
            seek = i+1;
        }
        else if(i==sz-1){ //dernier caract�re
            cpt += 1;
            lastSeek = seek;
        }
        i++;
    }
    //si on ne trouve pas la ligne demand�e
    if(cpt!=line)
        return -2;
    //printf("length = %d   i=%d\n",i-2-lastSeek, i);
    /*longueur de la ligne � extraire = diff�rence entre la position actuelle du pointeur (i) et la derni�re position enregistr�e (lastseek). On soustrait �ventuellement les 2 caract�res (\r et \n).*/
    length = (i<sz) ? i-2-lastSeek : i-lastSeek;
    if(length==0)
        return 0;

    //allocation dynamique de la cha�ne
    *read = (char*)calloc(length+1,1);
    if(*read==NULL)
        return -3;
    //on �crit la ligne � extraire � l'aide du pointeur writer
    char* writer = *read;
    i = 0;
    while(i<length){
        *writer = buffer[lastSeek+i++];
        writer++;
    }
    *writer = 0;
    return length;
}
