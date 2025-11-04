#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include "sistema.h"

using namespace std;

// ==================== INICIALIZACIÓN DEL SISTEMA ====================

SistemaOperativo* inicializarSistema() {
    SistemaOperativo* sistema = new SistemaOperativo;
    
    // Inicializar tabla de procesos
    sistema->tabla_procesos = new TablaProcesos;
    sistema->tabla_procesos->capacidad = 100;
    sistema->tabla_procesos->contador = 0;
    sistema->tabla_procesos->siguiente_pid = 1;
    sistema->tabla_procesos->procesos = new Proceso*[sistema->tabla_procesos->capacidad];
    
    // Inicializar colas de prioridad
    sistema->colas_listos = new ColaPrioridad;
    sistema->colas_listos->frente_alta = sistema->colas_listos->final_alta = nullptr;
    sistema->colas_listos->frente_media = sistema->colas_listos->final_media = nullptr;
    sistema->colas_listos->frente_baja = sistema->colas_listos->final_baja = nullptr;
    sistema->colas_listos->quantum = 4;
    
    // Inicializar gestor de memoria
    sistema->gestor_memoria = new GestorMemoria;
    inicializarMemoria(sistema->gestor_memoria, MEMORIA_TOTAL);
    
    // Inicializar otros componentes
    sistema->proceso_ejecutando = nullptr;
    sistema->reloj_sistema = 0;
    sistema->procesos_terminados = 0;
    sistema->total_procesos_creados = 0;
    
    cout << "Sistema inicializado correctamente.\n";
    return sistema;
}

void liberarSistema(SistemaOperativo* sistema) {
    // Liberar tabla de procesos
    for(int i = 0; i < sistema->tabla_procesos->contador; i++) {
        delete sistema->tabla_procesos->procesos[i];
    }
    delete[] sistema->tabla_procesos->procesos;
    delete sistema->tabla_procesos;
    
    // Liberar colas de prioridad
    delete sistema->colas_listos;
    
    // Liberar gestor de memoria
    BloqueMemoria* actual = sistema->gestor_memoria->lista_bloques;
    while(actual != nullptr) {
        BloqueMemoria* temp = actual;
        actual = actual->siguiente;
        delete temp;
    }
    delete sistema->gestor_memoria;
    
    delete sistema;
    cout << "Sistema liberado correctamente.\n";
}

// ==================== GESTIÓN DE PROCESOS ====================

Proceso* crearProceso(const char* nombre, int prioridad, int memoria) {
    Proceso* nuevo = new Proceso;
    static int pid_counter = 1;
    
    nuevo->pid = pid_counter++;
    strncpy(nuevo->nombre, nombre, MAX_NOMBRE - 1);
    nuevo->nombre[MAX_NOMBRE - 1] = '\0';
    nuevo->prioridad = prioridad;
    nuevo->estado = NUEVO;
    nuevo->memoria_requerida = memoria;
    nuevo->memoria_asignada = 0;
    nuevo->siguiente = nullptr;
    
    return nuevo;
}

void agregarProceso(TablaProcesos* tabla, Proceso* proceso) {
    if(tabla->contador >= tabla->capacidad) {
        cout << "Error: Tabla de procesos llena.\n";
        return;
    }
    
    tabla->procesos[tabla->contador] = proceso;
    tabla->contador++;
    tabla->siguiente_pid = proceso->pid + 1;
    
    cout << "Proceso '" << proceso->nombre << "' agregado con PID: " << proceso->pid << "\n";
}

void eliminarProceso(TablaProcesos* tabla, int pid) {
    int index = -1;
    
    // Buscar el proceso en la tabla
    for(int i = 0; i < tabla->contador; i++) {
        if(tabla->procesos[i]->pid == pid) {
            index = i;
            break;
        }
    }
    
    if(index == -1) {
        cout << "Error: Proceso con PID " << pid << " no encontrado.\n";
        return;
    }
    
    // Liberar memoria del proceso
    delete tabla->procesos[index];
    
    // Mover los procesos restantes
    for(int i = index; i < tabla->contador - 1; i++) {
        tabla->procesos[i] = tabla->procesos[i + 1];
    }
    
    tabla->contador--;
    cout << "Proceso con PID " << pid << " eliminado correctamente.\n";
}

Proceso* buscarProceso(TablaProcesos* tabla, int pid) {
    for(int i = 0; i < tabla->contador; i++) {
        if(tabla->procesos[i]->pid == pid) {
            return tabla->procesos[i];
        }
    }
    return nullptr;
}

Proceso* buscarProcesoPorNombre(TablaProcesos* tabla, const char* nombre) {
    for(int i = 0; i < tabla->contador; i++) {
        if(strcmp(tabla->procesos[i]->nombre, nombre) == 0) {
            return tabla->procesos[i];
        }
    }
    return nullptr;
}

void listarProcesos(TablaProcesos* tabla) {
    if(tabla->contador == 0) {
        cout << "No hay procesos en el sistema.\n";
        return;
    }
    
    cout << "========================================\n";
    cout << "          LISTA DE PROCESOS           \n";
    cout << "========================================\n";
    cout << "PID\tNombre\t\tPrioridad\tEstado\t\tMemoria\n";
    cout << "----------------------------------------\n";
    
    for(int i = 0; i < tabla->contador; i++) {
        Proceso* p = tabla->procesos[i];
        cout << p->pid << "\t" << p->nombre << "\t\t";
        
        // Prioridad
        switch(p->prioridad) {
            case ALTA: cout << "Alta\t\t"; break;
            case MEDIA: cout << "Media\t\t"; break;
            case BAJA: cout << "Baja\t\t"; break;
        }
        
        // Estado
        switch(p->estado) {
            case NUEVO: cout << "Nuevo\t\t"; break;
            case LISTO: cout << "Listo\t\t"; break;
            case EJECUTANDO: cout << "Ejecutando\t"; break;
            case TERMINADO: cout << "Terminado\t"; break;
        }
        
        cout << p->memoria_requerida << " MB\n";
    }
}

void modificarPrioridadProceso(TablaProcesos* tabla, int pid, int nueva_prioridad) {
    Proceso* proceso = buscarProceso(tabla, pid);
    if(proceso != nullptr) {
        proceso->prioridad = nueva_prioridad;
        cout << "Prioridad del proceso " << pid << " modificada correctamente.\n";
    } else {
        cout << "Error: Proceso no encontrado.\n";
    }
}

// ==================== PLANIFICACIÓN DE CPU ====================

void encolarProceso(ColaPrioridad* colas, Proceso* proceso) {
    proceso->estado = LISTO;
    
    switch(proceso->prioridad) {
        case ALTA:
            if(colas->frente_alta == nullptr) {
                colas->frente_alta = colas->final_alta = proceso;
            } else {
                colas->final_alta->siguiente = proceso;
                colas->final_alta = proceso;
            }
            break;
            
        case MEDIA:
            if(colas->frente_media == nullptr) {
                colas->frente_media = colas->final_media = proceso;
            } else {
                colas->final_media->siguiente = proceso;
                colas->final_media = proceso;
            }
            break;
            
        case BAJA:
            if(colas->frente_baja == nullptr) {
                colas->frente_baja = colas->final_baja = proceso;
            } else {
                colas->final_baja->siguiente = proceso;
                colas->final_baja = proceso;
            }
            break;
    }
    
    cout << "Proceso '" << proceso->nombre << "' encolado en prioridad ";
    switch(proceso->prioridad) {
        case ALTA: cout << "Alta"; break;
        case MEDIA: cout << "Media"; break;
        case BAJA: cout << "Baja"; break;
    }
    cout << "\n";
}

Proceso* desencolarProceso(ColaPrioridad* colas) {
    // Prioridad Alta primero
    if(colas->frente_alta != nullptr) {
        Proceso* proceso = colas->frente_alta;
        colas->frente_alta = colas->frente_alta->siguiente;
        if(colas->frente_alta == nullptr) colas->final_alta = nullptr;
        proceso->siguiente = nullptr;
        return proceso;
    }
    
    // Prioridad Media
    if(colas->frente_media != nullptr) {
        Proceso* proceso = colas->frente_media;
        colas->frente_media = colas->frente_media->siguiente;
        if(colas->frente_media == nullptr) colas->final_media = nullptr;
        proceso->siguiente = nullptr;
        return proceso;
    }
    
    // Prioridad Baja
    if(colas->frente_baja != nullptr) {
        Proceso* proceso = colas->frente_baja;
        colas->frente_baja = colas->frente_baja->siguiente;
        if(colas->frente_baja == nullptr) colas->final_baja = nullptr;
        proceso->siguiente = nullptr;
        return proceso;
    }
    
    return nullptr; // No hay procesos en cola
}

void mostrarColasPlanificacion(ColaPrioridad* colas) {
    cout << "========================================\n";
    cout << "        COLAS DE PLANIFICACION        \n";
    cout << "========================================\n";
    
    // Cola de Alta Prioridad
    cout << "ALTA PRIORIDAD:\n";
    if(colas->frente_alta == nullptr) {
        cout << "  [Vacia]\n";
    } else {
        Proceso* actual = colas->frente_alta;
        while(actual != nullptr) {
            cout << "  PID: " << actual->pid << " - " << actual->nombre << "\n";
            actual = actual->siguiente;
        }
    }
    
    // Cola de Media Prioridad
    cout << "\nMEDIA PRIORIDAD:\n";
    if(colas->frente_media == nullptr) {
        cout << "  [Vacia]\n";
    } else {
        Proceso* actual = colas->frente_media;
        while(actual != nullptr) {
            cout << "  PID: " << actual->pid << " - " << actual->nombre << "\n";
            actual = actual->siguiente;
        }
    }
    
    // Cola de Baja Prioridad
    cout << "\nBAJA PRIORIDAD:\n";
    if(colas->frente_baja == nullptr) {
        cout << "  [Vacia]\n";
    } else {
        Proceso* actual = colas->frente_baja;
        while(actual != nullptr) {
            cout << "  PID: " << actual->pid << " - " << actual->nombre << "\n";
            actual = actual->siguiente;
        }
    }
}

// ==================== GESTIÓN DE MEMORIA ====================

void inicializarMemoria(GestorMemoria* gestor, int memoria_total) {
    gestor->memoria_total = memoria_total;
    gestor->memoria_libre = memoria_total;
    gestor->politica = 0; // First-fit por defecto
    
    // Crear bloque inicial de memoria libre
    gestor->lista_bloques = new BloqueMemoria;
    gestor->lista_bloques->direccion_base = 0;
    gestor->lista_bloques->tamaño = memoria_total;
    gestor->lista_bloques->libre = 1;
    gestor->lista_bloques->pid_asignado = -1;
    gestor->lista_bloques->siguiente = nullptr;
}

int asignarMemoria(GestorMemoria* gestor, int tamaño, int pid) {
    if(tamaño > gestor->memoria_libre) {
        cout << "Error: Memoria insuficiente. Requerido: " << tamaño 
             << " MB, Disponible: " << gestor->memoria_libre << " MB\n";
        return -1;
    }
    
    BloqueMemoria* actual = gestor->lista_bloques;
    BloqueMemoria* mejor_bloque = nullptr;
    BloqueMemoria* anterior = nullptr;
    
    // Buscar bloque según política
    while(actual != nullptr) {
        if(actual->libre && actual->tamaño >= tamaño) {
            if(gestor->politica == 0) { // First-fit
                mejor_bloque = actual;
                break;
            } else if(gestor->politica == 1) { // Best-fit
                if(mejor_bloque == nullptr || actual->tamaño < mejor_bloque->tamaño) {
                    mejor_bloque = actual;
                    anterior = anterior;
                }
            } else if(gestor->politica == 2) { // Worst-fit
                if(mejor_bloque == nullptr || actual->tamaño > mejor_bloque->tamaño) {
                    mejor_bloque = actual;
                    anterior = anterior;
                }
            }
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    
    if(mejor_bloque == nullptr) {
        cout << "Error: No hay bloque de memoria contigua suficiente.\n";
        return -1;
    }
    
    // Asignar memoria
    if(mejor_bloque->tamaño == tamaño) {
        // Usar el bloque completo
        mejor_bloque->libre = 0;
        mejor_bloque->pid_asignado = pid;
    } else {
        // Dividir el bloque
        BloqueMemoria* nuevo_bloque = new BloqueMemoria;
        nuevo_bloque->direccion_base = mejor_bloque->direccion_base + tamaño;
        nuevo_bloque->tamaño = mejor_bloque->tamaño - tamaño;
        nuevo_bloque->libre = 1;
        nuevo_bloque->pid_asignado = -1;
        nuevo_bloque->siguiente = mejor_bloque->siguiente;
        
        mejor_bloque->tamaño = tamaño;
        mejor_bloque->libre = 0;
        mejor_bloque->pid_asignado = pid;
        mejor_bloque->siguiente = nuevo_bloque;
    }
    
    gestor->memoria_libre -= tamaño;
    cout << "Memoria asignada: " << tamaño << " MB al proceso " << pid 
         << " en direccion " << mejor_bloque->direccion_base << "\n";
    
    return mejor_bloque->direccion_base;
}

int liberarMemoria(GestorMemoria* gestor, int pid) {
    BloqueMemoria* actual = gestor->lista_bloques;
    BloqueMemoria* anterior = nullptr;
    int memoria_liberada = 0;
    
    while(actual != nullptr) {
        if(!actual->libre && actual->pid_asignado == pid) {
            memoria_liberada += actual->tamaño;
            actual->libre = 1;
            actual->pid_asignado = -1;
            cout << "Bloque liberado: " << actual->tamaño << " MB de PID " << pid << "\n";
        }
        anterior = actual;
        actual = actual->siguiente;
    }
    
    if(memoria_liberada > 0) {
        gestor->memoria_libre += memoria_liberada;
        cout << "Total liberado: " << memoria_liberada << " MB\n";
    } else {
        cout << "No se encontraron bloques asignados al PID " << pid << "\n";
    }
    
    return memoria_liberada;
}

void mostrarEstadoMemoria(GestorMemoria* gestor) {
    cout << "========================================\n";
    cout << "          ESTADO DE MEMORIA           \n";
    cout << "========================================\n";
    cout << "Memoria total: " << gestor->memoria_total << " MB\n";
    cout << "Memoria libre: " << gestor->memoria_libre << " MB\n";
    cout << "Memoria usada: " << (gestor->memoria_total - gestor->memoria_libre) << " MB\n";
    cout << "Politica: ";
    switch(gestor->politica) {
        case 0: cout << "First-fit"; break;
        case 1: cout << "Best-fit"; break;
        case 2: cout << "Worst-fit"; break;
    }
    cout << "\n\nBloques de memoria:\n";
    cout << "Direccion\tTamaño\tEstado\tPID\n";
    cout << "----------------------------------------\n";
    
    BloqueMemoria* actual = gestor->lista_bloques;
    while(actual != nullptr) {
        cout << actual->direccion_base << "\t\t" << actual->tamaño << " MB\t";
        cout << (actual->libre ? "Libre" : "Ocupado") << "\t";
        if(!actual->libre) {
            cout << actual->pid_asignado;
        } else {
            cout << "-";
        }
        cout << "\n";
        actual = actual->siguiente;
    }
}

// ==================== PERSISTENCIA ====================

void guardarEstadoSistema(SistemaOperativo* sistema, const char* archivo) {
    ofstream file(archivo, ios::binary);
    if(!file) {
        cout << "Error: No se pudo crear el archivo.\n";
        return;
    }
    
    // Guardar datos básicos del sistema
    file.write((char*)&sistema->reloj_sistema, sizeof(int));
    file.write((char*)&sistema->procesos_terminados, sizeof(int));
    file.write((char*)&sistema->total_procesos_creados, sizeof(int));
    
    // Guardar tabla de procesos
    file.write((char*)&sistema->tabla_procesos->contador, sizeof(int));
    for(int i = 0; i < sistema->tabla_procesos->contador; i++) {
        file.write((char*)sistema->tabla_procesos->procesos[i], sizeof(Proceso));
    }
    
    file.close();
    cout << "Estado del sistema guardado en '" << archivo << "'\n";
}

void cargarEstadoSistema(SistemaOperativo* sistema, const char* archivo) {
    ifstream file(archivo, ios::binary);
    if(!file) {
        cout << "Error: No se pudo abrir el archivo.\n";
        return;
    }
    
    // Cargar datos básicos del sistema
    file.read((char*)&sistema->reloj_sistema, sizeof(int));
    file.read((char*)&sistema->procesos_terminados, sizeof(int));
    file.read((char*)&sistema->total_procesos_creados, sizeof(int));
    
    // Cargar tabla de procesos
    int contador;
    file.read((char*)&contador, sizeof(int));
    
    for(int i = 0; i < contador; i++) {
        Proceso* proceso = new Proceso;
        file.read((char*)proceso, sizeof(Proceso));
        sistema->tabla_procesos->procesos[i] = proceso;
    }
    sistema->tabla_procesos->contador = contador;
    
    file.close();
    cout << "Estado del sistema cargado desde '" << archivo << "'\n";
}

// ==================== MENÚS DE INTERFAZ ====================

void agregarProcesoMenu(SistemaOperativo* sistema) {
    char nombre[MAX_NOMBRE];
    int prioridad, memoria;
    
    cout << "========================================\n";
    cout << "          AGREGAR NUEVO PROCESO       \n";
    cout << "========================================\n";
    
    cout << "Nombre del proceso: ";
    cin.ignore();
    cin.getline(nombre, MAX_NOMBRE);
    
    cout << "Prioridad (0: Alta, 1: Media, 2: Baja): ";
    cin >> prioridad;
    
    if(prioridad < 0 || prioridad > 2) {
        cout << "Error: Prioridad no válida.\n";
        return;
    }
    
    cout << "Memoria requerida (MB): ";
    cin >> memoria;
    
    if(memoria <= 0) {
        cout << "Error: Memoria debe ser mayor a 0.\n";
        return;
    }
    
    Proceso* nuevo = crearProceso(nombre, prioridad, memoria);
    agregarProceso(sistema->tabla_procesos, nuevo);
    sistema->total_procesos_creados++;
}

void eliminarProcesoMenu(SistemaOperativo* sistema) {
    int pid;
    
    cout << "========================================\n";
    cout << "           ELIMINAR PROCESO           \n";
    cout << "========================================\n";
    
    cout << "PID del proceso a eliminar: ";
    cin >> pid;
    
    // Liberar memoria primero
    liberarMemoria(sistema->gestor_memoria, pid);
    eliminarProceso(sistema->tabla_procesos, pid);
}

void buscarProcesoMenu(SistemaOperativo* sistema) {
    int opcion;
    char nombre[MAX_NOMBRE];
    int pid;
    Proceso* encontrado = nullptr;
    
    cout << "========================================\n";
    cout << "            BUSCAR PROCESO            \n";
    cout << "========================================\n";
    cout << "1. Buscar por PID\n";
    cout << "2. Buscar por nombre\n";
    cout << "Seleccione opcion: ";
    cin >> opcion;
    
    if(opcion == 1) {
        cout << "Ingrese PID: ";
        cin >> pid;
        encontrado = buscarProceso(sistema->tabla_procesos, pid);
    } else if(opcion == 2) {
        cout << "Ingrese nombre: ";
        cin.ignore();
        cin.getline(nombre, MAX_NOMBRE);
        encontrado = buscarProcesoPorNombre(sistema->tabla_procesos, nombre);
    } else {
        cout << "Opción no válida.\n";
        return;
    }
    
    if(encontrado != nullptr) {
        cout << "\nPROCESO ENCONTRADO:\n";
        cout << "PID: " << encontrado->pid << "\n";
        cout << "Nombre: " << encontrado->nombre << "\n";
        cout << "Prioridad: ";
        switch(encontrado->prioridad) {
            case ALTA: cout << "Alta"; break;
            case MEDIA: cout << "Media"; break;
            case BAJA: cout << "Baja"; break;
        }
        cout << "\nEstado: ";
        switch(encontrado->estado) {
            case NUEVO: cout << "Nuevo"; break;
            case LISTO: cout << "Listo"; break;
            case EJECUTANDO: cout << "Ejecutando"; break;
            case TERMINADO: cout << "Terminado"; break;
        }
        cout << "\nMemoria requerida: " << encontrado->memoria_requerida << " MB\n";
    } else {
        cout << "Proceso no encontrado.\n";
    }
}

void encolarProcesoMenu(SistemaOperativo* sistema) {
    int pid;
    
    cout << "========================================\n";
    cout << "         ENCOLAR PROCESO PARA EJECUCION\n";
    cout << "========================================\n";
    
    cout << "PID del proceso a encolar: ";
    cin >> pid;
    
    Proceso* proceso = buscarProceso(sistema->tabla_procesos, pid);
    if(proceso != nullptr) {
        // Asignar memoria antes de encolar
        if(proceso->memoria_asignada == 0) {
            int direccion = asignarMemoria(sistema->gestor_memoria, 
                                         proceso->memoria_requerida, proceso->pid);
            if(direccion != -1) {
                proceso->memoria_asignada = proceso->memoria_requerida;
            } else {
                cout << "Error: No se pudo asignar memoria al proceso.\n";
                return;
            }
        }
        
        encolarProceso(sistema->colas_listos, proceso);
    } else {
        cout << "Error: Proceso no encontrado.\n";
    }
}

void asignarMemoriaMenu(SistemaOperativo* sistema) {
    int pid, memoria;
    
    cout << "========================================\n";
    cout << "          ASIGNAR MEMORIA A PROCESO   \n";
    cout << "========================================\n";
    
    cout << "PID del proceso: ";
    cin >> pid;
    cout << "Memoria a asignar (MB): ";
    cin >> memoria;
    
    Proceso* proceso = buscarProceso(sistema->tabla_procesos, pid);
    if(proceso != nullptr) {
        int direccion = asignarMemoria(sistema->gestor_memoria, memoria, pid);
        if(direccion != -1) {
            proceso->memoria_asignada += memoria;
        }
    } else {
        cout << "Error: Proceso no encontrado.\n";
    }
}

void liberarMemoriaMenu(SistemaOperativo* sistema) {
    int pid;
    
    cout << "========================================\n";
    cout << "           LIBERAR MEMORIA            \n";
    cout << "========================================\n";
    
    cout << "PID del proceso: ";
    cin >> pid;
    
    Proceso* proceso = buscarProceso(sistema->tabla_procesos, pid);
    if(proceso != nullptr) {
        int liberada = liberarMemoria(sistema->gestor_memoria, pid);
        if(liberada > 0) {
            proceso->memoria_asignada = 0;
        }
    } else {
        cout << "Error: Proceso no encontrado.\n";
    }
}

void mostrarEstadisticas(SistemaOperativo* sistema) {
    cout << "========================================\n";
    cout << "           ESTADISTICAS DEL SISTEMA   \n";
    cout << "========================================\n";
    cout << "Tiempo del sistema: " << sistema->reloj_sistema << " unidades\n";
    cout << "Total procesos creados: " << sistema->total_procesos_creados << "\n";
    cout << "Procesos activos: " << sistema->tabla_procesos->contador << "\n";
    cout << "Procesos terminados: " << sistema->procesos_terminados << "\n";
    cout << "Memoria total: " << sistema->gestor_memoria->memoria_total << " MB\n";
    cout << "Memoria en uso: " << (sistema->gestor_memoria->memoria_total - 
                                 sistema->gestor_memoria->memoria_libre) << " MB\n";
    cout << "Memoria libre: " << sistema->gestor_memoria->memoria_libre << " MB\n";
    
    if(sistema->proceso_ejecutando != nullptr) {
        cout << "Proceso en ejecucion: " << sistema->proceso_ejecutando->nombre 
             << " (PID: " << sistema->proceso_ejecutando->pid << ")\n";
    } else {
        cout << "Proceso en ejecucion: Ninguno\n";
    }
}

void ejecutarProceso(SistemaOperativo* sistema) {
    Proceso* proceso = desencolarProceso(sistema->colas_listos);
    
    if(proceso != nullptr) {
        sistema->proceso_ejecutando = proceso;
        proceso->estado = EJECUTANDO;
        
        cout << "========================================\n";
        cout << "         EJECUTANDO PROCESO           \n";
        cout << "========================================\n";
        cout << "Proceso: " << proceso->nombre << " (PID: " << proceso->pid << ")\n";
        cout << "Prioridad: ";
        switch(proceso->prioridad) {
            case ALTA: cout << "Alta"; break;
            case MEDIA: cout << "Media"; break;
            case BAJA: cout << "Baja"; break;
        }
        cout << "\nMemoria asignada: " << proceso->memoria_asignada << " MB\n";
        cout << "Tiempo de ejecucion simulado...\n";
        
        // Simular ejecución
        sistema->reloj_sistema += sistema->colas_listos->quantum;
        
        // Terminar proceso después de ejecución
        proceso->estado = TERMINADO;
        liberarMemoria(sistema->gestor_memoria, proceso->pid);
        sistema->procesos_terminados++;
        sistema->proceso_ejecutando = nullptr;
        
        cout << "Proceso terminado correctamente.\n";
    } else {
        cout << "Error: No hay procesos en cola para ejecutar.\n";
    }
}