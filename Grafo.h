#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <vector>
#include <queue>
#include "estructuras.h"

using namespace std;

/*
   - Implementado con LISTA DE ADYACENCIA
   - Cada nodo es una ciudad
   - Cada arista tiene origen, destino y distancia
   - Incluye algoritmo de DIJKSTRA para ruta más corta
*/

class Grafo
{
private:
    NodoGrafo *primero; // Primer vértice de la lista
    int numVertices;

    // MÉTODOS PRIVADOS AUXILIARES

    // Busca un vértice (ciudad) en el grafo
    NodoGrafo *buscarVertice(string ciudad)
    {
        NodoGrafo *actual = primero;
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

    // Crea un nuevo vértice si no existe
    void agregarVertice(string ciudad)
    {
        // Verificar si ya existe
        if (buscarVertice(ciudad) != nullptr)
        {
            return; // Ya existe
        }

        // Crear nuevo nodo
        NodoGrafo *nuevo = new NodoGrafo(ciudad);

        // Insertar al inicio
        if (primero == nullptr)
        {
            primero = nuevo;
        }
        else
        {
            nuevo->siguiente = primero;
            primero = nuevo;
        }

        numVertices++;
    }

    // Agrega una arista dirigida de origen a destino
    void agregarAristaInterna(string origen, string destino, int distancia)
    {
        // Buscar el nodo origen
        NodoGrafo *nodoOrigen = buscarVertice(origen);

        if (nodoOrigen == nullptr)
        {
            cout << " Error: Ciudad origen no existe: " << origen << endl;
            return;
        }

        // Crear nueva arista
        Arista *nuevaArista = new Arista(destino, distancia);

        // Insertar al inicio de la lista de adyacencia
        if (nodoOrigen->adyacentes == nullptr)
        {
            nodoOrigen->adyacentes = nuevaArista;
        }
        else
        {
            nuevaArista->siguiente = nodoOrigen->adyacentes;
            nodoOrigen->adyacentes = nuevaArista;
        }
    }

    // Destruye todas las aristas de un vértice
    void destruirAristas(Arista *arista)
    {
        while (arista != nullptr)
        {
            Arista *siguiente = arista->siguiente;
            delete arista;
            arista = siguiente;
        }
    }

    // Genera el código DOT recursivamente
    void generarDOTRecursivo(ofstream &archivo)
    {
        NodoGrafo *actual = primero;

        while (actual != nullptr)
        {
            // Crear nodo
            archivo << "    \"" << actual->ciudad << "\" [style=filled, fillcolor=lightblue];" << endl;

            // Crear aristas
            Arista *arista = actual->adyacentes;
            while (arista != nullptr)
            {
                archivo << "    \"" << actual->ciudad << "\" -> \"" << arista->destino
                        << "\" [label=\"" << arista->distancia << " km\"];" << endl;
                arista = arista->siguiente;
            }

            actual = actual->siguiente;
        }
    }

public:
    // CONSTRUCTOR Y DESTRUCTOR

    Grafo()
    {
        primero = nullptr;
        numVertices = 0;
        cout << "Grafo de rutas creado" << endl;
    }

    ~Grafo()
    {
        NodoGrafo *actual = primero;

        while (actual != nullptr)
        {
            NodoGrafo *siguiente = actual->siguiente;

            // Destruir todas las aristas del nodo
            destruirAristas(actual->adyacentes);

            // Destruir el nodo
            delete actual;
            actual = siguiente;
        }

        cout << "Grafo destruido " << endl;
    }

    // MÉTODOS PÚBLICOS

    bool estaVacio()
    {
        return primero == nullptr;
    }

    int getNumVertices()
    {
        return numVertices;
    }

    // Agrega una ruta dirigida
    void agregarRuta(string origen, string destino, int distancia)
    {
        // Crear vértices si no existen
        agregarVertice(origen);
        agregarVertice(destino);

        // Agregar la arista
        agregarAristaInterna(origen, destino, distancia);
    }

    // Verifica si existe una ruta directa
    bool existeRuta(string origen, string destino)
    {
        NodoGrafo *nodoOrigen = buscarVertice(origen);
        if (nodoOrigen == nullptr)
        {
            return false;
        }

        Arista *arista = nodoOrigen->adyacentes;
        while (arista != nullptr)
        {
            if (arista->destino == destino)
            {
                return true;
            }
            arista = arista->siguiente;
        }

        return false;
    }

    // Muestra todas las rutas del grafo
    void mostrarRutas()
    {

        cout << "          RUTAS DEL GRAFO                   " << endl;
        cout << "  Total de ciudades: " << numVertices << endl;

        if (estaVacio())
        {
            cout << "  El grafo está vacío" << endl;
            return;
        }

        NodoGrafo *actual = primero;
        int totalRutas = 0;

        while (actual != nullptr)
        {
            cout << "\n  📍 " << actual->ciudad << ":" << endl;

            Arista *arista = actual->adyacentes;
            if (arista == nullptr)
            {
                cout << "    (Sin rutas salientes)" << endl;
            }
            else
            {
                while (arista != nullptr)
                {
                    cout << "     -> " << arista->destino
                         << " (" << arista->distancia << " km)" << endl;
                    arista = arista->siguiente;
                    totalRutas++;
                }
            }

            actual = actual->siguiente;
        }

        cout << "\n  Total de rutas: " << totalRutas << endl;
    }

    // ALGORITMO - RUTA MÁS CORTA

    void rutaMasCorta(string origen, string destino)
    {
        ;
        cout << "      CÁLCULO DE RUTA MÁS CORTA             " << endl;
        cout << "  Origen: " << origen << endl;
        cout << "  Destino: " << destino << endl;

        // Verificar que existan ambas ciudades
        if (buscarVertice(origen) == nullptr)
        {
            cout << "  Ciudad origen no existe: " << origen << endl;
            return;
        }
        if (buscarVertice(destino) == nullptr)
        {
            cout << "  Ciudad destino no existe: " << destino << endl;
            return;
        }

        // Estructuras para Dijkstra
        const int INF = numeric_limits<int>::max();
        vector<string> ciudades;
        vector<int> distancias;
        vector<string> previos;
        vector<bool> visitados;

        // Recopilar todas las ciudades
        NodoGrafo *actual = primero;
        while (actual != nullptr)
        {
            ciudades.push_back(actual->ciudad);
            distancias.push_back(INF);
            previos.push_back("");
            visitados.push_back(false);
            actual = actual->siguiente;
        }

        // Encontrar índice de origen
        int indiceOrigen = -1;
        for (size_t i = 0; i < ciudades.size(); i++)
        {
            if (ciudades[i] == origen)
            {
                indiceOrigen = i;
                break;
            }
        }

        // Inicializar distancia del origen
        distancias[indiceOrigen] = 0;

        // Algoritmo de Dijkstra
        for (size_t count = 0; count < ciudades.size(); count++)
        {
            // Encontrar el vértice no visitado con menor distancia
            int minDist = INF;
            int indiceMin = -1;

            for (size_t i = 0; i < ciudades.size(); i++)
            {
                if (!visitados[i] && distancias[i] < minDist)
                {
                    minDist = distancias[i];
                    indiceMin = i;
                }
            }

            if (indiceMin == -1)
            {
                break; // No hay más vértices alcanzables
            }

            // Marcar como visitado
            visitados[indiceMin] = true;
            string ciudadActual = ciudades[indiceMin];

            // Si llegamos al destino, podemos terminar
            if (ciudadActual == destino)
            {
                break;
            }

            // Actualizar distancias de vecinos
            NodoGrafo *nodo = buscarVertice(ciudadActual);
            Arista *arista = nodo->adyacentes;

            while (arista != nullptr)
            {
                // Encontrar índice del destino de la arista
                int indiceVecino = -1;
                for (size_t i = 0; i < ciudades.size(); i++)
                {
                    if (ciudades[i] == arista->destino)
                    {
                        indiceVecino = i;
                        break;
                    }
                }

                if (indiceVecino != -1 && !visitados[indiceVecino])
                {
                    int nuevaDistancia = distancias[indiceMin] + arista->distancia;

                    if (nuevaDistancia < distancias[indiceVecino])
                    {
                        distancias[indiceVecino] = nuevaDistancia;
                        previos[indiceVecino] = ciudadActual;
                    }
                }

                arista = arista->siguiente;
            }
        }

        // Encontrar índice de destino
        int indiceDestino = -1;
        for (size_t i = 0; i < ciudades.size(); i++)
        {
            if (ciudades[i] == destino)
            {
                indiceDestino = i;
                break;
            }
        }

        // Mostrar resultado
        if (distancias[indiceDestino] == INF)
        {
            cout << "\n   NO existe ruta entre " << origen << " y " << destino << endl;
            return;
        }

        // Reconstruir el camino
        vector<string> camino;
        string actual_ciudad = destino;

        while (actual_ciudad != "")
        {
            camino.insert(camino.begin(), actual_ciudad);

            // Encontrar índice de actual_ciudad
            int idx = -1;
            for (size_t i = 0; i < ciudades.size(); i++)
            {
                if (ciudades[i] == actual_ciudad)
                {
                    idx = i;
                    break;
                }
            }

            actual_ciudad = previos[idx];
        }

        // Mostrar la ruta
        cout << "\n  Ruta encontrada:" << endl;
        cout << "  ";
        for (size_t i = 0; i < camino.size(); i++)
        {
            cout << camino[i];
            if (i < camino.size() - 1)
            {
                cout << " → ";
            }
        }
        cout << endl;

        cout << "\n  Distancia total: " << distancias[indiceDestino] << " km" << endl;
    }

    // GRAPHVIZ

    void generarReporte()
    {
        if (estaVacio())
        {
            cout << "  El grafo está vacío" << endl;
            return;
        }

        ofstream archivo;
        archivo.open("grafica_grafo_rutas.dot", ios::out);

        if (!archivo.is_open())
        {
            cout << "  Error al crear archivo DOT" << endl;
            return;
        }

        archivo << "digraph GrafoRutas {" << endl;
        archivo << "    rankdir=LR;" << endl;
        archivo << "    node [shape=circle, style=filled];" << endl;
        archivo << "    edge [color=blue, fontcolor=red];" << endl;
        archivo << endl;

        generarDOTRecursivo(archivo);

        archivo << "}" << endl;
        archivo.close();

        // Generar imagen con Graphviz
        system("dot -Tpng grafica_grafo_rutas.dot -o grafica_grafo_rutas.png");

// Abrir imagen automáticamente
#ifdef _WIN32
        system("start grafica_grafo_rutas.png");
#endif

        cout << "  Reporte generado: grafica_grafo_rutas.png" << endl;
    }

    // CARGAR DESDE ARCHIVO

    bool cargarDesdeArchivo(string nombreArchivo)
    {

        cout << "    CARGANDO RUTAS DESDE ARCHIVO" << endl;

        ifstream archivo(nombreArchivo);

        if (!archivo.is_open())
        {
            cout << " Error: No se pudo abrir " << nombreArchivo << endl;
            return false;
        }

        cout << " Archivo abierto: " << nombreArchivo << endl;
        cout << "  Procesando rutas..." << endl;

        string linea;
        int contador = 0;

        // Formato: Origen/Destino/Distancia;
        while (getline(archivo, linea))
        {
            // Eliminar espacios en blanco y saltos de línea de forma simple
            string lineaLimpia = "";
            for (int i = 0; i < linea.length(); i++)
            {
                char c = linea[i];
                if (c != ' ' && c != '\r' && c != '\n')
                {
                    lineaLimpia += c;
                }
            }
            linea = lineaLimpia;

            // Ignorar líneas vacías
            if (linea.empty())
            {
                continue;
            }

            // Parsear la línea
            stringstream ss(linea);
            string origen, destino, distanciaStr;

            if (getline(ss, origen, '/') &&
                getline(ss, destino, '/') &&
                getline(ss, distanciaStr, ';'))
            {
                try
                {
                    int distancia = stoi(distanciaStr);
                    agregarRuta(origen, destino, distancia);
                    cout << "  -> " << origen << " -> " << destino << " (" << distancia << " km)" << endl;
                    contador++;
                }
                catch (...)
                {
                    cout << "  Error en línea: " << linea << endl;
                }
            }
        }

        archivo.close();

        cout << "\n Total de rutas cargadas: " << contador << endl;
        cout << "  Total de ciudades: " << numVertices << endl;

        return true;
    }
};

#endif