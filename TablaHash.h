#ifndef TABLA_HASH_H
#define TABLA_HASH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "estructuras.h"
#include "ListaSimpleH.h"

using namespace std;

/*
   - Tamaño: 19 (M = 19)
   - Función de dispersión: h(llave) = llave % 19
   - Manejo de colisiones: Encadenamiento (listas simples)
   - Almacena pilotos por su ID
*/

class TablaHash
{
private:
    int tamTabla;        // Tamaño = 19
    int numElementos;    // Cantidad de pilotos almacenados
    ListaSimpleH *tabla; // Array de listas simples

    // Convierte el ID del piloto (string) a un número para la función hash
    int convertirIDaNumero(string id)
    {
        int suma = 0;
        for (char c : id)
        {
            suma += (int)c; // Suma los valores ASCII
        }
        return suma;
    }

public:
    // CONSTRUCTOR Y DESTRUCTOR

    TablaHash()
    {
        tamTabla = 19; // Tamaño especificado en el proyecto
        numElementos = 0;
        tabla = new ListaSimpleH[tamTabla];

        cout << "Tabla Hash creada (Tamaño: " << tamTabla << ")" << endl;
    }

    ~TablaHash()
    {
        delete[] tabla;
        cout << "Tabla Hash destruida" << endl;
    }

    // FUNCIÓN HASH

    // Calcula el índice usando h(llave) = llave % M
    int Clave(string id)
    {
        int valor = convertirIDaNumero(id);
        int indice = valor % tamTabla;
        return indice;
    }

    // MÉTODOS PRINCIPALES

    // Inserta un piloto en la tabla
    void insertar(Piloto *piloto)
    {
        if (piloto == nullptr)
        {
            cout << " Error: Piloto nulo" << endl;
            return;
        }

        // Verificar si ya existe
        if (buscar(piloto->id) != nullptr)
        {
            cout << " Piloto ya existe en tabla hash: " << piloto->id << endl;
            return;
        }

        // Calcular índice
        int indice = Clave(piloto->id);

        // Insertar en la lista correspondiente
        tabla[indice].insertarFinal(piloto);
        numElementos++;

        cout << "  → Hash: ID=" << piloto->id
             << " → Índice [" << indice << "]" << endl;
    }

    // Busca un piloto por ID
    Piloto *buscar(string id)
    {
        int indice = Clave(id);
        return tabla[indice].buscar(id);
    }

    // Elimina un piloto por ID
    bool eliminar(string id)
    {
        int indice = Clave(id);

        if (tabla[indice].eliminar(id))
        {
            numElementos--;
            cout << "  Piloto eliminado de Tabla Hash: " << id << endl;
            return true;
        }

        cout << "  Piloto no encontrado en Tabla Hash: " << id << endl;
        return false;
    }

    // Verifica si la tabla está vacía
    bool estaVacia()
    {
        return numElementos == 0;
    }

    // Obtiene el número de elementos
    int getNumElementos()
    {
        return numElementos;
    }

    // VISUALIZACIÓN

    // Imprime la tabla completa en consola
    void imprimirTabla()
    {

        cout << "        TABLA HASH DE PILOTOS              " << endl;
        cout << "  Tamaño: " << tamTabla << " | Elementos: " << numElementos << endl;
        cout << "  Función: h(ID) = Σ(ASCII) % 19" << endl;

        for (int i = 0; i < tamTabla; i++)
        {
            cout << "  [" << i << "] -> ";
            tabla[i].visualizarLista();
            cout << endl;
        }
    }

    // REPORTE GRAPHVIZ

    void generarReporte()
    {
        ofstream archivo;
        archivo.open("grafica_tabla_hash.dot", ios::out);

        if (!archivo.is_open())
        {
            cout << "  Error al crear archivo DOT" << endl;
            return;
        }

        archivo << "digraph TablaHash {" << endl;
        archivo << "    rankdir=LR;" << endl;
        archivo << "    node [shape=record];" << endl;
        archivo << endl;

        // Crear nodo de la tabla (índices)
        archivo << "    tabla [label=\"";
        for (int i = 0; i < tamTabla; i++)
        {
            archivo << "<f" << i << "> " << i;
            if (i < tamTabla - 1)
            {
                archivo << " | ";
            }
        }
        archivo << "\", shape=record, style=filled, fillcolor=lightgray];" << endl;
        archivo << endl;

        // Crear nodos de las listas y conexiones
        for (int i = 0; i < tamTabla; i++)
        {
            if (!tabla[i].estaVacia())
            {
                NodoHash *actual = tabla[i].getPrimero();
                int contador = 0;

                // Conectar desde la tabla al primer nodo
                archivo << "    tabla:f" << i << " -> piloto_" << i << "_" << contador
                        << " [color=blue];" << endl;

                // Crear nodos de la lista
                while (actual != nullptr)
                {
                    archivo << "    piloto_" << i << "_" << contador
                            << " [label=\"{ID: " << actual->dato->id << " | "
                            << actual->dato->nombre << " | Horas: "
                            << actual->dato->horasVuelo << "}\", "
                            << "style=filled, fillcolor=lightblue];" << endl;

                    // Conectar con el siguiente
                    if (actual->siguiente != nullptr)
                    {
                        archivo << "    piloto_" << i << "_" << contador
                                << " -> piloto_" << i << "_" << (contador + 1)
                                << " [color=green];" << endl;
                    }

                    actual = actual->siguiente;
                    contador++;
                }

                archivo << endl;
            }
        }

        archivo << "}" << endl;
        archivo.close();

        // Generar imagen con Graphviz
        system("dot -Tpng grafica_tabla_hash.dot -o grafica_tabla_hash.png");

// Abrir imagen automáticamente
#ifdef _WIN32
        system("start grafica_tabla_hash.png");
#endif

        cout << "  Reporte generado: grafica_tabla_hash.png" << endl;
    }

    void mostrarEstadisticas()
    {

        cout << "     DATOS DE LA TABLA HASH          " << endl;

        int espaciosOcupados = 0;
        int maxColisiones = 0;

        for (int i = 0; i < tamTabla; i++)
        {
            int tam = tabla[i].getTamanio();
            if (tam > 0)
            {
                espaciosOcupados++;
                if (tam > maxColisiones)
                {
                    maxColisiones = tam;
                }
            }
        }

        float factorCarga = (float)numElementos / tamTabla;

        cout << "  Total de pilotos: " << numElementos << endl;
        cout << "  Espacios ocupados: " << espaciosOcupados << "/" << tamTabla << endl;
        cout << "  Factor de carga: " << factorCarga << endl;
        cout << "  Maxima colision: " << maxColisiones << " pilotos en un indice" << endl;
    }
};

#endif