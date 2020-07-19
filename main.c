#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Map.h"
#include "list.h"
#include "SortedMap.h"

typedef struct{
char pos[500];
char id[500];
char created[500];
char modified[500];
char title[500];
char url[500];
char title_type[500];
char description[500];
char rating[500];
char runtime[500];
char years[500];
char genres[500];
char votes[500];
char release_date[500];
char director[500];
list* listnombres;
char fav;
}peliculas;
typedef struct{
    SortedMap *listpelis;
    char nombre[30];
    char puntos[30];
}lista;
int cmp(const void * key1, const void * key2) {
    const char * Key1 = (char*)key1;
    const char * Key2 = (char*)key2;
    return (strcmp(key2,key1));

}
const char *get_csv_field (char * tmp, int k) {
    //se crea una copia del string tmp

    int open_mark = 0;
    char* ret=(char*) malloc (100*sizeof(char));
    int ini_i=0, i=0;
    int j=0;
    while(tmp[i+1]!='\0'){

        if(tmp[i]== '\"'){
            open_mark = 1-open_mark;
            if(open_mark) ini_i = i+1;
            i++;
            continue;
        }

        if(open_mark || tmp[i]!= ','){
            if(k==j) ret[i-ini_i] = tmp[i];
            i++;
            continue;
        }

        if(tmp[i]== ','){
            if(k==j) {
               ret[i-ini_i] = 0;
               return ret;
            }
            j++; ini_i = i+1;
        }

        i++;
    }

    if(k==j) {
       ret[i-ini_i] = 0;
       return ret;
    }


    return NULL;
}

void cargar_lista(SortedMap* ,SortedMap*);
void ver_lista_de_peliculas(SortedMap*);
void ver_peliculas_de_una_lista(SortedMap*);
void ver_informacion_de_una_pelicula(SortedMap*);
void marcar_pelicula_como_preferida(SortedMap*,SortedMap*);
void recomendar_pelicula(SortedMap*);

int main(){
    int entrada;
    SortedMap *mapa_pelis=createSortedMap(cmp),*mapa_listpeli=createSortedMap(cmp);
    while (entrada!=7){
        printf("****************************************\n");
        printf("[1] Cargar lista de peliculas\n");
        printf("[2] Ver listas de peliculas\n");
        printf("[3] Ver peliculas de una lista\n");
        printf("[4] Ver informacion de una pelicula\n");
        printf("[5] Marcar pelicula como preferida\n");
        printf("[6] Recomendar pelicula\n");
        printf("[7] Salir\n");
        printf("****************************************\n");
        do{
           scanf("%d",&entrada);
           if(entrada<=0||entrada>7){
           printf("Ingrese numero valido\n");
          }
        }while(entrada<=0||entrada>7);
        if(entrada==1){
        cargar_lista(mapa_pelis,mapa_listpeli);
        }
        if(entrada==2){
            ver_lista_de_peliculas(mapa_listpeli);
        }
        if(entrada==3){
            ver_peliculas_de_una_lista(mapa_listpeli);
        }
        if(entrada==4){
            ver_informacion_de_una_pelicula(mapa_pelis);
        }
        if(entrada==5){
            marcar_pelicula_como_preferida(mapa_pelis,mapa_listpeli);
        }
        if(entrada==6){
            recomendar_pelicula(mapa_listpeli);
        }
    }

    return 0;
}
void cargar_lista(SortedMap *mapa_pelis,SortedMap *mapa_listpeli){
    char *nombre_list;
    nombre_list=(char*)malloc(sizeof(char)*300);
    char temporal[300];
    char line[1024];
    char *aux1;
    FILE * file;
    lista *lista_peliculas;
    lista_peliculas=(lista*)malloc(sizeof(lista));
    lista_peliculas->listpelis=createSortedMap(cmp);
    strcpy(lista_peliculas->puntos,"0");
    peliculas *peli,*aux2;
    printf ("***********************************\n");
    printf ("                                     \n");
    printf (" Ingrese el nombre de la lista\n");
    printf ("                                     \n");
    printf ("***********************************\n");
    do{
        scanf("%s",nombre_list);
        strcpy(lista_peliculas->nombre,nombre_list);
        strcpy (temporal,nombre_list);
        strcat(temporal,".csv");
        file=fopen(temporal,"r");
        if(file==NULL){
            printf ("***********************************\n");
            printf ("                                     \n");
            printf ("    Ingrese una lista valida\n");
            printf ("                                     \n");
            printf ("***********************************\n");
        }
    }while(file==NULL);

    fgets(line,1023,file);
    while(fgets(line,1023,file)!=NULL){
        peli=(peliculas*)malloc(sizeof(peliculas));
        peli->listnombres=list_create_empty();
        peli->fav='N';
        aux1 = get_csv_field(line, 0);
        strcpy(peli->pos,aux1);
        aux1 = get_csv_field(line,1);
        strcpy(peli->id,aux1);
        aux1 = get_csv_field(line,2);
        strcpy(peli->created,aux1);
        aux1 = get_csv_field(line,3);
        strcpy(peli->modified,aux1);
        aux1 = get_csv_field(line,4);
        strcpy(peli->description,aux1);
        aux1 = get_csv_field(line,5);
        strcpy(peli->title,aux1);
        aux1 = get_csv_field(line,6);
        strcpy(peli->url,aux1);
        aux1 = get_csv_field(line,7);
        strcpy(peli->title_type,aux1);
        aux1 = get_csv_field(line,8);
        strcpy(peli->rating,aux1);
        aux1 = get_csv_field(line,9);
        strcpy(peli->runtime,aux1);
        aux1 = get_csv_field(line,10);
        strcpy(peli->years,aux1);
        aux1 = get_csv_field(line,11);
        strcpy(peli->genres,aux1);
        aux1 = get_csv_field(line,12);
        strcpy(peli->votes,aux1);
        aux1 = get_csv_field(line,13);
        strcpy(peli->release_date,aux1);
        aux1 = get_csv_field(line,14);
        strcpy(peli->director,aux1);
        list_push_back(peli->listnombres,nombre_list);
        aux2=searchSortedMap(mapa_pelis,peli->id);
        if(aux2==NULL){
           insertSortedMap(mapa_pelis,peli->id,peli);
           insertSortedMap(lista_peliculas->listpelis,peli->rating,peli);
        }else{
            insertSortedMap(lista_peliculas->listpelis,aux2->rating,aux2);
            list_push_back(aux2->listnombres,nombre_list);
            free(peli);
        }

       }
       insertSortedMap(mapa_listpeli,lista_peliculas->nombre,lista_peliculas);
       printf("===================================\n");
       printf("Se agrego exitosamente a la lista \n");
       printf("===================================\n");
    }

void ver_lista_de_peliculas(SortedMap *mapa_listpeli){
    lista *aux;
    aux=firstSortedMap(mapa_listpeli);
    while(aux!=NULL){
        printf("%s\n",aux->nombre);
        aux=nextSortedMap(mapa_listpeli);
    }

}

void ver_peliculas_de_una_lista(SortedMap*mapa_listpeli){
    lista* aux;
    char nombrelista[30];
    peliculas *auxpeli;
    printf ("***********************************\n");
    printf ("                                     \n");
    printf (" Ingrese el nombre de la lista\n");
    printf ("                                     \n");
    printf ("***********************************\n");
    scanf("%s",nombrelista);
    aux=searchSortedMap(mapa_listpeli,nombrelista);
    auxpeli=firstSortedMap(aux->listpelis);
    while(auxpeli!=NULL){
        printf("Nombre: %s\n",auxpeli->title);
        printf("Id: %s\n",auxpeli->id);
        printf("Rating: %s\n",auxpeli->rating);
        auxpeli=nextSortedMap(aux->listpelis);

    }

}

void ver_informacion_de_una_pelicula(SortedMap* mapa_pelis){
    peliculas *aux;
    char idpelicula[30];
    char *nombrelist;
    printf ("*****************************************\n");
    printf ("                                     \n");
    printf ("    Ingrese el id de la pelicula\n");
    printf ("                                     \n");
    printf ("*****************************************\n");
    scanf("%s",idpelicula);
    aux=searchSortedMap(mapa_pelis,idpelicula);
    printf("Titulo: %s\n",aux->title);
    printf("Year: %s\n",aux->years);
    printf("Director: %s\n",aux->director);
    printf("Rating: %s\n",aux->rating);
    printf("Genero: %s\n",aux->genres);
    nombrelist=list_first(aux->listnombres);
    printf("Listas asociadas:\n");
    while(nombrelist!=NULL){
        printf("%s\n",nombrelist);
        nombrelist=list_next(aux->listnombres);
    }
}

void marcar_pelicula_como_preferida(SortedMap* mapa_pelis,SortedMap* mapa_listpeli){
    peliculas *aux;
    char idpelicula[30];
    char *nombrelist;
    lista *listapeliculas;
    int puntos;
    printf ("*****************************************\n");
    printf ("                                     \n");
    printf ("    Ingrese el id de la pelicula\n");
    printf ("                                     \n");
    printf ("*****************************************\n");
    scanf("%s",idpelicula);
    aux=searchSortedMap(mapa_pelis,idpelicula);
    aux->fav='S';
    nombrelist=list_first(aux->listnombres);
    while(nombrelist!=NULL){
        listapeliculas=searchSortedMap(mapa_listpeli,nombrelist);
        puntos=atoi(listapeliculas->puntos);
        puntos++;
        sprintf(listapeliculas->puntos,"%d",puntos);
        nombrelist=list_next(aux->listnombres);
    }
       printf("===================================\n");
       printf("         %s\n",aux->title);
       printf("Se agrego exitosamente a favoritos \n");
       printf("===================================\n");
}
void recomendar_pelicula(SortedMap* mapa_listpeli){
    lista *aux, *listapuntos;
    int i=0;
    peliculas *aux2;
    aux=firstSortedMap(mapa_listpeli);
    listapuntos=aux;
    while(aux!=NULL){
        if(atoi(aux->puntos)>atoi(listapuntos->puntos)){
            listapuntos=aux;
        }
        aux=nextSortedMap(mapa_listpeli);
    }
    aux2=firstSortedMap(listapuntos->listpelis);
    printf("Top 10 Mejores Peliculas Para Ti:\n");
    while(i<10&&aux2!=NULL){
        if(aux2->fav=='N'){
            printf("(%d) %s %s %s\n",i+1,aux2->title,"--",aux2->rating);
            i++;
        }
        aux2=nextSortedMap(listapuntos->listpelis);
    }
}














