#include <mysql.h>
#include <iostream>
#include <sstream>

using namespace std;

class Marcas {
private:
    MYSQL* conector;

public:
    Marcas(MYSQL* con);
    void crearMarca(int idMarca, const string& marca);
    void mostrarMarcas();
    void modificarMarca(int idMarca, const string& nuevaMarca);
    void eliminarMarca(int idMarca);
    void menuMarcas(Marcas& marcas) {
        int opc, idMarca;
        string marca;

        while (true) {
            cout << "\n\t-------MENU MARCAS-------";
            cout << "\n\t1. Crear Marca";
            cout << "\n\t2. Mostrar Marcas";
            cout << "\n\t3. Modificar Marca";
            cout << "\n\t4. Eliminar Marca";
            cout << "\n\t5. Salir";
            cout << "\n\n\tSeleccione una opcion: ";
            cin >> opc;

            switch (opc) {
            case 1:
                cout << "Digite el ID de la Marca: ";
                cin >> idMarca;
                cout << "Digite el nombre de la Marca: ";
                cin.ignore();
                getline(cin, marca);
                marcas.crearMarca(idMarca, marca);
                system("pause");
                system("cls");
                break;
            case 2:
                marcas.mostrarMarcas();
                system("pause");
                system("cls");
                break;
            case 3:
                cout << "Digite el ID de la Marca a modificar: ";
                cin >> idMarca;
                cout << "Digite el nuevo nombre de la Marca: ";
                cin.ignore();
                getline(cin, marca);
                marcas.modificarMarca(idMarca, marca);
                system("pause");
                system("cls");
                break;
            case 4:
                cout << "Digite el ID de la Marca a eliminar: ";
                cin >> idMarca;
                marcas.eliminarMarca(idMarca);
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

Marcas::Marcas(MYSQL* con) : conector(con) {}

void Marcas::crearMarca(int idMarca, const string& marca) {
    stringstream ss;
    ss << "INSERT INTO marcas (idMarca, marca) VALUES (" << idMarca << ", '" << marca << "')";
    string query = ss.str();
    if (mysql_query(conector, query.c_str()) == 0) {
        cout << "Marca creada exitosamente." << endl;
    }
    else {
        cout << "Error al crear marca: " << mysql_error(conector) << endl;
    }
}

void Marcas::mostrarMarcas() {
    if (mysql_query(conector, "SELECT * FROM marcas") == 0) {
        MYSQL_RES* res = mysql_store_result(conector);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0] << " Marca: " << row[1] << endl;
        }
        mysql_free_result(res);
    }
    else {
        cout << "Error al mostrar marcas: " << mysql_error(conector) << endl;
    }
}

void Marcas::modificarMarca(int idMarca, const string& nuevaMarca) {
    stringstream ss;
    ss << "UPDATE marcas SET marca = '" << nuevaMarca << "' WHERE idMarca = " << idMarca;
    string query = ss.str();
    if (mysql_query(conector, query.c_str()) == 0) {
        cout << "Marca modificada exitosamente." << endl;
    }
    else {
        cout << "Error al modificar marca: " << mysql_error(conector) << endl;
    }
}

void Marcas::eliminarMarca(int idMarca) {
    stringstream ss;
    ss << "DELETE FROM marcas WHERE idMarca = " << idMarca;
    string query = ss.str();
    if (mysql_query(conector, query.c_str()) == 0) {
        cout << "Marca eliminada exitosamente." << endl;
    }
    else {
        cout << "Error al eliminar marca: " << mysql_error(conector) << endl;
    }
}