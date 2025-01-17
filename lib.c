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

static void **datosGlobal = NULL;

void Datos_alumno(){

    printf("Lorena Almoguera Romero\n");
    printf("lorena.almoguera@goumh.umh.es\n");

}

void EjerciciosExamen(){

    printf("Ejercicio 1: HECHO\n");
    printf("Ejercicio 2: HECHO / SIN HACER\n");
    printf("Ejercicio 3: HECHO\n");
    printf("Ejercicio 4: HECHO\n");

}

// 1 OK, 0 MAL
int Validar_decimal(char *cadena){

    int i, p=0;

    if(cadena[0] == '+' || cadena[0] == '-')
        i=1;
    else    
        i=0;

    if(strlen(cadena) == 1 && i==1){ // caso en el que solo escriben + o -
        return 0;
    }

    if(strlen(cadena) == 0){
        return 0;
    }

    if(i == 1 && strlen(cadena) == 2 && cadena[1] == '.' ){ //caso en el que nos escriben +. o -.
        return 0;
    }

    if(strlen(cadena) == 1 && i == 0 && cadena[0] == '.'){ // .
        return 0;
    }


    for(i; i<strlen(cadena); i++){
        if(cadena[i] < '0' || cadena[i] > '9'){
            if(cadena[i] == '.'){
                p++;
            }
            else{
                return 0;
            }
        }

        if(p == 2){
            return 0;
        }
    }

    return 1;
}

// 1 OK, 0 MAL
int Validar_entero (char *cad)
{
	int i, ini;
	
	if(cad[0]=='+')
		ini=1;
	else 
		ini=0;
	for (i=ini; i<strlen(cad); i++)
		{
			if (cad[i]<'0' || cad[i]>'9')
				return 0;
		}
	if (ini==1 && strlen(cad)==1)
		return 0;
	if (ini==0 && strlen(cad)==0)
		return 0;
    return 1;
}

int Validar_PosNeg (char *cad){
	int i, ini;
	
	if(cad[0]=='+' || cad[0]=='-')
		ini=1;
	else 
		ini=0;
	for (i=ini; i<strlen(cad); i++)
		{
			if (cad[i]<'0' || cad[i]>'9')
				return 0;
		}
	if (ini==1 && strlen(cad)==1)
		return 0;
	if (ini==0 && strlen(cad)==0)
		return 0;
    return 1;
}

//Esta funcion elimina los espacios en blanco del token
void Trim(char *cadena){

    int c, i, j, n=strlen(cadena);

    if(cadena == NULL || n<1){
        return;
    }

    n--;

    while(n>=0 && ((c=cadena[n]) == ' ' || c=='\t' || c=='\n' || c =='\r')){
        n--;
    }

    cadena[n+1] = '\0';

    i=0;

    while(i<=n && ((c=cadena[i]) ==' ' || c == '\t' || c=='\n' || c=='\r')){
        i++;
    }

    for(j=0;j <= n-i+1; j++){
        cadena[j] = cadena[j+i];
    }
}

Nodo* Load(char *nombre) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));
    nuevo->siguiente = NULL;
    char aux[50] = "./";

    FILE *fichero;
    TipoDato *t;
    char cab[1000], lin[1000], val[1000];
    int filas = 1, columnas, i;

    strcat(aux, nombre);

    if (strchr(nombre, '.') == NULL) { // Entramos en el caso de que no tenga extensión
        strcat(aux, ".csv");
    }

    fichero = fopen(aux, "r"); // Abrimos fichero en modo lectura

    if (fichero == NULL) {
        SetConsoleTextAttribute(hConsole, 4);
        printf("[ERROR] El fichero '%s' no existe.\n", aux);
        SetConsoleTextAttribute(hConsole, 7);
        return NULL;
    }

    // Leer cabecera
    if (fgets(cab, 1000, fichero) == NULL) {
        SetConsoleTextAttribute(hConsole, 4);
        printf("[ERROR] No se pudo leer la cabecera del archivo.\n");
        SetConsoleTextAttribute(hConsole, 7);
        fclose(fichero);
        return NULL;
    }

    Trim(cab);
    columnas = NumCampos(cab);

    if (columnas == 0) {
        SetConsoleTextAttribute(hConsole, 4);
        printf("[ERROR] La cabecera no tiene campos.\n");
        SetConsoleTextAttribute(hConsole, 7);
        fclose(fichero);
        return NULL;
    }

    // Leer segunda fila y validarla
    if (fgets(lin, 1000, fichero) == NULL) {
        SetConsoleTextAttribute(hConsole, 4);
        printf("[ERROR] El archivo no tiene suficientes filas para determinar los tipos.\n");
        SetConsoleTextAttribute(hConsole, 7);
        fclose(fichero);
        return NULL;
    }

    Trim(lin);
    int columnasSegundaFila = NumCampos(lin);

    // Validar si el número de columnas coincide
    if (columnasSegundaFila != columnas) {
        SetConsoleTextAttribute(hConsole, 4);
        printf("[ERROR] Inconsistencia detectada: La cabecera tiene %d columnas, pero la segunda fila tiene %d columnas.\n",
               columnas, columnasSegundaFila);
        SetConsoleTextAttribute(hConsole, 7);
        fclose(fichero);
        return NULL;
    }

    // Validar si hay valores nulos en la segunda fila
    for (i = 0; i < columnas; i++) {
        ObtenerCampo(lin, i, val);
        Trim(val);

        if (strlen(val) == 0) { // Valor nulo detectado
            SetConsoleTextAttribute(hConsole, 4);
            printf("[ERROR] Valor nulo detectado en la columna %d en la segunda fila.\n", i + 1);
            SetConsoleTextAttribute(hConsole, 7);
            fclose(fichero);
            return NULL;
        }
    }

    // Inicializar tipos de columnas
    t = malloc(sizeof(TipoDato) * columnas);
    int *contFecha = calloc(columnas, sizeof(int));
    int *contNumerico = calloc(columnas, sizeof(int));
    int *contTexto = calloc(columnas, sizeof(int));

    // Clasificar tipos de datos en la segunda fila
    for (i = 0; i < columnas; i++) {
        ObtenerCampo(lin, i, val);
        Trim(val);

        if (FechaValidarAMD(val)) {
            contFecha[i]++;
        } else if (Validar_decimal(val)) {
            contNumerico[i]++;
        } else {
            contTexto[i]++;
        }
    }

    // Procesar el resto de las filas
    while (!feof(fichero)) {
        if (fgets(lin, 1000, fichero) == NULL)
            break;

        Trim(lin);
        int columnasActuales = NumCampos(lin);

        // Validar si el número de columnas es inconsistente
        if (columnasActuales != columnas) {
            SetConsoleTextAttribute(hConsole, 4);
            printf("[ERROR] Inconsistencia detectada: La fila %d tiene %d columnas, pero se esperaban %d.\n",
                   filas + 1, columnasActuales, columnas);
            SetConsoleTextAttribute(hConsole, 7);
            fclose(fichero);
            free(contFecha);
            free(contNumerico);
            free(contTexto);
            return NULL;
        }

        for (i = 0; i < columnas; i++) {
            ObtenerCampo(lin, i, val);
            Trim(val);

            if (FechaValidarAMD(val)) {
                contFecha[i]++;
            } else if (Validar_decimal(val)) {
                contNumerico[i]++;
            } else {
                contTexto[i]++;
            }
        }
        filas++;
    }

    // Determinar el tipo dominante para cada columna
    for (i = 0; i < columnas; i++) {
        if (contFecha[i] > contNumerico[i] && contFecha[i] > contTexto[i]) {
            t[i] = FECHA;
        } else if (contNumerico[i] > contTexto[i]) {
            t[i] = NUMERICO;
        } else {
            t[i] = TEXTO;
        }
    }

    // Crear DataFrame
    nuevo->df = CrearDataFrame(filas, columnas, t, cab);

    if (!nuevo->df) {
        printf("[ERROR] No se pudo crear el DataFrame.\n");
        free(contFecha);
        free(contNumerico);
        free(contTexto);
        fclose(fichero);
        return NULL;
    }

    snprintf(nuevo->df->nombre, sizeof(nuevo->df->nombre), "%s", nombre);
    nuevo->df->nombre[sizeof(nuevo->df->nombre) - 1] = '\0';

    // Volver al inicio del archivo para cargar datos
    fseek(fichero, 0, SEEK_SET);
    fgets(lin, 1000, fichero);
    filas = 0;

    while (!feof(fichero)) {
        if (fgets(lin, 1000, fichero) == NULL)
            break;

        for (i = 0; i < columnas; i++) {
            ObtenerCampo(lin, i, val);
            Trim(val);

            if (t[i] == FECHA) {
                if (FechaValidarAMD(val)) {
                    nuevo->df->columna[i].esNulo[filas] = 0;
                    ((Fecha *)nuevo->df->columna[i].datos)[filas] = *AFecha(val);
                } else {
                    nuevo->df->columna[i].esNulo[filas] = 1;
                }
            } else if (t[i] == NUMERICO) {
                if (Validar_decimal(val)) {
                    nuevo->df->columna[i].esNulo[filas] = 0;
                    ((double *)nuevo->df->columna[i].datos)[filas] = atof(val);
                } else {
                    nuevo->df->columna[i].esNulo[filas] = 1;
                }
            } else { // TEXTO
                nuevo->df->columna[i].esNulo[filas] = 0;
                if (strlen(val) == 0) {
                    nuevo->df->columna[i].esNulo[filas] = 1;
                }
                ((char **)nuevo->df->columna[i].datos)[filas] = strdup(val);
            }
        }
        filas++;
    }

    // Liberar memoria de contadores
    free(contFecha);
    free(contNumerico);
    free(contTexto);
    fclose(fichero);

    return nuevo;
}



int AgregarFilasDesdeOtroDF(Dataframe *dfActivo, Dataframe *nuevoDF) {
    if (dfActivo == NULL || nuevoDF == NULL) {
        //printf("[Error] Dataframe activo o nuevo es NULL.\n");
        return -1;
    }

    if (dfActivo->numColumnas != nuevoDF->numColumnas) {
        //printf("[Error] El número de columnas no coincide entre los DataFrames.\n");
        return -1;
    }

    // Validar que los tipos de las columnas coincidan
    for (int i = 0; i < dfActivo->numColumnas; i++) {
        if (dfActivo->columna[i].tipo != nuevoDF->columna[i].tipo) {
            //printf("[Error] El tipo de la columna '%s' no coincide.\n", dfActivo->columna[i].nombre);
            return -1;
        }
    }

    // Agregar filas del nuevo DataFrame al activo
    for (int i = 0; i < nuevoDF->numFilas; i++) {
        //printf("[Depuración] Leyendo fila %d del nuevo DataFrame:\n", i + 1);

        void **valores = calloc(dfActivo->numColumnas, sizeof(void *));
        if (valores == NULL) {
            printf("[Error] No se pudo asignar memoria para los valores de la fila.\n");
            return -1;
        }

        for (int j = 0; j < nuevoDF->numColumnas; j++) {
            if (nuevoDF->columna[j].esNulo[i]) {
                valores[j] = NULL;
                //printf("  Columna %d (%s): Valor nulo\n", j + 1, nuevoDF->columna[j].nombre);
            } else if (nuevoDF->columna[j].tipo == TEXTO) {
                valores[j] = strdup(((char **)nuevoDF->columna[j].datos)[i]);
                //printf("  Columna %d (%s): TEXTO -> '%s'\n", j + 1, nuevoDF->columna[j].nombre, (char *)valores[j]);
            } else if (nuevoDF->columna[j].tipo == NUMERICO) {
                valores[j] = malloc(sizeof(double));
                if (valores[j]) {
                    *(double *)valores[j] = ((double *)nuevoDF->columna[j].datos)[i];
                  // printf("  Columna %d (%s): NUMERICO -> %f\n", j + 1, nuevoDF->columna[j].nombre, *(double *)valores[j]);
                }
            } else if (nuevoDF->columna[j].tipo == FECHA) {
                valores[j] = malloc(sizeof(Fecha));
                if (valores[j]) {
                    // Validar formato y existencia de la fecha
                    Fecha *fechaOriginal = &((Fecha *)nuevoDF->columna[j].datos)[i];
                    if (fechaOriginal->tm_year == 0 || fechaOriginal->tm_mon < 0 || fechaOriginal->tm_mon > 11 || 
                        fechaOriginal->tm_mday < 1 || fechaOriginal->tm_mday > 31) {
                        /*printf("[Error] Fecha inválida detectada en fila %d, columna %d: '%04d/%02d/%02d'\n",
                               i + 1, j + 1, fechaOriginal->tm_year, fechaOriginal->tm_mon + 1, fechaOriginal->tm_mday);*/
                        valores[j] = NULL; // Marcamos como nulo si la fecha es inválida
                    } else {
                        *(Fecha *)valores[j] = *fechaOriginal;
                        /*printf("  Columna %d (%s): FECHA -> %04d/%02d/%02d\n",
                               j + 1, nuevoDF->columna[j].nombre,
                               fechaOriginal->tm_year, fechaOriginal->tm_mon + 1, fechaOriginal->tm_mday);*/
                    }
                }
            }
        }

        // Agregar fila al DataFrame activo
        if (AgregarFila(dfActivo, valores) == -1) {
            //printf("[Error] No se pudo agregar la fila %d al DataFrame activo.\n", i + 1);
            for (int k = 0; k < dfActivo->numColumnas; k++) {
                free(valores[k]);
            }
            free(valores);
            return -1;
        }

        // Liberar memoria temporal
        for (int k = 0; k < dfActivo->numColumnas; k++) {
            free(valores[k]);
        }
        free(valores);
    }

    //printf("[Info] Todas las filas del nuevo DataFrame se agregaron correctamente.\n");
    return 1;
}




Fecha* AFecha(char *cadena){

    Fecha *f = (Fecha *)malloc(sizeof(Fecha));

    char *token;
    token = strtok(cadena, "-/");
    f->tm_year = atoi(token);
    token = strtok(NULL, "-/");
    f->tm_mon = atoi(token)-1;
    token = strtok(NULL, "-/");
    f->tm_mday = atoi(token);

    return f;
}

Fecha *AFecha2(char *cadena) {
    if (!cadena) return NULL;

    Fecha *f = (Fecha *)malloc(sizeof(Fecha));
    if (!f) return NULL;

    // Inicializamos la estructura Fecha
    memset(f, 0, sizeof(Fecha));

    // Tokenizar y convertir
    char *token = strtok(cadena, "-/");
    if (!token) {
        free(f);
        return NULL;
    }
    f->tm_year = atoi(token);

    token = strtok(NULL, "-/");
    if (!token) {
        free(f);
        return NULL;
    }
    f->tm_mon = atoi(token) - 1; // Meses en struct tm van de 0 a 11

    token = strtok(NULL, "-/");
    if (!token) {
        free(f);
        return NULL;
    }
    f->tm_mday = atoi(token);

    // Validar los valores
    if (f->tm_year < 0 || f->tm_mon < 0 || f->tm_mon > 11 || f->tm_mday < 1 || f->tm_mday > 31) {
        //printf("[Error] Fecha inválida: %s\n", cadena);
        free(f);
        return NULL;
    }

    //printf("[Depuración] Fecha creada correctamente: %d-%02d-%02d\n", f->tm_year, f->tm_mon + 1, f->tm_mday);
    return f;
}



Dataframe* CrearDataFrame(int filas, int columnas, TipoDato *t, char* cab){
    
    char* val = (char *)malloc(sizeof(char)*40);
    Dataframe *df = (Dataframe *)malloc(sizeof(Dataframe));
    df->numColumnas=columnas;
    df->numFilas=filas;
    df->indice = (int *)malloc(sizeof(int)*filas);
    if(df->indice == NULL)
        return NULL;
    for (int i=0; i<filas; i++){
        df->indice[i] = i;
    }
    df->columna = (Columna *)malloc(sizeof(Columna)*columnas);
    for (int i=0; i<columnas; i++){
        ObtenerCampo(cab,i, val);
        df->columna[i] = *CrearColumna(val, t[i], filas);
    }
    return df;
}

Columna* CrearColumna(char *nombre, TipoDato t, int filas){
    Columna *c = (Columna *)malloc(sizeof(Columna));
    
    strcpy(c->nombre, nombre);
    c->esNulo = (unsigned char *) malloc(sizeof(unsigned char)*filas);

    c->tipo = t;
    if (t == TEXTO){
        c->datos = (char **)malloc(sizeof(char*)*filas);
    }
    else if(t == NUMERICO){
        c->datos = (double *)malloc(sizeof(double)*filas);
    }
    else if(t == FECHA){
        c->datos = (Fecha *)malloc(sizeof(Fecha)*filas);
    }
    c->numFilas=filas;
    return c;
}

Columna *CrearColumnaTexto(int numFilas, const char *nombre) {
    // Asignar memoria para la columna
    Columna *col = malloc(sizeof(Columna));
    if (col == NULL) return NULL;

    // Asignar memoria para el arreglo de punteros de texto
    char **textData = malloc(numFilas * sizeof(char *));
    if (textData == NULL) {
        free(col);
        return NULL;
    }

    // Inicializar cada puntero de texto como NULL
    for (int i = 0; i < numFilas; i++) {
        textData[i] = NULL;
    }

    // Asignar el puntero de datos
    col->datos = (void *)textData;

    // Asignar memoria para los flags de valores nulos
    col->esNulo = malloc(numFilas * sizeof(unsigned char));
    if (col->esNulo == NULL) {
        free(textData);
        free(col);
        return NULL;
    }
    memset(col->esNulo, 0, numFilas * sizeof(unsigned char)); // Inicializar flags a 0

    // Copiar el nombre de la columna
    strncpy(col->nombre, nombre, sizeof(col->nombre) - 1);
    col->nombre[sizeof(col->nombre) - 1] = '\0';

    // Establecer el tipo y número de filas
    col->tipo = TEXTO; // Tipo texto
    col->numFilas = numFilas;

    return col;
}



int NumCampos (char *lin){
    char *sep = strdup(",");
	int x=1;
	int tamSep=strlen(sep);
	char *p1, *p2;

	if (lin==NULL || lin[0]=='\0')
		return 0;
	p2=strstr(lin, sep);
	while (p2!=NULL)
	{
		x++;
		p1=p2+tamSep;
		p2=strstr(p1, sep);
	}
	
	return x;
}
		
char* ObtenerCampo(char *lin, int id, char *val){
    char *sep = strdup(",");
	int i, tamSep=strlen(sep);
	char *p1, *p2;
	
	if (lin==NULL || lin[0]=='\0')
		return NULL;
	
	p1=lin;
	p2=strstr(lin, sep);
	
	for(i=0; i<id;i++)
	{
		p1=p2+tamSep;
		p2=strstr(p1, sep);
	}
	if(p2==NULL)
		strcpy(val,p1);
	else
	{
		strncpy(val, p1, p2-p1);
		val [p2-p1]='\0';
	}
	return val;
}

TipoDato Tipo(char *val){
    // printf("VAL: '%s' ->", val);
    if (FechaValidarAMD(val)){
        // printf("FECHA\n");
    	return FECHA;
    }
    else if (Validar_decimal(val)){
        // printf("NUMERICO\n");
        return NUMERICO;
    }
    else{
        // printf("TEXTO\n");
        return TEXTO;
    }
}

int FechaValidarAMD(char* fecha) {
    int d, m, a;

    // Validar si el valor es nulo
    if (fecha == NULL || strlen(fecha) == 0) {
        //printf("DEBUG: Fecha es nula o vacía, se considerará como #N/A.\n");
        return 0; // Tratar como no válida
    }

    // Validar longitud de la fecha
    if (strlen(fecha) != 10) {
        //printf("DEBUG: La fecha '%s' no tiene la longitud adecuada.\n", fecha);
        return 0;
    }

    // Validar formato de separadores
    if ((fecha[4] != '-' && fecha[4] != '/') || (fecha[4] != fecha[7])) {
        //printf("DEBUG: La fecha '%s' tiene separadores inconsistentes.\n", fecha);
        return 0; // Separadores inconsistentes
    }

    // Validar caracteres numéricos en las posiciones correspondientes
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) {
            continue; // Separadores
        }
        if (fecha[i] < '0' || fecha[i] > '9') {
            //printf("DEBUG: La fecha '%s' contiene caracteres inválidos.\n", fecha);
            return 0; // Caracter no válido
        }
    }

    // Extraer valores de año, mes y día
    a = (fecha[0] - '0') * 1000 + (fecha[1] - '0') * 100 + (fecha[2] - '0') * 10 + (fecha[3] - '0');
    m = (fecha[5] - '0') * 10 + (fecha[6] - '0');
    d = (fecha[8] - '0') * 10 + (fecha[9] - '0');

    // Imprimir para depuración
    //printf("DEBUG: Año = %d, Mes = %d, Día = %d\n", a, m, d);

    // Validar rango básico de año, mes y día
    if (a < 1 || m < 1 || m > 12 || d < 1 || d > 31) {
        //printf("DEBUG: La fecha '%s' está fuera de los rangos válidos.\n", fecha);
        return 0;
    }

    // Validar meses con 30 días
    if ((m == 4 || m == 6 || m == 9 || m == 11) && d > 30) {
        //printf("DEBUG: La fecha '%s' tiene un día inválido para el mes %d.\n", fecha, m);
        return 0;
    }

    // Validar febrero
    if (m == 2) {
        // Verificar si el año es bisiesto
        int esBisiesto = (a % 400 == 0) || (a % 4 == 0 && a % 100 != 0);
        if (esBisiesto) {
            if (d > 29) {
                //printf("DEBUG: La fecha '%s' tiene un día inválido para febrero en un año bisiesto.\n", fecha);
                return 0; // Máximo 29 días en febrero bisiesto
            }
        } else {
            if (d > 28) {
                //printf("DEBUG: La fecha '%s' tiene un día inválido para febrero en un año no bisiesto.\n", fecha);
                return 0; // Máximo 28 días en febrero no bisiesto
            }
        }
    }

    // Si todo es válido
    //printf("DEBUG: La fecha '%s' es válida.\n", fecha);
    return 1;
}


int anyadirNodo(Nodo *nuevo, Lista *laGranLista){
    Nodo *aux;
    if (laGranLista->primero == NULL){
        laGranLista->primero = nuevo;
        laGranLista->numDFs++;
        return 1;
    }
    else{
        aux = laGranLista->primero;
        while (aux->siguiente != NULL){
            //printf("a");
            aux = aux->siguiente;
        }
        aux->siguiente = nuevo;
        laGranLista->numDFs++;
        return 1;
    }
}

int numNulos(Columna col){

    int i, contador=0;
    for(i=0; i<col.numFilas; i++){
        if(col.esNulo[i] == 1){
            contador++;
        }
    }
    return contador;
}

char* printElemento(Dataframe *df, int colIdx, int filaIdx) {
    static char buffer[50]; // Usamos un buffer estático para devolver la cadena formateada
    Columna *col = &df->columna[colIdx];

    // Obtener el índice lógico para respetar el orden del dataframe
    int idx = df->indice[filaIdx];

    // Verificar si el valor es nulo
    if (col->esNulo[idx]) {
        return "#N/A"; // Valor nulo
    }

    // Manejar los diferentes tipos de datos
    if (col->tipo == TEXTO) {
        const char *valor = ((char **)col->datos)[idx];
        return valor ? (char *)valor : "#N/A"; // Realizamos un cast explícito
    } else if (col->tipo == NUMERICO) {
        snprintf(buffer, sizeof(buffer), "%.2f", ((double *)col->datos)[idx]);
        return buffer;
    } else if (col->tipo == FECHA) {
        // Extraer el valor de la fecha
        Fecha *fecha = &((Fecha *)col->datos)[idx];
        snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", fecha->tm_year, fecha->tm_mon + 1, fecha->tm_mday);
        return buffer;
    }

    return "#N/A"; // Caso por defecto para tipos no reconocidos
}




Columna* BuscarColumna(Dataframe *df, char *nombre){
    int i;
    for(i=0; i<df->numColumnas; i++){
        if(strcmp(df->columna[i].nombre, nombre) == 0){
            return &df->columna[i];
        }
    }
    return NULL;
}


void intercambiar(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

//Si dato1 > dato2 return + 
// Si dato1 < dato2 return - 
// Si dato1 = dato2 return 0
//el valor más pequeño SIEMPRE será nulo.
int compararValores(void *dato1, void *dato2, TipoDato tipo) {
    //printf("[Depuración] Inicio de compararValores\n");

    // Comprobar si alguno de los valores es nulo
    if (dato1 == NULL && dato2 == NULL) {
      //  printf("[Depuración] Ambos valores son nulos, retorno 0 (iguales)\n");
        return 0; // Ambos son nulos, son iguales
    }
    if (dato1 == NULL) {
        //printf("[Depuración] dato1 es nulo, retorno -1 (menor)\n");
        return -1; // Si dato1 es nulo, es menor
    }
    if (dato2 == NULL) {
        //printf("[Depuración] dato2 es nulo, retorno 1 (mayor)\n");
        return 1; // Si dato2 es nulo, es mayor
    }

    // Comparación de valores no nulos
    if (tipo == TEXTO) {
        //printf("[Depuración] Comparando TEXTO: '%s' vs '%s'\n", (char *)dato1, (char *)dato2);
        return strcmp((char *)dato1, (char *)dato2);
    } else if (tipo == NUMERICO) {
        //printf("[Depuración] Comparando NUMERICO: %.2f vs %.2f\n", *(double *)dato1, *(double *)dato2);
        return (*(double *)dato1 > *(double *)dato2) - (*(double *)dato1 < *(double *)dato2);
    } else if (tipo == FECHA) {
        Fecha *f1 = (Fecha *)dato1;
        Fecha *f2 = (Fecha *)dato2;
        /*printf("[Depuración] Comparando FECHA: %d-%02d-%02d vs %d-%02d-%02d\n",
               f1->tm_year, f1->tm_mon + 1, f1->tm_mday,
               f2->tm_year, f2->tm_mon + 1, f2->tm_mday);*/
        if (f1->tm_year != f2->tm_year) {
            //printf("[Depuración] Años diferentes, retorno %d\n", f1->tm_year - f2->tm_year);
            return f1->tm_year - f2->tm_year;
        } else if (f1->tm_mon != f2->tm_mon) {
            //printf("[Depuración] Meses diferentes, retorno %d\n", f1->tm_mon - f2->tm_mon);
            return f1->tm_mon - f2->tm_mon;
        } else {
            //printf("[Depuración] Días diferentes, retorno %d\n", f1->tm_mday - f2->tm_mday);
            return f1->tm_mday - f2->tm_mday;
        }
    }

//    printf("[Depuración] Tipo no reconocido, retorno 0 (iguales por defecto)\n");
    return 0; // Por defecto, consideramos iguales si no se puede comparar
}


int sort(void *dato1, void *dato2, TipoDato tipo) {
    if (tipo == TEXTO) {
        // printf("Comparando %s con %s\n", (char *)dato1, (char *)dato2);
        return strcmp((char *)dato1, (char *)dato2);
    } else if (tipo == NUMERICO) {
        // printf("Comparando %f con %f\n", *(double *)dato1, *(double *)dato2);
        return (*(double *)dato1 > *(double *)dato2) - (*(double *)dato1 < *(double *)dato2);
    } else if (tipo == FECHA) {
        // printf("Comparando %d/%d/%d con %d/%d/%d\n",
        //    ((Fecha *)dato1)->tm_mday, ((Fecha *)dato1)->tm_mon + 1, ((Fecha *)dato1)->tm_year,
        //    ((Fecha *)dato2)->tm_mday, ((Fecha *)dato2)->tm_mon + 1, ((Fecha *)dato2)->tm_year);
        Fecha *f1 = (Fecha *)dato1;
        Fecha *f2 = (Fecha *)dato2;
        if (f1->tm_year != f2->tm_year)
            return f1->tm_year - f2->tm_year;
        else if (f1->tm_mon != f2->tm_mon)
            return f1->tm_mon - f2->tm_mon;
        else 
            return f1->tm_mday - f2->tm_mday;
    }
    return 0; // Por defecto, consideramos iguales si no se puede comparar
}

void SortCol(Dataframe *df, Columna *col, int asc) {
    int n = df->numFilas;
    int *indice = df->indice;  // El índice a ordenar
    unsigned char *esNulo = col->esNulo;  // Arreglo de si es nulo o no

    // Algoritmo de burbuja modificado
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            int idx1 = indice[j];
            int idx2 = indice[j + 1];

            // Ordenar valores nulos según el modo de ordenamiento
            if (asc) {
                // En modo ascendente, los valores nulos van al principio
                if (!esNulo[idx1] && esNulo[idx2]) {
                    intercambiar(&indice[j], &indice[j + 1]);
                    continue;
                }
            } else {
                // En modo descendente, los valores nulos van al final
                if (esNulo[idx1] && !esNulo[idx2]) {
                    intercambiar(&indice[j], &indice[j + 1]);
                    continue;
                }
            }

            // Si ambos valores son no nulos, comparar normalmente
            if (!esNulo[idx1] && !esNulo[idx2]) {
                void *dato1;
                void *dato2;

                if (col->tipo == NUMERICO) {
                    dato1 = &((double *)col->datos)[idx1];
                    dato2 = &((double *)col->datos)[idx2];
                } else if (col->tipo == FECHA) {
                    dato1 = &((Fecha *)col->datos)[idx1];
                    dato2 = &((Fecha *)col->datos)[idx2];
                } else if (col->tipo == TEXTO) {
                    dato1 = ((char **)col->datos)[idx1];
                    dato2 = ((char **)col->datos)[idx2];
                }

                int cmp = compararValores(dato1, dato2, col->tipo);

                if ((asc && cmp > 0) || (!asc && cmp < 0)) {
                    intercambiar(&indice[j], &indice[j + 1]);
                }
            }
        }
    }
}



Columna* CopiarColumna(const Columna *colOriginal) {
    Columna *nuevaCol = (Columna *)malloc(sizeof(Columna));
    if (!nuevaCol) {
        printf("Error al asignar memoria para la nueva columna.\n");
        return NULL;
    }

    strncpy(nuevaCol->nombre, colOriginal->nombre, sizeof(nuevaCol->nombre));
    nuevaCol->tipo = colOriginal->tipo;
    nuevaCol->numFilas = colOriginal->numFilas;

    // Asignar memoria para los datos y copiar según el tipo de dato
    if (colOriginal->tipo == TEXTO) {
        nuevaCol->datos = malloc(nuevaCol->numFilas * sizeof(char *));
        if (!nuevaCol->datos) {
            printf("Error al asignar memoria para los datos de la columna.\n");
            free(nuevaCol);
            return NULL;
        }
        for (int i = 0; i < nuevaCol->numFilas; i++) {
            ((char **)nuevaCol->datos)[i] = strdup(((char **)colOriginal->datos)[i]);  // Copia profunda de cada cadena
        }
    } else if (colOriginal->tipo == NUMERICO) {
        nuevaCol->datos = malloc(nuevaCol->numFilas * sizeof(double));
        if (!nuevaCol->datos) {
            printf("Error al asignar memoria para los datos de la columna.\n");
            free(nuevaCol);
            return NULL;
        }
        memcpy(nuevaCol->datos, colOriginal->datos, nuevaCol->numFilas * sizeof(double));
    } else if (colOriginal->tipo == FECHA) {
        nuevaCol->datos = malloc(nuevaCol->numFilas * sizeof(Fecha));
        if (!nuevaCol->datos) {
            printf("Error al asignar memoria para los datos de la columna.\n");
            free(nuevaCol);
            return NULL;
        }
        memcpy(nuevaCol->datos, colOriginal->datos, nuevaCol->numFilas * sizeof(Fecha));
    }

    // Asignar memoria para esNulo y copiar
    nuevaCol->esNulo = (unsigned char *)malloc(nuevaCol->numFilas * sizeof(unsigned char));
    if (!nuevaCol->esNulo) {
        printf("Error al asignar memoria para esNulo.\n");
        free(nuevaCol->datos);
        free(nuevaCol);
        return NULL;
    }
    memcpy(nuevaCol->esNulo, colOriginal->esNulo, nuevaCol->numFilas * sizeof(unsigned char));

    return nuevaCol;
}

/* int Save(Dataframe *df, char *nombre) {
    FILE *fichero;

    char aux[50]="./";


    strcat(aux, nombre);

    if(strchr(nombre, '.') == NULL){ //Entramos en el caso de que no tenga .csv
        strcat(aux, ".csv");
    }

    fichero = fopen(aux, "w");

    if (fichero!=NULL) {

        for (int i = 0; i < df->numColumnas; i++){
            if (i>0)
                fprintf(fichero, ",");

            fprintf(fichero, "%s", df->columna[i].nombre);
        }

        for (int i = 0; i < df->numFilas; i++){
            fprintf(fichero, "\n");
            for (int j = 0; j < df->numColumnas; j++){
                if(j>0)
				    fprintf(fichero, ",");
                
                if (df->columna[j].esNulo[df->indice[i]] == 0){
                    fprintf(fichero, "%s", printElemento(df, j, i));
                }
            }
        }

    }
    else {
        return -1;
    }
    fclose(fichero);
    return 1;
} */

int Save(Dataframe *df, char *nombre) {
    FILE *fichero;
    char aux[100] = "./";

    // Si el nombre es NULL o vacío, usar el nombre del dataframe
    if (nombre == NULL || nombre[0] == '\0') {
        nombre = df->nombre;
    }

    // Asegurar que el archivo tiene extensión .csv
    strcat(aux, nombre);
    if (strchr(nombre, '.') == NULL) {
        strcat(aux, ".csv");
    }

    // Intentar abrir el archivo para escritura
    fichero = fopen(aux, "w");
    if (fichero == NULL) {
        //printf("[Error] No se pudo abrir el archivo '%s' para guardar.\n", aux);
        return -1;
    }

    // Escribir la cabecera
    for (int i = 0; i < df->numColumnas; i++) {
        if (i > 0) {
            fprintf(fichero, ",");
        }
        fprintf(fichero, "%s", df->columna[i].nombre);
    }
    fprintf(fichero, "\n");

    // Escribir los datos
    for (int i = 0; i < df->numFilas; i++) {
        for (int j = 0; j < df->numColumnas; j++) {
            if (j > 0) {
                fprintf(fichero, ",");
            }
            if (df->columna[j].esNulo[df->indice[i]] == 0) {
                // Escribir el valor real
                fprintf(fichero, "%s", printElemento(df, j, i));
            } else {
                // Dejar vacío para valores nulos
                fprintf(fichero, "");
            }
        }
        fprintf(fichero, "\n");
    }

    fclose(fichero);
    //printf("[Info] El DataFrame se guardó correctamente en '%s'.\n", aux);
    return 1;
}



int AgregarColumna(Dataframe *df, char *nombre , TipoDato t) {
    Columna *nuevaMem = (Columna *)realloc(df->columna, (df->numColumnas + 1) * sizeof(Columna));
    
    if (nuevaMem == NULL) {
        return -1;  // Error al reasignar memoria
    }
    df->columna = nuevaMem;
    
    df->columna[df->numColumnas] = *CrearColumna(nombre, t, df->numFilas);
    
    df->numColumnas++;
    
    return 1;
}

int AnyadirColumna(Dataframe *df, Columna *nuevaColumna) {
    // Reasignar memoria para incluir una nueva columna
    //printf("Añadiendo columna: %s\n", nuevaColumna->nombre); // Debug
    Columna *nuevaMem = (Columna *)realloc(df->columna, (df->numColumnas + 1) * sizeof(Columna));
    if (nuevaMem == NULL) {
        //printf("Error al reasignar memoria para la nueva columna\n"); // Debug
        return -1;
    }
    df->columna = nuevaMem;

    // Copiar la columna proporcionada
    df->columna[df->numColumnas] = *nuevaColumna;
    df->numColumnas++;

    //printf("Columna añadida correctamente: %s\n", df->columna[df->numColumnas - 1].nombre); // Debug
    return 1;
}



int Quarter(Dataframe *df, Columna *col, char* nombre) {
    if (AgregarColumna(df, nombre, TEXTO) == -1) {
        return -1;
    }



    for (int i = 0; i < df->numFilas; i++) {

        df->columna[df->numColumnas - 1].esNulo[i] = 0;
        if (col->esNulo[df->indice[i]] == 1) {
            ((char **)df->columna[df->numColumnas - 1].datos)[df->indice[i]] = strdup("#N/A");
            continue;
        }
        int month = ((Fecha *)col->datos)[df->indice[i]].tm_mon + 1;
        char *quarter;

        if (month >= 1 && month <= 3) {
            quarter = "Q1";
        } else if (month >= 4 && month <= 6) {
            quarter = "Q2";
        } else if (month >= 7 && month <= 9) {
            quarter = "Q3";
        } else {
            quarter = "Q4";
        }

        ((char **)df->columna[df->numColumnas - 1].datos)[df->indice[i]] = strdup(quarter);
    }

    return 1;

}

void LiberarColumna(Columna *columna) {
    if (columna->tipo == TEXTO) {
        for (int i = 0; i < columna->numFilas; i++) {
            if (((char **)columna->datos)[i] != NULL && columna->esNulo[i]==0) {
                free(((char **)columna->datos)[i]);
            }
        }
    }
    if (columna->datos != NULL) {  
        free(columna->datos);
    }
    if (columna->esNulo != NULL) {  
        free(columna->esNulo);
    }
}


void LiberarDataframe(Dataframe *df) {
    if (df != NULL) {
        for (int i = 0; i < df->numColumnas; i++) {
            LiberarColumna(&df->columna[i]);
        }
        if (df->columna != NULL) {
            free(df->columna);
        }
        if (df->indice != NULL) {
            free(df->indice);
        }
        free(df);  
    }
}

int EliminarColumna(Dataframe *df, int indiceColumna) {

    LiberarColumna(&df->columna[indiceColumna]);
    
    // Mover las columnas
    for (int i = indiceColumna; i < df->numColumnas - 1; i++) {
        df->columna[i] = df->columna[i + 1];  // Copiar la siguiente columna
    }
    
    df->numColumnas--;
    
    // Reasignar memoria
    Columna *nueva_memoria = (Columna *)realloc(df->columna, df->numColumnas * sizeof(Columna));
    
    if (nueva_memoria == NULL && df->numColumnas > 0) {
        return -1;  // Error al reasignar memoria
    }
    
    df->columna = nueva_memoria;
    
    return 1;  // Columna eliminada con éxito
}

int EliminarNulos (Nodo* nodo, Dataframe *df, Columna *col) {
    Dataframe *dfNuevo;
    int numFilas = df->numFilas;
    int n=0;
    char cab[1000]="";
    TipoDato *t;
    t = (TipoDato *)malloc(df->numColumnas * sizeof(TipoDato));

    for (int i = 0; i < df->numFilas; i++) {
        if (col->esNulo[i] == 1) {
            numFilas--;
        }
    }

    if (numFilas == df->numFilas) {
        printf("No hay valores nulos\n");
        return 1;
    }

    for (int i = 0; i < df->numColumnas; i++) {
        t[i] = df->columna[i].tipo;
        strcat(cab, df->columna[i].nombre);
        if (i < df->numColumnas - 1)
            strcat(cab, ",");
    }

    dfNuevo = CrearDataFrame(numFilas, df->numColumnas, t, cab);

    if (dfNuevo == NULL)
        return -1;

    for (int i = 0; i < df->numFilas; i++) {

        if (col->esNulo[df->indice[i]] == 0) {

            for (int j = 0; j < df->numColumnas; j++) {

                if(df->columna[j].esNulo[df->indice[i]]==1) {
                    dfNuevo->columna[j].esNulo[n] = 1;
                }
                else {
                    dfNuevo->columna[j].esNulo[n] = 0;
                    if (df->columna[j].tipo == TEXTO) {
                        ((char **)dfNuevo->columna[j].datos)[n] = strdup(((char **)df->columna[j].datos)[df->indice[i]]);
                    }
                    else if (df->columna[j].tipo == NUMERICO) {
                        ((double *)dfNuevo->columna[j].datos)[n] = ((double *)df->columna[j].datos)[df->indice[i]];
                    }
                    else if (df->columna[j].tipo == FECHA) {
                        ((Fecha *)dfNuevo->columna[j].datos)[n] = ((Fecha *)df->columna[j].datos)[df->indice[i]];
                    }
                }
            }
            n++;
        }
    }
    // Copiar el nombre del dataframe original

    strcpy(dfNuevo->nombre, df->nombre);
    // Actualizar el nodo y liberar el dataframe original
    nodo->df = dfNuevo;
    LiberarDataframe(df);

    return 1;
}

/*int Filtrar(Nodo *nodo, Dataframe *df, Columna *col, char *comp, char *valor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    Dataframe *dfNuevo;
    int numFilas = df->numFilas; // Número inicial de filas
    int n = 0; // Contador de filas copiadas
    char cab[1000] = "";
    TipoDato *t, tipo = col->tipo;
    t = (TipoDato *)malloc(df->numColumnas * sizeof(TipoDato));
    void *val;

    // Verificar que 'comp' no sea NULL
    if (comp == NULL) {
        SetConsoleTextAttribute(hConsole, 4);
        printf("[Error] Operador de comparación (comp) es NULL\n");
        SetConsoleTextAttribute(hConsole, 7);
        return -1; // Código de error
    }

    // Preparar el valor para comparar
    if (tipo == TEXTO) {
        val = strdup(valor);
    } else if (tipo == NUMERICO) {
        val = malloc(sizeof(double));
        *(double *)val = atof(valor);
    } else if (tipo == FECHA) {
        val = AFecha(valor);
        if (!val) {
            printf("[Error] Fecha inválida: %s\n", valor);
            return -1;
        }
    }

    // Crear la cabecera y los tipos de columnas
    for (int i = 0; i < df->numColumnas; i++) {
        t[i] = df->columna[i].tipo;
        strcat(cab, df->columna[i].nombre);
        if (i < df->numColumnas - 1) strcat(cab, ",");
    }

    dfNuevo = CrearDataFrame(numFilas, df->numColumnas, t, cab);
    if (dfNuevo == NULL) {
        free(t);
        free(val);
        return -1;
    }

    // Copiar el nombre del dataframe original al nuevo dataframe
    strcpy(dfNuevo->nombre, df->nombre);

    // Segundo bucle: Copiar filas que cumplen la condición
    for (int i = 0; i < df->numFilas; i++) {
        // Verificar si la fila es nula en la columna especificada
        if (col->esNulo[df->indice[i]] == 1) {
            continue; // Omitir filas con valores nulos
        }

        void *dato;
        if (tipo == NUMERICO) {
            dato = &((double *)col->datos)[df->indice[i]];
        } else if (tipo == FECHA) {
            dato = &((Fecha *)col->datos)[df->indice[i]];
        } else if (tipo == TEXTO) {
            dato = ((char **)col->datos)[df->indice[i]];
        }

        int cumple = 1;

        // Comparaciones
        if (strcmp(comp, "neq") == 0) {
            cumple = (compararValores(dato, val, tipo) != 0);
        } else if (strcmp(comp, "gt") == 0) {
            cumple = (compararValores(dato, val, tipo) > 0);
        } else if (strcmp(comp, "lt") == 0) {
            cumple = (compararValores(dato, val, tipo) < 0);
        } else if (strcmp(comp, "eq") == 0) {
            cumple = (compararValores(dato, val, tipo) == 0);
        }

        // Copiar fila si cumple
        if (cumple) {
            for (int j = 0; j < df->numColumnas; j++) {
                if (df->columna[j].esNulo[df->indice[i]] == 1) {
                    dfNuevo->columna[j].esNulo[n] = 1;
                } else {
                    dfNuevo->columna[j].esNulo[n] = 0;
                    if (df->columna[j].tipo == TEXTO) {
                        ((char **)dfNuevo->columna[j].datos)[n] = strdup(((char **)df->columna[j].datos)[df->indice[i]]);
                    } else if (df->columna[j].tipo == NUMERICO) {
                        ((double *)dfNuevo->columna[j].datos)[n] = ((double *)df->columna[j].datos)[df->indice[i]];
                    } else if (df->columna[j].tipo == FECHA) {
                        ((Fecha *)dfNuevo->columna[j].datos)[n] = ((Fecha *)df->columna[j].datos)[df->indice[i]];
                    }
                }
            }
            n++;
        }
    }

    // Ajustar el número real de filas en el nuevo dataframe
    dfNuevo->numFilas = n;

    // Actualizar el nodo actual con el nuevo dataframe
    nodo->df = dfNuevo;

    // Liberar memoria
    free(val);
    free(t);

    return 1;
}*/

int Filtrar(Nodo *nodo, Dataframe *df, Columna *col, char *comp, char *valor) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    Dataframe *dfNuevo;
    int numFilas = df->numFilas; // Número inicial de filas
    int n = 0; // Contador de filas copiadas
    char cab[1000] = "";
    TipoDato *t, tipo = col->tipo;
    t = (TipoDato *)malloc(df->numColumnas * sizeof(TipoDato));
    void *val;

    // Verificar que 'comp' no sea NULL
    if (comp == NULL) {
        SetConsoleTextAttribute(hConsole, 4);
        printf("[Error] Operador de comparación (comp) es NULL\n");
        SetConsoleTextAttribute(hConsole, 7);
        return -1; // Código de error
    }

    // Preparar el valor para comparar
    if (tipo == TEXTO) {
        val = strdup(valor);
    } else if (tipo == NUMERICO) {
        val = malloc(sizeof(double));
        *(double *)val = atof(valor);
    } else if (tipo == FECHA) {
        val = AFecha(valor);
        if (!val) {
            printf("[Error] Fecha inválida: %s\n", valor);
            return -1;
        }
    }

    // Crear la cabecera y los tipos de columnas
    for (int i = 0; i < df->numColumnas; i++) {
        t[i] = df->columna[i].tipo;
        strcat(cab, df->columna[i].nombre);
        if (i < df->numColumnas - 1) strcat(cab, ",");
    }

    dfNuevo = CrearDataFrame(numFilas, df->numColumnas, t, cab);
    if (dfNuevo == NULL) {
        free(t);
        if (tipo == TEXTO) free(val);
        else free(val);
        return -1;
    }

    // Inicializar todas las cadenas en el nuevo dataframe
    for (int i = 0; i < dfNuevo->numColumnas; i++) {
        if (dfNuevo->columna[i].tipo == TEXTO) {
            for (int j = 0; j < dfNuevo->numFilas; j++) {
                ((char **)dfNuevo->columna[i].datos)[j] = NULL;
            }
        }
    }

    // Copiar filas que cumplen la condición
    for (int i = 0; i < df->numFilas; i++) {
        if (col->esNulo[df->indice[i]] == 1) continue;

        void *dato;
        if (tipo == NUMERICO) {
            dato = &((double *)col->datos)[df->indice[i]];
        } else if (tipo == FECHA) {
            dato = &((Fecha *)col->datos)[df->indice[i]];
        } else if (tipo == TEXTO) {
            dato = ((char **)col->datos)[df->indice[i]];
        }

        int cumple = 1;
        if (strcmp(comp, "neq") == 0) cumple = (compararValores(dato, val, tipo) != 0);
        else if (strcmp(comp, "gt") == 0) cumple = (compararValores(dato, val, tipo) > 0);
        else if (strcmp(comp, "lt") == 0) cumple = (compararValores(dato, val, tipo) < 0);
        else if (strcmp(comp, "eq") == 0) cumple = (compararValores(dato, val, tipo) == 0);
        else if (strcmp(comp, "get") == 0) cumple = (compararValores(dato, val, tipo) >= 0);
        else if (strcmp(comp, "let") == 0) {
            if (tipo == TEXTO) {
                cumple = (compararValores(dato, val, tipo) <= 0) || 
                         (strncmp((char *)dato, (char *)val, strlen((char *)val)) == 0);
            } else {
                cumple = (compararValores(dato, val, tipo) <= 0);
            }
        }

        if (cumple) {
            for (int j = 0; j < df->numColumnas; j++) {
                if (df->columna[j].esNulo[df->indice[i]] == 1) {
                    dfNuevo->columna[j].esNulo[n] = 1;
                } else {
                    dfNuevo->columna[j].esNulo[n] = 0;
                    if (df->columna[j].tipo == TEXTO) {
                        if (((char **)dfNuevo->columna[j].datos)[n] != NULL) {
                            free(((char **)dfNuevo->columna[j].datos)[n]);
                        }
                        ((char **)dfNuevo->columna[j].datos)[n] = strdup(((char **)df->columna[j].datos)[df->indice[i]]);
                    } else if (df->columna[j].tipo == NUMERICO) {
                        ((double *)dfNuevo->columna[j].datos)[n] = ((double *)df->columna[j].datos)[df->indice[i]];
                    } else if (df->columna[j].tipo == FECHA) {
                        ((Fecha *)dfNuevo->columna[j].datos)[n] = ((Fecha *)df->columna[j].datos)[df->indice[i]];
                    }
                }
            }
            n++;
        }
    }

    // Asignar un nuevo nombre para el DataFrame filtrado
    snprintf(dfNuevo->nombre, sizeof(dfNuevo->nombre), "%s", df->nombre);

    // Liberar el dataframe previo si existe
    if (nodo->df != NULL) {
        for (int i = 0; i < nodo->df->numColumnas; i++) {
            if (nodo->df->columna[i].tipo == TEXTO) {
                for (int j = 0; j < nodo->df->numFilas; j++) {
                    if (((char **)nodo->df->columna[i].datos)[j] != NULL) {
                        free(((char **)nodo->df->columna[i].datos)[j]);
                    }
                }
            }
            free(nodo->df->columna[i].datos);
        }
        free(nodo->df->columna);
        free(nodo->df);
    }

    dfNuevo->numFilas = n;
    nodo->df = dfNuevo;

    free(val);
    free(t);

    return 1;
}







// funciones adicionales

// GROUPBY

Columna* BuscarColumna2(Dataframe *df, const char *nombre) {
    for (int i = 0; i < df->numColumnas; i++) {
        if (strcmp(df->columna[i].nombre, nombre) == 0) {
            return &df->columna[i];
        }
    }
    return NULL;
}


int GroupBy(Dataframe *df, const char *nombreColumna) {
    Columna *columna = BuscarColumna2(df, nombreColumna);
    if (columna == NULL) {
        printf("Error: La columna '%s' no existe\n", nombreColumna);
        return -1;
    }

    printf("DEBUG: Agrupando por la columna '%s'\n", columna->nombre);

    int *nuevoOrden = malloc(columna->numFilas * sizeof(int));
    for (int i = 0; i < columna->numFilas; i++) {
        nuevoOrden[i] = i;
    }

    OrdenarPorColumna(columna, nuevoOrden);

    for (int i = 0; i < df->numColumnas; i++) {
        ReordenarColumna(&df->columna[i], nuevoOrden);
    }
    free(nuevoOrden);

    printf("Dataframe agrupado correctamente por la columna '%s'\n", columna->nombre);
    return 1;
}


void OrdenarPorColumna(Columna *columna, int *orden) {
    for (int i = 0; i < columna->numFilas - 1; i++) {
        for (int j = 0; j < columna->numFilas - i - 1; j++) {
            int comparar;

            // Forzar que los valores nulos vayan al principio
            if (columna->esNulo[orden[j]] && columna->esNulo[orden[j + 1]]) {
                comparar = 0; // Ambos son nulos, no hay cambio
            } else if (columna->esNulo[orden[j]]) {
                comparar = -1; // Valor nulo debe permanecer en su posición
            } else if (columna->esNulo[orden[j + 1]]) {
                comparar = 1; // Valor nulo debe ir al principio
            } else {
                // Extraer punteros a datos para comparar
                void *dato1;
                void *dato2;

                if (columna->tipo == TEXTO) {
                    dato1 = ((char **)columna->datos)[orden[j]];
                    dato2 = ((char **)columna->datos)[orden[j + 1]];
                } else if (columna->tipo == NUMERICO) {
                    dato1 = &((double *)columna->datos)[orden[j]];
                    dato2 = &((double *)columna->datos)[orden[j + 1]];
                } else if (columna->tipo == FECHA) {
                    dato1 = &((Fecha *)columna->datos)[orden[j]];
                    dato2 = &((Fecha *)columna->datos)[orden[j + 1]];
                }

                // Usar compararValores para determinar el orden
                comparar = compararValores(dato1, dato2, columna->tipo);
            }

            if (comparar > 0) {
                // Intercambiar índices
                int temp = orden[j];
                orden[j] = orden[j + 1];
                orden[j + 1] = temp;
            }
        }
    }
}

void ReordenarColumna(Columna *columna, int *nuevoOrden) {
    void *nuevosDatos = malloc(columna->numFilas * (columna->tipo == TEXTO ? sizeof(char *) : 
                                                    (columna->tipo == NUMERICO ? sizeof(double) : sizeof(Fecha))));
    unsigned char *nulosReordenados = malloc(columna->numFilas * sizeof(unsigned char));

    if (columna->tipo == TEXTO) {
        char **datos = (char **)columna->datos;
        char **reordenados = (char **)nuevosDatos;
        for (int i = 0; i < columna->numFilas; i++) {
            reordenados[i] = datos[nuevoOrden[i]];
            nulosReordenados[i] = columna->esNulo[nuevoOrden[i]];
        }
    } else if (columna->tipo == NUMERICO) {
        double *datos = (double *)columna->datos;
        double *reordenados = (double *)nuevosDatos;
        for (int i = 0; i < columna->numFilas; i++) {
            reordenados[i] = datos[nuevoOrden[i]];
            nulosReordenados[i] = columna->esNulo[nuevoOrden[i]];
        }
    } else if (columna->tipo == FECHA) {
        Fecha *datos = (Fecha *)columna->datos;
        Fecha *reordenados = (Fecha *)nuevosDatos;
        for (int i = 0; i < columna->numFilas; i++) {
            reordenados[i] = datos[nuevoOrden[i]];  // Copiamos la fecha completa
            nulosReordenados[i] = columna->esNulo[nuevoOrden[i]];
        }
    }

    free(columna->datos);
    free(columna->esNulo);
    columna->datos = nuevosDatos;
    columna->esNulo = nulosReordenados;
}

// ELIMINAR DATAFRAME

//esta función es para extraer el nombre del dataframe del prompt y poder luego comparar
void extraerTexto(const char *cadena, char *resultado, size_t size) {
    // Localizar el carácter ':'
    const char *fin = strchr(cadena, ':');
    if (!fin) {
        resultado[0] = '\0'; // Si no se encuentra ':', devolver cadena vacía
        return;
    }
    
    // Calcular la longitud del texto hasta ':'
    size_t len = fin - cadena;
    if (len >= size) {
        len = size - 1; // Ajustar si excede el tamaño del buffer
    }
    
    // Copiar el texto hasta ':'
    strncpy(resultado, cadena, len);
    resultado[len] = '\0'; // Agregar terminación nula
}

int EliminarDataframe(Lista *laGranLista, const char *nombreDF) {
    if (!laGranLista || laGranLista->numDFs == 0) {
        //printf("Error: No hay dataframes cargados.\n");
        return -1;
    }

    Nodo *actual = laGranLista->primero;
    Nodo *anterior = NULL;

    while (actual != NULL) {
        if (strcmp(actual->df->nombre, nombreDF) == 0) {
            // Liberar el dataframe
            LiberarDataframe(actual->df);

            // Ajustar punteros de la lista
            if (anterior == NULL) {
                laGranLista->primero = actual->siguiente;
            } else {
                anterior->siguiente = actual->siguiente;
            }

            free(actual);
            laGranLista->numDFs--;

            //printf("Dataframe '%s' eliminado exitosamente.\n", nombreDF);
            return 0;
        }

        anterior = actual;
        actual = actual->siguiente;
    }

    //printf("Error: Dataframe '%s' no encontrado.\n", nombreDF);
    return -1;
}

// COPY

Nodo *BuscarNodoPorNombre(Lista *lista, const char *nombreDF) {
    Nodo *actual = lista->primero;
    while (actual) {
        if (strcmp(actual->df->nombre, nombreDF) == 0) {
            return actual;
        }
        actual = actual->siguiente;
    }
    return NULL;
}


Dataframe *DuplicarDataframe(const Dataframe *original) {
    // Crear un nuevo dataframe
    Dataframe *nuevoDF = malloc(sizeof(Dataframe));
    if (!nuevoDF) {
        //printf("Error: No se pudo asignar memoria para el nuevo dataframe.\n");
        return NULL;
    }

    // Copiar el nombre
    snprintf(nuevoDF->nombre, sizeof(nuevoDF->nombre), "%s_copy", original->nombre);

    // Copiar las dimensiones
    nuevoDF->numFilas = original->numFilas;
    nuevoDF->numColumnas = original->numColumnas;

    // Copiar el índice
    nuevoDF->indice = malloc(original->numFilas * sizeof(int));
    if (!nuevoDF->indice) {
        //printf("Error: No se pudo asignar memoria para el índice.\n");
        free(nuevoDF);
        return NULL;
    }
    memcpy(nuevoDF->indice, original->indice, original->numFilas * sizeof(int));

    // Copiar las columnas
    nuevoDF->columna = malloc(original->numColumnas * sizeof(Columna));
    if (!nuevoDF->columna) {
        //printf("Error: No se pudo asignar memoria para las columnas.\n");
        free(nuevoDF->indice);
        free(nuevoDF);
        return NULL;
    }

    for (int i = 0; i < original->numColumnas; i++) {
        Columna *colNueva = &nuevoDF->columna[i];
        Columna *colOriginal = &original->columna[i];

        strncpy(colNueva->nombre, colOriginal->nombre, sizeof(colNueva->nombre) - 1);
        colNueva->nombre[sizeof(colNueva->nombre) - 1] = '\0';
        colNueva->tipo = colOriginal->tipo;
        colNueva->numFilas = colOriginal->numFilas;

        size_t tamElemento = (colNueva->tipo == NUMERICO) ? sizeof(double) :
                             (colNueva->tipo == FECHA) ? sizeof(Fecha) : sizeof(char *);
        colNueva->datos = malloc(colNueva->numFilas * tamElemento);
        if (!colNueva->datos) {
            //printf("Error: No se pudo asignar memoria para los datos de la columna '%s'.\n", colNueva->nombre);
            LiberarDataframe(nuevoDF);
            return NULL;
        }

        if (colNueva->tipo == TEXTO) {
            char **originalDatos = (char **)colOriginal->datos;
            char **nuevosDatos = (char **)colNueva->datos;
            for (int j = 0; j < colNueva->numFilas; j++) {
                if (originalDatos[j]) {
                    nuevosDatos[j] = strdup(originalDatos[j]);
                    if (!nuevosDatos[j]) {
                        //printf("Error: No se pudo duplicar un valor de texto.\n");
                        LiberarDataframe(nuevoDF);
                        return NULL;
                    }
                } else {
                    nuevosDatos[j] = NULL;
                }
            }
        } else {
            memcpy(colNueva->datos, colOriginal->datos, colNueva->numFilas * tamElemento);
        }

        colNueva->esNulo = malloc(colNueva->numFilas * sizeof(unsigned char));
        if (!colNueva->esNulo) {
            //printf("Error: No se pudo asignar memoria para los valores nulos.\n");
            LiberarDataframe(nuevoDF);
            return NULL;
        }
        memcpy(colNueva->esNulo, colOriginal->esNulo, colNueva->numFilas * sizeof(unsigned char));
    }

    return nuevoDF;
}

// AGREGAR COLUMNA NUEVA DF

// Función auxiliar para eliminar espacios en blanco iniciales y finales
void TrimEspacios(char *str) {
    char *start = str;
    char *end = str + strlen(str) - 1;

    // Mover el puntero de inicio hasta el primer carácter no espacio
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    // Mover el puntero de fin hasta el último carácter no espacio
    while (end > start && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }

    // Mover la cadena al inicio si es necesario
    if (str != start) {
        memmove(str, start, end - start + 2); // +2 para incluir '\0'
    }
}


int AgregarColumnaNueva(Dataframe *df, const char *nombre, TipoDato tipo) {

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
            SetConsoleTextAttribute(hConsole, 4);
            //printf("Error: el comando no debe tener espacios al principio\n");
            SetConsoleTextAttribute(hConsole, 7);
    // Verificar si ya existe una columna con el mismo nombre
    for (int i = 0; i < df->numColumnas; i++) {
        if (strcmp(df->columna[i].nombre, nombre) == 0) {
            printf("Error: Ya existe una columna con el nombre '%s'.\n", nombre);
            return -1;
        }
    }

    // Redimensionar el array de columnas
    Columna *nuevaMemoria = realloc(df->columna, (df->numColumnas + 1) * sizeof(Columna));
    if (!nuevaMemoria) {
        //printf("Error: No se pudo asignar memoria para la nueva columna.\n");
        return -1;
    }
    df->columna = nuevaMemoria;

    // Crear la nueva columna
    Columna *nuevaColumna = &df->columna[df->numColumnas];
    strncpy(nuevaColumna->nombre, nombre, sizeof(nuevaColumna->nombre) - 1);
    nuevaColumna->nombre[sizeof(nuevaColumna->nombre) - 1] = '\0';
    nuevaColumna->tipo = tipo;
    nuevaColumna->numFilas = df->numFilas;

    // Asignar memoria para los datos
    size_t tamElemento = (tipo == NUMERICO) ? sizeof(double) :
                         (tipo == FECHA) ? sizeof(Fecha) : sizeof(char *);
    nuevaColumna->datos = calloc(df->numFilas, tamElemento);
    if (!nuevaColumna->datos) {
        SetConsoleTextAttribute(hConsole, 4);
        printf("Error: No se pudo asignar memoria para los datos de la nueva columna.\n");
        SetConsoleTextAttribute(hConsole, 7);
        return -1;
    }

    // Inicializar valores nulos
    nuevaColumna->esNulo = calloc(df->numFilas, sizeof(unsigned char));
    if (!nuevaColumna->esNulo) {
        //printf("Error: No se pudo asignar memoria para los valores nulos.\n");
        free(nuevaColumna->datos);
        return -1;
    }

    // Pedir valores al usuario
    printf("Introduce los valores para la nueva columna '%s' (fila por fila):\n", nombre);
    for (int i = 0; i < df->numFilas; i++) {
        while (1) { // Repetir hasta que el valor sea válido
            printf("Fila %d: ", i);
            char entrada[100];
            fgets(entrada, sizeof(entrada), stdin);

            // Limpiar el salto de línea y espacios
            entrada[strcspn(entrada, "\n")] = '\0';

            // Comprobar si la entrada está vacía
            if (strlen(entrada) == 0) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: El valor no puede estar vacio. Intenta de nuevo.\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue; // Pedir de nuevo la entrada
            }

            // Si el usuario introduce "NULL", marcar como nulo
            if (strcmp(entrada, "NULL") == 0) {
                nuevaColumna->esNulo[i] = 1; // Es nulo
                if (tipo == TEXTO) {
                    ((char **)nuevaColumna->datos)[i] = NULL;
                }
                break; // Continuar con la siguiente fila
            } else {
                nuevaColumna->esNulo[i] = 0; // No es nulo
                if (tipo == TEXTO) {
                    // Validar que la entrada contenga solo letras
                    int valido = 1;
                    for (int j = 0; entrada[j] != '\0'; j++) {
                        if (!isalpha((unsigned char)entrada[j])) {
                            valido = 0;
                            break;
                        }
                    }
                    if (!valido) {
                        SetConsoleTextAttribute(hConsole, 4);
                        printf("Error: El valor debe contener unicamente letras (A-Z, a-z). Intenta de nuevo.\n");
                        SetConsoleTextAttribute(hConsole, 7);
                        continue; // Pedir de nuevo la entrada
                    }
                    ((char **)nuevaColumna->datos)[i] = strdup(entrada);
                } else if (tipo == NUMERICO) {
                    // Validar que la entrada sea un número válido
                    char *endptr;
                    double valor = strtod(entrada, &endptr);
                    if (*endptr != '\0') {
                        SetConsoleTextAttribute(hConsole, 4);
                        printf("Error: El valor debe ser un numero valido. Intenta de nuevo.\n");
                        SetConsoleTextAttribute(hConsole, 7);
                        continue; // Pedir de nuevo la entrada
                    }
                    ((double *)nuevaColumna->datos)[i] = valor;
                } else if (tipo == FECHA) {
                    // Validar formato y existencia de la fecha
                    // Validar formato y existencia de la fecha
                    if (!FechaValidarAMD(entrada)) {
                        SetConsoleTextAttribute(hConsole, 4);
                        printf("Error: Fecha invalida. Verifica el formato y la existencia de la fecha (incluyendo anyos bisiestos). Intenta de nuevo.\n");
                        SetConsoleTextAttribute(hConsole, 7);
                        continue; // Pedir de nuevo la entrada
                    }

                    // Extraer el año, mes y día manualmente
                    int anio = 0, mes = 0, dia = 0;

                    // Extraer manualmente las partes de la fecha (AAAA-MM-DD o AAAA/MM/DD)
                    anio = (entrada[0] - '0') * 1000 + (entrada[1] - '0') * 100 + (entrada[2] - '0') * 10 + (entrada[3] - '0');
                    mes = (entrada[5] - '0') * 10 + (entrada[6] - '0');
                    dia = (entrada[8] - '0') * 10 + (entrada[9] - '0');

                    // Crear la estructura Fecha
                    Fecha fecha = {0};
                    fecha.tm_year = anio;
                    fecha.tm_mon = mes - 1; // Ajustar mes al rango 0-11
                    fecha.tm_mday = dia;

                    // Guardar la fecha en la columna
                    ((Fecha *)nuevaColumna->datos)[i] = fecha;

                    // Depuración
                    //printf("DEBUG: Año = %d, Mes = %d, Día = %d\n", fecha.tm_year, fecha.tm_mon + 1, fecha.tm_mday);

                }
                break; // Salir del bucle si todo es válido
            }
        }
    }
    // Incrementar el número de columnas
    df->numColumnas++;
    return 0;
}


// ELIMINAR TODOS LOS DATAFRAMES

void EliminarTodosLosDataframes(Lista *laGranLista) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (!laGranLista || !laGranLista->primero) {
        SetConsoleTextAttribute(hConsole, 4);
        printf("No hay dataframes para eliminar.\n");
        SetConsoleTextAttribute(hConsole, 7);
        return;
    }

    Nodo *actual = laGranLista->primero;
    while (actual) {
        Nodo *siguiente = actual->siguiente;

        // Liberar memoria del dataframe y del nodo
        if (actual->df) {
            //printf("Eliminando dataframe '%s'\n", actual->df->nombre);
            LiberarDataframe(actual->df);
            free(actual->df);
        }
        free(actual);
        actual = siguiente;
    }

    // Reiniciar la lista
    laGranLista->primero = NULL;
    laGranLista->numDFs = 0;

    // Actualizar el prompt
    //printf("Todos los dataframes han sido eliminados.\n");
}

int ReplaceDatoEnColumna(Dataframe *df, const char *nombreColumna, const char *dato, const char *datoReemplazar) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    if (!df || !nombreColumna || !dato || !datoReemplazar) {
        SetConsoleTextAttribute(hConsole, 4);
        //printf("Error: Parámetros inválidos.\n");
        SetConsoleTextAttribute(hConsole, 7);
        return -1;
    }

    // Buscar la columna
    Columna *col = NULL;
    for (int i = 0; i < df->numColumnas; i++) {
        if (strcmp(df->columna[i].nombre, nombreColumna) == 0) {
            col = &df->columna[i];
            break;
        }
    }

    if (!col) {
        SetConsoleTextAttribute(hConsole, 4);
       //printf("Error: Columna '%s' no encontrada.\n", nombreColumna);
        SetConsoleTextAttribute(hConsole, 7);
        return -1;
    }

    int reemplazosRealizados = 0;

    // Recorrer la columna y buscar coincidencias
    for (int i = 0; i < col->numFilas; i++) {
        //printf("[Depuración] Fila %d: Valor actual -> ", i);
        if (col->esNulo[i]) {
            printf("NULO\n");
            continue; // Ignorar valores ya nulos
        }

        if (col->tipo == TEXTO) {
            char *valor = ((char **)col->datos)[i];
            //printf("'%s' (Texto)\n", valor ? valor : "NULL");
            if (valor && strcmp(valor, dato) == 0) {
                free(((char **)col->datos)[i]); // Liberar la memoria actual
                if (strcmp(datoReemplazar, "NULL") == 0 || strcmp(datoReemplazar, "#N/A") == 0) {
                    ((char **)col->datos)[i] = strdup("#N/A"); // Guardar como texto explícito
                    col->esNulo[i] = 1; // Marcar como nulo
                } else {
                    ((char **)col->datos)[i] = strdup(datoReemplazar); // Asignar el nuevo valor
                    col->esNulo[i] = 0; // Marcar como no nulo
                }
                //printf("[Depuración] Reemplazado por '%s'\n", ((char **)col->datos)[i]);
                reemplazosRealizados++;
            }
        }if (col->tipo == NUMERICO) {
            double valor = ((double *)col->datos)[i];
            double valorBuscar = strtod(dato, NULL);
            //printf("[Depuración] %.2f (Numérico)\n", valor);
            if (valor == valorBuscar) {
                if (strcmp(datoReemplazar, "NULL") == 0 || strcmp(datoReemplazar, "#N/A") == 0) {
                    col->esNulo[i] = 1; // Marcar como nulo
                    //printf("[Depuración] Marcado como nulo (Numérico)\n");
                } else {
                    ((double *)col->datos)[i] = strtod(datoReemplazar, NULL); // Reemplazar valor
                    col->esNulo[i] = 0; // Marcar como no nulo
                    //printf("[Depuración] Reemplazado por %.2f\n", ((double *)col->datos)[i]);
                }
                reemplazosRealizados++;
            }
        } else if (col->tipo == FECHA) {
            Fecha fechaBuscar, fechaReemplazar, fechaColumna;

            if (!FechaValidarAMD((char *)dato)) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: Formato de fecha invalido. Use 'AAAA-MM-DD' o 'AAAA/MM/DD'.\n");
                SetConsoleTextAttribute(hConsole, 7);
                return -1;
            }

            sscanf(dato, "%d%*[-/]%d%*[-/]%d", &fechaBuscar.tm_year, &fechaBuscar.tm_mon, &fechaBuscar.tm_mday);
            fechaBuscar.tm_mon -= 1; // Ajustar mes al rango 0-11

            Fecha *valor = &((Fecha *)col->datos)[i];
            fechaColumna = *valor;

            //printf("[Depuración] Fecha actual -> %d-%02d-%02d\n",
                   //fechaColumna.tm_year, fechaColumna.tm_mon + 1, fechaColumna.tm_mday);

            if (fechaColumna.tm_year == fechaBuscar.tm_year &&
                fechaColumna.tm_mon == fechaBuscar.tm_mon &&
                fechaColumna.tm_mday == fechaBuscar.tm_mday) {
                if (strcmp(datoReemplazar, "NULL") == 0 || strcmp(datoReemplazar, "#N/A") == 0) {
                    ((char **)col->datos)[i] = strdup("#N/A"); // Guardar como texto explícito
                    col->esNulo[i] = 1; // Marcar como nulo
                    //printf("[Depuración] Fecha reemplazada por '#N/A'\n");
                } else if (FechaValidarAMD((char *)datoReemplazar)) {
                    sscanf(datoReemplazar, "%d%*[-/]%d%*[-/]%d",
                        &fechaReemplazar.tm_year, &fechaReemplazar.tm_mon, &fechaReemplazar.tm_mday);
                    fechaReemplazar.tm_mon -= 1;
                    ((Fecha *)col->datos)[i] = fechaReemplazar; // Asignar nueva fecha
                    col->esNulo[i] = 0; // Marcar como no nulo
                    //printf("[Depuración] Fecha reemplazada correctamente\n");
                }
                reemplazosRealizados++;
            }
        }
    }

    if (reemplazosRealizados == 0) {
        SetConsoleTextAttribute(hConsole, 4);
        printf("Error: No se encontraron coincidencias del valor '%s' en la columna '%s'.\n", dato, nombreColumna);
        SetConsoleTextAttribute(hConsole, 7);
        return -1;
    }

    return reemplazosRealizados;
}




// RENAME COL

int CambiarNombreColumna(Dataframe *df, const char *nombreActual, const char *nuevoNombre) {
    if (!df || !nombreActual || !nuevoNombre) {
        //printf("Error: Parámetros inválidos.\n");
        return -1;
    }

    // Verificar si la columna con el nombre actual existe
    Columna *columna = NULL;
    for (int i = 0; i < df->numColumnas; i++) {
        if (strcmp(df->columna[i].nombre, nombreActual) == 0) {
            columna = &df->columna[i];
            break;
        }
    }

    if (!columna) {
        //printf("Error: La columna '%s' no existe en el dataframe.\n", nombreActual);
        return -1;
    }

    // Verificar si el nuevo nombre ya está en uso
    for (int i = 0; i < df->numColumnas; i++) {
        if (strcmp(df->columna[i].nombre, nuevoNombre) == 0) {
            //printf("Error: Ya existe una columna con el nombre '%s'.\n", nuevoNombre);
            return -1;
        }
    }

    // Cambiar el nombre de la columna
    strncpy(columna->nombre, nuevoNombre, sizeof(columna->nombre) - 1);
    columna->nombre[sizeof(columna->nombre) - 1] = '\0'; // Asegurar terminación nula

    //printf("El nombre de la columna '%s' se ha cambiado a '%s'.\n", nombreActual, nuevoNombre);
    return 0;
}


// CALCULOS

int compararValoresNum(const double *a, const double *b) {
    return (*a > *b) - (*a < *b);
}

// Para operaciones matemáticas

double OperacionColumnaNumerica(Dataframe *df, const char *colname, const char *operacion) {
    // Validar dataframe
    if (!df) {
        //printf("Error: No hay dataframe activo.\n");
        return NAN; // Retorna "Not a Number"
    }

    // Buscar columna
    Columna *col = NULL;
    for (int i = 0; i < df->numColumnas; i++) {
        if (strcmp(df->columna[i].nombre, colname) == 0) {
            col = &df->columna[i];
            break;
        }
    }

    if (!col) {
        //printf("Error: La columna '%s' no existe.\n", colname);
        return NAN;
    }

    // Verificar que la columna sea numérica
    if (col->tipo != NUMERICO) {
        //printf("Error: La columna '%s' no es de tipo NUMERICO.\n", colname);
        return NAN;
    }

    // Preparar variables para los cálculos
    double suma = 0, min = INFINITY, max = -INFINITY;
    int count = 0;

    // Recorrer la columna
    for (int i = 0; i < col->numFilas; i++) {
        if (col->esNulo[i]) {
            continue; // Ignorar valores nulos
        }

        double valor = ((double *)col->datos)[i];
        suma += valor;
        if (valor < min) min = valor;
        if (valor > max) max = valor;
        count++;
    }

    if (count == 0) {
        //printf("Error: Todos los valores en la columna '%s' son nulos.\n", colname);
        return NAN;
    }

    // Calcular resultados
    if (strcmp(operacion, "SUM") == 0) {
        return suma;
    } else if (strcmp(operacion, "AVG") == 0) {
        return suma / count;
    } else if (strcmp(operacion, "MIN") == 0) {
        return min;
    } else if (strcmp(operacion, "MAX") == 0) {
        return max;
    } else if (strcmp(operacion, "MEDIAN") == 0) {
        // Copiar valores no nulos a un array temporal para calcular la mediana
        double *valores = malloc(count * sizeof(double));
        if (!valores) {
            //printf("Error: No se pudo asignar memoria para calcular la mediana.\n");
            return NAN;
        }

        int j = 0;
        for (int i = 0; i < col->numFilas; i++) {
            if (!col->esNulo[i]) {
                valores[j++] = ((double *)col->datos)[i];
            }
        }

        // Ordenar los valores
        qsort(valores, count, sizeof(double), (int (*)(const void *, const void *))compararValoresNum);

        // Calcular la mediana
        double mediana;
        if (count % 2 == 0) {
            mediana = (valores[count / 2 - 1] + valores[count / 2]) / 2.0;
        } else {
            mediana = valores[count / 2];
        }

        free(valores);
        return mediana;
    } else {
        //printf("Error: Operación '%s' no soportada.\n", operacion);
        return NAN;
    }
}


// FILTRAR ENTRE (NUEVA VERSIÓN, CON ENTRE !!!)
int Filtrar2(Nodo *nodo, Dataframe *df, Columna *col, char *comp, char *val1, char *val2) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    Dataframe *dfNuevo;
    int numFilas = df->numFilas, n = 0;
    char cab[1000] = "";
    TipoDato tipo = col->tipo;

    void *valor1 = NULL, *valor2 = NULL;

    //printf("[Depuración] Inicio de la función Filtrar2\n");
    //printf("[Depuración] Operador: %s, Valor1: %s, Valor2: %s\n", comp, val1, val2 ? val2 : "NULL");

    // Preparar valores para el operador "entre"
    if (strcmp(comp, "entre") == 0) {
        if (tipo == TEXTO) {
            valor1 = strdup(val1);
            valor2 = strdup(val2);
        } else if (tipo == NUMERICO) {
            valor1 = malloc(sizeof(double));
            *(double *)valor1 = atof(val1);
            valor2 = malloc(sizeof(double));
            *(double *)valor2 = atof(val2);
        } else if (tipo == FECHA) {
            valor1 = AFecha(val1);
            valor2 = AFecha(val2);
            if (!valor1 || !valor2) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("[Error] Fechas invalidas\n");
                SetConsoleTextAttribute(hConsole, 7);
                return -1;
            }
        }
    } else {
        // Preparar valor único para otros operadores
        if (tipo == TEXTO) {
            valor1 = strdup(val1);
        } else if (tipo == NUMERICO) {
            valor1 = malloc(sizeof(double));
            *(double *)valor1 = atof(val1);
        } else if (tipo == FECHA) {
            valor1 = AFecha(val1);
            if (!valor1) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("[Error] Fecha invalida\n");
                SetConsoleTextAttribute(hConsole, 7);
                return -1;
            }
        }
    }

    // Crear la cabecera y los tipos de columnas
    //printf("[Depuración] Creando cabecera y tipos para el nuevo DataFrame\n");
    TipoDato *tipos = (TipoDato *)malloc(df->numColumnas * sizeof(TipoDato));
    if (!tipos) {
        SetConsoleTextAttribute(hConsole, 4);
        //printf("[Error] No se pudo asignar memoria para los tipos de columna\n");
        SetConsoleTextAttribute(hConsole, 7);
        free(valor1);
        free(valor2);
        return -1;
    }

    for (int i = 0; i < df->numColumnas; i++) {
        strcat(cab, df->columna[i].nombre);
        tipos[i] = df->columna[i].tipo;
        if (i < df->numColumnas - 1) strcat(cab, ",");
    }

    dfNuevo = CrearDataFrame(numFilas, df->numColumnas, tipos, cab);

    if (!dfNuevo) {
        SetConsoleTextAttribute(hConsole, 4);
        //printf("[Error] No se pudo crear el nuevo DataFrame\n");
        SetConsoleTextAttribute(hConsole, 7);
        free(valor1);
        free(valor2);
        free(tipos);
        return -1;
    }

    strncpy(dfNuevo->nombre, df->nombre, sizeof(dfNuevo->nombre) - 1);
    dfNuevo->nombre[sizeof(dfNuevo->nombre) - 1] = '\0';
   // printf("[Depuración] Nombre del nuevo DataFrame: %s\n", dfNuevo->nombre);

    free(tipos);

    // Filtrar filas
   // printf("[Depuración] Filtrando filas\n");
    for (int i = 0; i < df->numFilas; i++) {
        void *dato;
        if (tipo == NUMERICO) {
            dato = &((double *)col->datos)[df->indice[i]];
        } else if (tipo == FECHA) {
            dato = &((Fecha *)col->datos)[df->indice[i]];
        } else if (tipo == TEXTO) {
            dato = ((char **)col->datos)[df->indice[i]];
        }

        int cumple = 0;

        if (strcmp(comp, "entre") == 0) {
            if (tipo == TEXTO) {
                char primeraLetraDato = '\0', primeraLetraVal1 = '\0', primeraLetraVal2 = '\0';

                // Extraer el primer carácter de dato y los valores de comparación
                if (dato != NULL && ((char *)dato)[0] != '\0') {
                    primeraLetraDato = tolower(((char *)dato)[0]);
                }

                if (valor1 != NULL && ((char *)valor1)[0] != '\0') {
                    primeraLetraVal1 = tolower(((char *)valor1)[0]);
                }

                if (valor2 != NULL && ((char *)valor2)[0] != '\0') {
                    primeraLetraVal2 = tolower(((char *)valor2)[0]);
                }

                // Comparación circular
                if (primeraLetraVal1 <= primeraLetraVal2) {
                    // Caso normal: rango continuo (por ejemplo, a - c)
                    cumple = (primeraLetraDato >= primeraLetraVal1 && primeraLetraDato <= primeraLetraVal2);
                } else {
                    // Caso circular: rango cruzado (por ejemplo, z - d)
                    cumple = (primeraLetraDato >= primeraLetraVal1 || primeraLetraDato <= primeraLetraVal2);
                }

                /*printf("[Depuración] Comparando texto 'entre' (circular): '%c' está entre [%c, %c] -> %s\n",
                    primeraLetraDato, primeraLetraVal1, primeraLetraVal2, cumple ? "Cumple" : "No cumple");*/
            } else {
                // Lógica para otros tipos de datos
                cumple = (compararValores(dato, valor1, tipo) >= 0 &&
                        compararValores(dato, valor2, tipo) <= 0);
            }

        } else if (strcmp(comp, "eq") == 0) {
            cumple = (compararValores(dato, valor1, tipo) == 0);
        } else if (strcmp(comp, "neq") == 0) {
            cumple = (compararValores(dato, valor1, tipo) != 0);
        } else if (strcmp(comp, "gt") == 0) {
            cumple = (compararValores(dato, valor1, tipo) > 0);
        } else if (strcmp(comp, "lt") == 0) {
            cumple = (compararValores(dato, valor1, tipo) < 0);
        }

        if (cumple) {
           // printf("[Depuración] Copiando fila %d al nuevo DataFrame\n", i);
            for (int j = 0; j < df->numColumnas; j++) {
                if (df->columna[j].esNulo[df->indice[i]]) {
                    dfNuevo->columna[j].esNulo[n] = 1;
                } else {
                    dfNuevo->columna[j].esNulo[n] = 0;
                    if (df->columna[j].tipo == TEXTO) {
                        ((char **)dfNuevo->columna[j].datos)[n] = strdup(((char **)df->columna[j].datos)[df->indice[i]]);
                    } else if (df->columna[j].tipo == NUMERICO) {
                        ((double *)dfNuevo->columna[j].datos)[n] = ((double *)df->columna[j].datos)[df->indice[i]];
                    } else if (df->columna[j].tipo == FECHA) {
                        ((Fecha *)dfNuevo->columna[j].datos)[n] = ((Fecha *)df->columna[j].datos)[df->indice[i]];
                    }
                }
            }
            n++;
        }
    }

    dfNuevo->numFilas = n;
    nodo->df = dfNuevo;

   // printf("[Depuración] Proceso completado. Filas filtradas: %d\n", n);

    free(valor1);
    free(valor2);

    return 1;
}

int Save2(Dataframe *df, char *nombre) {
    FILE *fichero;
    char aux[50] = "./";
    char respuesta[5]; // Buffer para la respuesta del usuario

    strcat(aux, nombre);

    if (strchr(nombre, '.') == NULL) { // Entramos en el caso de que no tenga .csv
        strcat(aux, ".csv");
    }

    // Verificar si el archivo ya existe
    fichero = fopen(aux, "r");
    if (fichero != NULL) { // El archivo existe
        fclose(fichero);
        printf("[Aviso] El fichero '%s' ya existe. ¿Desea sobreescribirlo? (y/yes para confirmar, cualquier otra tecla para cancelar): ", aux);
        fgets(respuesta, sizeof(respuesta), stdin);

        // Eliminar el carácter de nueva línea que puede incluirse en fgets
        respuesta[strcspn(respuesta, "\n")] = '\0';

        if (strcasecmp(respuesta, "y") != 0 && strcasecmp(respuesta, "yes") != 0) {
            //printf("[Info] Operacion de guardado cancelada.\n");
            return 0; // Cancelar la operación
        }
    }

    // Proceder a abrir el archivo en modo escritura
    fichero = fopen(aux, "w");
    if (fichero != NULL) {
        // Escribir la cabecera
        for (int i = 0; i < df->numColumnas; i++) {
            if (i > 0)
                fprintf(fichero, ",");

            fprintf(fichero, "%s", df->columna[i].nombre);
        }

        // Escribir los datos fila por fila
        for (int i = 0; i < df->numFilas; i++) {
            fprintf(fichero, "\n");
            for (int j = 0; j < df->numColumnas; j++) {
                if (j > 0)
                    fprintf(fichero, ",");

                if (df->columna[j].esNulo[df->indice[i]] == 0) {
                    fprintf(fichero, "%s", printElemento(df, j, i));
                }
            }
        }
    } else {
        //printf("[Error] No se pudo abrir el fichero para escritura.\n");
        return -1;
    }

    fclose(fichero);
    //printf("[Info] Datos guardados correctamente en '%s'.\n", aux);
    return 1;
}



int AgregarFila(Dataframe *df, void **valores) {
    if (df == NULL || valores == NULL) {
        //printf("[Error] Dataframe o valores proporcionados son nulos.\n");
        return -1;
    }

    //printf("[Depuración] Intentando agregar una nueva fila al DataFrame:\n");
    for (int i = 0; i < df->numColumnas; i++) {
        if (valores[i] == NULL) {
            //printf("  Columna %d (%s): Valor nulo\n", i + 1, df->columna[i].nombre);
        } else if (df->columna[i].tipo == TEXTO) {
            //printf("  Columna %d (%s): TEXTO -> '%s'\n", i + 1, df->columna[i].nombre, (char *)valores[i]);
        } else if (df->columna[i].tipo == NUMERICO) {
            //printf("  Columna %d (%s): NUMERICO -> %f\n", i + 1, df->columna[i].nombre, *(double *)valores[i]);
        } else if (df->columna[i].tipo == FECHA) {
            Fecha *fecha = (Fecha *)valores[i];
            /*printf("  Columna %d (%s): FECHA -> %04d/%02d/%02d\n",
                   i + 1, df->columna[i].nombre,
                   fecha->tm_year, fecha->tm_mon + 1, fecha->tm_mday);*/
        } else {
            //printf("  Columna %d (%s): Tipo desconocido\n", i + 1, df->columna[i].nombre);
        }
    }

    // Validar que el DataFrame tiene columnas
    if (df->numColumnas <= 0) {
        //printf("[Error] El dataframe no tiene columnas definidas.\n");
        return -1;
    }

    // Ampliar el almacenamiento del índice
    int *nuevoIndice = realloc(df->indice, (df->numFilas + 1) * sizeof(int));
    if (!nuevoIndice) {
        //printf("[Error] No se pudo asignar memoria para los índices.\n");
        return -1;
    }
    df->indice = nuevoIndice;
    df->indice[df->numFilas] = df->numFilas; // Nuevo índice para la fila

    // Agregar los valores a las columnas
    for (int i = 0; i < df->numColumnas; i++) {
        if (valores[i] == NULL) {
            df->columna[i].esNulo = realloc(df->columna[i].esNulo, (df->numFilas + 1) * sizeof(int));
            df->columna[i].esNulo[df->numFilas] = 1; // Valor nulo
            continue;
        }

        df->columna[i].esNulo = realloc(df->columna[i].esNulo, (df->numFilas + 1) * sizeof(int));
        df->columna[i].esNulo[df->numFilas] = 0; // No nulo

        if (df->columna[i].tipo == TEXTO) {
            char **temp = realloc(df->columna[i].datos, (df->numFilas + 1) * sizeof(char *));
            if (!temp) {
               // printf("[Error] No se pudo asignar memoria para la columna de texto.\n");
                return -1;
            }
            df->columna[i].datos = (void *)temp;
            ((char **)df->columna[i].datos)[df->numFilas] = strdup((char *)valores[i]);
        } else if (df->columna[i].tipo == NUMERICO) {
            double *temp = realloc(df->columna[i].datos, (df->numFilas + 1) * sizeof(double));
            if (!temp) {
                //printf("[Error] No se pudo asignar memoria para la columna numérica.\n");
                return -1;
            }
            df->columna[i].datos = (void *)temp;
            ((double *)df->columna[i].datos)[df->numFilas] = *(double *)valores[i];
        } else if (df->columna[i].tipo == FECHA) {
            Fecha *temp = realloc(df->columna[i].datos, (df->numFilas + 1) * sizeof(Fecha));
            if (!temp) {
                //printf("[Error] No se pudo asignar memoria para la columna de fecha.\n");
                return -1;
            }
            df->columna[i].datos = (void *)temp;
            ((Fecha *)df->columna[i].datos)[df->numFilas] = *(Fecha *)valores[i];
        }
    }

    
    // Incrementar el número de filas en el DataFrame
    df->numFilas++;
    //printf("[Info] Fila agregada correctamente. Total de filas: %d\n", df->numFilas);
    return 1;
}




int CrearYAgregarColumna(Dataframe *destino, Columna *origen) {
    // Mensaje de inicio
   // printf("[DEBUG] Creando nueva columna '%s' en el dataframe destino\n", origen->nombre);

    // Reasignar memoria para las columnas en el dataframe destino
    destino->columna = realloc(destino->columna, (destino->numColumnas + 1) * sizeof(Columna));
    if (destino->columna == NULL) {
     //   printf("Error: No se pudo reasignar memoria para la nueva columna\n");
        return -1;
    }

    // Referencia a la nueva columna en el dataframe destino
    Columna *nuevaColumna = &destino->columna[destino->numColumnas];
    strcpy(nuevaColumna->nombre, origen->nombre);
    nuevaColumna->tipo = origen->tipo;
    nuevaColumna->numFilas = destino->numFilas; // Igualamos al número de filas del dataframe destino

    // Asignar memoria para datos y valores nulos
    nuevaColumna->esNulo = malloc(nuevaColumna->numFilas * sizeof(unsigned char));
    if (nuevaColumna->esNulo == NULL) {
        //printf("Error: No se pudo asignar memoria para esNulo\n");
        return -1;
    }
    memset(nuevaColumna->esNulo, 1, nuevaColumna->numFilas); // Inicializar todo como nulo

    if (nuevaColumna->tipo == TEXTO) {
        nuevaColumna->datos = malloc(nuevaColumna->numFilas * sizeof(char *));
        if (nuevaColumna->datos == NULL) {
            //printf("Error: No se pudo asignar memoria para datos de tipo TEXTO\n");
            free(nuevaColumna->esNulo);
            return -1;
        }
        for (int i = 0; i < nuevaColumna->numFilas; i++) {
            ((char **)nuevaColumna->datos)[i] = NULL;
        }
    } else if (nuevaColumna->tipo == NUMERICO) {
        nuevaColumna->datos = malloc(nuevaColumna->numFilas * sizeof(double));
        if (nuevaColumna->datos == NULL) {
            printf("Error: No se pudo asignar memoria para datos de tipo NUMERICO\n");
            free(nuevaColumna->esNulo);
            return -1;
        }
    } else if (nuevaColumna->tipo == FECHA) {
        nuevaColumna->datos = malloc(nuevaColumna->numFilas * sizeof(Fecha));
        if (nuevaColumna->datos == NULL) {
            //printf("Error: No se pudo asignar memoria para datos de tipo FECHA\n");
            free(nuevaColumna->esNulo);
            return -1;
        }
    }

    // Copiar los datos de la columna origen a la nueva columna
    //printf("[DEBUG] Copiando valores a la nueva columna '%s'\n", origen->nombre);
    for (int i = 0; i < destino->numFilas; i++) {
        if (i < origen->numFilas && origen->esNulo[i] == 0) {
            nuevaColumna->esNulo[i] = 0;
            if (nuevaColumna->tipo == TEXTO) {
                ((char **)nuevaColumna->datos)[i] = strdup(((char **)origen->datos)[i]);
            } else if (nuevaColumna->tipo == NUMERICO) {
                ((double *)nuevaColumna->datos)[i] = ((double *)origen->datos)[i];
            } else if (nuevaColumna->tipo == FECHA) {
                char val[11];
                Fecha *origenFecha = &((Fecha *)origen->datos)[i];

                // Convertir la fecha a formato string (YYYY-MM-DD)
                sprintf(val, "%04d-%02d-%02d", origenFecha->tm_year, origenFecha->tm_mon + 1, origenFecha->tm_mday);

                if (FechaValidarAMD(val)) {
                    //printf("[DEBUG] FECHA válida encontrada: %s\n", val);
                    Fecha *destinoFecha = &((Fecha *)nuevaColumna->datos)[i];
                    destinoFecha->tm_year = origenFecha->tm_year;
                    destinoFecha->tm_mon = origenFecha->tm_mon;
                    destinoFecha->tm_mday = origenFecha->tm_mday;
                } else {
                    //printf("[DEBUG] FECHA no válida: %s\n", val);
                    nuevaColumna->esNulo[i] = 1;
                    Fecha *destinoFecha = &((Fecha *)nuevaColumna->datos)[i];
                    destinoFecha->tm_year = 0;
                    destinoFecha->tm_mon = 0;
                    destinoFecha->tm_mday = 0;
                }
            }
        } else {
            nuevaColumna->esNulo[i] = 1; // Marcar como nulo si no hay datos
            if (nuevaColumna->tipo == FECHA) {
                Fecha *destinoFecha = &((Fecha *)nuevaColumna->datos)[i];
                destinoFecha->tm_year = 0;
                destinoFecha->tm_mon = 0;
                destinoFecha->tm_mday = 0;

                //printf("[DEBUG] Valor nulo en columna '%s', fila %d (fecha marcada como 0-0-0)\n",
                       //nuevaColumna->nombre, i);
            }
        }
    }

    // Incrementar el contador de columnas en el dataframe destino
    destino->numColumnas++;
    //printf("[DEBUG] Columna '%s' añadida correctamente al dataframe destino\n", nuevaColumna->nombre);

    return 1;
}

//FUNCION AUXILIAR PARA RENOMBRAR EL NOMBRE DE LA COLUMNA AL HACER 
void RenombrarColumnaDuplicada(Dataframe *df, char *nombreColumna) {
    int contador = 1;
    char nuevoNombre[100];
    strcpy(nuevoNombre, nombreColumna);

    // Verificar si el nombre ya existe en el dataframe
    for (int i = 0; i < df->numColumnas; i++) {
        if (strcmp(df->columna[i].nombre, nuevoNombre) == 0) {
            // Generar un nuevo nombre único
            snprintf(nuevoNombre, sizeof(nuevoNombre), "%s_%d", nombreColumna, contador++);
            i = -1; // Reiniciar la búsqueda
        }
    }

    // Actualizar el nombre de la columna
    strcpy(nombreColumna, nuevoNombre);
}


// ELIMINAR VALORES DUPLICADOS EN UNA COLUMNA
int DropDuplicates(Dataframe *df, char *columnaNombre) {
    if (df == NULL) {
        //printf("[Error] No hay DataFrame activo.\n");
        return -1;
    }

    // Buscar la columna especificada
    Columna *columna = NULL;
    for (int i = 0; i < df->numColumnas; i++) {
        if (strcmp(df->columna[i].nombre, columnaNombre) == 0) {
            columna = &df->columna[i];
            break;
        }
    }

    if (columna == NULL) {
        //printf("[Error] La columna '%s' no existe en el DataFrame.\n", columnaNombre);
        return -1;
    }

    // Crear un conjunto para almacenar valores únicos
    void **valoresUnicos = malloc(df->numFilas * sizeof(void *));
    int *nuevoIndice = malloc(df->numFilas * sizeof(int));
    int numUnicos = 0;

    if (!valoresUnicos || !nuevoIndice) {
        //printf("[Error] No se pudo asignar memoria para el procesamiento.\n");
        free(valoresUnicos);
        free(nuevoIndice);
        return -1;
    }

    for (int i = 0; i < df->numFilas; i++) {
        int esDuplicado = 0;

        // Obtener el valor de la fila actual
        void *dato = NULL;
        if (columna->tipo == TEXTO) {
            dato = ((char **)columna->datos)[df->indice[i]];
        } else if (columna->tipo == NUMERICO) {
            dato = &((double *)columna->datos)[df->indice[i]];
        } else if (columna->tipo == FECHA) {
            dato = &((Fecha *)columna->datos)[df->indice[i]];
        }

        // Comparar con valores únicos ya encontrados
        for (int j = 0; j < numUnicos; j++) {
            if (columna->tipo == TEXTO) {
                if (strcmp((char *)dato, (char *)valoresUnicos[j]) == 0) {
                    esDuplicado = 1;
                    break;
                }
            } else if (columna->tipo == NUMERICO) {
                if (*(double *)dato == *(double *)valoresUnicos[j]) {
                    esDuplicado = 1;
                    break;
                }
            } else if (columna->tipo == FECHA) {
                Fecha *fechaDato = (Fecha *)dato;
                Fecha *fechaUnica = (Fecha *)valoresUnicos[j];
                if (fechaDato->tm_year == fechaUnica->tm_year &&
                    fechaDato->tm_mon == fechaUnica->tm_mon &&
                    fechaDato->tm_mday == fechaUnica->tm_mday) {
                    esDuplicado = 1;
                    break;
                }
            }
        }

        // Si no es duplicado, agregarlo a valores únicos
        if (!esDuplicado) {
            valoresUnicos[numUnicos] = dato;
            nuevoIndice[numUnicos] = df->indice[i];
            numUnicos++;
        }
    }

    // Actualizar el índice del DataFrame
    df->indice = realloc(df->indice, numUnicos * sizeof(int));
    if (!df->indice) {
        //printf("[Error] No se pudo reasignar memoria para el índice.\n");
        free(valoresUnicos);
        free(nuevoIndice);
        return -1;
    }

    for (int i = 0; i < numUnicos; i++) {
        df->indice[i] = nuevoIndice[i];
    }

    df->numFilas = numUnicos;

    // Liberar memoria
    free(valoresUnicos);
    free(nuevoIndice);

   // printf("[Info] Filas duplicadas eliminadas. Total de filas restantes: %d\n", df->numFilas);
    return 1;
}



//funcion que te genera el histograma de una función.
int Histogram(Dataframe *df, char *columnaNombre, int numBins) {
    if (df == NULL) {
        printf("[Error] No hay DataFrame activo.\n");
        return -1;
    }

    // Buscar la columna especificada
    Columna *columna = NULL;
    for (int i = 0; i < df->numColumnas; i++) {
        if (strcmp(df->columna[i].nombre, columnaNombre) == 0) {
            columna = &df->columna[i];
            break;
        }
    }

    if (columna == NULL) {
        printf("[Error] La columna '%s' no existe en el DataFrame.\n", columnaNombre);
        return -1;
    }

    // Verificar que la columna sea numérica
    if (columna->tipo != NUMERICO) {
        printf("[Error] La columna '%s' no es numerica.\n", columnaNombre);
        return -1;
    }

    // Encontrar el mínimo y máximo de la columna
    double min = DBL_MAX, max = -DBL_MAX;
    for (int i = 0; i < df->numFilas; i++) {
        if (columna->esNulo[df->indice[i]] == 0) { // Ignorar valores nulos
            double valor = ((double *)columna->datos)[df->indice[i]];
            if (valor < min) min = valor;
            if (valor > max) max = valor;
        }
    }

    if (min == DBL_MAX || max == -DBL_MAX) {
        printf("[Error] Todos los valores de la columna son nulos.\n");
        return -1;
    }

    // Calcular el tamaño de los intervalos (bins)
    double binSize = (max - min) / numBins;

    if (binSize <= 0) {
        printf("[Error] No se puede calcular el histograma con los valores actuales.\n");
        return -1;
    }

    // Inicializar los contadores de frecuencias
    int *frequencies = (int *)calloc(numBins, sizeof(int));
    if (!frequencies) {
        printf("[Error] No se pudo asignar memoria para las frecuencias.\n");
        return -1;
    }

    // Contar frecuencias
    for (int i = 0; i < df->numFilas; i++) {
        if (columna->esNulo[df->indice[i]] == 0) { // Ignorar valores nulos
            double valor = ((double *)columna->datos)[df->indice[i]];
            int binIndex = (int)((valor - min) / binSize);
            if (binIndex == numBins) binIndex--; // Incluir el valor máximo en el último bin
            frequencies[binIndex]++;
        }
    }

    // Mostrar el histograma
    //printf("\n[Histograma: %s]\n", columnaNombre);
    for (int i = 0; i < numBins; i++) {
        double binStart = min + i * binSize;
        double binEnd = binStart + binSize;
        printf("Intervalo %.2f - %.2f: %d\n", binStart, binEnd, frequencies[i]);
    }

    // Liberar memoria
    free(frequencies);

    return 1;
}


//SUMAR COLUMNA ENTRE UNA Y OTRA


// Función para verificar si un año es bisiesto
bool esBisiesto(int anio) {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

// Función para calcular los días totales desde el inicio (año base 0)
int diasDesdeBase(Fecha *fecha) {
    static const int diasMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int diasTotales = 0;

    // Sumar los días completos de los años anteriores
    for (int anio = 0; anio < fecha->tm_year + 1900; anio++) {
        diasTotales += esBisiesto(anio) ? 366 : 365;
    }

    // Sumar los días completos de los meses anteriores del año actual
    for (int mes = 0; mes < fecha->tm_mon; mes++) {
        diasTotales += diasMes[mes];
        if (mes == 1 && esBisiesto(fecha->tm_year + 1900)) {
            diasTotales++; // Agregar un día extra en febrero si es bisiesto
        }
    }

    // Sumar los días actuales del mes
    diasTotales += fecha->tm_mday;

    return diasTotales;
}

// Función principal SumFecha (Cuenta la cantidad de días entre una fecha y otra)
int SumFecha(Dataframe *df, char *col1, char *col2, char *col3) {
    // Validar el DataFrame
    if (df == NULL) {
        printf("[Error] No hay DataFrame activo.\n");
        return -1;
    }

    // Buscar las columnas col1 y col2
    Columna *column1 = NULL, *column2 = NULL;
    for (int i = 0; i < df->numColumnas; i++) {
        if (strcmp(df->columna[i].nombre, col1) == 0) {
            column1 = &df->columna[i];
        }
        if (strcmp(df->columna[i].nombre, col2) == 0) {
            column2 = &df->columna[i];
        }
        if (column1 && column2) break;
    }

    // Validar que ambas columnas existan
    if (column1 == NULL || column2 == NULL) {
        printf("[Error] Una o ambas columnas especificadas no existen.\n");
        return -1;
    }

    // Validar que no exista una columna con el nombre col3
    for (int i = 0; i < df->numColumnas; i++) {
        if (strcmp(df->columna[i].nombre, col3) == 0) {
            printf("[Error] Ya existe una columna con el nombre '%s'.\n", col3);
            return -1;
        }
    }

    // Determinar el tipo de la nueva columna
    TipoDato tipo1 = column1->tipo;
    TipoDato tipo2 = column2->tipo;

    if (!(tipo1 == FECHA && tipo2 == FECHA)) {
        printf("[Error] Ambas columnas deben ser de tipo fecha.\n");
        return -1;
    }

    // Crear la nueva columna
    Columna nuevaColumna;
    strncpy(nuevaColumna.nombre, col3, sizeof(nuevaColumna.nombre) - 1);
    nuevaColumna.nombre[sizeof(nuevaColumna.nombre) - 1] = '\0';
    nuevaColumna.tipo = NUMERICO; // Resultado de diferencias en días
    nuevaColumna.esNulo = malloc(df->numFilas * sizeof(int));
    nuevaColumna.datos = malloc(df->numFilas * sizeof(double));

    if (!nuevaColumna.esNulo || !nuevaColumna.datos) {
        printf("[Error] No se pudo asignar memoria para los datos de la nueva columna.\n");
        free(nuevaColumna.esNulo);
        free(nuevaColumna.datos);
        return -1;
    }

    // Realizar la diferencia fila por fila
    for (int i = 0; i < df->numFilas; i++) {
        if (column1->esNulo[df->indice[i]] || column2->esNulo[df->indice[i]]) {
            // Si alguna fecha es nula, el resultado es nulo
            nuevaColumna.esNulo[i] = 1;
            ((double *)nuevaColumna.datos)[i] = 0.0;
        } else {
            nuevaColumna.esNulo[i] = 0; // No nulo

            // Calcular la diferencia en días entre dos fechas
            Fecha *fecha1 = &((Fecha *)column1->datos)[df->indice[i]];
            Fecha *fecha2 = &((Fecha *)column2->datos)[df->indice[i]];

            int diasFecha1 = diasDesdeBase(fecha1);
            int diasFecha2 = diasDesdeBase(fecha2);

            // Usar el valor absoluto de la diferencia
            ((double *)nuevaColumna.datos)[i] = abs(diasFecha2 - diasFecha1);
        }
    }

    // Agregar la nueva columna al DataFrame
    df->columna = realloc(df->columna, (df->numColumnas + 1) * sizeof(Columna));
    if (!df->columna) {
        printf("[Error] No se pudo asignar memoria para agregar la nueva columna.\n");
        free(nuevaColumna.esNulo);
        free(nuevaColumna.datos);
        return -1;
    }
    df->columna[df->numColumnas] = nuevaColumna;
    df->numColumnas++;

    printf("[Info] Columna '%s' creada exitosamente con el resultado de la diferencia entre fechas.\n", col3);
    return 1;
}

//genera una columna resultante entre dos columnas sumando sus datos
int Sum(Dataframe *df, char *col1, char *col2, char *col3) {
    // Validar el DataFrame
    if (df == NULL) {
        printf("[Error] No hay DataFrame activo.\n");
        return -1;
    }

    // Buscar las columnas col1 y col2
    Columna *column1 = NULL, *column2 = NULL;
    for (int i = 0; i < df->numColumnas; i++) {
        if (strcmp(df->columna[i].nombre, col1) == 0) {
            column1 = &df->columna[i];
        }
        if (strcmp(df->columna[i].nombre, col2) == 0) {
            column2 = &df->columna[i];
        }
        if (column1 && column2) break;
    }

    // Validar que ambas columnas existan
    if (column1 == NULL || column2 == NULL) {
        printf("[Error] Una o ambas columnas especificadas no existen.\n");
        return -1;
    }

    // Validar que no exista una columna con el nombre col3
    for (int i = 0; i < df->numColumnas; i++) {
        if (strcmp(df->columna[i].nombre, col3) == 0) {
            printf("[Error] Ya existe una columna con el nombre '%s'.\n", col3);
            return -1;
        }
    }

    // Determinar el tipo de la nueva columna
    TipoDato tipo1 = column1->tipo;
    TipoDato tipo2 = column2->tipo;

    if (tipo1 != NUMERICO || tipo2 != NUMERICO) {
        printf("[Error] Ambas columnas deben ser de tipo numerico para realizar la suma.\n");
        return -1;
    }

    // Crear la nueva columna
    Columna nuevaColumna;
    strncpy(nuevaColumna.nombre, col3, sizeof(nuevaColumna.nombre) - 1);
    nuevaColumna.nombre[sizeof(nuevaColumna.nombre) - 1] = '\0';
    nuevaColumna.tipo = NUMERICO;
    nuevaColumna.esNulo = malloc(df->numFilas * sizeof(int));
    nuevaColumna.datos = malloc(df->numFilas * sizeof(double));

    if (!nuevaColumna.esNulo || !nuevaColumna.datos) {
        printf("[Error] No se pudo asignar memoria para los datos de la nueva columna.\n");
        free(nuevaColumna.esNulo);
        free(nuevaColumna.datos);
        return -1;
    }

    // Realizar la suma fila por fila
    for (int i = 0; i < df->numFilas; i++) {
        if (column1->esNulo[df->indice[i]] || column2->esNulo[df->indice[i]]) {
            // Si alguna columna es nula, el resultado es nulo
            nuevaColumna.esNulo[i] = 1;
            ((double *)nuevaColumna.datos)[i] = 0.0; // Inicializamos a 0 como precaución
        } else {
            nuevaColumna.esNulo[i] = 0; // No nulo
            // Sumar valores numéricos
            ((double *)nuevaColumna.datos)[i] =
                ((double *)column1->datos)[df->indice[i]] + ((double *)column2->datos)[df->indice[i]];
        }
    }

    // Agregar la nueva columna al DataFrame
    df->columna = realloc(df->columna, (df->numColumnas + 1) * sizeof(Columna));
    if (!df->columna) {
        printf("[Error] No se pudo asignar memoria para agregar la nueva columna.\n");
        free(nuevaColumna.esNulo);
        free(nuevaColumna.datos);
        return -1;
    }
    df->columna[df->numColumnas] = nuevaColumna;
    df->numColumnas++;

    printf("[Info] Columna '%s' creada exitosamente con el resultado de la suma.\n", col3);
    return 1;
}

//genera una columna resultante entre dos columnas restando sus datos
int Sub(Dataframe *df, char *col1, char *col2, char *col3) {
    // Validar el DataFrame
    if (df == NULL) {
        printf("[Error] No hay DataFrame activo.\n");
        return -1;
    }

    // Buscar las columnas col1 y col2
    Columna *column1 = NULL, *column2 = NULL;
    for (int i = 0; i < df->numColumnas; i++) {
        if (strcmp(df->columna[i].nombre, col1) == 0) {
            column1 = &df->columna[i];
        }
        if (strcmp(df->columna[i].nombre, col2) == 0) {
            column2 = &df->columna[i];
        }
        if (column1 && column2) break;
    }

    // Validar que ambas columnas existan
    if (column1 == NULL || column2 == NULL) {
        printf("[Error] Una o ambas columnas especificadas no existen.\n");
        return -1;
    }

    // Validar que no exista una columna con el nombre col3
    for (int i = 0; i < df->numColumnas; i++) {
        if (strcmp(df->columna[i].nombre, col3) == 0) {
            printf("[Error] Ya existe una columna con el nombre '%s'.\n", col3);
            return -1;
        }
    }

    // Determinar el tipo de la nueva columna
    TipoDato tipo1 = column1->tipo;
    TipoDato tipo2 = column2->tipo;

    if (tipo1 != NUMERICO || tipo2 != NUMERICO) {
        printf("[Error] Ambas columnas deben ser de tipo numerico para realizar la resta.\n");
        return -1;
    }

    // Crear la nueva columna
    Columna nuevaColumna;
    strncpy(nuevaColumna.nombre, col3, sizeof(nuevaColumna.nombre) - 1);
    nuevaColumna.nombre[sizeof(nuevaColumna.nombre) - 1] = '\0';
    nuevaColumna.tipo = NUMERICO;
    nuevaColumna.esNulo = malloc(df->numFilas * sizeof(int));
    nuevaColumna.datos = malloc(df->numFilas * sizeof(double));

    if (!nuevaColumna.esNulo || !nuevaColumna.datos) {
        printf("[Error] No se pudo asignar memoria para los datos de la nueva columna.\n");
        free(nuevaColumna.esNulo);
        free(nuevaColumna.datos);
        return -1;
    }

    // Realizar la resta fila por fila
    for (int i = 0; i < df->numFilas; i++) {
        if (column1->esNulo[df->indice[i]] || column2->esNulo[df->indice[i]]) {
            // Si alguna columna es nula, el resultado es nulo
            nuevaColumna.esNulo[i] = 1;
            ((double *)nuevaColumna.datos)[i] = 0.0; // Inicializamos a 0 como precaución
        } else {
            nuevaColumna.esNulo[i] = 0; // No nulo
            // Restar valores numéricos
            ((double *)nuevaColumna.datos)[i] =
                ((double *)column1->datos)[df->indice[i]] - ((double *)column2->datos)[df->indice[i]];
        }
    }

    // Agregar la nueva columna al DataFrame
    df->columna = realloc(df->columna, (df->numColumnas + 1) * sizeof(Columna));
    if (!df->columna) {
        printf("[Error] No se pudo asignar memoria para agregar la nueva columna.\n");
        free(nuevaColumna.esNulo);
        free(nuevaColumna.datos);
        return -1;
    }
    df->columna[df->numColumnas] = nuevaColumna;
    df->numColumnas++;

    printf("[Info] Columna '%s' creada exitosamente con el resultado de la resta.\n", col3);
    return 1;
}


// funcion que extrae el meta para el .txt
int GuardarMeta(Dataframe *df, char *nombreArchivo) {
    if (df == NULL) {
        printf("[Error] No hay DataFrame activo.\n");
        return -1;
    }

    FILE *fichero = fopen(nombreArchivo, "w");
    if (fichero == NULL) {
        printf("[Error] No se pudo abrir o crear el archivo '%s'.\n", nombreArchivo);
        return -1;
    }

    // Escribir metadatos generales
    fprintf(fichero, "Nombre del DataFrame: %s\n", df->nombre);
    fprintf(fichero, "Número de filas: %d\n", df->numFilas);
    fprintf(fichero, "Número de columnas: %d\n", df->numColumnas);
    fprintf(fichero, "=====================================\n");

    // Escribir información de cada columna
    for (int i = 0; i < df->numColumnas; i++) {
        Columna *col = &df->columna[i];
        fprintf(fichero, "Columna %d:\n", i + 1);
        fprintf(fichero, "  Nombre: %s\n", col->nombre);

        // Determinar el tipo de la columna
        if (col->tipo == NUMERICO) {
            fprintf(fichero, "  Tipo: NUMERICO\n");
        } else if (col->tipo == FECHA) {
            fprintf(fichero, "  Tipo: FECHA\n");
        } else if (col->tipo == TEXTO) {
            fprintf(fichero, "  Tipo: TEXTO\n");
        } else {
            fprintf(fichero, "  Tipo: DESCONOCIDO\n");
        }

        // Contar valores nulos
        int nulos = 0;
        for (int j = 0; j < df->numFilas; j++) {
            if (col->esNulo[df->indice[j]] == 1) {
                nulos++;
            }
        }
        fprintf(fichero, "  Valores nulos: %d\n", nulos);
        fprintf(fichero, "-------------------------------------\n");
    }

    fclose(fichero);
    //printf("[Info] Metadatos guardados exitosamente en '%s'.\n", nombreArchivo);
    return 1;
}

// FUNCION PARA CONCATENAR

// Función auxiliar para convertir una fecha a string
char *fechaAString(Fecha *fecha) {
    // Mensaje de depuración al inicio
    //printf("[Depuración] Iniciando conversión de fecha a string...\n");
    /*printf("[Depuración] Fecha original: Año=%d, Mes=%d, Día=%d\n",
           fecha->tm_year, fecha->tm_mon, fecha->tm_mday);*/
    // Validar que la fecha es válida
    if (fecha->tm_year < 0 || fecha->tm_mon < 0 || fecha->tm_mon > 11 || fecha->tm_mday <= 0 || fecha->tm_mday > 31) {
        //printf("[Depuración] Fecha inválida detectada. Devolviendo 'FECHA_INVALIDA'.\n");
        return strdup("FECHA_INVALIDA");
    }

    // Crear un buffer temporal para la fecha
    char *buffer = malloc(20); // Espacio suficiente para "YYYY/MM/DD\0"
    if (!buffer) {
       // printf("[Error] No se pudo asignar memoria para la conversión de la fecha.\n");
        return strdup("ERROR_MEMORIA");
    }

    // Convertir la fecha a string
    int anioReal = fecha->tm_year; // Ajustar el año base
    int mesReal = fecha->tm_mon + 1;      // Ajustar el mes a rango 1-12
    int diaReal = fecha->tm_mday;         // Día está en el rango 1-31

    snprintf(buffer, 20, "%04d/%02d/%02d", anioReal, mesReal, diaReal);

    // Mensaje de depuración sobre el resultado
    //printf("[Depuración] Fecha convertida a string: '%s'\n", buffer);

    return buffer;
}



int Concatenar(Dataframe *df, char *col1, char *col2, char *col3) {
    if (df == NULL) {
       // printf("[Error] No hay DataFrame activo.\n");
        return -1;
    }

    Columna *columna1 = NULL;
    Columna *columna2 = NULL;

    // Buscar las columnas col1 y col2
    for (int i = 0; i < df->numColumnas; i++) {
        if (strcmp(df->columna[i].nombre, col1) == 0) {
            columna1 = &df->columna[i];
        }
        if (strcmp(df->columna[i].nombre, col2) == 0) {
            columna2 = &df->columna[i];
        }
    }

    if (columna1 == NULL) {
        //printf("[Error] La columna '%s' no existe en el dataframe activo.\n", col1);
        return -1;
    }

    if (columna2 == NULL) {
        //printf("[Error] La columna '%s' no existe en el dataframe activo.\n", col2);
        return -1;
    }

    // Verificar que no exista una columna con el nombre col3
    for (int i = 0; i < df->numColumnas; i++) {
        if (strcmp(df->columna[i].nombre, col3) == 0) {
          //  printf("[Error] Ya existe una columna con el nombre '%s'.\n", col3);
            return -1;
        }
    }

    // Crear la nueva columna para almacenar el resultado
    Columna nuevaColumna;
    strcpy(nuevaColumna.nombre, col3);
    nuevaColumna.tipo = TEXTO; // El resultado de la concatenación siempre es texto
    nuevaColumna.esNulo = malloc(df->numFilas * sizeof(unsigned char));
    nuevaColumna.datos = malloc(df->numFilas * sizeof(char *));

    if (!nuevaColumna.datos || !nuevaColumna.esNulo) {
       // printf("[Error] No se pudo asignar memoria para la nueva columna '%s'.\n", col3);
        free(nuevaColumna.datos);
        free(nuevaColumna.esNulo);
        return -1;
    }

    // Inicializar los valores nulos y concatenar
    for (int i = 0; i < df->numFilas; i++) {
        nuevaColumna.esNulo[i] = columna1->esNulo[df->indice[i]] || columna2->esNulo[df->indice[i]];
        if (!nuevaColumna.esNulo[i]) {
            // Concatenar los valores de col1 y col2
            char buffer[256] = "";
         //   printf("[Depuración] Concatenando fila %d...\n", i);

            if (columna1->tipo == TEXTO) {
                strcat(buffer, ((char **)columna1->datos)[df->indice[i]]);
            } else if (columna1->tipo == NUMERICO) {
                char temp[50];
                snprintf(temp, sizeof(temp), "%.2f", ((double *)columna1->datos)[df->indice[i]]);
                strcat(buffer, temp);
            } else if (columna1->tipo == FECHA) {
                Fecha *fecha = &((Fecha *)columna1->datos)[df->indice[i]];
                /*printf("[Depuración] Fecha original en columna '%s', fila %d: Año=%d, Mes=%d, Día=%d\n",
                       col1, i, fecha->tm_year, fecha->tm_mon, fecha->tm_mday);*/

                char *fechaStr = fechaAString(fecha);
                strcat(buffer, fechaStr);
                free(fechaStr);
            }

            if (columna2->tipo == TEXTO) {
                strcat(buffer, ((char **)columna2->datos)[df->indice[i]]);
            } else if (columna2->tipo == NUMERICO) {
                char temp[50];
                snprintf(temp, sizeof(temp), "%.2f", ((double *)columna2->datos)[df->indice[i]]);
                strcat(buffer, temp);
            } else if (columna2->tipo == FECHA) {
                Fecha *fecha = &((Fecha *)columna2->datos)[df->indice[i]];
                /*printf("[Depuración] Fecha original en columna '%s', fila %d: Año=%d, Mes=%d, Día=%d\n",
                       col2, i, fecha->tm_year, fecha->tm_mon, fecha->tm_mday);*/

                char *fechaStr = fechaAString(fecha);
                strcat(buffer, fechaStr);
                free(fechaStr);
            }

            ((char **)nuevaColumna.datos)[df->indice[i]] = strdup(buffer);
        } else {
            ((char **)nuevaColumna.datos)[df->indice[i]] = NULL;
        }
    }

    // Agregar la nueva columna al DataFrame
    if (CrearYAgregarColumna(df, &nuevaColumna) == -1) {
        //printf("[Error] No se pudo agregar la nueva columna '%s'.\n", col3);
        free(nuevaColumna.datos);
        free(nuevaColumna.esNulo);
        return -1;
    }

   // printf("[Info] La columna '%s' ha sido creada concatenando '%s' y '%s'.\n", col3, col1, col2);
    return 1;
}

//UNIQUE
void *ObtenerValoresUnicosExclusivos(Columna *columna, int *numUnicos) {
    void *valoresUnicos = NULL;
    *numUnicos = 0;

    if (columna->tipo == TEXTO) {
        char **unicos = malloc(columna->numFilas * sizeof(char *));
        int *conteo = calloc(columna->numFilas, sizeof(int));
        for (int i = 0; i < columna->numFilas; i++) {
            if (!columna->esNulo[i]) {
                char *valor = ((char **)columna->datos)[i];
                int encontrado = -1;
                for (int j = 0; j < *numUnicos; j++) {
                    if (strcmp(unicos[j], valor) == 0) {
                        encontrado = j;
                        break;
                    }
                }
                if (encontrado == -1) {
                    unicos[*numUnicos] = strdup(valor);
                    conteo[*numUnicos] = 1;
                    (*numUnicos)++;
                } else {
                    conteo[encontrado]++;
                }
            }
        }
        // Filtrar solo los valores con conteo 1
        int numFiltrados = 0;
        for (int i = 0; i < *numUnicos; i++) {
            if (conteo[i] == 1) {
                unicos[numFiltrados++] = unicos[i];
            } else {
                free(unicos[i]);
            }
        }
        *numUnicos = numFiltrados;
        valoresUnicos = unicos;
        free(conteo);
    } else if (columna->tipo == NUMERICO) {
        double *unicos = malloc(columna->numFilas * sizeof(double));
        int *conteo = calloc(columna->numFilas, sizeof(int));
        for (int i = 0; i < columna->numFilas; i++) {
            if (!columna->esNulo[i]) {
                double valor = ((double *)columna->datos)[i];
                int encontrado = -1;
                for (int j = 0; j < *numUnicos; j++) {
                    if (unicos[j] == valor) {
                        encontrado = j;
                        break;
                    }
                }
                if (encontrado == -1) {
                    unicos[*numUnicos] = valor;
                    conteo[*numUnicos] = 1;
                    (*numUnicos)++;
                } else {
                    conteo[encontrado]++;
                }
            }
        }
        // Filtrar solo los valores con conteo 1
        int numFiltrados = 0;
        for (int i = 0; i < *numUnicos; i++) {
            if (conteo[i] == 1) {
                unicos[numFiltrados++] = unicos[i];
            }
        }
        *numUnicos = numFiltrados;
        valoresUnicos = unicos;
        free(conteo);
    } else if (columna->tipo == FECHA) {
        Fecha *unicos = malloc(columna->numFilas * sizeof(Fecha));
        int *conteo = calloc(columna->numFilas, sizeof(int));
        for (int i = 0; i < columna->numFilas; i++) {
            if (!columna->esNulo[i]) {
                Fecha valor = ((Fecha *)columna->datos)[i];
                int encontrado = -1;
                for (int j = 0; j < *numUnicos; j++) {
                    if (memcmp(&unicos[j], &valor, sizeof(Fecha)) == 0) {
                        encontrado = j;
                        break;
                    }
                }
                if (encontrado == -1) {
                    unicos[*numUnicos] = valor;
                    conteo[*numUnicos] = 1;
                    (*numUnicos)++;
                } else {
                    conteo[encontrado]++;
                }
            }
        }
        // Filtrar solo los valores con conteo 1
        int numFiltrados = 0;
        for (int i = 0; i < *numUnicos; i++) {
            if (conteo[i] == 1) {
                unicos[numFiltrados++] = unicos[i];
            }
        }
        *numUnicos = numFiltrados;
        valoresUnicos = unicos;
        free(conteo);
    }
    return valoresUnicos;
}

// Función para imprimir los valores únicos que aparecen solo una vez
void ImprimirValoresUnicosExclusivos(Columna *columna, void *valoresUnicos, int numUnicos) {
    //printf("Valores Únicos (una sola aparición) en la columna '%s':\n", columna->nombre);

    if (columna->tipo == TEXTO) {
        char **unicos = (char **)valoresUnicos;
        for (int i = 0; i < numUnicos; i++) {
            printf("- %s\n", unicos[i]);
            free(unicos[i]); // Liberar memoria
        }
        free(unicos);
    } else if (columna->tipo == NUMERICO) {
        double *unicos = (double *)valoresUnicos;
        for (int i = 0; i < numUnicos; i++) {
            printf("- %.2f\n", unicos[i]);
        }
        free(unicos);
    } else if (columna->tipo == FECHA) {
        Fecha *unicos = (Fecha *)valoresUnicos;
        for (int i = 0; i < numUnicos; i++) {
            printf("- %04d/%02d/%02d\n", unicos[i].tm_year, unicos[i].tm_mon + 1, unicos[i].tm_mday);
        }
        free(unicos);
    }
}

int *BuscarEnColumna(Columna *columna, void *valor, int *numResultados) {
    int *filasEncontradas = malloc(columna->numFilas * sizeof(int));
    *numResultados = 0;

    if (columna->tipo == TEXTO) {
        for (int i = 0; i < columna->numFilas; i++) {
            if (!columna->esNulo[i] && strcmp(((char **)columna->datos)[i], (char *)valor) == 0) {
                filasEncontradas[(*numResultados)++] = i;
            }
        }
    } else if (columna->tipo == NUMERICO) {
        for (int i = 0; i < columna->numFilas; i++) {
            if (!columna->esNulo[i] && ((double *)columna->datos)[i] == *(double *)valor) {
                filasEncontradas[(*numResultados)++] = i;
            }
        }
    } else if (columna->tipo == FECHA) {
        for (int i = 0; i < columna->numFilas; i++) {
            if (!columna->esNulo[i] &&
                memcmp(&((Fecha *)columna->datos)[i], (Fecha *)valor, sizeof(Fecha)) == 0) {
                filasEncontradas[(*numResultados)++] = i;
            }
        }
    }

    return filasEncontradas;
}