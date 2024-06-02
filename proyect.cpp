#include <mysql.h>
#include <iostream>
#include "Clientes.cpp"
#include "Puestos.cpp"
#include "Empleados.cpp"
#include "Productos.cpp"
#include "Marcas.cpp"
#include "Proveedores.cpp"
#include "Compra_maestro.cpp"
#include "Venta_maestro.cpp"

using namespace std;


// Función para conectar a la base de datos
MYSQL* conectarDB() {
    MYSQL* conector = mysql_init(0);
    conector = mysql_real_connect(conector, "localhost", "root", "danigero", "proyect_data_base", 3306, NULL, 0);
    if (conector == nullptr) {
        cout << "Error al conectar la base de datos: " << mysql_error(conector) << endl;
    }
    return conector;
}

// Función para desconectar de la base de datos
void desconectarDB(MYSQL* conector) {
    if (conector) {
        mysql_close(conector);
    }
}


// Función principal
int main() {
    // Conectar a la base de datos
    MYSQL* con = conectarDB();
    if (!con) {
        cerr << "Error: No se pudo conectar a la base de datos." << endl;
        return 1;
    }

    Puestos puestos(con);
    Clientes clientes(con);
    Empleados empleados(con);
    Productos productos(con);
    Marcas marcas(con);
    Proveedores proveedores(con);
    CompraMaestro compramaestro(con);
    VentaMaestro ventaMaestro(con);



    int opc;
    while (true) {
        cout << "\n\t----------MENU----------";
        cout << "\n\n\t1. Gestion de Puestos";
        cout << "\n\t2. Gestion de Clientes";
        cout << "\n\t3. Gestion de Empleados";
        cout << "\n\t4. Gestion de Productos";
        cout << "\n\t5. Gestion de Marcas";
        cout << "\n\t6. Gestion de Proveedores";
        cout << "\n\t7. Gestion de Ventas";
        cout << "\n\t8. Gestion de Compras";
        cout << "\n\t9. Salir";
        cout << "\n\n\tSeleccione una opcion: ";
        cin >> opc;

        switch (opc) {
        case 1:
            system("cls");
            puestos.menuPuestos(puestos);
            system("pause");
            system("cls");
            break;
        case 2:
            system("cls");
            clientes.menuClientes(clientes);
            system("pause");
            system("cls");
            break;
        case 3:
            system("cls");
            empleados.menuEmpleados(empleados, puestos);
            system("pause");
            system("cls");
            break;
        case 4:
            system("cls");
            productos.menuProductos(productos);
            system("pause");
            system("cls");
            break;
        case 5:
            system("cls");
            marcas.menuMarcas(marcas);
            system("pause");
            system("cls");
            break;
        case 6:
            system("cls");
            proveedores.menuProveedores(proveedores);
            system("pause");
            system("cls");
            break;
        case 7:
            system("cls");
            ventaMaestro.menuVentaMaestro(ventaMaestro);
            system("pause");
            system("cls");
            break;
        case 8:
            system("cls");
            compramaestro.menuCompras(compramaestro);
            system("pause");
            system("cls");
            break;
        case 9:
            desconectarDB(con);
            system("cls");
            exit(1);
        default:
            cout << "\nOpcion no valida, intente de nuevo." << endl;
            system("pause");
            system("cls");
            break;
        }
    }

    return 0;
};

