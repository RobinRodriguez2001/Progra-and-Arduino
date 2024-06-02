#include <mysql.h>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class Proveedores {
private:
    MYSQL* conector;

public:
    Proveedores(MYSQL* con);
    void crearProveedor(int idProveedor, const string& proveedor, const string& nit, const string& direccion, const string& telefono);
    void mostrarProveedores();
    void modificarProveedor(int idProveedor, const string& proveedor, const string& nit, const string& direccion, const string& telefono);
    void eliminarProveedor(int idProveedor);
    void menuProveedores(Proveedores& proveedores) {
        int opc, idProveedor;
        string proveedor, nit, direccion, telefono;

        while (true) {
            cout << "\n\t-------MENU PROVEEDORES-------";
            cout << "\n\t1. Crear Proveedor";
            cout << "\n\t2. Mostrar Proveedor";
            cout << "\n\t3. Modificar Proveedor";
            cout << "\n\t4. Eliminar Proveedor";
            cout << "\n\t5. Salir";
            cout << "\n\n\tSeleccione una opcion: ";
            cin >> opc;

            switch (opc) {
            case 1:
                cout << "Digite el ID del Producto: ";
                cin >> idProveedor;
                cin.ignore();
                cout << "Digite el nombre del Proveedor: ";
                getline(cin, proveedor);
                cout << "Digite el Nit: ";
                getline(cin, nit);
                cout << "Digite la Direccion del Proveedor: ";
                getline(cin, direccion);
                cout << "Digite el No. de Telefono del Proveedor: ";
                getline(cin, telefono);
                proveedores.crearProveedor(idProveedor, proveedor, nit, direccion, telefono);
                system("pause");
                system("cls");
                break;
            case 2:
                proveedores.mostrarProveedores();
                system("pause");
                system("cls");
                break;
            case 3:
                cout << "Digite el ID del Cliente a modificar: ";
                cin >> idProveedor;
                cout << "Digite el nuevo nombre del Proveedor: ";
                cin.ignore();
                getline(cin, proveedor);
                cout << "Digite el nuevo NIT del Proveedor: ";
                getline(cin, nit);
                cout << "Digite la nuevo direccion del Proveedor: ";
                getline(cin, direccion);
                cout << "Digite la nuevo telefono del Cliente: ";
                getline(cin, telefono);
                proveedores.modificarProveedor(idProveedor, proveedor, nit, direccion, telefono);
                system("pause");
                system("cls");
                break;
            case 4:
                cout << "Digite el ID del Proveedor a eliminar: ";
                cin >> idProveedor;
                proveedores.eliminarProveedor(idProveedor);
                system("pause");
                system("cls");
                break;
            case 5:
                return;
            default:
                cout << "Opcion no valida, intente de nuevo." << endl;
            }
        }
    }
};

Proveedores::Proveedores(MYSQL* con) : conector(con) {}

void Proveedores::crearProveedor(int idProveedor, const string& proveedor, const string& nit, const string& direccion, const string& telefono) {
    stringstream ss;
    ss << "INSERT INTO proveedores (idProveedor, proveedor, nit, direccion, telefono) VALUES ("
        << idProveedor << ", '" << proveedor << "', '" << nit << "', '" << direccion << "', '" << telefono << "')";
    string query = ss.str();
    if (mysql_query(conector, query.c_str()) == 0) {
        cout << "Proveedor creado exitosamente." << endl;
    }
    else {
        cout << "Error al crear proveedor: " << mysql_error(conector) << endl;
    }
}

void Proveedores::mostrarProveedores() {
    if (mysql_query(conector, "SELECT * FROM proveedores") == 0) {
        MYSQL_RES* res = mysql_store_result(conector);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            
            cout << " \nID: " << row[0] << endl;
            cout << " Proveedor: " << row[1] << endl;
            cout << " NIT: " << row[2] << endl;
            cout << " Dirección: " << row[3] << endl;
            cout << " Telefono:  " << row[4] << endl;
        }
        mysql_free_result(res);
    }
    else {
        cout << "Error al mostrar proveedores: " << mysql_error(conector) << endl;
    }
}

void Proveedores::modificarProveedor(int idProveedor, const string& proveedor, const string& nit, const string& direccion, const string& telefono) {
    stringstream ss;
    ss << "UPDATE proveedores SET proveedor = '" << proveedor << "', nit = '" << nit << "', direccion = '" << direccion
        << "', telefono = '" << telefono << "' WHERE idProveedor = " << idProveedor;
    string query = ss.str();
    if (mysql_query(conector, query.c_str()) == 0) {
        cout << "Proveedor modificado exitosamente." << endl;
    }
    else {
        cout << "Error al modificar proveedor: " << mysql_error(conector) << endl;
    }
}

void Proveedores::eliminarProveedor(int idProveedor) {
    stringstream ss;
    ss << "DELETE FROM proveedores WHERE idProveedor = " << idProveedor;
    string query = ss.str();
    if (mysql_query(conector, query.c_str()) == 0) {
        cout << "Proveedor eliminado exitosamente." << endl;
    }
    else {
        cout << "Error al eliminar proveedor: " << mysql_error(conector) << endl;
    }
}