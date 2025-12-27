#ifndef GESTOR_ARCHIVOS_H
#define GESTOR_ARCHIVOS_H

#include <iostream>
#include <fstream>
#include "json.hpp"
#include "estructuras.h"
#include "ListaCircularDoble.h"
#include "ArbolBB.h"
#include "TablaHash.h"
#include "ArbolB.h"

using json = nlohmann::json;
using namespace std;

class GestorArchivos
{

public:
    // Cargar aviones desde el Json
    static bool cargarAvionesArbolB(string nombreArchivo,
                                    ArbolB *arbolDisponibles,
                                    ListaCircularDoble *listaMantenimiento)
    {

        cout << "    CARGANDO AVIONES DESDE JSON" << endl;
        cout << "" << endl;

        ifstream archivo(nombreArchivo);

        if (!archivo.is_open())
        {
            cout << " Error: No se pudo abrir " << nombreArchivo << endl;
            return false;
        }

        cout << " Archivo abierto: " << nombreArchivo << endl;

        json datos;

        try
        {
            datos = json::parse(archivo);
            archivo.close();
            cout << " JSON parseado correctamente" << endl;
        }
        catch (json::parse_error &e)
        {
            cout << " Error al parsear JSON: " << e.what() << endl;
            archivo.close();
            return false;
        }

        if (!datos.is_array())
        {
            cout << " Error: El JSON debe ser un array de aviones" << endl;
            return false;
        }

        cout << "Total de aviones en archivo: " << datos.size() << endl;
        cout << "\nProcesando aviones..." << endl;
        cout << "────────────────────────────────────────" << endl;

        int contadorDisponibles = 0;
        int contadorMantenimiento = 0;

        for (auto &avionJSON : datos)
        {
            Avion *nuevoAvion = new Avion();

            nuevoAvion->vuelo = avionJSON.value("vuelo", "");
            nuevoAvion->numeroRegistro = avionJSON.value("numero_de_registro", "");
            nuevoAvion->modelo = avionJSON.value("modelo", "");
            nuevoAvion->fabricante = avionJSON.value("fabricante", "");
            nuevoAvion->aFabricacion = avionJSON.value("ano_fabricacion", 0);
            nuevoAvion->capacidad = avionJSON.value("capacidad", 0);
            nuevoAvion->pesoMaxDespegue = avionJSON.value("peso_max_despegue", 0);
            nuevoAvion->aerolinea = avionJSON.value("aerolinea", "");
            nuevoAvion->estado = avionJSON.value("estado", "Disponible");

            if (nuevoAvion->numeroRegistro.empty())
            {
                cout << "✗ Avión sin número de registro, ignorado" << endl;
                delete nuevoAvion;
                continue;
            }

            // Insertar según el estado
            if (nuevoAvion->estado == "Disponible")
            {
                arbolDisponibles->insertar(nuevoAvion);
                contadorDisponibles++;
            }
            else if (nuevoAvion->estado == "Mantenimiento")
            {
                listaMantenimiento->insertar(nuevoAvion);
                contadorMantenimiento++;
            }
            else
            {
                cout << " Estado inválido: " << nuevoAvion->estado << endl;
                delete nuevoAvion;
            }
        }

        cout << "────────────────────────────────────────" << endl;
        cout << "       RESUMEN DE CARGA DE AVIONES       " << endl;
        cout << "" << endl;
        cout << "  Aviones disponibles: " << contadorDisponibles << endl;
        cout << "  Aviones en mantenimiento: " << contadorMantenimiento << endl;
        cout << "  Total cargado: " << (contadorDisponibles + contadorMantenimiento) << endl;

        return true;
    }

    static bool cargarPilotos(string nombreArchivo,
                              ArbolBinarioBusqueda *arbolPilotos,
                              TablaHash *tablaPilotos)
    {

        cout << "    CARGANDO PILOTOS DESDE JSON" << endl;

        ifstream archivo(nombreArchivo);

        if (!archivo.is_open())
        {
            cout << " Error: No se pudo abrir " << nombreArchivo << endl;
            return false;
        }

        cout << " Archivo abierto: " << nombreArchivo << endl;

        json datos;

        try
        {
            datos = json::parse(archivo);
            archivo.close();
            cout << " JSON parseado correctamente" << endl;
        }
        catch (json::parse_error &e)
        {
            cout << " Error al parsear JSON: " << e.what() << endl;
            archivo.close();
            return false;
        }

        if (!datos.is_array())
        {
            cout << " Error: El JSON debe ser un array de pilotos" << endl;
            return false;
        }

        cout << "Total de pilotos en archivo: " << datos.size() << endl;
        cout << "\nProcesando pilotos..." << endl;

        int contador = 0;

        for (auto &pilotoJSON : datos)
        {
            // Crear nuevo piloto (solo UNA vez)
            Piloto *nuevoPiloto = new Piloto();

            nuevoPiloto->id = pilotoJSON.value("id", "");
            nuevoPiloto->nombre = pilotoJSON.value("nombre", "");
            nuevoPiloto->nacionalidad = pilotoJSON.value("nacionalidad", "");
            nuevoPiloto->numeroLicencia = pilotoJSON.value("numero_de_licencia", "");
            nuevoPiloto->horasVuelo = pilotoJSON.value("horas_de_vuelo", 0);
            nuevoPiloto->tipoLicencia = pilotoJSON.value("tipo_licencia", "Comercial");

            if (nuevoPiloto->id.empty())
            {
                cout << " Piloto sin ID, ignorado" << endl;
                delete nuevoPiloto;
                continue;
            }

            // IMPORTANTE: Insertar el MISMO puntero en ambas estructuras
            // No crear copias, usar el mismo piloto
            arbolPilotos->insertar(nuevoPiloto);
            tablaPilotos->insertar(nuevoPiloto);

            contador++;
        }

        cout << "────────────────────────────────────────" << endl;
        cout << "       RESUMEN DE CARGA DE PILOTOS       " << endl;
        cout << "" << endl;
        cout << "  Total de pilotos cargados: " << contador << endl;
        cout << "  -> Insertados en ABB (por horas de vuelo)" << endl;
        cout << "  -> Insertados en Tabla Hash (por ID)" << endl;

        return true;
    }
};
#endif
