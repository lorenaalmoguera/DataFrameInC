#ifndef _lib_H_
#define _lib_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <time.h>
//#include <dirent.h>
#include <ctype.h>
#include <windows.h>
#include <math.h>
#include <float.h>
#include <stdbool.h>
#include "lib.h"

typedef enum {
    TEXTO,
    NUMERICO,
    FECHA
}TipoDato;

typedef struct {

    char nombre[30];
    TipoDato tipo;
    void *datos;
    unsigned char *esNulo;
    int numFilas;
} Columna;

typedef struct {
    char nombre[51];
    Columna *columna;
    int numColumnas;
    int numFilas;
    int *indice;
} Dataframe;

typedef struct tm Fecha;


typedef struct NodoLista{
    Dataframe *df;
    struct NodoLista *siguiente;
}Nodo;

typedef struct {
    int numDFs;
    Nodo *primero;
}Lista;



void Datos_alumno();
void EjerciciosExamen();

int Validar_decimal(char *cadena);
int Validar_entero (char *cad);
int Validar_PosNeg (char *cad);
void Trim(char *cadena);
Nodo* Load(char *nombre);
int AgregarFilasDesdeOtroDF(Dataframe *dfActivo, Dataframe *nuevoDF);
int NumCampos (char *lin);
char* ObtenerCampo(char *lin, int id, char *val);
void ObtenerCampo2(char *linea, int numCampo, char *campo);
TipoDato Tipo(char *val);
int FechaValidarAMD (char* fecha);
Dataframe* CrearDataFrame(int filas, int columnas, TipoDato *t, char* cab);
Columna* CrearColumna(char *nombre, TipoDato t, int filas);
Columna *CrearColumnaTexto(int numFilas, const char *nombre);
Fecha* AFecha(char *cadena);
int anyadirNodo(Nodo *nuevo, Lista *laGranLista);
int numNulos(Columna col);
char* printElemento(Dataframe* df, int col, int fila);
Columna* BuscarColumna(Dataframe *df, char *nombre);
void SortCol(Dataframe *df, Columna *col, int asc);
int sortVal(void *dato1, void *dato2, TipoDato tipo);
int compararValores(void *dato1, void *dato2, TipoDato tipo);
void intercambiar(int *a, int *b);
Columna* CopiarColumna(const Columna *colOriginal);
int Save(Dataframe *df, char *nombre);
int EliminarColumna(Dataframe *df, int indiceColumna);
void LiberarColumna(Columna *columna);
void LiberarDataframe(Dataframe *df);
int AgregarColumna(Dataframe *df, char *nombre , TipoDato t);
int AnyadirColumna(Dataframe *df, Columna *nuevaColumna);
int Quarter(Dataframe *df, Columna *col, char* nombre);
int EliminarNulos (Nodo* nodo, Dataframe *df, Columna* col);
int Filtrar(Nodo* nodo, Dataframe* df, Columna *col, char *comp, char *valor);

//funciones auxiliares

// GROUP BY.... EN CASO DE QUE SALGA EN EL EXAMEN REVISAR ENUNCIADO YA QUE ESTE ES CONFUSO XD
Columna* BuscarColumna2(Dataframe *df, const char *nombre);
int GroupBy(Dataframe *df, const char *nombreColumna);
void OrdenarPorColumna(Columna *columna, int *orden);
void ReordenarColumna(Columna *columna, int *nuevoOrden);

// ELIMINAR DATAFRAME
int EliminarDataframe(Lista *laGranLista, const char *nombreDF);
void extraerTexto(const char *prompt, char *resultado, size_t size);

//COPY/DUPLICATE DATAFRAME
Nodo *BuscarNodoPorNombre(Lista *lista, const char *nombreDF);
Dataframe *DuplicarDataframe(const Dataframe *original);

//AGREGAR COLUMNA AL DATAFRAME
int AgregarColumnaNueva(Dataframe *df, const char *nombre, TipoDato tipo);

//ELIMINAR TODOS LOS DATAFRAMES
void EliminarTodosLosDataframes(Lista *laGranLista);

//REMPLAZAR DATO COLUMNA
int ReplaceDatoEnColumna(Dataframe *df, const char *nombreColumna, const char *dato, const char *datoReemplazar);

//CAMBIAR NOMBRE COLUMNA
int CambiarNombreColumna(Dataframe *df, const char *nombreActual, const char *nuevoNombre);

//CALCULOS
int compararValoresNum(const double *a, const double *b);
double OperacionColumnaNumerica(Dataframe *df, const char *colname, const char *operacion);

//FUNCION FILTRAR MODIFICADA, PARA FILTRAR ENTRE
int Filtrar2(Nodo *nodo, Dataframe *df, Columna *col, char *comp, char *val1, char *val2);

//GUARDAR CON CONFIRMACION
int Save2(Dataframe *df, char *nombre);

//AÑADIR FILA
int AgregarFila(Dataframe *df, void **valores);

//Añadir datos ficheros a un dataframe
int CrearYAgregarColumna(Dataframe *destino, Columna *origen);
void RenombrarColumnaDuplicada(Dataframe *df, char *nombreColumna);

//Borra filas con valores duplicados de una columna:
int DropDuplicates(Dataframe *df, char *columnaNombre);

//funcion para generar un histograma
int Histogram(Dataframe *df, char *columnaNombre, int numBins);

//funcion para realizar una suma entre dos columnas
int SumFecha(Dataframe *df, char *col1, char *col2, char *col3);
int Sum(Dataframe *df, char *col1, char *col2, char *col3);

//funciones para realizar las sumas y calcular el número de días para las fechas
bool esBisiesto(int anio);
int diasDesdeBase(Fecha *fecha);
int Sum(Dataframe *df, char *col1, char *col2, char *col3);

//funciones para realizar las sumas y calcular el número de días para las fechas
int Sub(Dataframe *df, char *col1, char *col2, char *col3);

// funcion que extrae el meta para el .txt
int GuardarMeta(Dataframe *df, char *nombreArchivo);

//FUNCION PARA CONCATENRAR
int Concatenar(Dataframe *df, char *col1, char *col2, char *col3);
char *fechaAString(Fecha *fecha);

//UNIQUE
void *ObtenerValoresUnicosExclusivos(Columna *columna, int *numUnicos);
void ImprimirValoresUnicosExclusivos(Columna *columna, void *valoresUnicos, int numUnicos);

//SEARCH
int *BuscarEnColumna(Columna *columna, void *valor, int *numResultados);
#endif

