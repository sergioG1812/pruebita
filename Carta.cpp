#include "Carta.h"

const char* Carta::colorACadena(Color color) {
    switch(color) {
        case Color::Rojo: return "Rojo";
        case Color::Azul: return "Azul";
        case Color::Verde: return "Verde";
        case Color::Morado: return "Morado";
        case Color::Cafe: return "Cafe";
        case Color::Naranja: return "Naranja";
        default: return "Desconocido";
    }
}

Carta::Carta(Color color) : color(color) {}

Color Carta::getColor() const {
    return color;
}