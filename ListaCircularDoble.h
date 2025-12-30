#ifndef LISTA_CIRCULAR_H
#define LISTA_CIRCULAR_H

#include "estructuras.h"

/*
   -Para manejar aviones en forma circular.
   -Usaremos DOS listas: una para disponibles, otra para mantenimiento
*/

class ListaCircularDoble
{
private:
    NodoCircularDoble *primero; // apuntandor a primer nodo
    int tamanio;

public:
    // Se ejcuta al crear la lista
    ListaCircularDoble()
    {
        primero = nullptr;
        tamanio = 0;
    }

    // Destructor
    ~ListaCircularDoble()
    {
        if (primero == nullptr)
            return; // si la lista esta vacia pues sale de la funcion

        NodoCircularDoble *actual = primero;
        NodoCircularDoble *siguiente;

        do
        {
            siguiente = actual->siguiente;
            delete actual->dato;
            delete actual;
            actual = siguiente;
        } while (actual != primero);

        primero = nullptr;
        tamanio = 0;
    }

    // METODO PARA INSERTAR
    void insertar(Avion *avion)
    {
        // crear un nuevo nodo con un avion
        NodoCircularDoble *nuevo = new NodoCircularDoble(avion);

        // ver si lista esta vacia
        if (primero == nullptr)
        {
            primero = nuevo;
            nuevo->siguiente = nuevo; // Apunta a sí mismo
            nuevo->anterior = nuevo;  // Apunta a sí mismo
        }
        else
        {
            NodoCircularDoble *ultimo = primero->anterior; // el anterior es el ultimo
            // conectamos los nodos
            nuevo->siguiente = primero; // nuevo -> primero
            nuevo->anterior = ultimo;   // ultimo <- nuevo

            // actualizamos
            ultimo->siguiente = nuevo; // ultimo -> nuevo
            primero->anterior = nuevo; // nuevo <- primero
        }

        tamanio++;
        cout << "el avion: " << avion->vuelo << " (" << avion->estado << " ) se icerto correctamente " << endl;
    }

    // Encontrar un avión por número de registro
    Avion *buscar(string numeroRegistro)
    {
        if (primero == nullptr)
        {
            return nullptr;
        }

        NodoCircularDoble *actual = primero;
        // recorrer lista
        do
        {
            // si se enuentra el avion, return el apuntador
            if (actual->dato->numeroRegistro == numeroRegistro)
            {
                return actual->dato;
            }
            actual = actual->siguiente;
        } while (actual != primero); // Hasta volver al inicio

        // No se encontro
        return nullptr;
    }

    // Eliminar - se quita un avion de la lista
    Avion *eliminar(string numeroRegistro)
    {
        if (primero == nullptr)
        {
            cout << "la lista esta vacia" << endl;
            return nullptr;
        }

        NodoCircularDoble *actual = primero;
        NodoCircularDoble *aEliminar = nullptr;

        // se busca el nodo
        do
        {
            if (actual->dato->numeroRegistro == numeroRegistro)
            {
                aEliminar = actual;
                break;
            }
            actual = actual->siguiente;
        } while (actual != primero);

        // Si no se encontró
        if (aEliminar == nullptr)
        {
            cout << "Avión no encontrado: " << numeroRegistro << endl;
            return nullptr;
        }

        Avion *avionEliminado = aEliminar->dato; // Guardar el avión antes de eliminar el nodo

        // si solo hay un solo elemento
        if (aEliminar->siguiente == aEliminar)
        {
            primero = nullptr;
        }
        else
        {
            NodoCircularDoble *anterior = aEliminar->anterior;
            NodoCircularDoble *siguiente = aEliminar->siguiente;

            // para saltar el nodo eliminado
            anterior->siguiente = siguiente;
            siguiente->anterior = anterior;

            // si se elimina el primero se actualiza el apuntador
            if (aEliminar == primero)
            {
                primero = siguiente;
            }
        }

        delete aEliminar; // Liberar el nodo,pero no el avión, lo retornamos
        tamanio--;
        cout << "Avion eliminado: " << avionEliminado->vuelo << endl;
        return avionEliminado;
    }

    // muestra todos los aviones
    void mostrar()
    {
        if (primero == nullptr)
        {
            cout << "Lista vacía" << endl;
            return;
        }

        NodoCircularDoble *actual = primero;
        int contador = 1;

        do
        {
            cout << contador << ". Vuelo: " << actual->dato->vuelo << endl;
            cout << "Registro: " << actual->dato->numeroRegistro << endl;
            cout << "Modelo: " << actual->dato->modelo << endl;
            cout << "Estado: " << actual->dato->estado << endl;

            actual = actual->siguiente;
            contador++;
        } while (actual != primero);
    }

    bool estaVacia()
    {
        return primero == nullptr;
    }

    int getTamanio()
    {
        return tamanio;
    }

    NodoCircularDoble *getPrimero()
    {
        return primero;
    }

    // GRAPHVIZ
    string generarDOT(string nombreLista)
    {
        string dot = "digraph " + nombreLista + " {\n";
        dot += "    node [shape=record];\n";
        dot += "    rankdir=LR;\n";
        dot += "    edge [dir=both];\n\n";

        if (primero == nullptr)
        {
            dot += "    vacio [label=\"Lista Vacía\", style=filled, fillcolor=lightgray];\n";
            dot += "}\n";
            return dot;
        }

        // Crear nodos
        NodoCircularDoble *actual = primero;
        int id = 0;
        vector<int> ids;

        do
        {
            ids.push_back(id);
            dot += "    nodo" + to_string(id) + " [label=\"{";
            dot += "<prev> | ";
            dot += "Vuelo: " + actual->dato->vuelo + "\\n";
            dot += "Registro: " + actual->dato->numeroRegistro + "\\n";
            dot += "Estado: " + actual->dato->estado;
            dot += " | <next>}\", style=filled, fillcolor=lightblue];\n";

            actual = actual->siguiente;
            id++;
        } while (actual != primero);

        // Crear conexiones
        for (size_t i = 0; i < ids.size(); i++)
        {
            int actualId = ids[i];
            int siguienteId = ids[(i + 1) % ids.size()];

            dot += "    nodo" + to_string(actualId) + ":next -> nodo" + to_string(siguienteId) + ":prev;\n";
            dot += "    nodo" + to_string(siguienteId) + ":prev -> nodo" + to_string(actualId) + ":next;\n";
        }

        // Título
        dot += "\n    titulo [label=\"Lista Circular - Aviones en Mantenimiento\", " + string("shape=plaintext, fontsize=16];\n");

        dot += "}\n";
        return dot;
    }
};
#endif