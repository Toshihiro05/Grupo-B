Sistema de Gestión de Procesos
Proyecto grupal — Estructura de Datos (Plan 2025)

- Descripción del proyecto
Este sistema simula el funcionamiento básico de un sistema operativo, gestionando procesos, memoria y planificación de CPU mediante estructuras de datos dinámicas implementadas desde cero en C++.
Permite:
Registrar, buscar, eliminar y modificar procesos.
Planificar la ejecución de procesos con distintas prioridades (alta, media, baja).
Asignar y liberar bloques de memoria simulados.
Guardar y cargar el estado del sistema desde un archivo.

- Estructuras de datos utilizadas
Lista enlazada: Para la gestión de procesos.
Colas de prioridad: Para la planificación del CPU.
Pila: Para la administración de bloques de memoria.
Cada estructura fue implementada sin usar STL, cumpliendo con los requerimientos del curso.

Principales funcionalidades

| Módulo               | Funcionalidades principales                    |
| -------------------- | ---------------------------------------------- |
| Gestión de Procesos  | Agregar, eliminar, modificar y listar procesos |
| Planificación de CPU | Encolar y ejecutar procesos por prioridad      |
| Gestión de Memoria   | Asignar y liberar bloques de memoria           |
| Persistencia         | Guardar y cargar el estado del sistema         |
| Interfaz de Usuario  | Menú de consola simple e intuitivo             |


Ejecución del programa
Requisitos
- Lenguaje: C++
- Entorno: Dev-C++ 6.3 o Code::Blocks
- SO: Windows 10 o superior

Grupo-B
- src   # Código fuente (.cpp)
- include     ----->  # Archivos de cabecera (.h)
- docs        ----->  # Documentación y reportes
- pruebas     ----->  # Capturas de validación
- README.md   ----->  # Este archivo
- main.cpp    ----->  # Archivo principal del sistema



Integrantes del equipo

| Integrante                      | Rol principal            | Aporte                                               | 
| --------------------------------| -----------------------  | -----------------------------------------------------|
| Gómez Varillas Mililio Zahid    | Lider de proyecto        | Diseño del análisis del problema y estructura general|
| Cáceres Salcedo Juan Francisco  | Desarrollador Backend    | Implementación de gestión de memoria                 |
| Takagi Arana Kuraji Ryutaro     | Desarrollador de Memoria | Módulo de planificación de CPU y control del sistema |
| Tomailla Contreras Alexis Anyelo| DEsarrollador de Interfaz| Manual de usuario y validaciones finales             |


