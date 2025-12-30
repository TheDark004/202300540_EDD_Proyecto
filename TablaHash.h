#ifndef TABLA_HASH_SIMPLE_H
#define TABLA_HASH_SIMPLE_H

#include <iostream>
#include <fstream>
#include <sstream>
#include "estructuras.h"

using namespace std;

// NODO SIMPLE PARA LA LISTA DE COLISIONES
class NodoHashSimple
{
public:
    string id;
    string nombre;
    int horasVuelo;
    NodoHashSimple *siguiente;

    NodoHashSimple(string i, string n, int h)
    {
        id = i;
        nombre = n;
        horasVuelo = h;
        siguiente = nullptr;
    }
};

// LISTA SIMPLE PARA COLISIONES
class ListaHashSimple
{
public:
    NodoHashSimple *primero;

    ListaHashSimple()
    {
        primero = nullptr;
    }

    void insertar(string id, string nombre, int horas)
    {
        NodoHashSimple *nuevo = new NodoHashSimple(id, nombre, horas);

        if (primero == nullptr)
        {
            primero = nuevo;
        }
        else
        {
            nuevo->siguiente = primero;
            primero = nuevo;
        }
    }

    bool eliminar(string id)
    {
        if (primero == nullptr)
            return false;

        if (primero->id == id)
        {
            NodoHashSimple *temp = primero;
            primero = primero->siguiente;
            delete temp;
            return true;
        }

        NodoHashSimple *actual = primero;
        while (actual->siguiente != nullptr)
        {
            if (actual->siguiente->id == id)
            {
                NodoHashSimple *temp = actual->siguiente;
                actual->siguiente = temp->siguiente;
                delete temp;
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }

    // Buscar y devolver el nodo completo
    NodoHashSimple *buscarNodo(string id)
    {
        NodoHashSimple *actual = primero;
        while (actual != nullptr)
        {
            if (actual->id == id)
                return actual;
            actual = actual->siguiente;
        }
        return nullptr;
    }

    bool existe(string id)
    {
        return (buscarNodo(id) != nullptr);
    }

    bool estaVacia()
    {
        return primero == nullptr;
    }

    ~ListaHashSimple()
    {
        while (primero != nullptr)
        {
            NodoHashSimple *temp = primero;
            primero = primero->siguiente;
            delete temp;
        }
    }
};

// TABLA HASH SIMPLE
class TablaHash
{
private:
    int tamTabla;
    int numElementos;
    ListaHashSimple *tabla;

    int convertirIDaNumero(string id)
    {
        int suma = 0;
        for (int i = 0; i < id.length(); i++)
        {
            suma += (int)id[i];
        }
        return suma;
    }

public:
    TablaHash()
    {
        tamTabla = 19;
        numElementos = 0;
        tabla = new ListaHashSimple[tamTabla];
        cout << " Tabla Hash creada (Tamaño: 19)" << endl;
    }

    ~TablaHash()
    {
        delete[] tabla;
        cout << " Tabla Hash destruida" << endl;
    }

    int Clave(string id)
    {
        int valor = convertirIDaNumero(id);
        return valor % tamTabla;
    }

    void insertar(Piloto *piloto)
    {
        if (piloto == nullptr || piloto->id.empty())
            return;

        int indice = Clave(piloto->id);

        if (tabla[indice].existe(piloto->id))
        {
            cout << "   Piloto ya existe en Hash: " << piloto->id << endl;
            return;
        }

        tabla[indice].insertar(piloto->id, piloto->nombre, piloto->horasVuelo);
        numElementos++;
        cout << "  → Hash: ID=" << piloto->id << " → Índice [" << indice << "]" << endl;
    }

    // BUSCAR CORREGIDO: Devuelve piloto completo
    Piloto *buscar(string id)
    {
        if (id.empty())
            return nullptr;

        int indice = Clave(id);
        NodoHashSimple *nodo = tabla[indice].buscarNodo(id);

        if (nodo != nullptr)
        {
            // Crear piloto con TODOS los datos
            Piloto *temp = new Piloto();
            temp->id = nodo->id;
            temp->nombre = nodo->nombre;
            temp->horasVuelo = nodo->horasVuelo;

            return temp;
        }
        return nullptr;
    }

    // ELIMINAR mejorado
    bool eliminar(string id)
    {
        if (id.empty())
            return false;

        int indice = Clave(id);

        // Verificar si existe antes de intentar eliminar
        if (!tabla[indice].existe(id))
        {
            cout << "   Piloto no encontrado en Hash: " << id << endl;
            return false;
        }

        if (tabla[indice].eliminar(id))
        {
            numElementos--;
            cout << "   Piloto eliminado de Tabla Hash: " << id << endl;
            return true;
        }

        cout << "   Error al eliminar de Hash: " << id << endl;
        return false;
    }

    bool estaVacia()
    {
        return numElementos == 0;
    }

    int getNumElementos()
    {
        return numElementos;
    }

    void imprimirTabla()
    {
        cout << "\n"
             << endl;
        cout << "         TABLA HASH DE PILOTOS              " << endl;
        cout << "" << endl;
        cout << "  Tamaño: " << tamTabla << " | Elementos: " << numElementos << endl;
        cout << "────────────────────────────────────────────" << endl;

        for (int i = 0; i < tamTabla; i++)
        {
            cout << "  [" << i << "] -> ";
            if (tabla[i].estaVacia())
            {
                cout << "Vacío";
            }
            else
            {
                NodoHashSimple *actual = tabla[i].primero;
                while (actual != nullptr)
                {
                    cout << "[" << actual->id << ": " << actual->nombre << "]";
                    if (actual->siguiente != nullptr)
                        cout << " -> ";
                    actual = actual->siguiente;
                }
            }
            cout << endl;
        }
        cout << "════════════════════════════════════════════\n"
             << endl;
    }

    void generarReporte()
    {
        ofstream archivo("grafica_tabla_hash.dot");

        archivo << "digraph TablaHash {" << endl;
        archivo << "    rankdir=TB;" << endl;
        archivo << "    node [shape=box, style=filled, fontname=\"Arial\"];" << endl;
        archivo << "    edge [arrowhead=vee];" << endl;
        archivo << "    graph [nodesep=0.3, ranksep=0.5];" << endl;
        archivo << endl;

        // Título
        archivo << "    titulo [label=\"TABLA HASH\\nTamaño: 19\\nPilotos por ID\", ";
        archivo << "shape=plaintext, fontsize=16];" << endl;
        archivo << endl;

        // Crear ÍNDICES del 0 al 18 en ORDEN
        archivo << "    // ===== ÍNDICES (0-18) ===== " << endl;

        // Primero, crear todos los índices en orden
        for (int i = 0; i < tamTabla; i++)
        {
            archivo << "    idx" << i << " [label=\"Índice " << i << "\", ";
            archivo << "fillcolor=lightyellow, width=1.2, height=0.5];" << endl;
        }

        archivo << endl;

        // Ahora, crear ELEMENTOS debajo de cada índice
        archivo << "    // ===== ELEMENTOS ===== " << endl;

        for (int i = 0; i < tamTabla; i++)
        {
            if (!tabla[i].estaVacia())
            {
                NodoHashSimple *actual = tabla[i].primero;
                int cont = 0;
                string anterior = "";

                while (actual != nullptr)
                {
                    string nodoId = "elem_" + to_string(i) + "_" + to_string(cont);

                    // Crear nodo con solo el ID
                    archivo << "    " << nodoId << " [label=\"" << actual->id << "\", ";
                    archivo << "fillcolor=lightblue, width=1.0];" << endl;

                    // Conectar con índice (si es el primero)
                    if (cont == 0)
                    {
                        archivo << "    idx" << i << " -> " << nodoId << ";" << endl;
                    }
                    // Conectar con elemento anterior (si hay)
                    else if (!anterior.empty())
                    {
                        archivo << "    " << anterior << " -> " << nodoId << ";" << endl;
                    }

                    anterior = nodoId;
                    actual = actual->siguiente;
                    cont++;
                }

                // Mostrar cuántos elementos tiene este índice
                archivo << "    // Índice " << i << " tiene " << cont << " elemento(s)" << endl;
            }
            else
            {
                // Índice vacío
                archivo << "    vacio" << i << " [label=\"Vacío\", ";
                archivo << "fillcolor=white, style=dashed, width=0.8];" << endl;
                archivo << "    idx" << i << " -> vacio" << i << " [style=dashed, color=gray];" << endl;
            }

            archivo << endl;
        }

        // ORDEN VERTICAL 
        archivo << "    // ORDENAR ÍNDICES " << endl;

        // Agrupar todos los índices al mismo nivel
        archivo << "    { rank=same; titulo; }" << endl;

        // Índices en el siguiente nivel, en orden
        archivo << "    { rank=same; ";
        for (int i = 0; i < tamTabla; i++)
        {
            archivo << "idx" << i;
            if (i < tamTabla - 1)
                archivo << "; ";
        }
        archivo << " }" << endl;

        // Forzar orden con conexiones invisibles
        for (int i = 0; i < tamTabla - 1; i++)
        {
            archivo << "    idx" << i << " -> idx" << (i + 1) << " [style=invis, weight=100];" << endl;
        }

        archivo << "}" << endl;
        archivo.close();

        cout << "   Reporte de tabla hash generado" << endl;
        cout << "  Total índices: " << tamTabla << endl;
        cout << "  Elementos cargados: " << numElementos << endl;

        //  Generar PNG
        cout << "  Generando imagen PNG..." << endl;
        int genResult = system("dot -Tpng grafica_tabla_hash.dot -o grafica_tabla_hash.png");

        if (genResult != 0)
        {
            cout << "  Error al generar la imagen" << endl;
            return;
        }

        //  Verificar que el archivo existe
        ifstream testPNG("grafica_tabla_hash.png", ios::binary);
        if (!testPNG.is_open())
        {
            cout << "   El archivo PNG no se creó" << endl;
            return;
        }
        testPNG.close();

        //  ABRIR LA IMAGEN - MÉTODO QUE SIEMPRE FUNCIONA EN WINDOWS
        cout << "  Abriendo imagen..." << endl;

        //  Usar 'explorer' que siempre funciona
        int openResult = system("explorer grafica_tabla_hash.png");

        if (openResult == 0)
        {
            cout << "  Imagen abierta correctamente" << endl;
        }
        else
        {
            // Si explorer falla, usar PowerShell
            system("powershell -Command \"& {Start-Process 'grafica_tabla_hash.png'}\"");
            cout << "  Imagen enviada a abrir" << endl;
        }
    }
};

#endif