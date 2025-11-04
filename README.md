# 🧠 Sistema de Gestión de Procesos  
Proyecto grupal — *Estructura de Datos (Plan 2025)*  

---
## 🎯 Integrantes del Grupo B
- **Gomez Varillas Millio Zahid**
- **Cáceres Salcedo Juan Francisco** 
- **Takagi Arana Kuraji Ryutaro**
- **Tomailla Contreras Alexis Anyelo**

---
## 📋 Descripción del poblema 
El problema planteado es implementar un sistema de software que simule el funcionamiento de un sistema operativo básico en lo relativo a los procesos y la gestión de recursos. En este momento, la forma manual de gestionar los procesos que se encuentran en ejecución, la forma de planificar la utilización de la CPU y la forma de gestionar dicha memoria es muy ineficiente y propensa a errores en todos los posibles procesos manuales no integrados. El sistema debe ser capaz de gestionar un conjunto de procesos que tiene atributos como un id de proceso, nombre de proceso y prioridad. Estos procesos compiten por los recursos del sistema, los cuales son tiempo de CPU y espacio de memoria. Por lo tanto, la falta de un sistema que haga esto automáticamente, que organice estos procesos para que el recurso CPU sea asignado como prioridad, etc., representa el problema a resolver.


---
## 📋 Descripción del proyecto  
Este sistema simula el funcionamiento básico de un **sistema operativo**, permitiendo la gestión de **procesos, memoria y planificación de CPU**.  
El objetivo es comprender e implementar **estructuras de datos dinámicas** (listas, pilas y colas) desarrolladas desde cero en C++.

---

## ⚙️ Estructuras de datos utilizadas  
- **Lista enlazada:** para la gestión de procesos.  
- **Colas de prioridad:** para la planificación de CPU.  
- **Pila:** para la administración de bloques de memoria.  

Cada estructura fue implementada manualmente sin el uso de STL, cumpliendo los requerimientos académicos del curso.

---

## 🧩 Funcionalidades principales  
| Módulo | Descripción |
|--------|--------------|
| **Gestión de Procesos** | Crear, eliminar, buscar, modificar y listar procesos. |
| **Planificación de CPU** | Encolar y ejecutar procesos según prioridad. |
| **Gestión de Memoria** | Asignar y liberar bloques de memoria. |
| **Persistencia** | Guardar y cargar el estado del sistema desde archivo. |

---

## 💻 Ejecución del programa  

### 🔸 Requisitos  
- **Lenguaje:** C++  
- **Entorno:** Dev-C++ 6.3 o Code::Blocks  
- **Sistema Operativo:** Windows 10 o superior  

### 🔸 Instalación  
1. Clonar el repositorio:  
   ```bash
   git clone https://github.com/Toshihiro05/Grupo-B.git
   ```
2. Abrir el archivo `main.cpp` en **Dev-C++**.  
3. Compilar con **F9**.  
4. Ejecutar con **F10**.  

---

## 🧮 Menú principal del sistema  

```
--- SISTEMA DE GESTIÓN DE PROCESOS ---
1. Gestión de Procesos
2. Planificación de CPU
3. Gestión de Memoria
4. Persistencia (Guardar / Cargar)
5. Salir
Seleccione una opción:
```

---

## 🧾 Manual de uso  

### 🔹 Gestión de Procesos
Permite crear, eliminar, modificar y visualizar procesos.  
Ejemplo:
```
Ingrese nombre: Navegador
Ingrese prioridad (0: Alta, 1: Media, 2: Baja): 1
Ingrese tamaño de memoria (KB): 128
Proceso agregado correctamente.
```

### 🔹 Planificación de CPU
Maneja las colas de prioridad (alta, media y baja).  
```
Colas actuales:
Alta: [PID 1 - EditorTexto]
Media: [PID 3 - Navegador]
Baja: []
```

### 🔹 Gestión de Memoria
Simula la asignación y liberación de memoria.  
```
Bloques de memoria asignados:
[PID 2 - 256 KB en dirección 0x1000]
[PID 5 - 128 KB en dirección 0x1100]
```

### 🔹 Persistencia
Permite guardar y cargar el estado del sistema.  
```
Guardando estado del sistema...
Archivo generado: sistema_guardado.txt
```

---

## ⚠️ Mensajes de error comunes  
| Mensaje | Causa | Solución |
|----------|--------|-----------|
| `PID no encontrado` | El proceso ingresado no existe. | Verifica el ID en la lista de procesos. |
| `Memoria insuficiente` | No hay espacio libre disponible. | Libera memoria antes de asignar. |
| `Entrada inválida` | Se ingresaron valores no válidos. | Reingresar con valores numéricos. |

---

## 🧑‍💻 Integrantes del equipo  

| Integrante | Rol | Aporte |
|-------------|------|--------|
| **Gómez Varillas Mililio Zahid** | Líder de proyecto | Coordinación general y gestión de procesos |
| **Cáceres Salcedo Juan Francisco** | Desarrollador backend | Planificación de CPU y estructuras de colas |
| **Takagi Arana Kuraji Ryutaro** | Desarrollador de Memoria | Gestión de memoria y control de sistema |
| **Tomailla Contreras Alexis Anyelo** | Desarrollador de Interfaz | Manual de usuario y validaciones finales |

---

## 🧩 Plan de trabajo  

| Fase | Actividad | Responsable | Fecha |
|------|------------|-------------|-------|
| 1 | Análisis del problema y diseño de estructuras | Zahid | 10/10 |
| 2 | Implementación del sistema | Juan y Ryutaro | 14/10 |
| 3 | Pruebas y documentación final | Alexis | 18/10 |

---

## 📸 Evidencias de trabajo en equipo  

### 🔹 Capturas y commits  
- Historial de commits en GitHub que muestra los aportes individuales de cada integrante.  
- Capturas del menú y ejecución del sistema.  
- Participación activa reflejada en ramas y merges del repositorio.  

### 🔹 Repositorio  
👉 [https://github.com/Toshihiro05/Grupo-B](https://github.com/Toshihiro05/Grupo-B)

### 🔹 Carpeta compartida  
👉 [Drive del grupo](https://drive.google.com/drive/folders/1h5ShRYsNVqKIOh4SLwsog7O0RqAChTsq?usp=drive_link)

---

## 🗓️ Cronograma general  

| Semana | Actividad | Entregable |
|---------|------------|-------------|
| Semana 1 | Análisis y requerimientos | Capítulo 1 del informe |
| Semana 2 | Diseño de estructuras y algoritmos | Capítulo 2 |
| Semana 3 | Codificación y pruebas | Capítulo 3 y  Evidencias y control de versiones | Capítulo 4 |

---

## 🧠 Recomendaciones de uso  
- Utiliza nombres únicos para cada proceso.  
- No asignes más memoria de la disponible.  
- Guarda el estado antes de salir para evitar pérdida de datos.  
- Puedes cargar estados anteriores al iniciar el programa.

---

## 🔗 Enlaces  
- **Repositorio GitHub:** [https://github.com/Toshihiro05/Grupo-B](https://github.com/Toshihiro05/Grupo-B)  
- **Carpeta Drive compartida:** [https://drive.google.com/drive/folders/1h5ShRYsNVqKIOh4SLwsog7O0RqAChTsq](https://drive.google.com/drive/folders/1h5ShRYsNVqKIOh4SLwsog7O0RqAChTsq?usp=drive_link)

---

## 🧾 Licencia  
Proyecto académico desarrollado para la asignatura **Estructura de Datos** de la **Universidad Continental (2025)**.  
Uso exclusivo con fines educativos.
