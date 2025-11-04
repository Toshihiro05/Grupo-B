#include <iostream>
#include <cstdlib>
#include <cstring>
#include "sistema.h"

using namespace std;

void mostrarMenuPrincipal();

int main() {
    SistemaOperativo* sistema = inicializarSistema();
    int opcion;
    
    do {
        mostrarMenuPrincipal();
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        
        switch(opcion) {
            case 1:
                agregarProcesoMenu(sistema);
                break;
            case 2:
                eliminarProcesoMenu(sistema);
                break;
            case 3:
                buscarProcesoMenu(sistema);
                break;
            case 4:
                listarProcesos(sistema->tabla_procesos);
                break;
            case 5:
                encolarProcesoMenu(sistema);
                break;
            case 6:
                ejecutarProceso(sistema);
                break;
            case 7:
                mostrarColasPlanificacion(sistema->colas_listos);
                break;
            case 8:
                asignarMemoriaMenu(sistema);
                break;
            case 9:
                liberarMemoriaMenu(sistema);
                break;
            case 10:
                mostrarEstadoMemoria(sistema->gestor_memoria);
                break;
            case 11:
                guardarEstadoSistema(sistema, "estado_sistema.dat");
                break;
            case 12:
                cargarEstadoSistema(sistema, "estado_sistema.dat");
                break;
            case 13:
                mostrarEstadisticas(sistema);
                break;
            case 0:
                cout << "Saliendo del sistema...\n";
                break;
            default:
                cout << "Opcion no valida. Intente nuevamente.\n";
        }
        
        cout << "\nPresione Enter para continuar...";
        cin.ignore();
        cin.get();
        
    } while(opcion != 0);
    
    liberarSistema(sistema);
    return 0;
}

void mostrarMenuPrincipal() {
    system("cls");
    cout << "========================================\n";
    cout << "     SISTEMA DE GESTION DE PROCESOS    \n";
    cout << "========================================\n";
    cout << "1. Agregar proceso\n";
    cout << "2. Eliminar proceso\n";
    cout << "3. Buscar proceso\n";
    cout << "4. Listar todos los procesos\n";
    cout << "5. Encolar proceso para ejecucion\n";
    cout << "6. Ejecutar siguiente proceso\n";
    cout << "7. Mostrar colas de planificacion\n";
    cout << "8. Asignar memoria a proceso\n";
    cout << "9. Liberar memoria\n";
    cout << "10. Mostrar estado de memoria\n";
    cout << "11. Guardar estado del sistema\n";
    cout << "12. Cargar estado del sistema\n";
    cout << "13. Mostrar estadisticas\n";
    cout << "0. Salir\n";
    cout << "========================================\n";
}