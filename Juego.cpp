#include "Juego.h"
#include <iostream>
#include <limits>

using namespace std;

Juego::Juego(int numJugadores) 
    : numJugadores(numJugadores), jugadorActual(0), juegoTerminado(false) {
    for (int i = 0; i < numJugadores; ++i) {
        jugadores.push_back(Jugador(i, 18));
    }
}

void Juego::iniciar() {
    while (!juegoTerminado) {
        turnoJugador(jugadorActual);
        jugadorActual = (jugadorActual + 1) % numJugadores;
        juegoTerminado = verificarFinJuego();
    }
    finalizarJuego();
}

void Juego::turnoJugador(int jugadorId) {
    cout << "\n=== Turno del Jugador " << jugadorId << " ===" << endl;
    mostrarEstado();

    Jugador& jugador = jugadores[jugadorId];
    bool accionCompletada = false;
    
    while (!accionCompletada) {
        int opcion;
        cout << "Opciones:\n1. Robar cartas\n2. Realizar trayecto\n3. Ver mapa\nEleccion: ";
        cin >> opcion;

        if (opcion == 1) {
            // Robar dos cartas
            try {
                jugador.robarCarta(baraja.robarCarta());
                jugador.robarCarta(baraja.robarCarta());
                cout << "Robaste 2 cartas!" << endl;
            } catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
            accionCompletada = true;
        } 
        else if (opcion == 2) {
            // Realizar trayecto
            tablero.mostrar();
            int rutaIndex;
            cout << "Selecciona ruta (0-" << tablero.getRutas().size()-1 << ") o -1 para cancelar: ";
            cin >> rutaIndex;
            
            if (rutaIndex == -1) {
                continue;
            }
            
            if (rutaIndex < 0 || rutaIndex >= static_cast<int>(tablero.getRutas().size())) {
                cout << "Índice de ruta inválido!" << endl;
                continue;
            }
            
            const Ruta& ruta = tablero.getRutas()[rutaIndex];
            
            if (tablero.rutaReclamada(rutaIndex)) {
                cout << "Esta ruta ya ha sido reclamada!" << endl;
                continue;
            }
            
            if (jugador.getVagones() < ruta.longitud) {
                cout << "No tienes suficientes vagones para esta ruta!" << endl;
                continue;
            }
            
            // Verificar si tiene suficientes cartas (se asume que se pueden usar de cualquier color)
            if (jugador.getCartas().size() < ruta.longitud) {
                cout << "No tienes suficientes cartas para esta ruta!" << endl;
                continue;
            }
            
            // Reclamar la ruta
            jugador.reclamarRuta(ruta.longitud);
            tablero.reclamarRuta(rutaIndex, jugadorId);
            
            // Descartar cartas usadas (se descartan aleatoriamente)
            vector<Carta> cartasUsadas;
            vector<Carta> cartas = jugador.getCartas();
            int cartasADescartar = ruta.longitud;
            
            // Tomar las primeras cartas
            for (int i = 0; i < cartasADescartar && i < cartas.size(); i++) {
                cartasUsadas.push_back(cartas[i]);
            }
            baraja.descartar(cartasUsadas);
            
            // Eliminar cartas del jugador
            for (int i = 0; i < cartasADescartar; i++) {
                jugador.descartarCartas(0); // Descarta la primera carta
            }
            
            cout << "Ruta reclamada con éxito!" << endl;
            accionCompletada = true;
        }
        else if (opcion == 3) {
            tablero.mostrar();
        }
        else {
            cout << "Opción inválida. Intenta de nuevo." << endl;
        }
    }
}

void Juego::mostrarEstado() {
    cout << "\n--- Estado Actual ---" << endl;
    for (const auto& jugador : jugadores) {
        cout << "Jugador " << jugador.getId() 
             << " | Vagones: " << jugador.getVagones()
             << " | Puntos: " << jugador.calcularPuntuacion() 
             << " | Cartas: " << jugador.getCartas().size() << endl;
    }
    cout << "Cartas restantes: " << baraja.tamano() << endl;
}

bool Juego::verificarFinJuego() {
    for (const auto& jugador : jugadores) {
        if (jugador.getVagones() < 5) {
            cout << "\n=== FINAL DEL JUEGO ===" << endl;
            cout << "Jugador " << jugador.getId() << " tiene menos de 5 vagones!" << endl;
            return true;
        }
    }
    
    if (tablero.todasRutasReclamadas()) {
        cout << "\n=== FINAL DEL JUEGO ===" << endl;
        cout << "¡Todas las rutas han sido reclamadas!" << endl;
        return true;
    }
    
    return false;
}

void Juego::finalizarJuego() {
    cout << "\n=== PUNTUACIONES FINALES ===" << endl;
    int maxPuntos = -1;
    int ganador = -1;
    
    for (const auto& jugador : jugadores) {
        int puntos = jugador.calcularPuntuacion();
        cout << "Jugador " << jugador.getId() << ": " << puntos << " puntos" << endl;
        
        if (puntos > maxPuntos) {
            maxPuntos = puntos;
            ganador = jugador.getId();
        }
    }
    cout << "\n¡Jugador " << ganador << " gana con " << maxPuntos << " puntos!" << endl;
}