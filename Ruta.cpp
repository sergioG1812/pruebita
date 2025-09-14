#include "Ruta.h"

using namespace std;

Ruta::Ruta(string orig, string dest, int f, int c, 
           Direccion dir, int lon, Color color) 
    : ciudadOrigen(orig), ciudadDestino(dest), fila(f), columna(c),
      direccion(dir), longitud(lon), colorRequerido(color), 
      jugadorId(-1) {}