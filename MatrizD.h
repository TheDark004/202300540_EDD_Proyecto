#ifndef MATRIZ_DISPERSA_H
#define MATRIZ_DISPERSA_H

#include <iostream>
#include <fstream>
#include "estructuras.h"

using namespace std;

class MatrizDispersa
{
private:
    CabeceraFila *primeraFila;       // Lista de cabeceras de filas (pilotos)
    CabeceraColumna *primeraColumna; // Lista de cabeceras de columnas (ciudades)
    int numFilas;
    int numColumnas;
    int contadorFilas;    // Para asignar índices únicos
    int contadorColumnas; // Para asignar índices únicos

    // MÉTODOS PRIVADOS AUXILIARES

    // Busca una cabecera de fila por ID de piloto
    CabeceraFila *buscarFila(string idPiloto)
    {
        CabeceraFila *actual = primeraFila;
        while (actual != nullptr)
        {
            if (actual->idPiloto == idPiloto)
            {
                return actual;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }

    // Busca una cabecera de columna por nombre de ciudad
    CabeceraColumna *buscarColumna(string ciudad)
    {
        CabeceraColumna *actual = primeraColumna;
        while (actual != nullptr)
        {
            if (actual->ciudad == ciudad)
            {
                return actual;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }

    // Crea una nueva cabecera de fila si no existe
    CabeceraFila *crearFila(string idPiloto)
    {
        CabeceraFila *fila = buscarFila(idPiloto);
        if (fila != nullptr)
        {
            return fila; // Ya existe
        }

        // Crear nueva cabecera
        CabeceraFila *nueva = new CabeceraFila(contadorFilas++, idPiloto);

        // Insertar al inicio
        if (primeraFila == nullptr)
        {
            primeraFila = nueva;
        }
        else
        {
            nueva->siguiente = primeraFila;
            primeraFila = nueva;
        }

        numFilas++;
        return nueva;
    }

    // Crea una nueva cabecera de columna si no existe
    CabeceraColumna *crearColumna(string ciudad)
    {
        CabeceraColumna *columna = buscarColumna(ciudad);
        if (columna != nullptr)
        {
            return columna; // Ya existe
        }

        // Crear nueva cabecera
        CabeceraColumna *nueva = new CabeceraColumna(contadorColumnas++, ciudad);

        // Insertar al inicio
        if (primeraColumna == nullptr)
        {
            primeraColumna = nueva;
        }
        else
        {
            nueva->siguiente = primeraColumna;
            primeraColumna = nueva;
        }

        numColumnas++;
        return nueva;
    }

    // Elimina una fila completa (cuando se da de baja un piloto)
    void eliminarFilaCompleta(CabeceraFila *cabecera)
    {
        if (cabecera == nullptr)
        {
            return;
        }

        // Eliminar todos los nodos de esta fila
        NodoMatriz *actual = cabecera->primero;
        while (actual != nullptr)
        {
            NodoMatriz *siguiente = actual->derecha;

            // Desconectar de la columna
            CabeceraColumna *columna = buscarColumna(""); // Necesitamos buscar por índice
            CabeceraColumna *tempCol = primeraColumna;
            while (tempCol != nullptr)
            {
                if (tempCol->indiceColumna == actual->columna)
                {
                    // Desconectar de la lista vertical
                    if (tempCol->primero == actual)
                    {
                        tempCol->primero = actual->abajo;
                    }
                    else
                    {
                        NodoMatriz *anterior = tempCol->primero;
                        while (anterior != nullptr && anterior->abajo != actual)
                        {
                            anterior = anterior->abajo;
                        }
                        if (anterior != nullptr)
                        {
                            anterior->abajo = actual->abajo;
                        }
                    }
                    break;
                }
                tempCol = tempCol->siguiente;
            }

            delete actual;
            actual = siguiente;
        }

        cabecera->primero = nullptr;
    }

    // Elimina cabeceras de columnas vacías
    void limpiarColumnasVacias()
    {
        CabeceraColumna *actual = primeraColumna;
        CabeceraColumna *anterior = nullptr;

        while (actual != nullptr)
        {
            CabeceraColumna *siguiente = actual->siguiente;

            if (actual->primero == nullptr)
            {
                // Columna vacía, eliminarla
                if (anterior == nullptr)
                {
                    primeraColumna = siguiente;
                }
                else
                {
                    anterior->siguiente = siguiente;
                }

                delete actual;
                numColumnas--;
            }
            else
            {
                anterior = actual;
            }

            actual = siguiente;
        }
    }

public:
    // CONSTRUCTOR Y DESTRUCTOR

    MatrizDispersa()
    {
        primeraFila = nullptr;
        primeraColumna = nullptr;
        numFilas = 0;
        numColumnas = 0;
        contadorFilas = 0;
        contadorColumnas = 0;

        cout << "Matriz Dispersa creada" << endl;
    }

    ~MatrizDispersa()
    {
        // Liberar todas las filas
        CabeceraFila *filaActual = primeraFila;
        while (filaActual != nullptr)
        {
            CabeceraFila *siguienteFila = filaActual->siguiente;

            // Liberar nodos de la fila
            NodoMatriz *nodoActual = filaActual->primero;
            while (nodoActual != nullptr)
            {
                NodoMatriz *siguienteNodo = nodoActual->derecha;
                delete nodoActual;
                nodoActual = siguienteNodo;
            }

            delete filaActual;
            filaActual = siguienteFila;
        }

        // Liberar todas las columnas
        CabeceraColumna *colActual = primeraColumna;
        while (colActual != nullptr)
        {
            CabeceraColumna *siguienteCol = colActual->siguiente;
            delete colActual;
            colActual = siguienteCol;
        }

        cout << "Matriz Dispersa destruida (memoria liberada)" << endl;
    }

    // MÉTODOS PÚBLICOS

    bool estaVacia()
    {
        return primeraFila == nullptr && primeraColumna == nullptr;
    }

    // Inserta una asignación: Piloto → Vuelo → Ciudad
    void insertar(string idPiloto, string vuelo, string ciudad)
    {
        // Crear o buscar cabeceras
        CabeceraFila *fila = crearFila(idPiloto);
        CabeceraColumna *columna = crearColumna(ciudad);

        // Crear nuevo nodo
        NodoMatriz *nuevo = new NodoMatriz(fila->indiceFila, columna->indiceColumna, vuelo);

        // Insertar en la fila (lista horizontal)
        if (fila->primero == nullptr)
        {
            fila->primero = nuevo;
        }
        else
        {
            NodoMatriz *actual = fila->primero;
            NodoMatriz *anterior = nullptr;

            // Insertar ordenado por columna
            while (actual != nullptr && actual->columna < nuevo->columna)
            {
                anterior = actual;
                actual = actual->derecha;
            }

            if (anterior == nullptr)
            {
                nuevo->derecha = fila->primero;
                fila->primero = nuevo;
            }
            else
            {
                nuevo->derecha = actual;
                anterior->derecha = nuevo;
            }
        }

        // Insertar en la columna (lista vertical)
        if (columna->primero == nullptr)
        {
            columna->primero = nuevo;
        }
        else
        {
            NodoMatriz *actual = columna->primero;
            NodoMatriz *anterior = nullptr;

            // Insertar ordenado por fila
            while (actual != nullptr && actual->fila < nuevo->fila)
            {
                anterior = actual;
                actual = actual->abajo;
            }

            if (anterior == nullptr)
            {
                nuevo->abajo = columna->primero;
                columna->primero = nuevo;
            }
            else
            {
                nuevo->abajo = actual;
                anterior->abajo = nuevo;
            }
        }

        cout << "  Asignado: Piloto " << idPiloto << " → Vuelo " << vuelo
             << " ->  " << ciudad << endl;
    }

    // Busca una asignación específica
    string buscar(string idPiloto, string ciudad)
    {
        CabeceraFila *fila = buscarFila(idPiloto);
        if (fila == nullptr)
        {
            return "";
        }

        CabeceraColumna *columna = buscarColumna(ciudad);
        if (columna == nullptr)
        {
            return "";
        }

        NodoMatriz *actual = fila->primero;
        while (actual != nullptr)
        {
            if (actual->columna == columna->indiceColumna)
            {
                return actual->vuelo;
            }
            actual = actual->derecha;
        }

        return "";
    }

    // Elimina un piloto completo (dar de baja)
    bool eliminarPiloto(string idPiloto)
    {
        CabeceraFila *fila = buscarFila(idPiloto);
        if (fila == nullptr)
        {
            cout << "  Piloto no encontrado en matriz: " << idPiloto << endl;
            return false;
        }

        // Eliminar toda la fila
        eliminarFilaCompleta(fila);

        // Eliminar la cabecera de la fila
        if (primeraFila == fila)
        {
            primeraFila = fila->siguiente;
        }
        else
        {
            CabeceraFila *anterior = primeraFila;
            while (anterior != nullptr && anterior->siguiente != fila)
            {
                anterior = anterior->siguiente;
            }
            if (anterior != nullptr)
            {
                anterior->siguiente = fila->siguiente;
            }
        }

        delete fila;
        numFilas--;

        // Limpiar columnas vacías
        limpiarColumnasVacias();

        cout << "  Piloto eliminado de matriz: " << idPiloto << endl;
        return true;
    }

    // Muestra la matriz en consola
    void mostrar()
    {

        cout << "      MATRIZ DISPERSA (Pilotos-Ciudades)    " << endl;

        cout << "  Filas (Pilotos): " << numFilas << endl;
        cout << "  Columnas (Ciudades): " << numColumnas << endl;

        if (estaVacia())
        {
            cout << "  Matriz vacía" << endl;
            return;
        }

        CabeceraFila *fila = primeraFila;
        while (fila != nullptr)
        {
            cout << "\n  Piloto " << fila->idPiloto << ":" << endl;

            NodoMatriz *nodo = fila->primero;
            if (nodo == nullptr)
            {
                cout << "    (Sin asignaciones)" << endl;
            }
            else
            {
                while (nodo != nullptr)
                {
                    // Buscar nombre de la ciudad
                    CabeceraColumna *col = primeraColumna;
                    while (col != nullptr)
                    {
                        if (col->indiceColumna == nodo->columna)
                        {
                            cout << "   -> Vuelo " << nodo->vuelo
                                 << " hacia " << col->ciudad << endl;
                            break;
                        }
                        col = col->siguiente;
                    }
                    nodo = nodo->derecha;
                }
            }

            fila = fila->siguiente;
        }
    }

    // REPORTE GRAPHVIZ

    void generarReporte()
    {
        if (estaVacia())
        {
            cout << "  La matriz está vacía" << endl;
            return;
        }

        ofstream archivo;
        archivo.open("grafica_matriz_dispersa.dot", ios::out);

        archivo << "digraph MatrizDispersa {" << endl;
        archivo << "    rankdir=TB;" << endl;
        archivo << "    node [shape=box];" << endl;
        archivo << endl;

        // Nodo raíz
        archivo << "    raiz [label=\"Raíz\", style=filled, fillcolor=yellow];" << endl;

        // Cabeceras de columnas
        CabeceraColumna *col = primeraColumna;
        while (col != nullptr)
        {
            archivo << "    col" << col->indiceColumna
                    << " [label=\"" << col->ciudad
                    << "\", style=filled, fillcolor=lightblue];" << endl;
            col = col->siguiente;
        }

        // Cabeceras de filas
        CabeceraFila *fila = primeraFila;
        while (fila != nullptr)
        {
            archivo << "    fila" << fila->indiceFila
                    << " [label=\"" << fila->idPiloto
                    << "\", style=filled, fillcolor=lightgreen];" << endl;
            fila = fila->siguiente;
        }

        // Nodos internos
        fila = primeraFila;
        while (fila != nullptr)
        {
            NodoMatriz *nodo = fila->primero;
            while (nodo != nullptr)
            {
                archivo << "    nodo" << nodo->fila << "_" << nodo->columna
                        << " [label=\"" << nodo->vuelo << "\", style=filled, fillcolor=lightyellow];" << endl;
                nodo = nodo->derecha;
            }
            fila = fila->siguiente;
        }

        archivo << endl;

        // Conexiones desde raíz a columnas
        col = primeraColumna;
        while (col != nullptr)
        {
            archivo << "    raiz -> col" << col->indiceColumna << ";" << endl;
            col = col->siguiente;
        }

        // Conexiones desde raíz a filas
        fila = primeraFila;
        while (fila != nullptr)
        {
            archivo << "    raiz -> fila" << fila->indiceFila << ";" << endl;
            fila = fila->siguiente;
        }

        // Conexiones horizontales y verticales
        fila = primeraFila;
        while (fila != nullptr)
        {
            // Desde cabecera de fila al primer nodo
            if (fila->primero != nullptr)
            {
                archivo << "    fila" << fila->indiceFila << " -> nodo"
                        << fila->primero->fila << "_" << fila->primero->columna << ";" << endl;

                // Entre nodos de la misma fila
                NodoMatriz *nodo = fila->primero;
                while (nodo != nullptr && nodo->derecha != nullptr)
                {
                    archivo << "    nodo" << nodo->fila << "_" << nodo->columna
                            << " -> nodo" << nodo->derecha->fila << "_" << nodo->derecha->columna
                            << ";" << endl;
                    nodo = nodo->derecha;
                }
            }
            fila = fila->siguiente;
        }

        // Conexiones verticales
        col = primeraColumna;
        while (col != nullptr)
        {
            if (col->primero != nullptr)
            {
                archivo << "    col" << col->indiceColumna << " -> nodo"
                        << col->primero->fila << "_" << col->primero->columna << ";" << endl;

                NodoMatriz *nodo = col->primero;
                while (nodo != nullptr && nodo->abajo != nullptr)
                {
                    archivo << "    nodo" << nodo->fila << "_" << nodo->columna
                            << " -> nodo" << nodo->abajo->fila << "_" << nodo->abajo->columna
                            << ";" << endl;
                    nodo = nodo->abajo;
                }
            }
            col = col->siguiente;
        }

        archivo << "}" << endl;
        archivo.close();

        system("dot -Tpng grafica_matriz_dispersa.dot -o grafica_matriz_dispersa.png");

#ifdef _WIN32
        system("start grafica_matriz_dispersa.png");
#endif

        cout << " Reporte generado: grafica_matriz_dispersa.png" << endl;
    }
};

#endif