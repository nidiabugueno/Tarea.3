#include "list.h"
#include "treemap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

typedef struct {
  char nombre[100];
  char anio[100];
  int valoracion;
  int precio;
} Juegos;

//Prototipos
char *get_csv_field (char * , int);
void Importarjuegos (char * , TreeMap *, TreeMap *,TreeMap *, TreeMap * );
void agregarJuego(TreeMap * ,TreeMap * ,TreeMap * ,TreeMap *, Juegos *);
void MostrarPorPrecio(TreeMap *, int);
void MostrarPorValoracion(TreeMap *, int);
void MostrarJuegoDelAnio(TreeMap *, char *);
void BuscarJuego(TreeMap *, char *);
void Exportar(TreeMap *);


int lower_than_string(void * key1, void * key2) {
    if(strcmp((char*)key1, (char*)key2) < 0) return 1;
    return 0;
}

int lower_than_int(void * key1, void * key2) {
    return key1<key2;
}

int main() 
{
  //Creamos los mapas
  TreeMap* MapNombre = createTreeMap(lower_than_string);
  TreeMap* MapAnio = createTreeMap(lower_than_string);
  TreeMap* MapValoracion = createTreeMap(lower_than_int);
  TreeMap* MapPrecio = createTreeMap(lower_than_int);

  Juegos * tmpJuego = (Juegos *)malloc(sizeof(Juegos));

  char nombre[100];
  char anio[100];
  int valoracion;
  int precio;
  char archivo[100];
  int valor;
  int opcion;
  char fecha[100];
  char nombreJ[100];
  
  int op;
  while(op!=8){
  
    printf("---------------  MENÚ  ---------------\n");
    printf("1. Importar archivo de juegos \n");
    printf("2. Agregar juego \n");
    printf("3. Mostrar juego(s) por precio \n");
    printf("4. Filtrar juego(s) por valoración \n");
    printf("5. Mostrar juego(s) del año \n");
    printf("6. Buscar juego \n");
    printf("7. Exportar archivo \n");
    printf("8. Salir del programa \n");
    printf("--------------------------------------\n");
    printf("\n¿Que operación desea realizar?\n");
    scanf("%d", &op);
    printf("\n");
    switch(op)
    {
      case 1 : 
      {
        printf("1. --- Importar Archivo de Juegos ---\n");
        printf("\nIngrese el nombre del archivo: \n");
        getchar();
        scanf("%100[^\n]s", archivo);
        Importarjuegos(archivo, MapNombre, MapAnio, MapValoracion, MapPrecio);
        break;
      }
      case 2 : 
      {
        printf("2. --- Agregar Juego --- \n");
        printf("Ingrese el nombre del juego: \n");
        getchar();
        scanf("%100[^\n]s",tmpJuego->nombre);
        getchar();
        printf("Ingrese la fecha en el que salió [DD/MM/AAAA]: \n");
        scanf("%100[^\n]s", tmpJuego->anio);
        getchar();
        printf("Ingrese la valoración: \n");
        scanf("%d", &tmpJuego->valoracion);
        getchar();
        printf("Ingrese el precio del juego: \n");
        scanf("%d", &tmpJuego->precio);
        getchar();
        agregarJuego(MapNombre, MapAnio, MapValoracion, MapPrecio,tmpJuego);
        break;
      }
      case 3 :
      {
        printf("3. --- Mostrar Juego(s) Por Precio ---\n");
        printf("Ingrese la opcion:\n 1. Mostrar de menor a mayor\n 2. Mostrar de mayor a menor\n");
        scanf("%d",&opcion);
        
        MostrarPorPrecio(MapPrecio,opcion);
        break; 
      }
      case 4 :
      {
        printf("4. --- Filtrar Juegos(s) Por Valoración ---\n");
        printf("Ingrese la valoración mínima: \n");
        scanf("%d",&valor);
        MostrarPorValoracion(MapValoracion,valor);
        break;  
      }
      case 5 :
      {
        printf("5. --- Mostrar Juego(s) Del Año ---\n");
        printf("Ingrese el año: \n");
        getchar();
        scanf("%100[^\n]s",fecha);
        getchar();
        MostrarJuegoDelAnio(MapValoracion, fecha);
        break;
      } 
      case 6 :
      {
        printf("6. --- Buscar Juego ---\n");
        printf("Ingrese el nombre del juego a buscar: \n");
        getchar();
        scanf("%100[^\n]s",nombreJ);
        getchar();
        BuscarJuego(MapNombre, nombreJ);
        break;
        
      }
      case 7 : 
      {
        printf("7. --- Exportar archivo ---\n");
        Exportar(MapNombre);
        break;
          
      }
      case 8 : 
      {
        printf("8. --- Salir del programa ---\n");
        printf("Saliendo...\n");
        break;
      }
    }
    printf("\n");

  }
  
  return 0;
}

char *get_csv_field (char * tmp, int k) {
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


void Importarjuegos (char * nombreArchivo, TreeMap * MapNombre, TreeMap * MapAnio,TreeMap * MapValoracion, TreeMap * MapPrecio){

  FILE* juegos = fopen (nombreArchivo, "r");

  if (juegos == NULL)
  {
    printf("Archivo no encontrado\n");
    return;
  }
  else
  {
    printf("Archivo abierto correctamente\n");
  }
 
  char linea[1024];
  fgets (linea, 1023, juegos);
    
  while (fgets (linea, 1023, juegos) != NULL)
  {
    linea[strlen(linea) - 1] = 0;

    Juegos* tmpJuego = (Juegos*)malloc(sizeof(Juegos));
    for (int i = 0 ; i < 6 ; i++) 
    {
      char* aux = get_csv_field(linea, i); //Se copian los datos a una variable aux, que leerá las líneas.
      switch (i)
      {
        case 0:
        {
          strcpy(tmpJuego->nombre, aux);
          break;
        }
        case 1: 
        {
          strcpy(tmpJuego->anio, aux);
          break;
        }
        case 2:
        {
          tmpJuego->valoracion = atoi(aux);
          break;
        }
        case 3:
        {
          tmpJuego->precio = atoi(aux);
          break;
        }
      }
    }
    agregarJuego(MapNombre, MapAnio, MapValoracion, MapPrecio, tmpJuego);
  }
  fclose(juegos);
  
}

void agregarJuego(TreeMap * MapNombre, TreeMap * MapAnio, TreeMap * MapValoracion, TreeMap *MapPrecio, Juegos* tmpJuego){

  //SE INSERTAN A LAS LISTAS Y A LOS MAPAS.
  
  //MAP NOMBRE
  
  if(searchTreeMap(MapNombre, tmpJuego->nombre) == NULL)
  {
    
    List* lista = createList();
    pushBack(lista, tmpJuego);
    insertTreeMap(MapNombre, tmpJuego->nombre, lista);
    
  } 
  else
  {
    List * list = searchTreeMap(MapNombre, tmpJuego->nombre)->value;
    pushBack(list, tmpJuego->nombre);
  }

  //MAP AÑO
  
  if(searchTreeMap(MapAnio, tmpJuego->anio) == NULL)
  {
    
    List* lista = createList();
    pushBack(lista, tmpJuego);
    insertTreeMap(MapAnio, tmpJuego->anio, lista);
    
  } 
  else
  {
    List * list = searchTreeMap(MapAnio, tmpJuego->anio)->value;
    pushBack(list, tmpJuego->nombre);
  }

  //MAP VALORACION
  
  if(searchTreeMap(MapValoracion, tmpJuego->valoracion) == NULL)
  {
    List* lista = createList();
    pushBack(lista, tmpJuego);
    insertTreeMap(MapValoracion, tmpJuego->valoracion, lista);
    
    
  } 
  else
  {
    List * list = searchTreeMap(MapValoracion, tmpJuego->valoracion)->value;
    pushBack(list, tmpJuego->nombre);
  } 

  //MAP PRECIO
  
  if(searchTreeMap(MapPrecio, tmpJuego->precio) == NULL)
  {
    List* lista = createList();
    pushBack(lista, tmpJuego);
    insertTreeMap(MapPrecio, tmpJuego->precio, lista);
    
  } 
  else
  {
    List * list = searchTreeMap(MapPrecio, tmpJuego->precio)->value;
    pushBack(list, tmpJuego->nombre);
  } 


  return;
}

void MostrarPorPrecio(TreeMap * MapPrecio, int opcion){
  //MOSTRAR DE MENOR A MAYOR
  if (opcion == 1)
  {
    Pair *aux = firstTreeMap(MapPrecio);
    
    while(aux != NULL)
    {
      List * lista = aux->value;
      Juegos * tmpJuego = firstList(lista);
      while(tmpJuego != NULL)
      {
        printf("%s %d\n",tmpJuego->nombre, tmpJuego->precio);
        tmpJuego = nextList(lista);
      }
      aux = nextTreeMap(MapPrecio); 
    }
    return;
  }

  //MOSTRAR DE MAYOR A MENOR
  if (opcion == 2)
  {
    Pair *aux = lastTreeMap(MapPrecio);
    
    while(aux != NULL)
    {
      List * lista = aux->value;
      Juegos * tmpJuego = firstList(lista);
      
      while(tmpJuego != NULL)
      {
        printf("%s %d\n",tmpJuego->nombre,tmpJuego->precio);
        tmpJuego = nextList(lista);
      }
      aux = prevTreeMap(MapPrecio); 
    }
    return;
  }
}

void MostrarPorValoracion(TreeMap * MapValoracion, int valor){
  
  Pair* aux = upperBound(MapValoracion,valor);
  
  while(aux != NULL)
  {
    List* lista = aux->value;
    Juegos* tmpJuego = firstList(lista);
    while(tmpJuego != NULL)
    {
      printf("%s %d\n",tmpJuego->nombre,tmpJuego->valoracion);
      tmpJuego = nextList(lista);
    }
    
    aux = nextTreeMap(MapValoracion);
  }
}

void MostrarJuegoDelAnio(TreeMap* MapValoracion, char* Fecha){
  Pair* aux = lastTreeMap(MapValoracion);
  int cont = 0;
  int valorac;

  while(aux != NULL)
  {
    List* lista = aux->value;
    Juegos* tmpJuego = lastList(lista);
    while(tmpJuego != NULL)
    {
      //COMPARAMOS LAS FECHAS
      if(tmpJuego->anio[6] == Fecha[0])
      { 
        if(tmpJuego->anio[7] == Fecha[1])
        {
          if(tmpJuego->anio[8] == Fecha[2])
          {
            if(tmpJuego->anio[9] == Fecha[3])
            { //IMPRIMIMOS EL PRIMER JUEGO, YA QUE ESE TIENE MAYOR VALORACION, SEGUN EL ORDEN DE NUESTRO ÁRBOL.
              if(cont == 0)
              {
                printf("\n%s %s %d %d\n",tmpJuego->nombre,tmpJuego->anio,tmpJuego->valoracion,tmpJuego->precio);
                cont++;
                valorac = tmpJuego->valoracion;
              }
              else //VEMOS SI HAY MÁS DE 1 JUEGO CON LA MISMA VALORACIÓN.
              {
                if(valorac == tmpJuego->valoracion)
                {
                  printf("\n%s %s %d %d\n",tmpJuego->nombre,tmpJuego->anio,tmpJuego->valoracion,tmpJuego->precio); 
                }
              }
            }
          }
        }
      }
      tmpJuego = prevList(lista);
    }
    aux = prevTreeMap(MapValoracion);
  }
}

void BuscarJuego(TreeMap * MapNombre, char * nombreJ){
  Pair *aux = firstTreeMap(MapNombre);
  
  int op,op2;
  char nombreN[100];
  char anioN[100];
  int valoracionN;
  int precioN;
  
  while(aux != NULL)
  {
    List* lista = aux->value;
    Juegos* tmpJuego = firstList(lista);
    while(tmpJuego != NULL)
    {
      if(strcmp(tmpJuego->nombre, nombreJ) == 0)
      {
        printf("\n");
        printf("Datos del juego:\n");
        printf("%s %s %d %d\n",tmpJuego->nombre,tmpJuego->anio,tmpJuego->valoracion,tmpJuego->precio);
        printf("\n--------------------------------------------------");
        printf("\n¿Quiere modificar o eliminar los datos del juego?\n");
        printf(" 1. Modificar los datos \n 2. Eliminar los datos\n 3. Volver al menú\n");
        scanf("%d", &op);
        switch(op)
        {
          case 1 : 
          {
            while(op2 != 6)
            {
              printf("\n1. --- Modificar los datos ---\n");
              printf("¿Qué datos modificará?\n");
              printf(" 1. Nombre \n 2. Año de salida\n 3. Valoracion\n 4. Precio\n 5. Mostrar los datos actualizados\n 6. Volver al menú\n");
              scanf("%d", &op2);
              switch(op2)
              { //SEGUN EL DATO COPIA LO QUE DICE EL USUARIO 
                case 1 : 
                {
                  printf("\nIngrese el nuevo nombre: \n");
                  getchar();
                  scanf("%100[^\n]s",nombreN);
                  getchar();
                  strcpy(tmpJuego->nombre, nombreN); 
                  printf("Nombre Actualizado...\n");
                  break;
                }
                case 2 : 
                {
                  printf("\nIngrese la nueva fecha: \n");
                  getchar();
                  scanf("%100[^\n]s",anioN);
                  getchar();
                  strcpy(tmpJuego->anio, anioN);
                  printf("Fecha Actualizada...\n");
                  break;
                }
                case 3 : 
                {
                  printf("\nIngrese la nueva valoracion: \n");
                  scanf("%d",&valoracionN);
                  tmpJuego->valoracion = valoracionN;
                  printf("Valoración Actualizada...\n");
                  break;
                }
                case 4 : 
                {
                  printf("\nIngrese el nuevo precio: \n");
                  scanf("%d",&precioN);
                  tmpJuego->precio = precioN;
                  printf("Precio Actualizado...\n");
                  break;
                }
                case 5 : 
                {
                  printf("\nDatos actualizados: \n");
                  printf("%s %s %d %d\n", tmpJuego->nombre, tmpJuego->anio, tmpJuego->valoracion, tmpJuego->precio);
                  break;
                }
                case 6 : 
                {
                  printf("Volviendo al menú...\n");
                  break;
                }
              }
            }
            break;
          }
          case 2:
          {
            printf("\n2. --- Eliminar los datos ---\n");
            popCurrent(aux->value);
            printf("Datos eliminados");
            break;
          }
          case 3:
          {
            printf("\nVolviendo al menú...\n");
            break;
          }
        }
      }
      tmpJuego = nextList(lista);
    }
    aux = nextTreeMap(MapNombre);
  }
}

void Exportar(TreeMap *MapNombre){
  
  char Narchivo[30];
  
  printf("Ingrese el nombre del archivo: ");
  getchar();
  scanf("%100[^\n]s", Narchivo);
  getchar();

  FILE *x = fopen(Narchivo, "w");
  if(x == NULL)
  {
    printf("Ingrese un archivo válido\n");
    return;
  } 
  else
  {
    fprintf(x,"Nombre,Año de salida,Valoracion,Precio\n");
    Pair *aux = firstTreeMap(MapNombre);
    while(aux != NULL)
    {
      List* lista = aux->value;
      Juegos *data = firstList(lista);
      while(data != NULL)
      {
        fprintf(x,"%s,%s,%d,%d", data->nombre,data->anio,data->valoracion,data->precio);
        fprintf(x,"\n");
        data = nextList(lista);
        
      }
      aux = nextTreeMap(MapNombre);
    }
  }
  fclose(x);
}