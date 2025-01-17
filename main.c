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

//gcc lib.c main.c -o main.exe


int main(){

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    //Variables 
    char prompt[100] = "?";
    char com[100];
    char aux[100];
    char nombre[51];
    char *numeroDf;
    char auxiliarnombre[51];

    char *comtok[100];
    char separadores[10] = " \n\t\r";

    int numarg, n, asc, i, numDF=0;

    Lista *laGranLista = NULL;
    Nodo *nodoActivo=NULL, *nodoAux=NULL;
    Dataframe *dfActivo = NULL;
    Columna *col1=NULL, *col2=NULL, *colRes=NULL;
    
    laGranLista = malloc(sizeof(Lista));
    laGranLista->numDFs=0;
    laGranLista->primero=NULL;



    //Logica

    EjerciciosExamen();

    Datos_alumno();

    while(1){

        /*if(dfActivo!=NULL){
            printf("\n%s\n", dfActivo->nombre);
        }*/
        Trim(prompt);
        fflush(stdout);
        printf("[%s]:>", prompt);

        gets(com);

        // Verificar si hay espacios al principio de la entrada
        if (isspace((unsigned char)com[0])) {
            SetConsoleTextAttribute(hConsole, 4);
            printf("Error: el comando no debe tener espacios al principio\n");
            SetConsoleTextAttribute(hConsole, 7);
            continue; // Saltar esta iteración y esperar nueva entrada
        }

        numarg  = 0;

        comtok[numarg] = strtok(com, separadores);

        numarg++;

        while((comtok[numarg] = strtok(NULL, separadores)) != NULL){
            numarg++;
        }

        if(comtok[0] == NULL){
            continue;
        }

        //tokenice
        // for(int i = 0; i < numarg; i++){
        //     SetConsoleTextAttribute(hConsole, 2);
        //     printf("%s\n", comtok[i]);
        //     SetConsoleTextAttribute(hConsole, 7);
        // }


        if(strcmp(comtok[0], "quit") == 0){
            if (numarg > 1) {
                    SetConsoleTextAttribute(hConsole, 4);
                    //printf("[Error] El comando 'quit' no permite argumentos adicionales.\n");
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }

                // Debug: Notify the start of the quit process
                SetConsoleTextAttribute(hConsole, 6);
                //printf("[Debug] Iniciando proceso de quit...\n");
                SetConsoleTextAttribute(hConsole, 7);

                // Free all DataFrames in laGranLista
                if (laGranLista) {
                    Nodo *current = laGranLista->primero;
                    int count = 0;

                    while (current != NULL) {
                        Nodo *next = current->siguiente; // Save the next node

                        if (current->df) {
                            //printf("[Debug] Liberando DataFrame: '%s'\n", current->df->nombre);
                            LiberarDataframe(current->df);
                            // Si el DataFrame liberado es el mismo que dfActivo, ajustamos dfActivo
                            if (current->df == dfActivo) {
                                dfActivo = NULL; // Evitar liberar dfActivo nuevamente
                            }
                        } else {
                           // printf("[Debug] Nodo sin DataFrame asociado.\n");
                        }

                        free(current); // Free the current node
                        current = next; // Move to the next node
                        count++;
                    }

                    //printf("[Debug] Se liberaron %d nodos de la lista.\n", count);

                    // Reset the list metadata
                    laGranLista->primero = NULL;
                    laGranLista->numDFs = 0;
                } else {
                   // printf("[Debug] La lista de DataFrames esta vacia.\n");
                }

                // Reset the active DataFrame (if not already freed)
                if (dfActivo) {
                    //printf("[Debug] Liberando DataFrame activo: '%s'\n", dfActivo->nombre);
                    LiberarDataframe(dfActivo);
                    dfActivo = NULL;
                } else {
                    //printf("[Debug] No hay DataFrame activo o ya fue liberado.\n");
                }

                // Confirm quit
                SetConsoleTextAttribute(hConsole, 2);
                printf("EXIT PROGRAM\n");
                SetConsoleTextAttribute(hConsole, 7);

                // Exit program
                return 0;
        }
        else if(strcmp(comtok[0], "load") == 0){

            if (numarg<2) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: load necesita un argumento\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if(numarg>2){
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: load solo permite un argumento\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            nodoAux = Load(comtok[1]);

            if (nodoAux ==NULL)
                continue;
            else{
                anyadirNodo(nodoAux,laGranLista);
                numDF=laGranLista->numDFs-1;
                sprintf(aux, "df%d", numDF);
                strcpy(nombre, aux);
                sprintf(aux, "df%d: %d,%d", numDF, nodoAux->df->numFilas, nodoAux->df->numColumnas);
                strcpy(prompt, aux);
               
                dfActivo = nodoAux->df;
                nodoActivo = nodoAux;
                strncpy(nodoActivo->df->nombre, nombre, sizeof(nodoActivo->df->nombre) - 1);
                nodoActivo->df->nombre[sizeof(nodoActivo->df->nombre) - 1] = '\0'; // terminación en nulo
                nodoAux = NULL;
            } 
            // cargar el CSV
            // printf("Entrando en CSV\n");


        }
        else if(strcmp(comtok[0], "meta") == 0){

            //TO DO
            //printf("Entrando en meta\n");
            SetConsoleTextAttribute(hConsole, 2);
            
            if (numarg>1) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: meta no permite argumentos adicionales\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (dfActivo == NULL){
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: no hay dataframe activo\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }
            for (int i = 0; i < dfActivo->numColumnas; i++) {
                printf("Columna %d: %s\t", i, dfActivo->columna[i].nombre);
                if (dfActivo->columna[i].tipo == 0) {
                    printf("Tipo: TEXTO\t");
                } else if (dfActivo->columna[i].tipo == 1) {
                    printf("Tipo: NUMERICO\t");
                } else if (dfActivo->columna[i].tipo == 2) {
                    printf("Tipo: FECHA\t");
                }
                printf("Filas Nulas: %d\n", numNulos(dfActivo->columna[i]));
            }

            SetConsoleTextAttribute(hConsole, 7);

        }
        else if(strcmp(comtok[0], "view") == 0) {
            n = 10; // Default value for n  
            if (numarg > 2) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: view no acepta mas de 2 argumentos\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }
            if (numarg == 2) {    
                if (Validar_PosNeg(comtok[1]) == 0) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: view necesita un argumento numerico\n");
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;                
                }
                n = atoi(comtok[1]);
                if (n == 0) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: el valor de n no puede ser 0\n");
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }
            }

            if (dfActivo == NULL) { // Check for active dataframe
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay dataframe activo\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (n > 0) { // Positive n: Display first n rows
                if (n > dfActivo->numFilas) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: valor de n (%d) es mayor al numero de filas disponibles (%d) en este dataframe\n", n, dfActivo->numFilas);
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }

                SetConsoleTextAttribute(hConsole, 2);
                for (int i = 0; i < dfActivo->numColumnas; i++) {
                    printf("%-20s", dfActivo->columna[i].nombre);
                }
                printf("\n");
                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < dfActivo->numColumnas; j++) {
                        printf("%-20s", printElemento(dfActivo, j, i));
                    }
                    printf("\n");
                }
                SetConsoleTextAttribute(hConsole, 7);

            } else { // Negative n: Display last -n rows in reverse order
                n = -n; // Convert n to positive for easier handling
                if (n > dfActivo->numFilas) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: valor de n (%d) es mayor al numero de filas disponibles (%d) en este dataframe\n", n, dfActivo->numFilas);
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }

                SetConsoleTextAttribute(hConsole, 2);
                for (int i = 0; i < dfActivo->numColumnas; i++) {
                    printf("%-20s", dfActivo->columna[i].nombre);
                }
                printf("\n");
                for (int i = dfActivo->numFilas - 1; i >= dfActivo->numFilas - n; i--) {
                    for (int j = 0; j < dfActivo->numColumnas; j++) {
                        printf("%-20s", printElemento(dfActivo, j, i));
                    }
                    printf("\n");
                }
                SetConsoleTextAttribute(hConsole, 7);
            }
        }

        else if(strcmp(comtok[0], "sort") == 0){

            asc=1;

            if (numarg>3) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: sort no acepta mas de dos argumentos\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }
            if (numarg==3){    
                if (strcmp(comtok[2], "asc")==0){
                    asc=1;
                }
                else if(strcmp(comtok[2], "des")==0){
                    asc=0;
                }
                else{
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: el segundo argumento tiene que ser asc o des\n");
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;                
                }
            }
            //printf("es ascendente o descendente %d\n", asc);
            if(numarg == 1){
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: sort debe tener minimo un argumento\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }
            if(dfActivo == NULL){
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay dataframe activo\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }
            col1 = BuscarColumna(dfActivo, comtok[1]);
            if (BuscarColumna(dfActivo, comtok[1]) == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: Columna no encontrada\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }
            /*for (int i = 0; i < dfActivo->numFilas; i++) {
                printf("%d\n", dfActivo->indice[i]);
            }*/
            SortCol(dfActivo, col1, asc);          

        }
        else if(strcmp(comtok[0], "save") == 0) {
            if (numarg != 2 && numarg != 1) { // Permitir 1 o 2 argumentos
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: save necesita uno o ningún argumento\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (dfActivo == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay dataframe activo\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Determinar si se proporciona un nombre o usar el nombre del dataframe
            char *nombreArchivo = (numarg == 2) ? comtok[1] : NULL;

            if (Save(dfActivo, nombreArchivo) == -1) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No se ha podido abrir el archivo\n");
                SetConsoleTextAttribute(hConsole, 7);
            }
        }

       else if (strcmp(comtok[0], "filter") == 0) {
            // Validar el número de argumentos
            if (numarg < 4) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: filter necesita al menos 3 argumentos\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            Trim(comtok[2]);
            fflush(stdout);

            // Validar si hay un dataframe activo
            if (dfActivo == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay dataframe activo\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Validar si la columna existe
            Columna *col = BuscarColumna(dfActivo, comtok[1]);
            if (col == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: Columna '%s' no encontrada\n", comtok[1]);
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Validar el operador
            if (strcmp(comtok[2], "eq") != 0 &&
                strcmp(comtok[2], "neq") != 0 &&
                strcmp(comtok[2], "gt") != 0 &&
                strcmp(comtok[2], "lt") != 0 &&
                strcmp(comtok[2], "get") != 0 &&
                strcmp(comtok[2], "let") != 0) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: Operador de comparación no válido: '%s'\n", comtok[2]);
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Llamar a la función Filtrar
            if (Filtrar(nodoActivo, dfActivo, col, comtok[2], comtok[3]) == -1) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No se ha podido filtrar. El comando no existe.\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            } else {
                // Actualizar el dataframe activo y el prompt con el nuevo nombre y dimensiones
                dfActivo = nodoActivo->df;
                snprintf(aux, sizeof(aux), "%s: %d,%d", dfActivo->nombre, dfActivo->numFilas, dfActivo->numColumnas);
                strcpy(prompt, aux);

                SetConsoleTextAttribute(hConsole, 2);
                //printf("Filtro aplicado correctamente. Nuevo DataFrame activo: '%s'\n", dfActivo->nombre);
                SetConsoleTextAttribute(hConsole, 7);
            }



        }else if(strcmp(comtok[0], "delnull") == 0) {

            if (numarg != 2) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: delnull necesita exactamente un argumento (nombre de la columna).\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (dfActivo == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay DataFrame activo.\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            Columna *columna = BuscarColumna(dfActivo, comtok[1]);
            if (columna == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: Columna '%s' no encontrada.\n", comtok[1]);
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Comprobar si la columna tiene valores nulos
            int tieneNulos = 0;
            for (int i = 0; i < dfActivo->numFilas; i++) {
                if (columna->esNulo[dfActivo->indice[i]]) {
                    tieneNulos = 1;
                    break;
                }
            }

            if (!tieneNulos) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: La columna '%s' no tiene valores nulos.\n", comtok[1]);
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Eliminar valores nulos
            if (EliminarNulos(nodoActivo, dfActivo, columna) == -1) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No se ha podido eliminar los valores nulos de la columna '%s'.\n", comtok[1]);
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            } else {
                SetConsoleTextAttribute(hConsole, 2);
                printf("Se han eliminado los valores nulos de la columna '%s'.\n", comtok[1]);
                SetConsoleTextAttribute(hConsole, 7);

                dfActivo = nodoActivo->df;
                sprintf(aux, "%s: %d,%d", dfActivo->nombre, dfActivo->numFilas, dfActivo->numColumnas);
                strcpy(prompt, aux);
            }
        }

        else if(strcmp(comtok[0], "delcolum") == 0){

            if (numarg!=2) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: delcolum necesita un argumento\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }
            if(dfActivo == NULL){
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay dataframe activo\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }
            if (BuscarColumna(dfActivo, comtok[1]) == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: Columna no encontrada\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            for(i=0; i<dfActivo->numColumnas; i++){
                if(strcmp(dfActivo->columna[i].nombre, comtok[1]) == 0){
                    break;
                }
            }
            
            if (EliminarColumna(dfActivo, i) == -1) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No se ha podido eliminar la columna\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }
            else {
                sprintf(aux, "%s: %d,%d", dfActivo->nombre, dfActivo->numFilas, dfActivo->numColumnas);
                strcpy(prompt, aux); // Actualizar el prompt global
            }

        }
        else if(strcmp(comtok[0], "quarter") == 0){

            if (numarg!=3) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: quarter necesita un argumento\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }
            if(dfActivo == NULL){
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay dataframe activo\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }
            if (BuscarColumna(dfActivo, comtok[1]) == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: Columna no encontrada\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }
            if (BuscarColumna(dfActivo, comtok[1])->tipo != FECHA) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: La columna no es tipo fecha\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }
            if (BuscarColumna(dfActivo, comtok[2]) != NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: Columna ya existente\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            Quarter(dfActivo, BuscarColumna(dfActivo, comtok[1]), comtok[2]);

            sprintf(aux, "%s: %d,%d", dfActivo->nombre, dfActivo->numFilas, dfActivo->numColumnas);
            strcpy(prompt, aux); // Actualizar el prompt global

        }else if (strcmp(comtok[0], "name") == 0) { // Command to change the name of the active dataframe
            if (numarg != 2) { // Validate the correct number of arguments
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: nombre necesita un argumento\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (dfActivo == NULL) { // Check if there's an active dataframe
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay dataframe activo\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            char *newName = comtok[1];

            if (strlen(newName) > 50) { // Validate the length of the new name
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: El nombre no puede exceder de 50 caracteres\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Check for unique name in the list
            Nodo *current = laGranLista->primero;
            int isUnique = 1;
            while (current != NULL) {
                if (strcmp(current->df->nombre, newName) == 0) {
                    isUnique = 0;
                    break;
                }
                current = current->siguiente;
            }

            if (!isUnique) { // If the name is not unique
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: Ya existe un dataframe con el nombre '%s'\n", newName);
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Change the name of the active dataframe
            strncpy(dfActivo->nombre, newName, sizeof(dfActivo->nombre) - 1);
            dfActivo->nombre[sizeof(dfActivo->nombre) - 1] = '\0'; // Ensure null termination

            // Update the prompt
            sprintf(prompt, "%s: %d,%d", dfActivo->nombre, dfActivo->numFilas, dfActivo->numColumnas);

            // Confirm success
            SetConsoleTextAttribute(hConsole, 2);
            //printf("Nombre cambiado a '%s'\n", dfActivo->nombre);
            SetConsoleTextAttribute(hConsole, 7);
        }else if (strcmp(comtok[0], "list") == 0) {
            if (numarg != 1) { // Ensure no arguments are provided
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: list no acepta argumentos\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (laGranLista->numDFs == 0) { // Check if no dataframes are loaded
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay dataframes cargados en memoria\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            /*SetConsoleTextAttribute(hConsole, 2); // Set text color to green
            printf("Listado de dataframes cargados en memoria:\n");
            */
            Nodo *current = laGranLista->primero;
            while (current != NULL) {
                printf("%s: %d filas, %d columnas\n",
                    current->df->nombre, current->df->numFilas, current->df->numColumnas);
                current = current->siguiente;
            }

            SetConsoleTextAttribute(hConsole, 7); // Reset text color
        }else if (strcmp(comtok[0], "prefix") == 0) {
            if (numarg != 4) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: prefix requiere exactamente 3 argumentos\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (dfActivo == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay dataframe activo\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            char *columna = comtok[1];
            char *nStr = comtok[2];
            char *nuevaColumna = comtok[3];

            if (Validar_entero(nStr) == 0 || atoi(nStr) <= 0) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: 'n' debe ser un numero entero mayor que 0\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }
            int n = atoi(nStr);

            Columna *col1 = BuscarColumna(dfActivo, columna);
            if (col1 == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: La columna '%s' no existe\n", columna);
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (col1->tipo != TEXTO) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: La columna '%s' no es de tipo texto\n", columna);
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (BuscarColumna(dfActivo, nuevaColumna) != NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: La columna '%s' ya existe\n", nuevaColumna);
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            Columna *colRes = CrearColumnaTexto(dfActivo->numFilas, nuevaColumna);
            if (colRes == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No se pudo crear la nueva columna\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            char **textData = (char **)col1->datos;
            char **newTextData = (char **)colRes->datos;

            for (int i = 0; i < dfActivo->numFilas; i++) {
                char *valor = textData[i];
                
                // Depuración: Mostrar el valor original
                //printf("DEBUG: Valor original en la fila %d: %s\n", i, valor ? valor : "NULL");

                if (valor == NULL) {
                    newTextData[i] = NULL;
                    //printf("DEBUG: Valor NULL en la fila %d. Prefijo no generado.\n", i);
                    continue;
                }

                char prefijo[51];
                memset(prefijo, 0, sizeof(prefijo));

                // Extraer el prefijo
                strncpy(prefijo, valor, n);
                prefijo[n] = '\0';

                // Depuración: Mostrar el prefijo generado
                //printf("DEBUG: Prefijo generado en la fila %d: %s\n", i, prefijo);

                // Asignar el prefijo a la nueva columna
                newTextData[i] = strdup(prefijo);
                if (newTextData[i] == NULL) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: No se pudo asignar memoria para la fila %d en la nueva columna\n", i);
                    SetConsoleTextAttribute(hConsole, 7);
                    break;
                }

                // Depuración: Confirmar que el prefijo se ha asignado correctamente
                //printf("DEBUG: Prefijo asignado a la nueva columna en la fila %d: %s\n", i, newTextData[i]);
            }

            if (AnyadirColumna(dfActivo, colRes) == -1) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No se pudo agregar la nueva columna al dataframe\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }
            
            // Actualizar el prompt con el nombre, filas y columnas del dataframe activo
            sprintf(aux, "%s: %d,%d", dfActivo->nombre, dfActivo->numFilas, dfActivo->numColumnas);
            strcpy(prompt, aux);

            SetConsoleTextAttribute(hConsole, 2);
            //printf("Columna '%s' creada exitosamente con prefijos de '%s'\n", nuevaColumna, columna);
            SetConsoleTextAttribute(hConsole, 7);
        }

        // A PARTIR DE AQUÍ ENCONTRAMOS FUNCIONES EXTRAS //



                    // GROUPBY
        


        /*else if (strcmp(comtok[0], "groupby") == 0) {
            if (numarg != 2) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: groupby necesita el nombre de una columna\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }
            if (GroupBy(dfActivo, comtok[1]) == -1) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error al agrupar el dataframe\n");
                SetConsoleTextAttribute(hConsole, 7);
            }
        }*/
        


                        //  DELETE borra un dataframe



        /*else if (strcmp(comtok[0], "delete") == 0) {
        
            if (numarg != 2) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: delete necesita un argumento (nombre del dataframe).\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            const char *nombreDF = comtok[1];
            if (EliminarDataframe(laGranLista, nombreDF) == 0) {


                // Verificar si el dataframe activo era el eliminado
                extraerTexto(prompt, auxiliarnombre, sizeof(auxiliarnombre)); 
                if(strcmp(auxiliarnombre, nombreDF) == 0) {
                    dfActivo = NULL;
                    nodoActivo = NULL;
                    strcpy(prompt, "?");
                }
            }else{
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: no se ha podido borrar el dataframe.\n");
                SetConsoleTextAttribute(hConsole, 7);
            }
        }*/
        


                    //COPY copia un dataframe.


        
        /*else if (strcmp(comtok[0], "copy") == 0) {
                if (numarg != 2) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: copy necesita el nombre del dataframe como argumento.\n");
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }

                Nodo *nodoOriginal = BuscarNodoPorNombre(laGranLista, comtok[1]);
                if (!nodoOriginal) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: No se encontro el dataframe con el nombre '%s'.\n", comtok[1]);
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }

                Dataframe *nuevoDF = DuplicarDataframe(nodoOriginal->df);
                if (!nuevoDF) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: No se pudo duplicar el dataframe '%s'.\n", comtok[1]);
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }

                Nodo *nuevoNodo = malloc(sizeof(Nodo));
                if (!nuevoNodo) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: No se pudo asignar memoria para el nuevo nodo.\n");
                    SetConsoleTextAttribute(hConsole, 7);
                    LiberarDataframe(nuevoDF);
                    continue;
                }
                nuevoNodo->df = nuevoDF;
                nuevoNodo->siguiente = NULL;
                anyadirNodo(nuevoNodo, laGranLista);

                // Hacer el dataframe duplicado activo
                dfActivo = nuevoDF;
                nodoActivo = nuevoNodo;

                // Actualizar el prompt
                snprintf(prompt, sizeof(prompt), "%s: %d,%d", nuevoDF->nombre, nuevoDF->numFilas, nuevoDF->numColumnas);
                //printf("Dataframe '%s' duplicado como '%s'.\n", comtok[1], nuevoDF->nombre);
            }*/
            



                        // ADDCOLUMN, añade una columna



            /*else if (strcmp(comtok[0], "addcolumn") == 0) {
                if (numarg != 3) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: Uso incorrecto del comando. Uso: addcolumn <nombre> <tipo>\n");
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }

                const char *nombreColumna = comtok[1];
                TipoDato tipoColumna;

                // Determinar el tipo de la columna
                if (strcmp(comtok[2], "TEXTO") == 0) {
                    tipoColumna = TEXTO;
                } else if (strcmp(comtok[2], "NUMERICO") == 0) {
                    tipoColumna = NUMERICO;
                } else if (strcmp(comtok[2], "FECHA") == 0) {
                    tipoColumna = FECHA;
                } else {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: Tipo de columna invalido. Debe ser TEXTO, NUMERICO o FECHA.\n");
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }

                // Agregar la columna
                if (!dfActivo) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: No hay dataframe activo.\n");
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }

                if (AgregarColumnaNueva(dfActivo, nombreColumna, tipoColumna) == 0) {
                    //printf("Columna '%s' añadida correctamente al dataframe '%s'.\n", nombreColumna, dfActivo->nombre);
                    sprintf(aux, "%s: %d,%d", dfActivo->nombre, dfActivo->numFilas, dfActivo->numColumnas);
                    strcpy(prompt, aux);
                } else {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: No se pudo añadir la columna '%s'.\n", nombreColumna);
                    SetConsoleTextAttribute(hConsole, 7);
                }
                
                
            }*/ 
            


                            //DELETEALL borra todos los dataframes
            

            
            //else if (strcmp(comtok[0], "deleteall") == 0) {

                //version con confirmación:
                // Confirmar si el usuario desea eliminar todos los dataframes
                /*char confirmacion[10];
                printf("¿Estás seguro de que deseas eliminar todos los dataframes? (sí/no): ");
                fgets(confirmacion, sizeof(confirmacion), stdin);
                confirmacion[strcspn(confirmacion, "\n")] = '\0'; // Eliminar el salto de línea

                if (strcmp(confirmacion, "Si") == 0 || strcmp(confirmacion, "si") == 0) {
                    // Llamar a la función para eliminar todos los dataframes
                    EliminarTodosLosDataframes(laGranLista);

                    // Actualizar el dataframe activo y el prompt
                    dfActivo = NULL;
                    nodoActivo = NULL;
                    strcpy(prompt, "?");
                } else {
                    printf("Operación cancelada.\n");
                }*/

               //version sin confirmacion, si es con confirmacion comentar, probar no me acuerdo 

                /*EliminarTodosLosDataframes(laGranLista); //Version sin confirmación:

                // Actualizar el dataframe activo y el prompt
                dfActivo = NULL;
                nodoActivo = NULL;
                strcpy(prompt, "?");

            }*/
            


                           // REPLACE reemplaza un valor concreto por otro



            /*else if (strcmp(comtok[0], "replace") == 0) {
                if (numarg != 4) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: Uso correcto: replace <columna> <dato> <datoReemplazar>\n");
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }

                const char *nombreColumna = comtok[1];
                const char *dato = comtok[2];
                const char *datoReemplazar = comtok[3];

                if (ReplaceDatoEnColumna(dfActivo, nombreColumna, dato, datoReemplazar) == -1) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: No se pudo completar el reemplazo.\n");
                    SetConsoleTextAttribute(hConsole, 7);
                }
            }*/
            



                            // RENAMECOLUMN renombra una columna



            /*else if (strcmp(comtok[0], "renamecolumn") == 0) {
                if (numarg != 3) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: Uso correcto: renamecolumn <nombreActual> <nuevoNombre>\n");
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }

                const char *nombreActual = comtok[1];
                const char *nuevoNombre = comtok[2];

                if (CambiarNombreColumna(dfActivo, nombreActual, nuevoNombre) == -1) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: No se pudo cambiar el nombre de la columna.\n");
                    SetConsoleTextAttribute(hConsole, 7);
                }
            }*/
            


                // FUNCIONES SUM AVG MAX MIN MEDIAN


        /*    
            else if (strcmp(comtok[0], "SUM") == 0 || strcmp(comtok[0], "AVG") == 0 || strcmp(comtok[0], "MAX") == 0 || strcmp(comtok[0], "MIN") == 0 ||  strcmp(comtok[0], "MEDIAN") == 0) {
                if (numarg != 2) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: Uso correcto: <%s> <nombre_columna>\n", comtok[0]);
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }

                const char *operacion = comtok[0];
                const char *colname = comtok[1];

                double resultado = OperacionColumnaNumerica(dfActivo, colname, operacion);
                if (!isnan(resultado)) {
                    printf("%s: %.2f\n", operacion, resultado);
                    
                }else{
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: La columna NO existe, o NO es numerica\n", comtok[0]);
                    SetConsoleTextAttribute(hConsole, 7);
                }
        }*/
        


                        // FUNCION FILTER CON ENTRE     filter <columna> entre <val1> <val2>


        /*else if(strcmp(comtok[0], "filter2") == 0) {
            // Validar el número de argumentos
            if (numarg < 4) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: filter necesita al menos 3 argumentos\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if ((strcmp(comtok[2], "entre") == 0 && numarg != 5) || 
                (strcmp(comtok[2], "entre") != 0 && numarg != 4)) {
                SetConsoleTextAttribute(hConsole, 4);
                if (strcmp(comtok[2], "entre") == 0) {
                    printf("Error: filter con el operador 'entre' necesita exactamente 5 argumentos\n");
                } else {
                    printf("Error: filter necesita 3 argumentos para operadores diferentes de 'entre'\n");
                }
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            Trim(comtok[2]);
            fflush(stdout);

            // Validar si hay un dataframe activo
            if (dfActivo == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay dataframe activo\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Validar si la columna existe
            Columna *columna = BuscarColumna(dfActivo, comtok[1]);
            if (columna == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: Columna no encontrada\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Validar el tipo para "entre"
            if (strcmp(comtok[2], "entre") == 0) {
                if (columna->tipo != Tipo(comtok[3]) || columna->tipo != Tipo(comtok[4])) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: Los tipos de los valores no corresponden con el tipo de la columna\n");
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }
            }

            // Validar el operador
            if (strcmp(comtok[2], "eq") != 0 &&
                strcmp(comtok[2], "neq") != 0 &&
                strcmp(comtok[2], "gt") != 0 &&
                strcmp(comtok[2], "lt") != 0 &&
                strcmp(comtok[2], "entre") != 0) { // Validar el operador "entre"
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: Operador de comparacion no valido: '%s'\n", comtok[2]);
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Llamar a la función Filtrar
            if (strcmp(comtok[2], "entre") == 0) {
                if (Filtrar2(nodoActivo, dfActivo, columna, comtok[2], comtok[3], comtok[4]) == -1) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: No se ha podido aplicar el filtro 'entre'\n");
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }
            } else {
                if (Filtrar2(nodoActivo, dfActivo, columna, comtok[2], comtok[3], NULL) == -1) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: No se ha podido filtrar. El comando no existe.\n");
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }
            }

            // Actualizar el dataframe activo y el prompt
            dfActivo = nodoActivo->df;
            sprintf(aux, "%s: %d,%d", nodoActivo->df->nombre, dfActivo->numFilas, dfActivo->numColumnas);
            strcpy(prompt, aux);
        }*/




                        //      FUNCION SAVE2 CON CONFIRMACION




        /*else if(strcmp(comtok[0], "save2") == 0){

            if (numarg!=2) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: save necesita un argumento\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }
            if(dfActivo == NULL){
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay dataframe activo\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }
            if (Save2(dfActivo, comtok[1])==-1){
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No se ha podido abrir el archivo\n");
                SetConsoleTextAttribute(hConsole, 7);
            }
        }*/
        
        

                        // FUNCION ADDROW AÑADIR FILA



        /*else if (strcmp(comtok[0], "addrow") == 0) {
            if (!dfActivo) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay DataFrame activo.\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Crear un arreglo para almacenar los valores de la nueva fila
            void **valores = malloc(dfActivo->numColumnas * sizeof(void *));
            if (!valores) {
                printf("Error: No se pudo asignar memoria para los valores de la fila.\n");
                continue;
            }

            // Solicitar valores para cada columna
            for (int i = 0; i < dfActivo->numColumnas; i++) {
                printf("Ingrese valor para la columna '%s' (%s): ", 
                    dfActivo->columna[i].nombre, 
                    dfActivo->columna[i].tipo == TEXTO ? "TEXTO" : 
                    (dfActivo->columna[i].tipo == NUMERICO ? "NUMERICO" : "FECHA"));

                char input[100];
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = '\0'; // Eliminar salto de línea

                if (strlen(input) == 0) { // Si el usuario no ingresa nada, se marca como nulo
                    valores[i] = NULL;
                    dfActivo->columna[i].esNulo[dfActivo->numFilas] = 1;
                } else {
                    if (dfActivo->columna[i].tipo == TEXTO) {
                        valores[i] = strdup(input); // Guardar texto
                    } else if (dfActivo->columna[i].tipo == NUMERICO) {
                        double *num = malloc(sizeof(double));
                        if (num) {
                            *num = atof(input);
                            valores[i] = num; // Guardar número
                        } else {
                            //printf("Error al asignar memoria para numero.\n");
                            valores[i] = NULL;
                        }
                    } else if (dfActivo->columna[i].tipo == FECHA) {
                        Fecha *fecha = AFecha(input); // Convertir a formato de fecha
                        if (fecha) {
                            valores[i] = fecha; // Guardar fecha
                        } else {
                            //printf("Error: Fecha inválida. Marcara como nulo.\n");
                            valores[i] = NULL;
                        }
                    }
                }
            }

            // Llamar a la función para agregar la fila
            if (AgregarFila(dfActivo, valores) == 1) {
                printf("Fila añadida correctamente.\n");
            } else {
                printf("Error al añadir la fila.\n");
            }

            // Liberar memoria de los valores temporales
            for (int i = 0; i < dfActivo->numColumnas; i++) {
                if (dfActivo->columna[i].tipo == TEXTO && valores[i]) {
                    free(valores[i]);
                } else if (dfActivo->columna[i].tipo == NUMERICO && valores[i]) {
                    free(valores[i]);
                } else if (dfActivo->columna[i].tipo == FECHA && valores[i]) {
                    free(valores[i]);
                }
            }
            sprintf(prompt, "%s: %d,%d", dfActivo->nombre, dfActivo->numFilas, dfActivo->numColumnas);
        }*/


        
        
                        // ADDFILE FUNCION FUSIONAR UN DATAFRAME CON LOS DATOS DE UN FICHERO




    else if (strcmp(comtok[0], "add") == 0) {
        if (numarg < 2 || numarg > 3) {
            SetConsoleTextAttribute(hConsole, 4);
            printf("[ERROR] add requiere 1 o 2 argumentos (archivo y separador opcional)\n");
            SetConsoleTextAttribute(hConsole, 7);
            continue;
        }

        if (dfActivo == NULL) {
            SetConsoleTextAttribute(hConsole, 4);
            printf("[ERROR] No hay dataframe activo\n");
            SetConsoleTextAttribute(hConsole, 7);
            continue;
        }

        Nodo *nuevo = Load(comtok[1]);
        if (nuevo == NULL || nuevo->df == NULL) {
            SetConsoleTextAttribute(hConsole, 4);
            printf("[ERROR] No se pudo cargar el archivo %s\n", comtok[1]);
            SetConsoleTextAttribute(hConsole, 7);
            continue;
        }

        // Comprobar si el número de columnas coincide
        if (dfActivo->numColumnas != nuevo->df->numColumnas) {
            SetConsoleTextAttribute(hConsole, 4);
            printf("[ERROR] El numero de columnas del DataFrame en '%s' (%d) no coincide con el DataFrame activo (%d).\n",
                comtok[1], nuevo->df->numColumnas, dfActivo->numColumnas);
            SetConsoleTextAttribute(hConsole, 7);

            // Liberar memoria del nuevo DataFrame antes de continuar
            LiberarDataframe(nuevo->df);
            free(nuevo);
            continue;
        }

        // Agregar las filas del nuevo DataFrame al activo
        if (AgregarFilasDesdeOtroDF(dfActivo, nuevo->df) == 1) {
            // Actualizar el prompt
            sprintf(aux, "%s: %d,%d", dfActivo->nombre, dfActivo->numFilas, dfActivo->numColumnas);
            strcpy(prompt, aux);

            SetConsoleTextAttribute(hConsole, 2);
            //printf("Archivo %s añadido con éxito como filas.\n", comtok[1]);
            SetConsoleTextAttribute(hConsole, 7);
        }else{
            SetConsoleTextAttribute(hConsole, 4);
            printf("[ERROR]: %s contiene tipos INVALIDOS en el dataframe.\n", comtok[1]);
            SetConsoleTextAttribute(hConsole, 7);
        }

        // Liberar memoria del nuevo nodo
        LiberarDataframe(nuevo->df);
        free(nuevo);
    }






                    // ADDDF AÑADIR UN DF A OTRO DF

        /*else if (strcmp(comtok[0], "adddf") == 0) {
            if (numarg != 2) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: adddf necesita un argumento\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (dfActivo == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay dataframe activo\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Buscar el dataframe por nombre en la lista de dataframes
            char *input = comtok[1]; // El argumento es el nombre del dataframe a añadir
            Nodo *current = laGranLista->primero;
            Dataframe *nuevoDF = NULL; // Inicializar como NULL
            while (current != NULL) {
                if (strcmp(current->df->nombre, input) == 0) { // Coincidencia encontrada
                    nuevoDF = current->df;
                    break;
                }
                current = current->siguiente;
            }

            if (nuevoDF == NULL) { // Si no se encuentra el dataframe
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: Dataframe con nombre '%s' no encontrado\n", input);
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Ajustar el número de filas en el dataframe activo si es necesario
            while (dfActivo->numFilas < nuevoDF->numFilas) {
                void **valores = calloc(dfActivo->numColumnas, sizeof(void *)); // Inicializar valores como nulos
                if (!valores) {
                    printf("[Error] No se pudo asignar memoria para los valores iniciales.\n");
                    exit(EXIT_FAILURE);
                }

                for (int i = 0; i < dfActivo->numColumnas; i++) {
                    if (dfActivo->columna[i].tipo == TEXTO) {
                        valores[i] = NULL; // Para texto, dejar como NULL
                    } else if (dfActivo->columna[i].tipo == NUMERICO) {
                        valores[i] = malloc(sizeof(double)); // Para numérico, asignar 0.0
                        if (valores[i]) *(double *)valores[i] = 0.0;
                    } else if (dfActivo->columna[i].tipo == FECHA) {
                        valores[i] = malloc(sizeof(Fecha)); // Para fecha, asignar 0-0-0
                        if (valores[i]) memset(valores[i], 0, sizeof(Fecha));
                    }
                }

                // Agregar fila al dataframe activo
                if (AgregarFila(dfActivo, valores) == -1) {
                    printf("[Error] No se pudo agregar una nueva fila al dataframe activo.\n");
                    exit(EXIT_FAILURE);
                }

                // Liberar memoria temporal para los valores
                for (int i = 0; i < dfActivo->numColumnas; i++) {
                    free(valores[i]);
                }
                free(valores);
            }

            // Agregar las columnas del dataframe proporcionado al dataframe activo
            for (int i = 0; i < nuevoDF->numColumnas; i++) {
                // Hacer una copia del nombre de la columna para no modificar el original
                char nombreCopia[100];
                strcpy(nombreCopia, nuevoDF->columna[i].nombre);

                // Renombrar la columna solo en el dataframe activo
                RenombrarColumnaDuplicada(dfActivo, nombreCopia);

                // Crear una copia de la columna con el nombre ajustado
                Columna nuevaColumna = nuevoDF->columna[i];
                strcpy(nuevaColumna.nombre, nombreCopia);

                // Agregar la columna al dataframe activo
                if (CrearYAgregarColumna(dfActivo, &nuevaColumna) == -1) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: No se pudo agregar la columna %s\n", nuevaColumna.nombre);
                    SetConsoleTextAttribute(hConsole, 7);
                }
            }

            // Actualizar el prompt
            sprintf(aux, "%s: %d,%d", dfActivo->nombre, dfActivo->numFilas, dfActivo->numColumnas);
            strcpy(prompt, aux);

            // Mensaje de éxito
            SetConsoleTextAttribute(hConsole, 2);
            printf("Dataframe '%s' anyadido con exito al dataframe activo\n", input);
            SetConsoleTextAttribute(hConsole, 7);
        }*/


                        // DROPDUPLICATES BORRAR FILAS CON VALORES DUPLICADOS

    
        /*else if (strcmp(comtok[0], "dropduplicates") == 0) {
            if (numarg != 2) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: dropduplicates necesita exactamente 1 argumento (el nombre de la columna).\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (!dfActivo) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay DataFrame activo.\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (DropDuplicates(dfActivo, comtok[1]) == 1) {
                //printf("[Info] Operación dropduplicates completada.\n");
                // Actualizar el prompt con el estado actual del DataFrame
                sprintf(aux, "%s: %d,%d", dfActivo->nombre, dfActivo->numFilas, dfActivo->numColumnas);
                strcpy(prompt, aux);
                printf("[Info] Prompt actualizado: %s\n", prompt);
            } else {
                SetConsoleTextAttribute(hConsole,4);
                printf("[Error] No se pudo completar la operacion dropduplicates.\n");
                SetConsoleTextAttribute(hConsole,7);
            }

            
        }*/




            // FILLNA   Reemplaza valores nulos en una columna específica por un valor insertado por el usuario.




        /*else if (strcmp(comtok[0], "fillna") == 0) {
            if (numarg != 2) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: fillna necesita un argumento (nombre de la columna)\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (dfActivo == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay dataframe activo\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            char *nombreColumna = comtok[1];
            Columna *columna = NULL;

            // Buscar la columna en el dataframe activo
            for (int i = 0; i < dfActivo->numColumnas; i++) {
                if (strcmp(dfActivo->columna[i].nombre, nombreColumna) == 0) {
                    columna = &dfActivo->columna[i];
                    break;
                }
            }

            if (columna == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: La columna '%s' no existe en el dataframe activo\n", nombreColumna);
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Pedir al usuario el valor con el que se reemplazarán los nulos
            printf("Introduce el valor con el que deseas reemplazar los nulos en '%s':\n", nombreColumna);
            char input[100];
            fgets(input, 100, stdin);
            Trim(input);

            // Validar y procesar el valor según el tipo de la columna
            if (columna->tipo == TEXTO) {
                // Validación específica para texto (se acepta cualquier cadena)
                for (int i = 0; i < dfActivo->numFilas; i++) {
                    if (columna->esNulo[i]) {
                        columna->esNulo[i] = 0;
                        ((char **)columna->datos)[i] = strdup(input);
                    }
                }
            } else if (columna->tipo == NUMERICO) {
                // Validar si el valor ingresado es un número válido
                if (!Validar_decimal(input)) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: El valor proporcionado no es numerico valido\n");
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }

                double valor = atof(input);
                for (int i = 0; i < dfActivo->numFilas; i++) {
                    if (columna->esNulo[i]) {
                        columna->esNulo[i] = 0;
                        ((double *)columna->datos)[i] = valor;
                    }
                }
            } else if (columna->tipo == FECHA) {
                // Validar si el valor ingresado es una fecha válida
                if (!FechaValidarAMD(input)) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: El valor proporcionado no es una fecha valida (formato esperado: AAAA/MM/DD)\n");
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }

                Fecha valorFecha = *AFecha(input);
                for (int i = 0; i < dfActivo->numFilas; i++) {
                    if (columna->esNulo[i]) {
                        columna->esNulo[i] = 0;
                        ((Fecha *)columna->datos)[i] = valorFecha;
                    }
                }
            } else {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: Tipo de columna no soportado para 'fillna'\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Mensaje de éxito
            SetConsoleTextAttribute(hConsole, 2);
            printf("Valores nulos en la columna '%s' reemplazados correctamente\n", nombreColumna);
            SetConsoleTextAttribute(hConsole, 7);
        }*/





            // HISTOGRAM            funcion que genera un histograma




        /*else if (strcmp(comtok[0], "histogram") == 0) {
            if (numarg != 3) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: histogram necesita 2 argumentos (nombre de la columna y numero de bins).\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (!dfActivo) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay DataFrame activo.\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            int numBins = atoi(comtok[2]);
            if (numBins <= 0) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: El numero de bins debe ser mayor que 0.\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (Histogram(dfActivo, comtok[1], numBins) == 1) {
                //printf("[Info] Histograma generado correctamente.\n");
            } else {
                SetConsoleTextAttribute(hConsole, 4);
                printf("[Error] No se pudo generar el histograma.\n");
                SetConsoleTextAttribute(hConsole, 7);
            }
        }*/




            // TRUNC            funcion que trunca decimales en una columna



        /*else if (strcmp(comtok[0], "trunc") == 0) {
            if (numarg != 2) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: trunc necesita un argumento (nombre de la columna)\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (dfActivo == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay dataframe activo\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            char *nombreColumna = comtok[1];
            Columna *columna = NULL;

            // Buscar la columna en el dataframe activo
            for (int i = 0; i < dfActivo->numColumnas; i++) {
                if (strcmp(dfActivo->columna[i].nombre, nombreColumna) == 0) {
                    columna = &dfActivo->columna[i];
                    break;
                }
            }

            if (columna == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: La columna '%s' no existe en el dataframe activo\n", nombreColumna);
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Verificar que la columna sea de tipo numérico
            if (columna->tipo != NUMERICO) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: La columna '%s' no es de tipo numerico\n", nombreColumna);
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Truncar los valores decimales
            for (int i = 0; i < dfActivo->numFilas; i++) {
                if (!columna->esNulo[i]) {
                    double *valor = &((double *)columna->datos)[i];
                    *valor = (double)((int)(*valor)); // Truncar el valor convirtiéndolo a entero
                }
            }

            // Mensaje de éxito
            //SetConsoleTextAttribute(hConsole, 2);
            //printf("Los valores decimales en la columna '%s' han sido truncados correctamente\n", nombreColumna);
            //SetConsoleTextAttribute(hConsole, 7);
        }*/



        // SUMCOL       funcion que genera la suma de dos clumnas numéricas y calcula la cantidad de días entre dos fechas


        /*else if (strcmp(comtok[0], "sumCol") == 0) {
            if (numarg != 4) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: El comando sumCol necesita 3 argumentos (columna1, columna2, columna3).\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (!dfActivo) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay DataFrame activo.\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Buscar las columnas y validar los tipos
            Columna *col1 = BuscarColumna(dfActivo, comtok[1]);
            Columna *col2 = BuscarColumna(dfActivo, comtok[2]);

            if (!col1 || !col2) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: Una o ambas columnas especificadas no existen.\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Verificar tipos de columnas
            if (col1->tipo == NUMERICO && col2->tipo == NUMERICO) {
                // Llamar a Sum si ambas columnas son numéricas
                if (Sum(dfActivo, comtok[1], comtok[2], comtok[3]) != 1) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("[Error] No se pudo completar la operacion sum para columnas numericas.\n");
                    SetConsoleTextAttribute(hConsole, 7);
                }
            } else if (col1->tipo == FECHA && col2->tipo == FECHA) {
                // Llamar a SumFecha si ambas columnas son fechas
                if (SumFecha(dfActivo, comtok[1], comtok[2], comtok[3]) != 1) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("[Error] No se pudo completar la operacion sum para columnas de tipo fecha.\n");
                    SetConsoleTextAttribute(hConsole, 7);
                }
            } else {
                // Error si los tipos no coinciden o si son de tipo texto
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: Tipos de columnas incompatibles o no soportados. Ambas columnas deben ser numericas o de tipo fecha.\n");
                SetConsoleTextAttribute(hConsole, 7);
            }

            // Actualizar el prompt con el nuevo estado del DataFrame
            sprintf(aux, "%s: %d,%d", dfActivo->nombre, dfActivo->numFilas, dfActivo->numColumnas);
            strcpy(prompt, aux);
        }*/



                // SUBCOL       funcion que genera la resta de dos clumnas numéricas y calcula la cantidad de días entre dos fechas


        /*else if (strcmp(comtok[0], "subCol") == 0) {
            if (numarg != 4) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: El comando subCol necesita 3 argumentos (columna1, columna2, columna3).\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (!dfActivo) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay DataFrame activo.\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Buscar las columnas y validar los tipos
            Columna *col1 = BuscarColumna(dfActivo, comtok[1]);
            Columna *col2 = BuscarColumna(dfActivo, comtok[2]);

            if (!col1 || !col2) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: Una o ambas columnas especificadas no existen.\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Verificar tipos de columnas
            if (col1->tipo == NUMERICO && col2->tipo == NUMERICO) {
                // Llamar a Sub si ambas columnas son numéricas
                if (Sub(dfActivo, comtok[1], comtok[2], comtok[3]) != 1) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("[Error] No se pudo completar la operacion sub para columnas numericas.\n");
                    SetConsoleTextAttribute(hConsole, 7);
                }
            } else if (col1->tipo == FECHA && col2->tipo == FECHA) {
                // Llamar a SumFecha si ambas columnas son de tipo fecha
                if (SumFecha(dfActivo, comtok[1], comtok[2], comtok[3]) != 1) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("[Error] No se pudo completar la operacion sub para columnas de tipo fecha.\n");
                    SetConsoleTextAttribute(hConsole, 7);
                }
            } else {
                // Error si los tipos no coinciden o si son de tipo texto
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: Tipos de columnas incompatibles o no soportados. Ambas columnas deben ser numericas o de tipo fecha.\n");
                SetConsoleTextAttribute(hConsole, 7);
            }

            // Actualizar el prompt con el nuevo estado del DataFrame
            sprintf(aux, "%s: %d,%d", dfActivo->nombre, dfActivo->numFilas, dfActivo->numColumnas);
            strcpy(prompt, aux);
        }*/



                //  SAVEMETA            FUNCION QUE GUARDA EL META EN UN .TXT


        /*
        else if (strcmp(comtok[0], "saveMeta") == 0) {
            if (numarg != 1) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: El comando saveMeta no necesita argumentos.\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (!dfActivo) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay DataFrame activo.\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (GuardarMeta(dfActivo, "meta.txt") != 1) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("[Error] No se pudo guardar los metadatos del DataFrame.\n");
                SetConsoleTextAttribute(hConsole, 7);
            }
        }*/


                    // CONCAT funcion que concatena dos columnas, da igual su contenido.
        /*
        else if (strcmp(comtok[0], "concat") == 0) {
            if (numarg != 4) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: concat necesita tres argumentos (<col1> <col2> <col3>).\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (!dfActivo) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay dataframe activo.\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (Concatenar(dfActivo, comtok[1], comtok[2], comtok[3]) != 1) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("[Error] No se pudo completar la operacion concat.\n");
                SetConsoleTextAttribute(hConsole, 7);
            }

            sprintf(aux, "%s: %d,%d", dfActivo->nombre, dfActivo->numFilas, dfActivo->numColumnas);
            strcpy(prompt, aux);
        }*/



        //  UNIQUE funcion que devuelve los valores únicos de una columna



        /*else if (strcmp(comtok[0], "unique") == 0) {
            if (numarg != 2) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: unique necesita un argumento (<columna>)\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (dfActivo == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay dataframe activo\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            char *nombreColumna = comtok[1];
            Columna *columna = NULL;

            // Buscar la columna
            for (int i = 0; i < dfActivo->numColumnas; i++) {
                if (strcmp(dfActivo->columna[i].nombre, nombreColumna) == 0) {
                    columna = &dfActivo->columna[i];
                    break;
                }
            }

            if (columna == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: La columna '%s' no existe en el dataframe activo\n", nombreColumna);
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Obtener y mostrar los valores únicos que aparecen una vez
            int numUnicos = 0;
            void *valoresUnicos = ObtenerValoresUnicosExclusivos(columna, &numUnicos);
            if (valoresUnicos == NULL || numUnicos == 0) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("No hay valores unicos (con una sola aparicion) en la columna '%s'\n", nombreColumna);
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            ImprimirValoresUnicosExclusivos(columna, valoresUnicos, numUnicos);
        }*/




        // SEARCH comando que ayuda a buscar un dato dentro de una columna



        /*else if (strcmp(comtok[0], "search") == 0) {
            if (numarg != 3) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: search necesita dos argumentos (<columna> <valor>)\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            if (dfActivo == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: No hay dataframe activo\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            char *nombreColumna = comtok[1];
            char *valor = comtok[2];
            Columna *columna = NULL;

            // Buscar la columna
            for (int i = 0; i < dfActivo->numColumnas; i++) {
                if (strcmp(dfActivo->columna[i].nombre, nombreColumna) == 0) {
                    columna = &dfActivo->columna[i];
                    break;
                }
            }

            if (columna == NULL) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: La columna '%s' no existe en el dataframe activo\n", nombreColumna);
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            // Convertir el valor al tipo correcto
            void *valorConvertido = NULL;
            if (columna->tipo == TEXTO) {
                valorConvertido = valor;
            } else if (columna->tipo == NUMERICO) {
                double *num = malloc(sizeof(double));
                *num = atof(valor);
                valorConvertido = num;
            } else if (columna->tipo == FECHA) {
                Fecha *fecha = malloc(sizeof(Fecha));
                if (!FechaValidarAMD(valor)) {
                    SetConsoleTextAttribute(hConsole, 4);
                    printf("Error: El valor '%s' no es una fecha valida en formato AAAA/MM/DD\n", valor);
                    SetConsoleTextAttribute(hConsole, 7);
                    continue;
                }
                *fecha = *AFecha(valor);
                valorConvertido = fecha;
            }

            // Buscar el valor en la columna
            int numResultados = 0;
            int *filasEncontradas = BuscarEnColumna(columna, valorConvertido, &numResultados);

            if (numResultados == 0) {
                SetConsoleTextAttribute(hConsole, 4);
                printf("No se encontraron filas con el valor '%s' en la columna '%s'\n", valor, nombreColumna);
                SetConsoleTextAttribute(hConsole, 7);
                free(filasEncontradas);
                continue;
            }

            // Imprimir las filas encontradas
            SetConsoleTextAttribute(hConsole, 2);
            //printf("Filas con el valor '%s' en la columna '%s':\n", valor, nombreColumna);
            SetConsoleTextAttribute(hConsole, 7);
            for (int i = 0; i < numResultados; i++) {
                printf("Fila %d:\n", filasEncontradas[i]);
                for (int j = 0; j < dfActivo->numColumnas; j++) {
                    printf("  %s: ", dfActivo->columna[j].nombre);
                    if (dfActivo->columna[j].esNulo[filasEncontradas[i]]) {
                        printf("#N/A");
                    } else if (dfActivo->columna[j].tipo == TEXTO) {
                        printf("%s", ((char **)dfActivo->columna[j].datos)[filasEncontradas[i]]);
                    } else if (dfActivo->columna[j].tipo == NUMERICO) {
                        printf("%.2f", ((double *)dfActivo->columna[j].datos)[filasEncontradas[i]]);
                    } else if (dfActivo->columna[j].tipo == FECHA) {
                        Fecha *fecha = &((Fecha *)dfActivo->columna[j].datos)[filasEncontradas[i]];
                        printf("%04d/%02d/%02d", fecha->tm_year, fecha->tm_mon + 1, fecha->tm_mday);
                    }
                    printf("\n");
                }
                printf("\n");
            }

            free(filasEncontradas);
            if (columna->tipo != TEXTO) {
                free(valorConvertido);
            }
        }*/



        else {
            if (numarg > 1) { // Asegurar que no haya argumentos adicionales
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: DF no acepta argumentos adicionales o Comando erroneo\n");
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }

            char *input = comtok[0]; // El primer token es el posible nombre del dataframe
            Nodo *current = laGranLista->primero;
            int found = 0;

            while (current != NULL) {
                if (strcmp(current->df->nombre, input) == 0) { // Coincidencia del nombre del dataframe
                    found = 1;
                    nodoActivo = current;
                    dfActivo = current->df;

                    // Actualizar el prompt para reflejar el nuevo dataframe activo
                    sprintf(prompt, "%s: %d,%d", dfActivo->nombre, dfActivo->numFilas, dfActivo->numColumnas);

                    // Actualizar los nombres de las columnas
                    //printf("[DEBUG] Cambiando a dataframe '%s' con %d columnas\n", dfActivo->nombre, dfActivo->numColumnas);
                    for (int i = 0; i < dfActivo->numColumnas; i++) {
                        //printf("[DEBUG] Columna %d: %s\n", i + 1, dfActivo->columna[i].nombre);
                    }

                    break;
                }
                current = current->siguiente;
            }

            if (!found) { // Si no se encuentra el nombre del dataframe
                SetConsoleTextAttribute(hConsole, 4);
                printf("Error: Dataframe con nombre '%s' no encontrado o Comando erroneo\n", input);
                SetConsoleTextAttribute(hConsole, 7);
                continue;
            }
        }
    }
    return 0;
}

