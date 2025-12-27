#ifndef ARBOL_B_H
#define ARBOL_B_H

#include "estructuras.h"
#include <fstream>

/*
   - Se usa para almacenar aviones DISPONIBLES
   - La llave es el número de registro del avión
   - Se mantiene balanceado automáticamente
*/

class ArbolB
{
private:
    NodoB *raiz;

    // Divide un nodo hijo lleno (con 4 claves) en dos nodos
    void dividirHijo(NodoB *padre, int indice)
    {
        NodoB *nodoLleno = padre->hijos[indice];
        NodoB *nuevoNodo = new NodoB(nodoLleno->esHoja);

        // se divide en 2 nodos de 2 claves -  La clave del medio sube al padre
        nuevoNodo->numClaves = 2; // Mitad derecha tendrá 2 claves

        // Copiar las últimas 2 claves al nuevo nodo
        for (int i = 0; i < 2; i++)
        {
            nuevoNodo->claves[i] = nodoLleno->claves[i + 2];
            nuevoNodo->aviones[i] = nodoLleno->aviones[i + 2];
        }

        // Si no es hoja, copiar también los últimos hijos
        if (!nodoLleno->esHoja)
        {
            for (int i = 0; i < 3; i++)
            {
                nuevoNodo->hijos[i] = nodoLleno->hijos[i + 2];
            }
        }

        nodoLleno->numClaves = 2; // El nodo original queda con 2 claves

        // Mover los hijos del padre para hacer espacio
        for (int i = padre->numClaves; i > indice; i--)
        {
            padre->hijos[i + 1] = padre->hijos[i];
        }

        // Conectar el nuevo nodo como hijo del padre
        padre->hijos[indice + 1] = nuevoNodo;

        // Mover las claves del padre para hacer espacio
        for (int i = padre->numClaves - 1; i >= indice; i--)
        {
            padre->claves[i + 1] = padre->claves[i];
            padre->aviones[i + 1] = padre->aviones[i];
        }

        // Subir la clave del medio (índice 1) al padre
        padre->claves[indice] = nodoLleno->claves[1];
        padre->aviones[indice] = nodoLleno->aviones[1];
        padre->numClaves++;
    }

    // Inserta en un nodo que NO está lleno
    void insertarNoLleno(NodoB *nodo, string numeroRegistro, Avion *avion)
    {
        int i = nodo->numClaves - 1;

        if (nodo->esHoja)
        {
            // Inserción en hoja: desplazar claves mayores a la derecha
            while (i >= 0 && numeroRegistro < nodo->claves[i])
            {
                nodo->claves[i + 1] = nodo->claves[i];
                nodo->aviones[i + 1] = nodo->aviones[i];
                i--;
            }

            // Insertar la nueva clave
            nodo->claves[i + 1] = numeroRegistro;
            nodo->aviones[i + 1] = avion;
            nodo->numClaves++;
        }
        else
        {
            // Inserción en nodo interno: encontrar el hijo correcto
            while (i >= 0 && numeroRegistro < nodo->claves[i])
            {
                i--;
            }
            i++; // Ahora i es el índice del hijo donde debe ir

            // Si el hijo está lleno, dividirlo primero
            if (nodo->hijos[i]->numClaves == NodoB::MAX_CLAVES)
            {
                dividirHijo(nodo, i);

                // Después de dividir decidir en cuál de los dos nodos insertar
                if (numeroRegistro > nodo->claves[i])
                {
                    i++;
                }
            }

            insertarNoLleno(nodo->hijos[i], numeroRegistro, avion);
        }
    }

    // Busca un avión recursivamente
    Avion *buscarRecursivo(NodoB *nodo, string numeroRegistro)
    {
        if (nodo == nullptr)
        {
            return nullptr;
        }

        int i = 0;

        // Buscar la posición donde debería estar la clave
        while (i < nodo->numClaves && numeroRegistro > nodo->claves[i])
        {
            i++;
        }

        // Si se encontró la clave
        if (i < nodo->numClaves && numeroRegistro == nodo->claves[i])
        {
            return nodo->aviones[i];
        }

        // Si es hoja y no se encontró, no existe
        if (nodo->esHoja)
        {
            return nullptr;
        }

        // Buscar en el hijo correspondiente
        return buscarRecursivo(nodo->hijos[i], numeroRegistro);
    }

    // Elimina un avión del árbol
    void eliminarDeNodo(NodoB *nodo, string numeroRegistro)
    {
        int i = 0;
        while (i < nodo->numClaves && numeroRegistro > nodo->claves[i])
        {
            i++;
        }

        if (i < nodo->numClaves && numeroRegistro == nodo->claves[i])
        {
            // Clave encontrada en este nodo
            if (nodo->esHoja)
            {
                eliminarDeHoja(nodo, i);
            }
            else
            {
                eliminarDeNoInterno(nodo, i);
            }
        }
        else
        {
            // La clave no está en este nodo
            if (nodo->esHoja)
            {
                return; // No existe
            }

            bool estaEnUltimoHijo = (i == nodo->numClaves);

            if (nodo->hijos[i]->numClaves < 2)
            {
                llenarHijo(nodo, i);
            }

            if (estaEnUltimoHijo && i > nodo->numClaves)
            {
                eliminarDeNodo(nodo->hijos[i - 1], numeroRegistro);
            }
            else
            {
                eliminarDeNodo(nodo->hijos[i], numeroRegistro);
            }
        }
    }

    // Elimina una clave de un nodo hoja
    void eliminarDeHoja(NodoB *nodo, int idx)
    {
        // Desplazar todas las claves después de idx una posición a la izquierda
        for (int i = idx + 1; i < nodo->numClaves; i++)
        {
            nodo->claves[i - 1] = nodo->claves[i];
            nodo->aviones[i - 1] = nodo->aviones[i];
        }
        nodo->numClaves--;
    }

    // Elimina una clave de un nodo interno
    void eliminarDeNoInterno(NodoB *nodo, int idx)
    {
        string k = nodo->claves[idx];

        if (nodo->hijos[idx]->numClaves >= 2)
        {
            // Obtener predecesor
            auto pred = obtenerPredecesor(nodo, idx);
            nodo->claves[idx] = pred.first;
            nodo->aviones[idx] = pred.second;
            eliminarDeNodo(nodo->hijos[idx], pred.first);
        }
        else if (nodo->hijos[idx + 1]->numClaves >= 2)
        {
            // Obtener sucesor
            auto suc = obtenerSucesor(nodo, idx);
            nodo->claves[idx] = suc.first;
            nodo->aviones[idx] = suc.second;
            eliminarDeNodo(nodo->hijos[idx + 1], suc.first);
        }
        else
        {
            // Fusionar con hermano
            fusionar(nodo, idx);
            eliminarDeNodo(nodo->hijos[idx], k);
        }
    }

    // Obtiene el predecesor (máximo del subárbol izquierdo)
    pair<string, Avion *> obtenerPredecesor(NodoB *nodo, int idx)
    {
        NodoB *actual = nodo->hijos[idx];
        while (!actual->esHoja)
        {
            actual = actual->hijos[actual->numClaves];
        }
        return make_pair(actual->claves[actual->numClaves - 1],
                         actual->aviones[actual->numClaves - 1]);
    }

    // Obtiene el sucesor (mínimo del subárbol derecho)
    pair<string, Avion *> obtenerSucesor(NodoB *nodo, int idx)
    {
        NodoB *actual = nodo->hijos[idx + 1];
        while (!actual->esHoja)
        {
            actual = actual->hijos[0];
        }
        return make_pair(actual->claves[0], actual->aviones[0]);
    }

    // Llena un hijo que tiene menos claves de las requeridas
    void llenarHijo(NodoB *nodo, int idx)
    {
        // Si el hermano anterior tiene claves extra
        if (idx != 0 && nodo->hijos[idx - 1]->numClaves >= 2)
        {
            tomarDelAnterior(nodo, idx);
        }
        // Si el hermano siguiente tiene claves extra
        else if (idx != nodo->numClaves && nodo->hijos[idx + 1]->numClaves >= 2)
        {
            tomarDelSiguiente(nodo, idx);
        }
        // Fusionar con hermano
        else
        {
            if (idx != nodo->numClaves)
            {
                fusionar(nodo, idx);
            }
            else
            {
                fusionar(nodo, idx - 1);
            }
        }
    }

    // Toma una clave del hermano anterior
    void tomarDelAnterior(NodoB *nodo, int idx)
    {
        NodoB *hijo = nodo->hijos[idx];
        NodoB *hermano = nodo->hijos[idx - 1];

        // Mover todas las claves del hijo una posición adelante
        for (int i = hijo->numClaves - 1; i >= 0; i--)
        {
            hijo->claves[i + 1] = hijo->claves[i];
            hijo->aviones[i + 1] = hijo->aviones[i];
        }

        if (!hijo->esHoja)
        {
            for (int i = hijo->numClaves; i >= 0; i--)
            {
                hijo->hijos[i + 1] = hijo->hijos[i];
            }
        }

        // Bajar clave del padre
        hijo->claves[0] = nodo->claves[idx - 1];
        hijo->aviones[0] = nodo->aviones[idx - 1];

        if (!hijo->esHoja)
        {
            hijo->hijos[0] = hermano->hijos[hermano->numClaves];
        }

        // Subir clave del hermano al padre
        nodo->claves[idx - 1] = hermano->claves[hermano->numClaves - 1];
        nodo->aviones[idx - 1] = hermano->aviones[hermano->numClaves - 1];

        hijo->numClaves++;
        hermano->numClaves--;
    }

    // Toma una clave del hermano siguiente
    void tomarDelSiguiente(NodoB *nodo, int idx)
    {
        NodoB *hijo = nodo->hijos[idx];
        NodoB *hermano = nodo->hijos[idx + 1];

        // Bajar clave del padre
        hijo->claves[hijo->numClaves] = nodo->claves[idx];
        hijo->aviones[hijo->numClaves] = nodo->aviones[idx];

        if (!hijo->esHoja)
        {
            hijo->hijos[hijo->numClaves + 1] = hermano->hijos[0];
        }

        // Subir primera clave del hermano al padre
        nodo->claves[idx] = hermano->claves[0];
        nodo->aviones[idx] = hermano->aviones[0];

        // Mover claves del hermano una posición atrás
        for (int i = 1; i < hermano->numClaves; i++)
        {
            hermano->claves[i - 1] = hermano->claves[i];
            hermano->aviones[i - 1] = hermano->aviones[i];
        }

        if (!hermano->esHoja)
        {
            for (int i = 1; i <= hermano->numClaves; i++)
            {
                hermano->hijos[i - 1] = hermano->hijos[i];
            }
        }

        hijo->numClaves++;
        hermano->numClaves--;
    }

    // Fusiona un hijo con su hermano
    void fusionar(NodoB *nodo, int idx)
    {
        NodoB *hijo = nodo->hijos[idx];
        NodoB *hermano = nodo->hijos[idx + 1];

        // Bajar clave del padre
        hijo->claves[1] = nodo->claves[idx];
        hijo->aviones[1] = nodo->aviones[idx];

        // Copiar claves del hermano
        for (int i = 0; i < hermano->numClaves; i++)
        {
            hijo->claves[i + 2] = hermano->claves[i];
            hijo->aviones[i + 2] = hermano->aviones[i];
        }

        // Copiar punteros a hijos
        if (!hijo->esHoja)
        {
            for (int i = 0; i <= hermano->numClaves; i++)
            {
                hijo->hijos[i + 2] = hermano->hijos[i];
            }
        }

        // Mover claves del padre
        for (int i = idx + 1; i < nodo->numClaves; i++)
        {
            nodo->claves[i - 1] = nodo->claves[i];
            nodo->aviones[i - 1] = nodo->aviones[i];
        }

        // Mover punteros de hijos
        for (int i = idx + 2; i <= nodo->numClaves; i++)
        {
            nodo->hijos[i - 1] = nodo->hijos[i];
        }

        hijo->numClaves += hermano->numClaves + 1;
        nodo->numClaves--;

        delete hermano;
    }

    // Genera código DOT recursivamente
    void generarDOTRecursivo(NodoB *nodo, ofstream &archivo, int &contador)
    {
        if (nodo == nullptr)
            return;

        int idNodo = contador++;

        // Generar etiqueta del nodo
        archivo << "    nodo" << idNodo << " [label=\"";

        for (int i = 0; i < nodo->numClaves; i++)
        {
            if (i > 0)
                archivo << " | ";
            archivo << nodo->claves[i];
        }

        archivo << "\"];\n";

        // Generar conexiones con hijos
        if (!nodo->esHoja)
        {
            for (int i = 0; i <= nodo->numClaves; i++)
            {
                if (nodo->hijos[i] != nullptr)
                {
                    int idHijo = contador;
                    archivo << "    nodo" << idNodo << " -> nodo" << idHijo << ";\n";
                    generarDOTRecursivo(nodo->hijos[i], archivo, contador);
                }
            }
        }
    }

    // Destructor recursivo
    void destruirArbol(NodoB *nodo)
    {
        if (nodo == nullptr)
            return;

        if (!nodo->esHoja)
        {
            for (int i = 0; i <= nodo->numClaves; i++)
            {
                destruirArbol(nodo->hijos[i]);
            }
        }

        // Liberar aviones
        for (int i = 0; i < nodo->numClaves; i++)
        {
            delete nodo->aviones[i];
        }

        delete nodo;
    }

public:

    // CONSTRUCTOR Y DESTRUCTOR
    ArbolB()
    {
        raiz = nullptr;
        cout << "Árbol B de orden 5 creado" << endl;
    }

    ~ArbolB()
    {
        destruirArbol(raiz);
        cout << "Árbol B destruido" << endl;
    }

   
    // MÉTODOS PÚBLICOS
    
    // Inserta un avión en el árbol
    void insertar(Avion *avion)
    {
        if (avion == nullptr)
        {
            cout << "Error: Avión nulo" << endl;
            return;
        }

        string numeroRegistro = avion->numeroRegistro;

        // Verificar si ya existe
        if (buscar(numeroRegistro) != nullptr)
        {
            cout << "Error: El avión " << numeroRegistro << " ya existe" << endl;
            return;
        }

        // Si el árbol está vacío
        if (raiz == nullptr)
        {
            raiz = new NodoB(true);
            raiz->claves[0] = numeroRegistro;
            raiz->aviones[0] = avion;
            raiz->numClaves = 1;
            cout << "Avión insertado como raíz: " << numeroRegistro << endl;
            return;
        }

        // Si la raíz está llena, dividirla
        if (raiz->numClaves == NodoB::MAX_CLAVES)
        {
            NodoB *nuevaRaiz = new NodoB(false);
            nuevaRaiz->hijos[0] = raiz;
            dividirHijo(nuevaRaiz, 0);
            raiz = nuevaRaiz;
        }

        insertarNoLleno(raiz, numeroRegistro, avion);
        cout << "Avión insertado en Árbol B: " << numeroRegistro << endl;
    }

    // Busca un avión por número de registro
    Avion *buscar(string numeroRegistro)
    {
        return buscarRecursivo(raiz, numeroRegistro);
    }

    // Elimina un avión del árbol y lo retorna
    Avion *eliminar(string numeroRegistro)
    {
        if (raiz == nullptr)
        {
            cout << "El árbol está vacío" << endl;
            return nullptr;
        }

        Avion *avionEliminado = buscar(numeroRegistro);
        if (avionEliminado == nullptr)
        {
            cout << "Avión no encontrado: " << numeroRegistro << endl;
            return nullptr;
        }

        eliminarDeNodo(raiz, numeroRegistro);

        // Si la raíz quedó vacía
        if (raiz->numClaves == 0)
        {
            NodoB *tmp = raiz;
            if (raiz->esHoja)
            {
                raiz = nullptr;
            }
            else
            {
                raiz = raiz->hijos[0];
            }
            delete tmp;
        }

        cout << "Avión eliminado del Árbol B: " << numeroRegistro << endl;
        return avionEliminado;
    }

    bool estaVacio()
    {
        return raiz == nullptr;
    }

    // Genera archivo DOT para Graphviz
    string generarDOT()
    {
        string dot = "digraph ArbolB {\n";
        dot += "    node [shape=record];\n";
        dot += "    rankdir=TB;\n\n";

        if (raiz == nullptr)
        {
            dot += "    vacio [label=\"Árbol Vacío\" shape=box style=filled fillcolor=lightgray];\n";
        }
        else
        {
            ofstream temp("temp_arbolb.dot");
            temp << dot;
            int contador = 0;
            generarDOTRecursivo(raiz, temp, contador);
            temp.close();

            ifstream leer("temp_arbolb.dot");
            string linea;
            dot = "";
            while (getline(leer, linea))
            {
                dot += linea + "\n";
            }
            leer.close();
        }

        dot += "}\n";
        return dot;
    }
};

#endif