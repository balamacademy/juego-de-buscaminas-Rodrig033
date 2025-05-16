# include <iostream>
# include <ctime>
# include <vector>
# include <stdlib.h>
#include <cstdlib>

using namespace std;

// Constantes globales, para usar en la función de minas: 
const int MAX_FILAS = 100;
const int MAX_COLUMNAS = 100;

void inicializarTablero(int tablero[][MAX_COLUMNAS], bool visible[][MAX_COLUMNAS],int filas, int columnas);
void colocarMinas(int tablero[][MAX_COLUMNAS], int filas, int columnas);
void incrementarAdyacentes(int tablero[][MAX_COLUMNAS], int x, int y, int filas, int columnas);
void destapar_ceros(vector<vector<int>> &tablero, vector<vector<bool>> &visible, int x, int y, int filas, int columnas);
void mostrarTablero(int tablero[][MAX_COLUMNAS], bool visible[][MAX_COLUMNAS], int filas, int columnas);
void jugar(int tablero[][MAX_COLUMNAS], bool visible[][MAX_COLUMNAS], int filas, int columnas);
bool juego_ganado(int tablero[][MAX_COLUMNAS], bool visible[][MAX_COLUMNAS], int filas, int columnas);

int main(){
    srand(time(NULL));
    // Definicion de las variables o constantes:
    const int MINA = -1;
    char CELDA_DESCUBIERTA = ' ';
    char CELDA_OCULTA = '.';
    int filas, columnas, minas;
    int x , y;
  

    cout << "\nIngrese el numero de filas: ";
    cin >> filas;
    cout << "Ingrese el numero de columnas: ";
    cin >> columnas;
    if (filas > MAX_FILAS || columnas > MAX_COLUMNAS )
    {
        cout << "\nEl tamaño maximo del tablero permitido es:" << MAX_FILAS << " x " << MAX_COLUMNAS << ".\n";
        return 1;
    }
    

    int tablero[MAX_FILAS][MAX_COLUMNAS];
    bool visible[MAX_FILAS][MAX_COLUMNAS];

    // Inicializa las celdas en 0:
    
    for (int i = 0; i < filas; i++)
    {
       for (int j = 0; j < columnas; j++)
       {
            tablero[i][j] = 0;
            visible[i][j] = false;
       }
       
    }
    colocarMinas(tablero, filas, columnas);
    jugar(tablero, visible, filas, columnas);
    // Ten cuidado con inicializar el tablero en ceros (otra vez).

    return 0;
}



void colocarMinas(int tablero[][MAX_COLUMNAS], int filas, int columnas){
    int cantidad = columnas + 1;
    int minas_colocadas = 0;

    while (minas_colocadas < cantidad) 
    {
        int x = rand() % filas;
        int y = rand() % columnas;

        if (tablero[x][y] != -1)
    {
        tablero[x][y] = -1;
        minas_colocadas++;
        incrementarAdyacentes(tablero, x, y, filas, columnas);
    }
    }

    
}

void incrementarAdyacentes(int tablero[][MAX_COLUMNAS], int x, int y, int filas, int columnas){
    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
           if (dx == 0 && dy == 0)
           {
                continue; // No procesa la celda de la mina
           }
          int ix = x + dx;
          int iy = y + dy; 
           if (ix >= 0 && ix < filas && iy >= 0 && iy < columnas)
           {
                if (tablero[ix][iy] != -1) // No se incrementan las celdas que ya tienen minas.
                {
                    tablero[ix][iy]++; // Aumenta el número de minas adyacentes a la celda.
                }      
           }
           
        }
        
    }
    

}

void destapar_ceros(int tablero[][MAX_COLUMNAS], bool visible[][MAX_COLUMNAS], int x, int y, int filas, int columnas) {
   
    // Limites de la celda:
    if (x < 0 || x >= filas || y < 0 || y >= columnas || visible[x][y])
        return;

    visible[x][y] = true;

    // Si la celda tiene un número distinto de 0, no expandimos más
    if (tablero[x][y] != 0){
        return;
    }
    // Llamadas recursivas a las 8 posiciones alrededor: 
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx != 0 || dy != 0)
                destapar_ceros(tablero, visible, x + dx, y + dy, filas, columnas);
        }
    }
}


void mostrarTablero(int tablero[][MAX_COLUMNAS], bool visible[][MAX_COLUMNAS], int filas, int columnas){

    system("clear");

    cout << "-- Tablero visible --" << endl;
    cout << "\n";
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            cout << tablero[i][j] << " ";
        }
        cout << endl;
    }


    // Tablero oculto:
    cout << "\n";

    for (int i = 0; i < filas; i++)
    {
        cout << endl;
        for (int j = 0; j < columnas; j++)
        {
            if (visible[i][j])
            {
                if (tablero[i][j] == -1)
                {
                    cout << "M "; // Mina destapada
                }else{
                    cout << tablero[i][j] << " ";
                }
                
            }else{
                cout << "* "; // Celda oculta
            }
        }
    }
    cout << endl;
}

void jugar(int tablero[][MAX_COLUMNAS], bool visible[][MAX_COLUMNAS], int filas, int columnas){
    bool game_over = false;
    bool win = false;
    int x, y;

    while (!game_over)
    {
        mostrarTablero(tablero, visible, filas, columnas);

        cout << "\nIngrese fila y columna (separadas por un espacio): ";
        cin >> x >> y;

        if (x < 0 || x >= filas || y < 0 || y >= columnas )
        {
            cout << "\nPosicion fuera del rango. Intenta de nuevo. \n";
            continue;
        }
        

        if (visible[x][y])
        {
            cout << "\n¡Celda ya descubierta!" << endl;
            continue;
        }

        if (tablero[x][y] == -1)
        {
            for (int i = 0; i < filas; i++)
            {
                for (int j = 0; j < columnas; j++)
                {
                    visible[i][j] = true;
                }
            }
            mostrarTablero(tablero, visible, filas, columnas);
            cout << "\n¡BOOOM! Pisaste una mina, ¡¡call an ambulance!!.\n" << endl;
            game_over = true;
            break;
        }


        if (tablero[x][y] == 0)
        {
            destapar_ceros(tablero, visible, x, y, filas, columnas);
        }else{
            visible[x][y] = true;
        }
        if(juego_ganado(tablero, visible, filas, columnas)){
            mostrarTablero(tablero, visible, filas, columnas);
            cout << "\n¡¡POLE POSITION!!. You´re a champion" << endl;
            game_over = true;
            break;
        }
    }
}

bool juego_ganado(int tablero[][MAX_COLUMNAS], bool visible[][MAX_COLUMNAS], int filas, int columnas){
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            if (tablero[i][j] != -1 && !visible[i][j])
            {
                return false;
            }
        }
    }
    return true;

}



void inicializarTablero(int tablero[][MAX_COLUMNAS], bool visible[][MAX_COLUMNAS],int filas, int columnas){
    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            tablero[i][j] = 0;
            visible[i][j] = false;
        }
    }
}
