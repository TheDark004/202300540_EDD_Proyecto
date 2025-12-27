#ifndef ARBOL_BINARIO_BUSQUEDA_H
#define ARBOL_BINARIO_BUSQUEDA_H

#include <iostream>
#include <fstream>
#include "estructuras.h"

using namespace std;

class ArbolBinarioBusqueda
{
private:
    NodoABB *raiz;
    ofstream archivo;

    // Inserta un piloto de forma recursiva
    NodoABB *insertarNodo(Piloto *piloto, NodoABB *nodoPtr)
    {
        if (nodoPtr == nullptr)
        {
            // Crea nuevo nodo
            NodoABB *nuevo = new NodoABB(piloto);
            nodoPtr = nuevo;
            cout << "  → Piloto insertado: " << piloto->nombre
                 << " (" << piloto->horasVuelo << " hrs)" << endl;
        }
        else if (piloto->horasVuelo < nodoPtr->dato->horasVuelo)
        {
            // Ir a la izquierda- menos horas
            nodoPtr->izquierdo = insertarNodo(piloto, nodoPtr->izquierdo);
        }
        else if (piloto->horasVuelo > nodoPtr->dato->horasVuelo)
        {
            // Ir a la derecha- mas horas
            nodoPtr->derecho = insertarNodo(piloto, nodoPtr->derecho);
        }
        else
        {
            // Mismo numero de horas - comparar por id para evitar duplicados
            if (piloto->id < nodoPtr->dato->id)
            {
                nodoPtr->izquierdo = insertarNodo(piloto, nodoPtr->izquierdo);
            }
            else if (piloto->id > nodoPtr->dato->id)
            {
                nodoPtr->derecho = insertarNodo(piloto, nodoPtr->derecho);
            }
            else
            {
                cout << " Piloto duplicado (ID: " << piloto->id << ")" << endl;
            }
        }

        return nodoPtr;
    }

    // Busca un piloto por id
    NodoABB *buscarNodo(string id, NodoABB *nodoPtr)
    {
        if (nodoPtr == nullptr)
        {
            return nullptr;
        }
        else if (id == nodoPtr->dato->id)
        {
            return nodoPtr;
        }
        else
        {
            // Buscar en ambos subárboles
            NodoABB *resultado = buscarNodo(id, nodoPtr->izquierdo);
            if (resultado != nullptr)
            {
                return resultado;
            }
            return buscarNodo(id, nodoPtr->derecho);
        }
    }

    // Encuentra el nodo con valor mínimo - mas a la izquierda
    NodoABB *encontrarMinimo(NodoABB *nodo)
    {
        while (nodo->izquierdo != nullptr)
        {
            nodo = nodo->izquierdo;
        }
        return nodo;
    }

    // Elimina un piloto por ID
    NodoABB *eliminarNodo(string id, NodoABB *nodoPtr)
    {
        if (nodoPtr == nullptr)
        {
            return nullptr;
        }

        // Si encontramos el nodo
        if (id == nodoPtr->dato->id)
        {
            //  Nodo sin hijos (hoja)
            if (nodoPtr->izquierdo == nullptr && nodoPtr->derecho == nullptr)
            {
                delete nodoPtr->dato;
                delete nodoPtr;
                return nullptr;
            }
            //  Nodo con un solo hijo (derecho)
            else if (nodoPtr->izquierdo == nullptr)
            {
                NodoABB *temp = nodoPtr->derecho;
                delete nodoPtr->dato;
                delete nodoPtr;
                return temp;
            }
            //  Nodo con un solo hijo (izquierdo)
            else if (nodoPtr->derecho == nullptr)
            {
                NodoABB *temp = nodoPtr->izquierdo;
                delete nodoPtr->dato;
                delete nodoPtr;
                return temp;
            }
            // Nodo con dos hijos
            else
            {
                // Encontrar el sucesor
                NodoABB *sucesor = encontrarMinimo(nodoPtr->derecho);

                // Copiar datos del sucesor al nodo actual
                Piloto *tempPiloto = nodoPtr->dato;
                nodoPtr->dato = sucesor->dato;
                sucesor->dato = tempPiloto;

                // Eliminar el sucesor
                nodoPtr->derecho = eliminarNodo(id, nodoPtr->derecho);
            }
        }
        else
        {
            // Buscar en ambos subárboles
            nodoPtr->izquierdo = eliminarNodo(id, nodoPtr->izquierdo);
            nodoPtr->derecho = eliminarNodo(id, nodoPtr->derecho);
        }

        return nodoPtr;
    }

    // RECORRIDOS
    void preorden(NodoABB *nodoPtr)
    {
        if (nodoPtr != nullptr)
        {
            cout << "  • " << nodoPtr->dato->nombre
                 << " - ID: " << nodoPtr->dato->id
                 << " - Horas: " << nodoPtr->dato->horasVuelo << endl;
            preorden(nodoPtr->izquierdo);
            preorden(nodoPtr->derecho);
        }
    }

    void inorden(NodoABB *nodoPtr)
    {
        if (nodoPtr != nullptr)
        {
            inorden(nodoPtr->izquierdo);
            cout << "  • " << nodoPtr->dato->nombre
                 << " - ID: " << nodoPtr->dato->id
                 << " - Horas: " << nodoPtr->dato->horasVuelo << endl;
            inorden(nodoPtr->derecho);
        }
    }

    void postorden(NodoABB *nodoPtr)
    {
        if (nodoPtr != nullptr)
        {
            postorden(nodoPtr->izquierdo);
            postorden(nodoPtr->derecho);
            cout << "  • " << nodoPtr->dato->nombre
                 << " - ID: " << nodoPtr->dato->id
                 << " - Horas: " << nodoPtr->dato->horasVuelo << endl;
        }
    }

    // GRAPHVIZ
    void imprimirNodos(NodoABB *nodoPtr)
    {
        if (nodoPtr == nullptr)
        {
            return;
        }

        // Crear etiqueta del nodo con información del piloto
        archivo << "    nodo" << nodoPtr->dato->id
                << " [label=\"" << nodoPtr->dato->nombre << "\\n"
                << "ID: " << nodoPtr->dato->id << "\\n"
                << "Horas: " << nodoPtr->dato->horasVuelo << "\"];" << endl;

        // Conexión con hijo izquierdo
        if (nodoPtr->izquierdo != nullptr)
        {
            archivo << "    nodo" << nodoPtr->dato->id << " -> nodo"
                    << nodoPtr->izquierdo->dato->id << ";" << endl;
        }

        imprimirNodos(nodoPtr->izquierdo);

        // Conexión con hijo derecho
        if (nodoPtr->derecho != nullptr)
        {
            archivo << "    nodo" << nodoPtr->dato->id << " -> nodo"
                    << nodoPtr->derecho->dato->id << ";" << endl;
        }

        imprimirNodos(nodoPtr->derecho);
    }

    // Destructor recursivo
    void destruirArbol(NodoABB *nodoPtr)
    {
        if (nodoPtr != nullptr)
        {
            destruirArbol(nodoPtr->izquierdo);
            destruirArbol(nodoPtr->derecho);
            delete nodoPtr->dato;
            delete nodoPtr;
        }
    }

public:
    // CONSTRUCTOR Y DESTRUCTOR

    ArbolBinarioBusqueda()
    {
        this->raiz = nullptr;
        cout << "Árbol Binario de Búsqueda creado" << endl;
    }

    ~ArbolBinarioBusqueda()
    {
        destruirArbol(raiz);
        cout << "Árbol Binario destruido" << endl;
    }

    bool estaVacio()
    {
        return (raiz == nullptr);
    }

    NodoABB *getRaiz()
    {
        return this->raiz;
    }

    // Inserta un piloto
    void insertar(Piloto *piloto)
    {
        if (piloto == nullptr)
        {
            cout << "Error: Piloto nulo" << endl;
            return;
        }
        raiz = insertarNodo(piloto, raiz);
    }

    // Busca un piloto por ID
    Piloto *buscar(string id)
    {
        NodoABB *nodo = buscarNodo(id, raiz);
        if (nodo != nullptr)
        {
            return nodo->dato;
        }
        return nullptr;
    }

    // Elimina un piloto por ID
    bool eliminar(string id)
    {
        if (buscar(id) == nullptr)
        {
            cout << "Piloto no encontrado: " << id << endl;
            return false;
        }

        raiz = eliminarNodo(id, raiz);
        cout << " Piloto eliminado del ABB: " << id << endl;
        return true;
    }

    // RECORRIDOS PÚBLICOS

    void recorridoPreorden()
    {

        cout << "    RECORRIDO PREORDEN (Raíz-Izq-Der)    " << endl;

        if (estaVacio())
        {
            cout << "  Árbol vacío" << endl;
        }
        else
        {
            preorden(raiz);
        }
        cout << endl;
    }

    void recorridoInorden()
    {

        cout << "    RECORRIDO INORDEN (Izq-Raíz-Der)     " << endl;

        if (estaVacio())
        {
            cout << "  Árbol vacío" << endl;
        }
        else
        {
            inorden(raiz);
        }
        cout << endl;
    }

    void recorridoPostorden()
    {
        cout << "   RECORRIDO POSTORDEN (Izq-Der-Raíz)    " << endl;
        if (estaVacio())
        {
            cout << "  Árbol vacío" << endl;
        }
        else
        {
            postorden(raiz);
        }
        cout << endl;
    }

    // GRAPHVIZ
    void generarReporte()
    {
        if (estaVacio())
        {
            cout << "  El árbol está vacío" << endl;
            return;
        }

        archivo.open("grafica_abb_pilotos.dot", ios::out);
        archivo << "digraph ABB {" << endl;
        archivo << "    node [shape=circle, style=filled, fillcolor=lightblue];" << endl;

        imprimirNodos(raiz);

        archivo << "}" << endl;
        archivo.close();

        // Generar imagen con Graphviz
        system("dot -Tpng grafica_abb_pilotos.dot -o grafica_abb_pilotos.png");

// Abrir imagen automáticamente
#ifdef _WIN32
        system("start grafica_abb_pilotos.png");
#endif

        cout << "   Reporte generado: grafica_abb_pilotos.png" << endl;
    }
};

#endif