#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"
#include "estructuras.h"
#include "ListaCircularDoble.h"
#include "ArbolB.h"
#include "ArbolBB.h"
#include "TablaHash.h"
#include "Grafo.h"
#include "MatrizD.h"
#include "GestorArchivos.h"
#include "GestorReportes.h"

using json = nlohmann::json;
using namespace std;

void pausar()
{
    cout << "\nPresiona Enter para continuar...";
    cin.ignore(10000, '\n');
    cin.get();
}

void limpiarPantalla()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// DAR DE BAJA A UN PILOTO 
void darDeBajaPiloto(ArbolBinarioBusqueda *arbolPilotos,
                     TablaHash *tablaPilotos,
                     MatrizDispersa *matriz)
{
    cout << "       DAR DE BAJA A PILOTO" << endl;

    string idPiloto;
    cout << "Ingrese el ID del piloto: ";
    getline(cin, idPiloto);

    // Verificar que existe en la tabla hash
    Piloto *piloto = tablaPilotos->buscar(idPiloto);
    if (piloto == nullptr)
    {
        cout << "\nPiloto no encontrado con ID: " << idPiloto << endl;
        pausar();
        return;
    }

    cout << "\nPiloto encontrado:" << endl;
    cout << "  Nombre: " << piloto->nombre << endl;
    cout << "  Horas de vuelo: " << piloto->horasVuelo << endl;

    cout << "\n¿Está seguro de dar de baja a este piloto? (s/n): ";
    string confirmacion;
    getline(cin, confirmacion);

    if (confirmacion != "s" && confirmacion != "S")
    {
        cout << "Operación cancelada" << endl;
        pausar();
        return;
    }

    cout << "\nEliminando piloto de todas las estructuras..." << endl;
    cout << "────────────────────────────────────────" << endl;

    // Eliminar de las 3 estructuras
    bool exito1 = arbolPilotos->eliminar(idPiloto);
    bool exito2 = tablaPilotos->eliminar(idPiloto);
    bool exito3 = matriz->eliminarPiloto(idPiloto);

    cout << "────────────────────────────────────────" << endl;

    if (exito1 && exito2)
    {
        cout << "\n Piloto dado de baja exitosamente" << endl;
    }
    else
    {
        cout << "\nAlgunos errores durante la eliminación" << endl;
    }

    pausar();
}

// MOVER AVION ENTRE ESTRUCTURAS (ArbolB <--> ListaCircular)
void moverAvion(ArbolB *arbolDisponibles,
                ListaCircularDoble *listaMantenimiento,
                string tipoMovimiento,
                string numRegistro)
{
    if (tipoMovimiento == "Ingreso")
    {
        // De Disponibles (ArbolB) a Mantenimiento (ListaCircular)
        Avion *avion = arbolDisponibles->eliminar(numRegistro);
        if (avion != nullptr)
        {
            avion->estado = "Mantenimiento";
            listaMantenimiento->insertar(avion);
            cout << "  Avión movido a Mantenimiento: " << numRegistro << endl;
        }
        else
        {
            cout << "  Avión no encontrado en Disponibles: " << numRegistro << endl;
        }
    }
    else if (tipoMovimiento == "Salida")
    {
        // De Mantenimiento (ListaCircular) a Disponibles (ArbolB)
        Avion *avion = listaMantenimiento->eliminar(numRegistro);
        if (avion != nullptr)
        {
            avion->estado = "Disponible";
            arbolDisponibles->insertar(avion);
            cout << "  Avión movido a Disponibles: " << numRegistro << endl;
        }
        else
        {
            cout << " Avión no encontrado en Mantenimiento: " << numRegistro << endl;
        }
    }
}

// CARGAR MOVIMIENTOS (actualizado para el proyecto)
void cargarMovimientos(ArbolB *arbolDisponibles,
                       ListaCircularDoble *listaMantenimiento,
                       ArbolBinarioBusqueda *arbolPilotos,
                       TablaHash *tablaPilotos,
                       MatrizDispersa *matriz)
{

    cout << "       CARGAR MOVIMIENTOS" << endl;
    cout << "" << endl;

    string archivo;
    cout << "Sugerencia: datos\\movimientos.txt" << endl;
    cout << "Ingrese el nombre del archivo: ";
    getline(cin, archivo);

    ifstream archivoTXT(archivo);
    if (!archivoTXT.is_open())
    {
        cout << " No se pudo abrir el archivo" << endl;
        pausar();
        return;
    }

    cout << "\nProcesando los movimientos..." << endl;
    cout << "────────────────────────────────────────" << endl;

    string linea;
    int contador = 0;

    while (getline(archivoTXT, linea))
    {
        // Ignorar líneas vacías
        if (linea.empty())
        {
            continue;
        }

        // Eliminar punto y coma final
        if (!linea.empty() && linea.back() == ';')
        {
            linea.pop_back();
        }

        contador++;
        cout << "\n[" << contador << "] " << linea << endl;

        // Parsear comando
        size_t pos = linea.find('(');
        if (pos == string::npos)
        {
            cout << "  Formato inválido" << endl;
            continue;
        }

        string comando = linea.substr(0, pos);
        string parametros = linea.substr(pos + 1);

        // Eliminar paréntesis final
        if (!parametros.empty() && parametros.back() == ')')
        {
            parametros.pop_back();
        }

        // Procesar comandos
        if (comando == "MantenimientoAviones")
        {
            // Formato: MantenimientoAviones(Ingreso,N12345) o (Salida,N12345)
            size_t coma = parametros.find(',');
            if (coma != string::npos)
            {
                string tipo = parametros.substr(0, coma);
                string registro = parametros.substr(coma + 1);

                moverAvion(arbolDisponibles, listaMantenimiento, tipo, registro);
            }
        }
        else if (comando == "DarDeBaja")
        {
            // Formato: DarDeBaja(P001)
            string idPiloto = parametros;

            cout << "  Dando de baja a piloto: " << idPiloto << endl;

            bool exito1 = arbolPilotos->eliminar(idPiloto);
            bool exito2 = tablaPilotos->eliminar(idPiloto);
            bool exito3 = matriz->eliminarPiloto(idPiloto);

            if (exito1 && exito2)
            {
                cout << "  Piloto dado de baja exitosamente" << endl;
            }
        }
        else
        {
            cout << "  Comando desconocido: " << comando << endl;
        }
    }

    archivoTXT.close();

    cout << "────────────────────────────────────────" << endl;
    cout << "\n Movimientos procesados: " << contador << endl;
    pausar();
}

// ASIGNAR PILOTO A VUELO
void asignarPilotoVuelo(TablaHash *tablaPilotos, MatrizDispersa *matriz)
{

    cout << "     ASIGNAR PILOTO A VUELO" << endl;

    string idPiloto, numeroVuelo, ciudad;

    cout << "ID del piloto: ";
    getline(cin, idPiloto);

    // Verificar que el piloto exista
    Piloto *piloto = tablaPilotos->buscar(idPiloto);
    if (piloto == nullptr)
    {
        cout << "\n Piloto no encontrado: " << idPiloto << endl;
        pausar();
        return;
    }

    cout << "Piloto: " << piloto->nombre << endl;
    cout << "\nNúmero de vuelo: ";
    getline(cin, numeroVuelo);

    cout << "Ciudad destino: ";
    getline(cin, ciudad);

    // Insertar en matriz dispersa
    matriz->insertar(idPiloto, numeroVuelo, ciudad);

    cout << "\n Asignación registrada exitosamente" << endl;
    pausar();
}

// MENU DE RECORRIDOS DEL ABB
void menuRecorridosABB(ArbolBinarioBusqueda *arbol)
{
    int opcion;
    do
    {
        limpiarPantalla();

        cout << "     RECORRIDOS DEL ÁRBOL BINARIO" << endl;
        cout << "" << endl;
        cout << "1. Recorrido Preorden" << endl;
        cout << "2. Recorrido Inorden" << endl;
        cout << "3. Recorrido Postorden" << endl;
        cout << "4. Volver" << endl;
        cout << "\nOpción: ";

        cin >> opcion;
        cin.ignore();

        limpiarPantalla();

        switch (opcion)
        {
        case 1:
            arbol->recorridoPreorden();
            break;
        case 2:
            arbol->recorridoInorden();
            break;
        case 3:
            arbol->recorridoPostorden();
            break;
        case 4:
            cout << "Volviendo..." << endl;
            break;
        default:
            cout << " Opción inválida" << endl;
        }

        if (opcion != 4)
        {
            pausar();
        }

    } while (opcion != 4);
}

// CALCULAR RUTA MAS CORTA
void calcularRutaMasCorta(Grafo *grafo)
{

    cout << "     CALCULAR RUTA MÁS CORTA" << endl;
    cout << "" << endl;

    string origen, destino;

    cout << "Ciudad origen: ";
    getline(cin, origen);

    cout << "Ciudad destino: ";
    getline(cin, destino);

    // Calcular con Dijkstra
    grafo->rutaMasCorta(origen, destino);

    pausar();
}

// MENU DE REPORTES (actualizado)
void menuReportes(ArbolB *arbolDisponibles,
                  ListaCircularDoble *listaMantenimiento,
                  ArbolBinarioBusqueda *arbolPilotos,
                  TablaHash *tablaPilotos,
                  Grafo *grafo,
                  MatrizDispersa *matriz)
{
    int opcion;
    do
    {
        limpiarPantalla();

        cout << "         GENERAR REPORTES" << endl;
        cout << "" << endl;
        cout << "1. Árbol B (Aviones Disponibles)" << endl;
        cout << "2. Lista Circular (Aviones Mantenimiento)" << endl;
        cout << "3. Árbol Binario (Pilotos por Horas)" << endl;
        cout << "4. Tabla Hash (Pilotos por ID)" << endl;
        cout << "5. Grafo (Rutas)" << endl;
        cout << "6. Matriz Dispersa (Asignaciones)" << endl;
        cout << "7. Volver al menú principal" << endl;
        cout << "\nOpción: ";

        cin >> opcion;
        cin.ignore();

        limpiarPantalla();

        switch (opcion)
        {
        case 1:
            GestorReportes::reporteArbolB(arbolDisponibles);
            break;
        case 2:
            GestorReportes::reporteListaCircular(listaMantenimiento);
            break;
        case 3:
            GestorReportes::reporteArbolBinario(arbolPilotos);
            break;
        case 4:
            GestorReportes::reporteTablaHash(tablaPilotos);
            break;
        case 5:
            GestorReportes::reporteGrafo(grafo);
            break;
        case 6:
            GestorReportes::reporteMatrizDispersa(matriz);
            break;
        case 7:
            cout << "Volviendo al menú principal..." << endl;
            break;
        default:
            cout << " Opción inválida" << endl;
        }

        if (opcion != 7)
        {
            pausar();
        }

    } while (opcion != 7);
}

int main()
{
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif

    cout << "  SISTEMA DE GESTIÓN DE AEROPUERTO" << endl;
    cout << "" << endl;

    // Crear todas las estructuras
    ArbolB *arbolDisponibles = new ArbolB();
    ListaCircularDoble *listaMantenimiento = new ListaCircularDoble();
    ArbolBinarioBusqueda *arbolPilotos = new ArbolBinarioBusqueda();
    TablaHash *tablaPilotos = new TablaHash();
    Grafo *grafoRutas = new Grafo();
    MatrizDispersa *matriz = new MatrizDispersa();

    cout << "\nSistema listo" << endl;
    pausar();

    int opcion;

    do
    {
        limpiarPantalla();

        cout << "  SISTEMA DE GESTIÓN DE AEROPUERTO" << endl;
        cout << "" << endl;
        cout << "1.  Cargar Aviones" << endl;
        cout << "2.  Cargar Pilotos" << endl;
        cout << "3.  Cargar Rutas" << endl;
        cout << "4.  Cargar Movimientos" << endl;
        cout << "5.  Dar de Baja a Piloto" << endl;
        cout << "6.  Recorridos del Árbol Binario" << endl;
        cout << "7.  Calcular Ruta Más Corta" << endl;
        cout << "8.  Asignar Piloto a Vuelo" << endl;
        cout << "9.  Generar Reportes" << endl;
        cout << "10. Salir" << endl;
        cout << "────────────────────────────────────────" << endl;
        cout << "Seleccione una opción: ";

        cin >> opcion;
        cin.ignore();

        limpiarPantalla();

        switch (opcion)
        {
        case 1:
            // Cargar aviones
            {

                cout << "         CARGAR AVIONES" << endl;
                cout << "" << endl;

                string archivo;
                cout << "Sugerencia: datos\\aviones.json" << endl;
                cout << "Nombre del archivo: ";
                getline(cin, archivo);

                GestorArchivos::cargarAvionesArbolB(archivo, arbolDisponibles, listaMantenimiento);
                pausar();
            }
            break;

        case 2:
            // Cargar pilotos
            {

                cout << "         CARGAR PILOTOS" << endl;
                cout << "" << endl;

                string archivo;
                cout << "Sugerencia: datos\\pilotos.json" << endl;
                cout << "Nombre del archivo: ";
                getline(cin, archivo);

                GestorArchivos::cargarPilotos(archivo, arbolPilotos, tablaPilotos);
                pausar();
            }
            break;

        case 3:
            // Cargar rutas
            {

                cout << "         CARGAR RUTAS" << endl;
                cout << "" << endl;

                string archivo;
                cout << "Sugerencia: datos\\rutas.txt" << endl;
                cout << "Nombre del archivo: ";
                getline(cin, archivo);

                grafoRutas->cargarDesdeArchivo(archivo);
                pausar();
            }
            break;

        case 4:
            // Cargar movimientos
            cargarMovimientos(arbolDisponibles, listaMantenimiento,
                              arbolPilotos, tablaPilotos, matriz);
            break;

        case 5:
            // Dar de baja a piloto
            darDeBajaPiloto(arbolPilotos, tablaPilotos, matriz);
            break;

        case 6:
            // Recorridos del ABB
            menuRecorridosABB(arbolPilotos);
            break;

        case 7:
            // Calcular ruta más corta
            calcularRutaMasCorta(grafoRutas);
            break;

        case 8:
            // Asignar piloto a vuelo
            asignarPilotoVuelo(tablaPilotos, matriz);
            break;

        case 9:
            // Generar reportes
            menuReportes(arbolDisponibles, listaMantenimiento,
                         arbolPilotos, tablaPilotos, grafoRutas, matriz);
            break;

        case 10:

            cout << "  Cerrando sistema..." << endl;
            cout << "" << endl;
            break;

        default:
            cout << "Opción inválida. Intente de nuevo." << endl;
            pausar();
        }

    } while (opcion != 10);

    // Liberar memoria
    cout << "\nLiberando memoria..." << endl;

    delete arbolDisponibles;
    delete listaMantenimiento;
    delete arbolPilotos;
    delete tablaPilotos;
    delete grafoRutas;
    delete matriz;

    cout << " Memoria liberada correctamente" << endl;
    cout << "Sistema cerrado exitosamente\n"
         << endl;

    return 0;
}