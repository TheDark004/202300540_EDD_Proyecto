#ifndef ESTRUCTURAS_H // evitara incluir muchas veces el archivo
#define ESTRUCTURAS_H

#include <iostream>
#include <string>
using namespace std;

// CLASES

class Avion
{
public:
    string vuelo;          // Número de vuelo
    string numeroRegistro; // Número único del avión - llave para Arbol B
    string modelo;
    string fabricante;
    int aFabricacion;    // año de fabricacion
    int capacidad;       // Cantidad de pasajeros
    int pesoMaxDespegue; // Peso máximo en kg
    string aerolinea;
    string estado; // "Disponible" o "Mantenimiento"

    // CONSTRUCTOR
    Avion()
    {
        vuelo = "";
        numeroRegistro = "";
        modelo = "";
        fabricante = "";
        aFabricacion = 0;
        capacidad = 0;
        pesoMaxDespegue = 0;
        aerolinea = "";
        estado = "Disponible";
    }
};

class Pasajero
{
public:
    string nombre;
    string nacionalidad;
    string numPasaporte;
    string vuelo;
    int asiento;
    string destino;
    string origen;
    int equipajeFacturado;

    // CONSTRUCTOR
    Pasajero()
    {
        nombre = "";
        nacionalidad = "";
        numPasaporte = "";
        vuelo = "";
        asiento = 0;
        destino = "";
        origen = "";
        equipajeFacturado = 0;
    }
};

class Piloto
{
public:
    string id; // ID único del piloto
    string nombre;
    string nacionalidad;
    string numeroLicencia;
    int horasVuelo;      // Para ordenar en ABB
    string tipoLicencia; // Comercial, Privado, etc.

    Piloto()
    {
        id = "";
        nombre = "";
        nacionalidad = "";
        numeroLicencia = "";
        horasVuelo = 0;
        tipoLicencia = "";
    }
};

// NODOS

/*
   NODO CIRCULAR DOBLE
    [anterior] <--- [DATO] ---> [siguiente]
         ^                           |
        |___________________________|
    - Doble: tiene apuntadores a ANTERIOR y SIGUIENTE
    - Circular: el último apunta al primero
*/
class NodoCircularDoble
{
public:
    Avion *dato; // Apuntador al avión que guarda
    NodoCircularDoble *siguiente;
    NodoCircularDoble *anterior;

    // CONSTRUCTOR
    NodoCircularDoble(Avion *avion)
    {
        this->dato = avion;
        this->siguiente = nullptr;
        this->anterior = nullptr;
    }
};

/*
   NODO DE COLA
   [PASAJERO] ---> [PASAJERO] ---> [PASAJERO] ---> nullptr
   (primero)                                      (último)
   - Solo tiene apuntador al SIGUIENTE (en una dirección)
*/
class NodoCola
{
public:
    Pasajero *dato;
    NodoCola *siguiente;

    // CONSTRUCTOR
    NodoCola(Pasajero *pasajero)
    {
        this->dato = pasajero;
        this->siguiente = nullptr;
    }
};

/*
    NODO DE PILA
    [EQUIPAJE 3]  <- tope
    [EQUIPAJE 2]
    [EQUIPAJE 1]

    - Solo tiene apuntador al SIGUIENTE (abajo)
*/
class NodoPila
{
public:
    int numEquipaje;  // Id equipaje
    string pasaporte; // A quien le pertenece
    NodoPila *siguiente;

    NodoPila(int num, string pasa)
    {
        this->numEquipaje = num;
        this->pasaporte = pasa;
        this->siguiente = nullptr;
    }
};

/*
    NODO LISTA DOBLE
    nullptr <--- [PASAJERO] <---> [PASAJERO] <---> [PASAJERO] ---> nullptr
*/

class NodoListaDoble
{
public:
    Pasajero *dato;
    NodoListaDoble *siguiente;
    NodoListaDoble *anterior;

    NodoListaDoble(Pasajero *pasajero)
    {
        this->dato = pasajero; // Pasajero almacenado
        this->anterior = nullptr;
        this->siguiente = nullptr;
    }
};

class NodoB
{
public:
    static const int ORDEN = 5;              // Orden del árbol B
    static const int MAX_CLAVES = ORDEN - 1; // Máximo 4 claves
    static const int MAX_HIJOS = ORDEN;      // Máximo 5 hijos

    bool esHoja;
    int numClaves;              // Cantidad actual de claves
    string claves[MAX_CLAVES];  // Array de números de registro
    Avion *aviones[MAX_CLAVES]; // Array de punteros a aviones
    NodoB *hijos[MAX_HIJOS];    // Array de punteros a hijos

    NodoB(bool hoja = true)
    {
        esHoja = hoja;
        numClaves = 0;

        // Inicializar arrays
        for (int i = 0; i < MAX_CLAVES; i++)
        {
            claves[i] = "";
            aviones[i] = nullptr;
        }

        for (int i = 0; i < MAX_HIJOS; i++)
        {
            hijos[i] = nullptr;
        }
    }
};

// NODO ÁRBOL BINARIO DE BÚSQUEDA
class NodoABB
{
public:
    Piloto *dato;
    NodoABB *izquierdo;
    NodoABB *derecho;

    NodoABB(Piloto *piloto)
    {
        this->dato = piloto;
        this->izquierdo = nullptr;
        this->derecho = nullptr;
    }
};

// NODO TABLA HASH

class NodoHash
{
public:
    Piloto *dato;
    NodoHash *siguiente; // Para encadenamiento (manejo de colisiones)

    NodoHash(Piloto *piloto)
    {
        this->dato = piloto;
        this->siguiente = nullptr;
    }
};

// NODOS PARA GRAFO 

class Arista
{
public:
    string destino;    // Nombre de la ciudad destino
    int distancia;     // Distancia en km
    Arista *siguiente; // Siguiente arista en la lista

    Arista(string dest, int dist)
    {
        this->destino = dest;
        this->distancia = dist;
        this->siguiente = nullptr;
    }
};

class NodoGrafo
{
public:
    string ciudad;          // Nombre de la ciudad (vértice)
    Arista *adyacentes;     // Lista de aristas 
    NodoGrafo *siguiente;   // Siguiente nodo en la lista de vértices

    NodoGrafo(string c)
    {
        this->ciudad = c;
        this->adyacentes = nullptr;
        this->siguiente = nullptr;
    }
};

// NODOS PARA MATRIZ DISPERSA


class NodoMatriz
{
public:
    int fila;              // Índice de fila (piloto)
    int columna;           // Índice de columna (ciudad)
    string vuelo;          // Número de vuelo asignado
    NodoMatriz *derecha;   // Siguiente nodo en la fila
    NodoMatriz *abajo;     // Siguiente nodo en la columna

    NodoMatriz(int f, int c, string v)
    {
        this->fila = f;
        this->columna = c;
        this->vuelo = v;
        this->derecha = nullptr;
        this->abajo = nullptr;
    }
};

// Nodo cabecera para filas (pilotos)
class CabeceraFila
{
public:
    int indiceFila;
    string idPiloto;           // ID del piloto
    NodoMatriz *primero;       // Primer nodo de esta fila
    CabeceraFila *siguiente;   // Siguiente cabecera de fila

    CabeceraFila(int idx, string id)
    {
        this->indiceFila = idx;
        this->idPiloto = id;
        this->primero = nullptr;
        this->siguiente = nullptr;
    }
};

// Nodo cabecera para columnas (ciudades)
class CabeceraColumna
{
public:
    int indiceColumna;
    string ciudad;             // Nombre de la ciudad
    NodoMatriz *primero;       // Primer nodo de esta columna
    CabeceraColumna *siguiente; // Siguiente cabecera de columna

    CabeceraColumna(int idx, string c)
    {
        this->indiceColumna = idx;
        this->ciudad = c;
        this->primero = nullptr;
        this->siguiente = nullptr;
    }
};

#endif
