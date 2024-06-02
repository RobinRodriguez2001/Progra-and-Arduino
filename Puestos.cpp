#include <iostream>
#include <mysql.h>
#include <sstream>

using namespace std;

class Puestos {
private:
    MYSQL* conector;

public:
    Puestos(MYSQL* con);
    void crearPuesto(const string& puesto);
    void mostrarPuestos();
    void modificarPuesto(int idPuesto, const string& nuevoPuesto);
    void eliminarPuesto(int idPuesto);
    void menuPuestos(Puestos& puestos) {
        int opc, idPuesto;
        string puesto;

        while (true) {
            cout << "\n\t-------MENU PUESTOS-------";
            cout << "\n\t1. Crear Puesto";
            cout << "\n\t2. Mostrar Puestos";
            cout << "\n\t3. Modificar Puesto";
            cout << "\n\t4. Eliminar Puesto";
            cout << "\n\t5. Salir";
            cout << "\n\n\tSeleccione una opcion: ";
            cin >> opc;

            switch (opc) {
            case 1: {
                string puesto;
                cout << "Ingrese el nombre del puesto: ";
                cin.ignore();
                getline(cin, puesto);
                puestos.crearPuesto(puesto);
                system("pause");
                system("cls");
                break;
            }
            case 2:
                puestos.mostrarPuestos();
                system("pause");
                system("cls");
                break;
            case 3: {
                string nuevoPuesto;
                cout << "Ingrese ID del puesto a modificar: ";
                cin >> idPuesto;
                cout << "Ingrese el nuevo nombre del puesto: ";
                cin.ignore();
                getline(cin, nuevoPuesto);
                puestos.modificarPuesto(idPuesto, nuevoPuesto);
                system("pause");
                system("cls");
                break;
            }
            case 4: {
                cout << "Ingrese ID del puesto a eliminar: ";
                cin >> idPuesto;
                puestos.eliminarPuesto(idPuesto);
                system("pause");
                system("cls");
                break;
            }
            case 5:
                return;
            default:
                cout << "Opción no valida, intente de nuevo." << endl;
            }
        }
    }
};

Puestos::Puestos(MYSQL* con) : conector(con) {}

void Puestos::crearPuesto(const string& puesto) {
    stringstream ss;
    ss << "INSERT INTO Puestos (puesto) VALUES ('" << puesto << "')";
    string query = ss.str();
    if (mysql_query(conector, query.c_str()) == 0) {
        cout << "Puesto creado exitosamente." << endl;
    }
    else {
        cout << "Error al crear puesto: " << mysql_error(conector) << endl;
    }
}

void Puestos::mostrarPuestos() {
    if (mysql_query(conector, "SELECT * FROM Puestos") == 0) {
        MYSQL_RES* res = mysql_store_result(conector);
        MYSQL_ROW row;
        cout << "\n\t-------PUESTOS-------\n\n";
        while ((row = mysql_fetch_row(res))) {

            cout << "\n\tID: " << row[0] << " Puesto: " << row[1] << endl;
        }
        mysql_free_result(res);
    }
    else {
        cout << "Error al mostrar puestos: " << mysql_error(conector) << endl;
    }
}

void Puestos::modificarPuesto(int idPuesto, const string& nuevoPuesto) {
    stringstream ss;
    ss << "UPDATE Puestos SET puesto = '" << nuevoPuesto << "' WHERE idPuesto = " << idPuesto;
    string query = ss.str();
    if (mysql_query(conector, query.c_str()) == 0) {
        cout << "Puesto modificado exitosamente." << endl;
    }
    else {
        cout << "Error al modificar puesto: " << mysql_error(conector) << endl;
    }
}

void Puestos::eliminarPuesto(int idPuesto) {
    stringstream ss;
    ss << "DELETE FROM Puestos WHERE idPuesto = " << idPuesto;
    string query = ss.str();
    if (mysql_query(conector, query.c_str()) == 0) {
        cout << "Puesto eliminado exitosamente." << endl;
    }
    else {
        cout << "Error al eliminar puesto: " << mysql_error(conector) << endl;
    }
}