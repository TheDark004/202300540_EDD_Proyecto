#ifndef GESTOR_REPORTES_H
#define GESTOR_REPORTES_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#ifdef _WIN32
    #include <direct.h>
    #define mkdir_custom(path) _mkdir(path)
#endif

#include "ArbolB.h"
#include "ListaCircularDoble.h"
#include "ArbolBB.h"
#include "TablaHash.h"
#include "Grafo.h"
#include "MatrizD.h"

using namespace std;

class GestorReportes
{
private:
    static void crearCarpeta(string ruta)
    {
        mkdir_custom(ruta.c_str());
    }

    static void inicializarCarpetas()
    {
        crearCarpeta("reportes");
        crearCarpeta("reportes\\aviones");
        crearCarpeta("reportes\\pilotos");
        crearCarpeta("reportes\\rutas");
        crearCarpeta("reportes\\asignaciones");
    }
    
    static void abrirImagen(string rutaArchivo)
    {
        #ifdef _WIN32
            system(("start " + rutaArchivo).c_str());
        #endif
    }

public:
    
    // REPORTE ÁRBOL B 
    
    static void reporteArbolB(ArbolB* arbol)
    {
        
        cout << "  REPORTE: Árbol B - Aviones Disponibles" << endl;
        cout << "" << endl;
        
        if (arbol->estaVacio())
        {
            cout << " No hay aviones disponibles para reportar" << endl;
            return;
        }
        
        inicializarCarpetas();
        
        string archivoDOT = "reportes\\aviones\\arbol_b_disponibles.dot";
        string archivoPNG = "reportes\\aviones\\arbol_b_disponibles.png";
        
        cout << "Generando código DOT..." << endl;
        string codigoDOT = arbol->generarDOT();
        
        cout << "Guardando archivo .dot..." << endl;
        ofstream archivo(archivoDOT);
        
        if (!archivo.is_open())
        {
            cout << "Error al crear archivo .dot" << endl;
            return;
        }
        
        archivo << codigoDOT;
        archivo.close();
        
        cout << "Generando imagen PNG..." << endl;
        string comando = "dot -Tpng " + archivoDOT + " -o " + archivoPNG;
        
        if (system(comando.c_str()) != 0)
        {
            cout << " Error al ejecutar Graphviz" << endl;
            return;
        }
        
        cout << "Abriendo imagen..." << endl;
        abrirImagen(archivoPNG);
        
        cout << "\n Reporte generado exitosamente" << endl;
        cout << "  Ubicación: " << archivoPNG << endl;
    }
    
    
    // REPORTE LISTA CIRCULAR (AVIONES EN MANTENIMIENTO)
    
    static void reporteListaCircular(ListaCircularDoble* lista)
    {
        
        cout << "  REPORTE: Lista Circular - Mantenimiento" << endl;
        cout << "" << endl;
        
        if (lista->estaVacia())
        {
            cout << " No hay aviones en mantenimiento" << endl;
            return;
        }
        
        inicializarCarpetas();
        
        string archivoDOT = "reportes\\aviones\\lista_mantenimiento.dot";
        string archivoPNG = "reportes\\aviones\\lista_mantenimiento.png";
        
        cout << "Generando código DOT..." << endl;
        string codigoDOT = lista->generarDOT("AvionesMantenimiento");
        
        cout << "Guardando archivo .dot..." << endl;
        ofstream archivo(archivoDOT);
        
        if (!archivo.is_open())
        {
            cout << " Error al crear archivo .dot" << endl;
            return;
        }
        
        archivo << codigoDOT;
        archivo.close();
        
        cout << "Generando imagen PNG..." << endl;
        string comando = "dot -Tpng " + archivoDOT + " -o " + archivoPNG;
        
        if (system(comando.c_str()) != 0)
        {
            cout << " Error al ejecutar Graphviz" << endl;
            return;
        }
        
        cout << "Abriendo imagen..." << endl;
        abrirImagen(archivoPNG);
        
        cout << "\n Reporte generado exitosamente" << endl;
        cout << "  Ubicación: " << archivoPNG << endl;
    }
    
    
    // REPORTE ÁRBOL BINARIO (PILOTOS POR HORAS DE VUELO)
    
    static void reporteArbolBinario(ArbolBinarioBusqueda* arbol)
    {
        
        cout << "  REPORTE: Árbol Binario - Pilotos" << endl;
        cout << "" << endl;
        
        if (arbol->estaVacio())
        {
            cout << " No hay pilotos registrados" << endl;
            return;
        }
        
        inicializarCarpetas();
        
        // Usar el método generarReporte() del ABB que ya abre la imagen
        arbol->generarReporte();
        
        cout << "\nReporte generado exitosamente" << endl;
    }
    
    
    // REPORTE TABLA HASH (PILOTOS POR ID)
    
    static void reporteTablaHash(TablaHash* tabla)
    {
       
        cout << "  REPORTE: Tabla Hash - Pilotos" << endl;
        cout << "" << endl;
        
        if (tabla->estaVacia())
        {
            cout << " No hay pilotos en la tabla hash" << endl;
            return;
        }
        
        inicializarCarpetas();
        
        // Usar el método generarReporte() de la tabla hash
        tabla->generarReporte();
        
        cout << "\n✓Reporte generado exitosamente" << endl;
    }
    
    
    // REPORTE GRAFO (RUTAS)
    
    static void reporteGrafo(Grafo* grafo)
    {
        
        cout << "  REPORTE: Grafo - Rutas" << endl;
        cout << "" << endl;
        
        if (grafo->estaVacio())
        {
            cout << " No hay rutas en el grafo" << endl;
            return;
        }
        
        inicializarCarpetas();
        
        // Usar el método generarReporte() del grafo
        grafo->generarReporte();
        
        cout << "\n Reporte generado exitosamente" << endl;
    }
    
    
    // REPORTE MATRIZ DISPERSA (ASIGNACIONES)
    
    static void reporteMatrizDispersa(MatrizDispersa* matriz)
    {
        
        cout << "  REPORTE: Matriz Dispersa - Asignaciones" << endl;
        cout << "" << endl;
        
        if (matriz->estaVacia())
        {
            cout << " No hay asignaciones en la matriz" << endl;
            return;
        }
        
        inicializarCarpetas();
        
        // Usar el método generarReporte() de la matriz
        matriz->generarReporte();
        
        cout << "\n Reporte generado exitosamente" << endl;
    }
};

#endif