#ifndef LISTA_DOBLE_H
#define LISTA_DOBLE_H

#include "estructuras.h"

/*
   LISTA DOBLE ORDENADA

   Esta lista almacena pasajeros que ya pasaron el registro.
   Los va a ordenar por: 1) Número de vuelo, 2) Número de asiento

   nullptr ← [P1] ↔ [P2] ↔ [P3] → nullptr
            vuelo:101 vuelo:101 vuelo:102
            asiento:5 asiento:8 asiento:3
*/

class ListaDoble
{
private:
    NodoListaDoble *primero;
    NodoListaDoble *ultimo;
    int tamanio;

public:
    // CONSTRUCTOR
    ListaDoble()
    {
        primero = nullptr;
        ultimo = nullptr;
        tamanio = 0;

        cout << "Lista de pasajeros creada..." << endl;
    }

    ~ListaDoble()
    {
        NodoListaDoble *actual = primero;

        while (actual != nullptr)
        {
            NodoListaDoble *siguiente = actual->siguiente;
            delete actual->dato;
            delete actual;
            actual = siguiente;
        }
        cout << "Lista de pasajeros destruida" << endl;
    }

    bool debeIrAntes(Pasajero *p1, Pasajero *p2)
    {
        /*
       Retorna TRUE si pasajero1 debe ir ANTES que pasajero2
       - Vuelo de pasajero1 < Vuelo de pasajero2 → TRUE
       - Vuelos iguales y Asiento1 < Asiento2 → TRUE
       - Caso contrario → FALSE
    */

        // Comparar por vuelo primero
        if (p1->vuelo < p2->vuelo)
        {
            return true; // p1 tiene vuelo menor
        }

        if (p1->vuelo > p2->vuelo)
        {
            return false; // p1 tiene vuelo mayor
        }

        // Si los vuelos son iguales, comparar por asiento
        return p1->asiento < p2->asiento;
    }

    void insertarOrdenado(Pasajero *pasajero)
    {

        /*
       Esta función inserta un pasajero manteniendo el orden:
       1. Primero ordenae el VUELO de menor a mayor
       2. Si el vuelo es igual lo ordenara por asiento menor es decir de menor a mayor
    */
        NodoListaDoble *nuevo = new NodoListaDoble(pasajero);

        if (primero == nullptr)
        {
            primero = nuevo;
            ultimo = nuevo;
            tamanio++;
            cout << "Pasajero ingresado correctamente (Primero): " << pasajero->nombre << endl;
            return;
        }

        // el nuevo pasajero debe ir al inicio
        if (debeIrAntes(pasajero, primero->dato))
        {
            nuevo->siguiente = primero;
            primero->anterior = nuevo;
            primero = nuevo;
            tamanio++;
            cout << "El pasajero se ha colocado de primero: " << pasajero->nombre << endl;
            return;
        }

        // el nuevo debe de ir al final
        if (!debeIrAntes(pasajero, ultimo->dato))
        {
            nuevo->anterior = ultimo;
            ultimo->siguiente = nuevo;
            ultimo = nuevo;
            tamanio++;
            cout << " Pasajero colocado al final: " << pasajero->nombre << endl;
            return;
        }

        // si el nuevo debe ir en medio
        NodoListaDoble *actual = primero;

        while (actual != nullptr)
        {
            if (!debeIrAntes(actual->dato, pasajero))
            {
                nuevo->siguiente = actual;
                nuevo->anterior = actual->anterior;
                actual->anterior->siguiente = nuevo;
                actual->anterior = nuevo;

                tamanio++;
                cout << "Pasajero colocado en medio: " << pasajero->nombre << endl;
                return;
            }

            actual = actual->siguiente;
        }
    }

    bool estaVacia()
    {
        return primero == nullptr;
    }

    int getTamanio()
    {
        return tamanio;
    }

    Pasajero *buscar(string numPasaporte)
    {
        if (primero == nullptr)
        {
            cout << "La lista está vacía" << endl;
            return nullptr;
        }

        NodoListaDoble *actual = primero;

        // Recorrer toda la lista
        while (actual != nullptr)
        {
            if (actual->dato->numPasaporte == numPasaporte)
            {
                return actual->dato;
            }
            actual = actual->siguiente;
        }

        cout << "Pasajero no encontrado: " << numPasaporte << endl;
        return nullptr;
    }

    void mostrar()
    {
        if (primero == nullptr)
        {
            cout << "       LISTA DE PASAJEROS VACÍA       " << endl;
            cout << "______________________________________" << endl;
            return;
        }

        cout << "          LISTA DE PASAJEROS REGISTRADOS " << endl;
        cout << "          (Ordenados por Vuelo y Asiento)" << endl;
        cout << "__________________________________________________________" << endl;

        NodoListaDoble *actual = primero;
        int posicion = 1;

        while (actual != nullptr)
        {
            cout << "Pasajero #" << posicion << endl;
            cout << "Nombre: " << actual->dato->nombre << endl;
            cout << "Pasaporte: " << actual->dato->numPasaporte << endl;
            cout << "Vuelo: " << actual->dato->vuelo << endl;
            cout << "Asiento: " << actual->dato->asiento << endl;
            cout << "Origen: " << actual->dato->origen << endl;
            cout << "Destino: " << actual->dato->destino << endl;
            cout << "Nacionalidad: " << actual->dato->nacionalidad << endl;
            cout << "Equipaje: " << actual->dato->equipajeFacturado << " maletas" << endl;

            if (actual->siguiente != nullptr)
            {
                cout << "" << endl;
            }

            actual = actual->siguiente;
            posicion++;
        }

        cout << "Total de pasajeros registrados: " << tamanio << "\n"
             << endl;
    }

    void mostrarPasajero(string numeroPasaporte)
    {
        Pasajero *p = buscar(numeroPasaporte);

        if (p == nullptr)
        {
            cout << "\n✗ Pasajero no encontrado con pasaporte: " << numeroPasaporte << "\n"
                 << endl;
            return;
        }

        cout << "               INFORMACIÓN DEL PASAJERO                    " << endl;
        cout << "___________________________________________________________" << endl;
        cout << " Nombre: " << p->nombre << endl;
        cout << " Pasaporte: " << p->numPasaporte << endl;
        cout << " Nacionalidad: " << p->nacionalidad << endl;
        cout << " Vuelo: " << p->vuelo << endl;
        cout << " Asiento: " << p->asiento << endl;
        cout << " Origen: " << p->origen << endl;
        cout << " Destino: " << p->destino << endl;
        cout << " Equipaje Facturado: " << p->equipajeFacturado << " maletas" << endl;
    }

    string generarDOT()
    {
        string dot = "digraph ListaPasajeros {\n";
        dot += "    rankdir=LR;\n"; // Orientación izquierda a derecha
        dot += "    node [shape=record];\n\n";

        // Si la lista está vacía
        if (primero == nullptr)
        {
            dot += "    vacio [label=\"Lista Vacía\" shape=box style=filled fillcolor=lightgray];\n";
            dot += "}\n";
            return dot;
        }

        NodoListaDoble *actual = primero;
        int id = 0;

        // Generar nodos
        while (actual != nullptr)
        {
            dot += "    nodo" + to_string(id) + " [label=\"{";
            dot += "Nombre: " + actual->dato->nombre + "\\n";
            dot += "Pasaporte: " + actual->dato->numPasaporte + "\\n";
            dot += "Vuelo: " + actual->dato->vuelo + "\\n";
            dot += "Asiento: " + to_string(actual->dato->asiento) + "\\n";
            dot += "Equipaje: " + to_string(actual->dato->equipajeFacturado);
            dot += "}\"];\n";

            actual = actual->siguiente;
            id++;
        }

        dot += "\n";

        // Generar conexiones (bidireccionales)
        for (int i = 0; i < id - 1; i++)
        {
            dot += "    nodo" + to_string(i) + " -> nodo" + to_string(i + 1) + ";\n";
            dot += "    nodo" + to_string(i + 1) + " -> nodo" + to_string(i) + ";\n";
        }

        dot += "}\n";
        return dot;
    }
};

#endif