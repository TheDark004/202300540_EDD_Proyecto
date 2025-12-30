#ifndef ARBOL_BINARIO_BUSQUEDA_H
#define ARBOL_BINARIO_BUSQUEDA_H

#include <iostream>
#include <fstream>
#include "estructuras.h"

using namespace std;

/*
   ÁRBOL BINARIO DE BÚSQUEDA (ABB)
   - Almacena pilotos ordenados por HORAS DE VUELO
   - Permite recorridos: preorden, inorden, postorden
   - Soporta eliminación de pilotos (dar de baja)
*/

class ArbolBinarioBusqueda
{
private:
    NodoABB *raiz;
    ofstream archivo;

    // MÉTODOS PRIVADOS (RECURSIVOS)
    void generarDOTRecursivo(NodoABB *nodo, ofstream &archivo, int &contador)
    {
        if (nodo == nullptr)
            return;

        int idActual = contador++;

        // Crear etiqueta del nodo
        archivo << "    nodo" << idActual << " [label=\"";
        archivo << nodo->dato->nombre << "\\n";
        archivo << "ID: " << nodo->dato->id << "\\n";
        archivo << "Horas: " << nodo->dato->horasVuelo << "\"];" << endl;

        // Hijo izquierdo
        if (nodo->izquierdo != nullptr)
        {
            int idIzquierdo = contador;
            archivo << "    nodo" << idActual << " -> nodo" << idIzquierdo << ";" << endl;
            generarDOTRecursivo(nodo->izquierdo, archivo, contador);
        }

        // Hijo derecho
        if (nodo->derecho != nullptr)
        {
            int idDerecho = contador;
            archivo << "    nodo" << idActual << " -> nodo" << idDerecho << ";" << endl;
            generarDOTRecursivo(nodo->derecho, archivo, contador);
        }
    }

    // Inserta un piloto de forma recursiva
    NodoABB *insertarNodo(Piloto *piloto, NodoABB *nodoPtr)
    {
        if (nodoPtr == nullptr)
        {
            // Crear nuevo nodo
            NodoABB *nuevo = new NodoABB(piloto);
            nodoPtr = nuevo;
            cout << "  → Piloto insertado: " << piloto->nombre
                 << " (" << piloto->horasVuelo << " hrs)" << endl;
        }
        else if (piloto->horasVuelo < nodoPtr->dato->horasVuelo)
        {
            // Ir a la izquierda (menos horas)
            nodoPtr->izquierdo = insertarNodo(piloto, nodoPtr->izquierdo);
        }
        else if (piloto->horasVuelo > nodoPtr->dato->horasVuelo)
        {
            // Ir a la derecha (más horas)
            nodoPtr->derecho = insertarNodo(piloto, nodoPtr->derecho);
        }
        else
        {
            // Mismo número de horas - comparar por ID para evitar duplicados
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
                cout << "  Piloto duplicado (ID: " << piloto->id << ")" << endl;
            }
        }

        return nodoPtr;
    }

    // Busca un piloto por ID
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
            // Buscar en ambos subárboles (porque ordenamos por horas, no por ID)
            NodoABB *resultado = buscarNodo(id, nodoPtr->izquierdo);
            if (resultado != nullptr)
            {
                return resultado;
            }
            return buscarNodo(id, nodoPtr->derecho);
        }
    }

    // Encuentra el nodo con valor mínimo (más a la izquierda)
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
            // Caso 1: Nodo sin hijos (hoja)
            if (nodoPtr->izquierdo == nullptr && nodoPtr->derecho == nullptr)
            {
                delete nodoPtr->dato;
                delete nodoPtr;
                return nullptr;
            }
            // Caso 2: Nodo con un solo hijo (derecho)
            else if (nodoPtr->izquierdo == nullptr)
            {
                NodoABB *temp = nodoPtr->derecho;
                delete nodoPtr->dato;
                delete nodoPtr;
                return temp;
            }
            // Caso 3: Nodo con un solo hijo (izquierdo)
            else if (nodoPtr->derecho == nullptr)
            {
                NodoABB *temp = nodoPtr->izquierdo;
                delete nodoPtr->dato;
                delete nodoPtr;
                return temp;
            }
            // Caso 4: Nodo con dos hijos
            else
            {
                // Encontrar el sucesor (mínimo del subárbol derecho)
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

    // MÉTODOS PÚBLICOS

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

    // Elimina un piloto por ID (dar de baja)
    bool eliminar(string id)
    {
        if (buscar(id) == nullptr)
        {
            cout << "   Piloto no encontrado: " << id << endl;
            return false;
        }

        raiz = eliminarNodo(id, raiz);
        cout << "   Piloto eliminado del ABB: " << id << endl;
        return true;
    }

    // RECORRIDOS PÚBLICOS

    void recorridoPreorden()
    {
        cout << "\n"
             << endl;
        cout << "    RECORRIDO PREORDEN (Raíz-Izq-Der)    " << endl;
        cout << "" << endl;
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
        cout << "\n"
             << endl;
        cout << "  RECORRIDO INORDEN (Izq-Raíz-Der)    " << endl;
        cout << "" << endl;
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
        cout << "\n"
             << endl;
        cout << "   RECORRIDO POSTORDEN (Izq-Der-Raíz)    " << endl;
        cout << " " << endl;
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

    // REPORTE GRAPHVIZ

    void generarReporte()
    {
        if (estaVacio())
        {
            cout << "   El árbol está vacío" << endl;
            return;
        }

        ofstream archivo("grafica_abb_pilotos.dot");

        archivo << "digraph ABB {" << endl;
        archivo << "    node [shape=ellipse, style=filled, fillcolor=lightblue];" << endl;
        archivo << "    edge [arrowhead=vee];" << endl;
        archivo << endl;

        archivo << "    titulo [label=\"Árbol Binario - Pilotos por Horas de Vuelo\", "
                << "shape=plaintext, fontsize=16];" << endl;

        // Generar nodos recursivamente
        int contador = 0;
        generarDOTRecursivo(raiz, archivo, contador);

        archivo << "}" << endl;
        archivo.close();

        system("dot -Tpng grafica_abb_pilotos.dot -o grafica_abb_pilotos.png 2>nul");

#ifdef _WIN32
        system("start grafica_abb_pilotos.png");
#endif

        cout << "    Reporte generado: grafica_abb_pilotos.png" << endl;
    }
};

#endif