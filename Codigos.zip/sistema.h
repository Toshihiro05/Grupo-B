#ifndef SISTEMA_H
#define SISTEMA_H

#define MAX_NOMBRE 50
#define MEMORIA_TOTAL 1024  // 1GB en MB

// Estados del proceso
enum EstadoProceso {
    NUEVO = 0,
    LISTO = 1,
    EJECUTANDO = 2,
    TERMINADO = 3
};

// Prioridades del proceso
enum Prioridad {
    ALTA = 0,
    MEDIA = 1,
    BAJA = 2
};

// Estructura del proceso
struct Proceso {
    int pid;
    char nombre[MAX_NOMBRE];
    int prioridad;
    int estado;
    int memoria_requerida;
    int memoria_asignada;
    Proceso* siguiente;
};

// Tabla de procesos
struct TablaProcesos {
    Proceso** procesos;
    int capacidad;
    int contador;
    int siguiente_pid;
};

// Colas de prioridad para planificación
struct ColaPrioridad {
    Proceso* frente_alta;
    Proceso* final_alta;
    Proceso* frente_media;
    Proceso* final_media;
    Proceso* frente_baja;
    Proceso* final_baja;
    int quantum;
};

// Bloque de memoria
struct BloqueMemoria {
    int direccion_base;
    int tamaño;
    int libre;
    int pid_asignado;
    BloqueMemoria* siguiente;
};

// Gestor de memoria
struct GestorMemoria {
    BloqueMemoria* lista_bloques;
    int memoria_total;
    int memoria_libre;
    int politica;  // 0:First-fit, 1:Best-fit, 2:Worst-fit
};

// Sistema operativo completo
struct SistemaOperativo {
    TablaProcesos* tabla_procesos;
    ColaPrioridad* colas_listos;
    Proceso* proceso_ejecutando;
    GestorMemoria* gestor_memoria;
    int reloj_sistema;
    int procesos_terminados;
    int total_procesos_creados;
};

// Prototipos de funciones
// Inicialización del sistema
SistemaOperativo* inicializarSistema();
void liberarSistema(SistemaOperativo* sistema);

// Gestión de procesos
Proceso* crearProceso(const char* nombre, int prioridad, int memoria);
void agregarProceso(TablaProcesos* tabla, Proceso* proceso);
void eliminarProceso(TablaProcesos* tabla, int pid);
Proceso* buscarProceso(TablaProcesos* tabla, int pid);
Proceso* buscarProcesoPorNombre(TablaProcesos* tabla, const char* nombre);
void listarProcesos(TablaProcesos* tabla);
void modificarPrioridadProceso(TablaProcesos* tabla, int pid, int nueva_prioridad);

// Planificación de CPU
void encolarProceso(ColaPrioridad* colas, Proceso* proceso);
Proceso* desencolarProceso(ColaPrioridad* colas);
void mostrarColasPlanificacion(ColaPrioridad* colas);

// Gestión de memoria
void inicializarMemoria(GestorMemoria* gestor, int memoria_total);
int asignarMemoria(GestorMemoria* gestor, int tamaño, int pid);
int liberarMemoria(GestorMemoria* gestor, int pid);
void mostrarEstadoMemoria(GestorMemoria* gestor);

// Persistencia
void guardarEstadoSistema(SistemaOperativo* sistema, const char* archivo);
void cargarEstadoSistema(SistemaOperativo* sistema, const char* archivo);

// Menús de interfaz de usuario
void agregarProcesoMenu(SistemaOperativo* sistema);
void eliminarProcesoMenu(SistemaOperativo* sistema);
void buscarProcesoMenu(SistemaOperativo* sistema);
void encolarProcesoMenu(SistemaOperativo* sistema);
void asignarMemoriaMenu(SistemaOperativo* sistema);
void liberarMemoriaMenu(SistemaOperativo* sistema);
void mostrarEstadisticas(SistemaOperativo* sistema);
void ejecutarProceso(SistemaOperativo* sistema);

#endif