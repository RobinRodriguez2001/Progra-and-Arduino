#include <iostream>
#include <mysql.h>
#include <sstream>

using namespace std;

// Declaración de la clase Clientes
class Clientes {
private:
    MYSQL* conector;

public:
    Clientes(MYSQL* con);
    void crearCliente(int idCliente, const string& nombre, const string& apellido, const string& nit, bool genero, const string& telefono, const string& correo, const string& fechaIngreso);
    void mostrarClientes();
    void modificarCliente(int idCliente, const string& nuevoNombre, const string& nuevoApellido, const string& nit, const string& genero, const string& telefono, const string& correo, const string& fechaIngreso);
    void eliminarCliente(int idCliente);
    void menuClientes(Clientes& clientes) {
        int opc, idCliente;
        string nombre, apellido, nit, telefono, correo, fechaIngreso;
        bool genero;

        while (true) {
            cout << "\n\t-------MENU CLIENTES-------";
            cout << "\n\t1. Crear Cliente";
            cout << "\n\t2. Mostrar Clientes";
            cout << "\n\t3. Modificar Cliente";
            cout << "\n\t4. Eliminar Cliente";
            cout << "\n\t5. Salir";
            cout << "\n\n\tSeleccione una opcion: ";
            cin >> opc;

            switch (opc) {
            case 1:
                system("cls");
                cout << "------------------CLIENTES--------------------\n";
                cout << "Digite el ID del Cliente: ";
                cin >> idCliente;
                cout << "Digite el nombre del Cliente: ";
                cin.ignore();
                getline(cin, nombre);
                cout << "Digite el apellido del Cliente: ";
                getline(cin, apellido);
                cout << "Digite el NIT del Cliente: ";
                getline(cin, nit);
                cout << "Digite el genero del Cliente (0 para masculino, 1 para femenino): ";
                cin >> genero;
                cout << "Digite el telefono del Cliente: ";
                cin.ignore();
                getline(cin, telefono);
                cout << "Digite el correo del Cliente: ";
                getline(cin, correo);
                clientes.crearCliente(idCliente, nombre, apellido, nit, genero, telefono, correo, fechaIngreso);
                system("cls");
                system("pause");
                break;
            case 2:
                system("cls");
                clientes.mostrarClientes();
                system("pause");
                system("cls");
                break;
            case 3:
                system("cls");
                cout << "Digite el ID del Cliente a modificar: ";
                cin >> idCliente;
                cout << "Digite el nuevo nombre del Cliente: ";
                cin.ignore();
                getline(cin, nombre);
                cout << "Digite el nuevo apellido del Cliente: ";
                getline(cin, apellido);
                cout << "Digite el nuevo NIT del Cliente: ";
                getline(cin, nit);
                cout << "Digite el nuevo genero del Cliente (0 para masculino, 1 para femenino): ";
                cin >> genero;
                cout << "Digite el nuevo teléfono del Cliente: ";
                cin.ignore();
                getline(cin, telefono);
                cout << "Digite el nuevo correo del Cliente: ";
                getline(cin, correo);
                cout << "Digite la nueva fecha de ingreso del Cliente (YYYY-MM-DD): ";
                getline(cin, fechaIngreso);
                clientes.modificarCliente(idCliente, nombre, apellido, nit, to_string(genero), telefono, correo, fechaIngreso);
                system("pause");
                system("cls");
                break;
            case 4:
                cout << "Digite el ID del Cliente a eliminar: ";
                cin >> idCliente;
                clientes.eliminarCliente(idCliente);
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

// Implementación de los métodos de la clase Clientes
Clientes::Clientes(MYSQL* con) : conector(con) {}

void Clientes::crearCliente(int idCliente, const string& nombre, const string& apellido, const string& nit, bool genero, const string& telefono, const string& correo, const string& fechaIngreso) {
    stringstream ss;
    ss << "INSERT INTO Clientes (idCliente, nombres, apellidos, NIT, genero, telefono, correo_electronico, fechaingreso) VALUES ("
        << idCliente << ", '" << nombre << "', '" << apellido << "', '" << nit << "', " << (genero ? 1 : 0) << ", '" << telefono << "', '" << correo << "', NOW())";
    string query = ss.str();
    if (mysql_query(conector, query.c_str()) == 0) {
        cout << "Cliente creado exitosamente." << endl;
    }
    else {
        cout << "Error al crear cliente: " << mysql_error(conector) << endl;
    }
}

void Clientes::mostrarClientes() {
    if (mysql_query(conector, "SELECT * FROM Clientes") == 0) {
        MYSQL_RES* res = mysql_store_result(conector);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << " \nID: " << row[0] << endl;
            cout << " Nombre: " << row[1] << endl;
            cout << " Apellido: " << row[2] << endl;
            cout << " NIT: " << row[3] << endl;
            cout << " Genero: " << row[4] << endl;
            cout << " Telefono: " << row[5] << endl;
            cout << " Correo: " << row[6] << endl;
            cout << " Fecha de Ingreso: " << row[7] << endl;
            cout << "\n-----------------------------------------\n";
        }
        mysql_free_result(res);
    }
    else {
        cout << "Error al mostrar clientes: " << mysql_error(conector) << endl;
    }
}

void Clientes::modificarCliente(int idCliente, const string& nuevoNombre, const string& nuevoApellido, const string& nit, const string& genero, const string& telefono, const string& correo, const string& fechaIngreso) {
    stringstream ss;
    ss << "UPDATE Clientes SET nombres = '" << nuevoNombre << "', apellidos = '" << nuevoApellido << "', NIT = '" << nit
        << "', genero = '" << genero << "', telefono = '" << telefono << "', correo_electronico = '" << correo
        << "', fechaingreso = '" << fechaIngreso << "' WHERE idCliente = " << idCliente;
    string query = ss.str();
    if (mysql_query(conector, query.c_str()) == 0) {
        cout << "Cliente modificado exitosamente." << endl;
    }
    else {
        cout << "Error al modificar cliente: " << mysql_error(conector) << endl;
    }
}

void Clientes::eliminarCliente(int idCliente) {
    // Iniciar transacción
    mysql_autocommit(conector, 0);

    // Actualizar ventas asociadas al cliente
    string queryActualizarVentas = "UPDATE ventas SET idCliente = 10 WHERE idCliente = " + to_string(idCliente);
    if (mysql_query(conector, queryActualizarVentas.c_str())) {
        cerr << "Error al modificar ventas del cliente: " << mysql_error(conector) << endl;
        mysql_rollback(conector);
        return;
    }

    // Eliminar el cliente
    string queryEliminarCliente = "DELETE FROM Clientes WHERE idCliente = " + to_string(idCliente);
    if (mysql_query(conector, queryEliminarCliente.c_str())) {
        cerr << "Error al eliminar cliente: " << mysql_error(conector) << endl;
        mysql_rollback(conector);
        return;
    }

    // Commit de la transacción
    mysql_commit(conector);
    cout << "Cliente y ventas asociadas actualizadas correctamente." << endl;

    // Restaurar autocommit
    mysql_autocommit(conector, 1);
}