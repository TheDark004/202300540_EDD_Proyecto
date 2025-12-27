#ifndef LISTA_SIMPLE_H_H
#define LISTA_SIMPLE_h_H

#include "estructuras.h"

class ListaSimpleH
{
private:
    NodoHash *primero;
    int tamanio;

public:
    // Constructor
    ListaSimpleH()
    {
        primero = nullptr;
        tamanio = 0;
    }

    // Destructor
    ~ListaSimpleH()
    {
        NodoHash *actual = primero;
        while (actual != nullptr)
        {
            NodoHash *siguiente = actual->siguiente;
            delete actual->dato; // Liberar el piloto
            delete actual;       // Liberar el nodo
            actual = siguiente;
        }
    }

    // Verifica si está vacía
    bool estaVacia()
    {
        return primero == nullptr;
    }

    // Obtiene el tamaño
    int getTamanio()
    {
        return tamanio;
    }

    // Inserta un piloto al FINAL de la lista
    void insertarFinal(Piloto *piloto)
    {
        NodoHash *nuevo = new NodoHash(piloto);

        if (estaVacia())
        {
            primero = nuevo;
        }
        else
        {
            NodoHash *actual = primero;
            while (actual->siguiente != nullptr)
            {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }

        tamanio++;
    }

    // Busca un piloto por ID
    Piloto *buscar(string id)
    {
        NodoHash *actual = primero;

        while (actual != nullptr)
        {
            if (actual->dato->id == id)
            {
                return actual->dato;
            }
            actual = actual->siguiente;
        }

        return nullptr; // No encontrado
    }

    // Elimina un piloto por ID
    bool eliminar(string id)
    {
        if (estaVacia())
        {
            return false;
        }

        // Si está en el primero
        if (primero->dato->id == id)
        {
            NodoHash *temp = primero;
            primero = primero->siguiente;
            delete temp->dato;
            delete temp;
            tamanio--;
            return true;
        }

        // Buscar en el resto
        NodoHash *actual = primero;
        while (actual->siguiente != nullptr)
        {
            if (actual->siguiente->dato->id == id)
            {
                NodoHash *temp = actual->siguiente;
                actual->siguiente = temp->siguiente;
                delete temp->dato;
                delete temp;
                tamanio--;
                return true;
            }
            actual = actual->siguiente;
        }

        return false; // No encontrado
    }

    // Visualiza la lista (para debugging)
    void visualizarLista()
    {
        if (estaVacia())
        {
            cout << "Vacía";
        }
        else
        {
            NodoHash *actual = primero;
            while (actual != nullptr)
            {
                cout << "[" << actual->dato->id << ": " 
                     << actual->dato->nombre << "]";
                if (actual->siguiente != nullptr)
                {
                    cout << " -> ";
                }
                actual = actual->siguiente;
            }
        }
    }

    // Retorna el primer nodo (para graphviz)
    NodoHash *getPrimero()
    {
        return primero;
    }
};

#endif