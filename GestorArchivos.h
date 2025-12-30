#ifndef GESTOR_ARCHIVOS_H
#define GESTOR_ARCHIVOS_H

#include <iostream>
#include <unordered_map>
#include <fstream>
#include "json.hpp"
#include "estructuras.h"
#include "ListaCircularDoble.h"
#include "ArbolBB.h"
#include "TablaHash.h"
#include "ArbolB.h"
#include "MatrizD.h"

using json = nlohmann::json;
using namespace std;

class GestorArchivos
{

private:
    // Mapa auxiliar para relacionar vuelo con ciudad destino
    static unordered_map<string, string> vuelosCiudades;

public:
    // Cargar aviones desde el Json
    static bool cargarAvionesArbolB(string nombreArchivo,
                                    ArbolB *arbolDisponibles,
                                    ListaCircularDoble *listaMantenimiento,
                                    MatrizDispersa *matriz)
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
            nuevoAvion->ciudadDestino = avionJSON.value("ciudad_destino", "");

            if (nuevoAvion->numeroRegistro.empty())
            {
                cout << " Avión sin número de registro, ignorado" << endl;
                delete nuevoAvion;
                continue;
            }

            if (!nuevoAvion->vuelo.empty() && !nuevoAvion->ciudadDestino.empty())
            {
                vuelosCiudades[nuevoAvion->vuelo] = nuevoAvion->ciudadDestino;
                cout << "  → Mapa vuelos: " << nuevoAvion->vuelo 
                     << " → " << nuevoAvion->ciudadDestino << endl;
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

            if (!nuevoAvion->vuelo.empty() && !nuevoAvion->ciudadDestino.empty())
            {
                // Esto depende de cómo estructures tu matriz
                // Necesitarías también el ID del piloto asignado
                cout << "  → Avión " << nuevoAvion->numeroRegistro 
                     << " tiene vuelo " << nuevoAvion->vuelo 
                     << " a " << nuevoAvion->ciudadDestino << endl;
                // La asignación completa se hará cuando se carguen los pilotos
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
                              TablaHash *tablaPilotos,
                            MatrizDispersa *matriz)
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

            nuevoPiloto->id = pilotoJSON.value("numero_de_id", pilotoJSON.value("id", ""));
            nuevoPiloto->nombre = pilotoJSON.value("nombre", "");
            nuevoPiloto->nacionalidad = pilotoJSON.value("nacionalidad", "");
            nuevoPiloto->vuelo = pilotoJSON.value("vuelo", "");
            nuevoPiloto->horasVuelo = pilotoJSON.value("horas_de_vuelo", 0);
            nuevoPiloto->tipoLicencia = pilotoJSON.value("tipo_licencia", "Comercial");
            

            if (nuevoPiloto->id.empty())
            {
                cout << " Piloto sin ID, ignorado" << endl;
                delete nuevoPiloto;
                continue;
            }

             // Insertar en estructuras de pilotos
            arbolPilotos->insertar(nuevoPiloto);
            tablaPilotos->insertar(nuevoPiloto);

            if (!nuevoPiloto->vuelo.empty())
            {
                // Buscar si este vuelo tiene una ciudad destino asignada
                auto it = vuelosCiudades.find(nuevoPiloto->vuelo);
                if (it != vuelosCiudades.end())
                {
                    // Insertar en matriz: piloto → vuelo → ciudad
                    matriz->insertar(nuevoPiloto->id, nuevoPiloto->vuelo, it->second);
                    cout << "  → Matriz: " << nuevoPiloto->id << " → " 
                         << nuevoPiloto->vuelo << " → " << it->second << endl;
                }
                else
                {
                    // Si no hay ciudad destino, usar una por defecto
                    string ciudadDefault = "Ciudad_" + nuevoPiloto->vuelo;
                    matriz->insertar(nuevoPiloto->id, nuevoPiloto->vuelo, ciudadDefault);
                    cout << "  → Matriz (default): " << nuevoPiloto->id << " → " 
                         << nuevoPiloto->vuelo << " → " << ciudadDefault << endl;
                }
            }
            else
            {
                cout << "  → Piloto sin vuelo asignado: " << nuevoPiloto->id << endl;
            }

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

     static bool cargarAsignaciones(string nombreArchivo, 
                                   ArbolB *arbolAviones,
                                   ArbolBinarioBusqueda *arbolPilotos,
                                   MatrizDispersa *matriz)
    {
        ifstream archivo(nombreArchivo);
        if (!archivo.is_open())
        {
            cout << " Error al abrir archivo de asignaciones" << endl;
            return false;
        }
        
        string linea;
        int contador = 0;
        
        while (getline(archivo, linea))
        {
            // Formato: Vuelo;PilotoID;AvionRegistro;CiudadDestino
            size_t pos1 = linea.find(';');
            size_t pos2 = linea.find(';', pos1 + 1);
            size_t pos3 = linea.find(';', pos2 + 1);
            
            if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos)
            {
                string vuelo = linea.substr(0, pos1);
                string pilotoID = linea.substr(pos1 + 1, pos2 - pos1 - 1);
                string avionRegistro = linea.substr(pos2 + 1, pos3 - pos2 - 1);
                string ciudadDestino = linea.substr(pos3 + 1);
                
                // Buscar piloto
                Piloto *piloto = arbolPilotos->buscar(pilotoID);
                if (piloto == nullptr)
                {
                    cout << "  Piloto no encontrado: " << pilotoID << endl;
                    continue;
                }
                
                // Buscar avión
                Avion *avion = arbolAviones->buscar(avionRegistro);
                if (avion == nullptr)
                {
                    cout << "  Avión no encontrado: " << avionRegistro << endl;
                    continue;
                }
                
                // Asignar vuelo al avión
                avion->vuelo = vuelo;
                avion->ciudadDestino = ciudadDestino;
                
                // Asignar vuelo al piloto
                piloto->vuelo = vuelo;
                
                
                // Registrar en matriz dispersa
                matriz->insertar(pilotoID, vuelo, ciudadDestino);
                
                contador++;
                cout << "  → Asignación: " << vuelo << " | Piloto: " << pilotoID 
                     << " | Avión: " << avionRegistro << " | Destino: " << ciudadDestino << endl;
            }
        }
        
        archivo.close();
        cout << "  Total asignaciones cargadas: " << contador << endl;
        return true;
    }
};
unordered_map<string, string> GestorArchivos::vuelosCiudades;
#endif
