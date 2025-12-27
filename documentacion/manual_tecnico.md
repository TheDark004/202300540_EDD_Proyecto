# Manual Técnico
## Sistema de Gestión de Aeropuerto

---

**Universidad de San Carlos de Guatemala**  
**Facultad de Ingeniería**  
**Escuela de Ciencias y Sistemas**  
**Estructuras de Datos**

**Estudiante:** René Sebastian Gutiérrez Contreras 
**Carnet:** 202300540 
**Fecha:** Diciembre 2025

---

## 1. Requisitos del Sistema

### Hardware
- Procesador: Intel Core i3 o superior
- RAM: 4 GB mínimo
- Espacio en disco: 100 MB

### Software
- Windows 10 o superior
- MinGW (compilador g++)
- Graphviz para generar reportes

---

## 2. Instalación de Herramientas

### 2.1 MinGW (Compilador)
1. Descargar de: https://sourceforge.net/projects/mingw-w64/
2. Instalar en `C:\mingw64`
3. Agregar al PATH: `C:\mingw64\bin`

### 2.2 Graphviz
1. Descargar de: https://graphviz.org/download/
2. Instalar
3. Agregar al PATH: `C:\Program Files\Graphviz\bin`

### 2.3 Verificación
```bash
g++ --version
dot -V
```

---

## 3. Estructuras de Datos

### 3.1 Árbol B (Orden 5)
- **Uso:** Almacena aviones disponibles
- **Llave:** Número de registro
- **Características:** Máximo 4 claves por nodo, auto-balanceado

### 3.2 Lista Circular Doble
- **Uso:** Aviones en mantenimiento
- **Características:** Doble enlace, circular

### 3.3 Árbol Binario de Búsqueda
- **Uso:** Pilotos ordenados por horas de vuelo
- **Recorridos:** Preorden, Inorden, Postorden

### 3.4 Tabla Hash
- **Uso:** Pilotos por ID
- **Tamaño:** 19 posiciones
- **Función:** h(k) = k mod 19
- **Colisiones:** Encadenamiento

### 3.5 Grafo Dirigido
- **Uso:** Rutas entre ciudades
- **Algoritmo:** Dijkstra para ruta más corta
- **Implementación:** Lista de adyacencia

### 3.6 Matriz Dispersa
- **Uso:** Relaciona pilotos con vuelos y ciudades
- **Características:** Cabeceras dinámicas

---

## 4. Archivos del Proyecto

```
proyecto/
├── main.cpp
├── estructuras.h
├── ArbolB.h
├── ArbolBinarioBusqueda.h
├── ListaCircularDoble.h
├── ListaSimpleHash.h
├── TablaHash.h
├── Grafo.h
├── MatrizDispersa.h
├── GestorArchivos.h
├── GestorReportes.h
├── json.hpp
└── datos/
    ├── aviones.json
    ├── pilotos.json
    ├── rutas.txt
    └── movimientos.txt
```

---

## 5. Formato de Archivos de Entrada

### 5.1 aviones.json
```json
[
    {
        "vuelo": "AA100",
        "numero_de_registro": "N12345",
        "modelo": "Boeing 737",
        "fabricante": "Boeing",
        "ano_fabricacion": 2015,
        "capacidad": 189,
        "peso_max_despegue": 79000,
        "aerolinea": "American Airlines",
        "estado": "Disponible"
    }
]
```

### 5.2 pilotos.json
```json
[
    {
        "id": "P001",
        "nombre": "Carlos Mendoza",
        "nacionalidad": "Guatemala",
        "numero_de_licencia": "GT-12345",
        "horas_de_vuelo": 5000,
        "tipo_licencia": "Comercial"
    }
]
```

### 5.3 rutas.txt
```
Guatemala/Mexico/450;
ElSalvador/Honduras/150;
CostaRica/Panama/550;
```

### 5.4 movimientos.txt
```
MantenimientoAviones(Ingreso,N12345);
MantenimientoAviones(Salida,N67890);
DarDeBaja(P007);
```

---

## 6. Compilación y Ejecución

### Compilar
```bash
g++ -o aeropuerto main.cpp -std=c++11
```

### Ejecutar
```bash
aeropuerto.exe
```

---

## 7. Funcionalidades

### Menú Principal
1. Cargar Aviones - Lee JSON e inserta en Árbol B o Lista Circular
2. Cargar Pilotos - Lee JSON e inserta en ABB y Tabla Hash
3. Cargar Rutas - Lee TXT y crea grafo
4. Cargar Movimientos - Procesa comandos de movimiento
5. Dar de Baja a Piloto - Elimina de ABB, Hash y Matriz
6. Recorridos del Árbol Binario - Muestra preorden, inorden, postorden
7. Calcular Ruta Más Corta - Usa algoritmo de Dijkstra
8. Asignar Piloto a Vuelo - Inserta en matriz dispersa
9. Generar Reportes - Crea imágenes con Graphviz
10. Salir

---

## 8. Reportes

Los reportes se generan en formato PNG usando Graphviz:

- Árbol B: `reportes/aviones/arbol_b_disponibles.png`
- Lista Circular: `reportes/aviones/lista_mantenimiento.png`
- ABB: `grafica_abb_pilotos.png`
- Tabla Hash: `grafica_tabla_hash.png`
- Grafo: `grafica_grafo_rutas.png`
- Matriz Dispersa: `grafica_matriz_dispersa.png`

---

## 9. Manejo de Memoria

El programa usa memoria dinámica con `new` y `delete`. Cada estructura tiene un destructor que libera la memoria correctamente.

Los pilotos se almacenan en 3 estructuras (ABB, Hash, Matriz) pero se crea un solo objeto y se comparte el puntero. Al eliminar un piloto, se elimina de las 3 estructuras pero la memoria se libera solo una vez.

---

**Fin del Manual Técnico**