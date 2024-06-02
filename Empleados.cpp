#include <iostream>
#include <mysql.h>
#include <sstream>

using namespace std;

// Declaración de la clase Empleados
class Empleados {
private:
    MYSQL* conector;

public:
    Empleados(MYSQL* con);
    void crearEmpleado(int idEmpleado, const string& nombres, const string& apellidos, const string& direccion, const string& telefono, const string& dpi, bool genero, const string& fecha_nacimiento, int idPuesto, const string& fecha_inicio_labores, const string& fechaingreso);
    void mostrarEmpleados();
    void modificarEmpleado(int idEmpleado, const string& nuevosNombres, const string& nuevosApellidos, const string& nuevaDireccion, const string& nuevoTelefono, const string& nuevoDpi, bool nuevoGenero, const string& nuevaFechaNacimiento, int nuevoIdPuesto, const string& nuevaFechaInicioLabores, const string& nuevaFechaIngreso);
    void eliminarEmpleado(int idEmpleado);
    void menuEmpleados(Empleados& empleados, Puestos& puestos) {
        int opc, idEmpleado, idPuesto;
        string nombres, apellidos, direccion, telefono, dpi, fecha_nacimiento, fecha_inicio_labores, fechaingreso;
        bool genero;

        while (true) {
            cout << "\n\t-------MENU EMPLEADOS-------";
            cout << "\n\t1. Crear Empleado";
            cout << "\n\t2. Mostrar Empleados";
            cout << "\n\t3. Modificar Empleado";
            cout << "\n\t4. Eliminar Empleado";
            cout << "\n\t5. Mostrar Puestos";
            cout << "\n\t6. Salir";
            cout << "\n\n\tSeleccione una opcion: ";
            cin >> opc;

            switch (opc) {
            case 1:
                cout << "Digite el ID del Empleado: ";
                cin >> idEmpleado;
                cout << "Digite los nombres del Empleado: ";
                cin.ignore();
                getline(cin, nombres);
                cout << "Digite los apellidos del Empleado: ";
                getline(cin, apellidos);
                cout << "Digite la direccion del Empleado: ";
                getline(cin, direccion);
                cout << "Digite el teléfono del Empleado: ";
                getline(cin, telefono);
                cout << "Digite el DPI del Empleado: ";
                getline(cin, dpi);
                cout << "Digite el genero del Empleado (0 para masculino, 1 para femenino): ";
                cin >> genero;
                cout << "Digite la fecha de nacimiento del Empleado (YYYY-MM-DD): ";
                cin.ignore();
                getline(cin, fecha_nacimiento);
                cout << "Digite el ID del Puesto del Empleado: ";
                cin >> idPuesto;
                cout << "Digite la fecha de inicio de labores del Empleado (YYYY-MM-DD): ";
                cin.ignore();
                getline(cin, fecha_inicio_labores);
                cout << "Digite la fecha de ingreso del Empleado (YYYY-MM-DD HH:MM:SS): ";
                getline(cin, fechaingreso);
                empleados.crearEmpleado(idEmpleado, nombres, apellidos, direccion, telefono, dpi, genero, fecha_nacimiento, idPuesto, fecha_inicio_labores, fechaingreso);
                system("pause");
                system("cls");
                break;
            case 2:
                empleados.mostrarEmpleados();
                system("pause");
                system("cls");
                break;
            case 3:
                cout << "Digite el ID del Empleado a modificar: ";
                cin >> idEmpleado;
                cout << "Digite los nuevos nombres del Empleado: ";
                cin.ignore();
                getline(cin, nombres);
                cout << "Digite los nuevos apellidos del Empleado: ";
                getline(cin, apellidos);
                cout << "Digite la nueva dirección del Empleado: ";
                getline(cin, direccion);
                cout << "Digite el nuevo teléfono del Empleado: ";
                getline(cin, telefono);
                cout << "Digite el nuevo DPI del Empleado: ";
                getline(cin, dpi);
                cout << "Digite el nuevo genero del Empleado (0 para masculino, 1 para femenino): ";
                cin >> genero;
                cout << "Digite la nueva fecha de nacimiento del Empleado (YYYY-MM-DD): ";
                cin.ignore();
                getline(cin, fecha_nacimiento);
                cout << "Digite el nuevo ID del Puesto del Empleado: ";
                cin >> idPuesto;
                cout << "Digite la nueva fecha de inicio de labores del Empleado (YYYY-MM-DD): ";
                cin.ignore();
                getline(cin, fecha_inicio_labores);
                cout << "Digite la nueva fecha de ingreso del Empleado (YYYY-MM-DD HH:MM:SS): ";
                getline(cin, fechaingreso);
                empleados.modificarEmpleado(idEmpleado, nombres, apellidos, direccion, telefono, dpi, genero, fecha_nacimiento, idPuesto, fecha_inicio_labores, fechaingreso);
                system("pause");
                system("cls");
                break;
            case 4:
                cout << "Digite el ID del Empleado a eliminar: ";
                cin >> idEmpleado;
                empleados.eliminarEmpleado(idEmpleado);
                system("pause");
                system("cls");
                break;
            case 5:
                puestos.mostrarPuestos();
                system("pause");
                system("cls");
                break;
            case 6:
                return;
            default:
                cout << "Opcion no valida, intente de nuevo." << endl;
            }
        }
    }
};

// Implementación de los métodos de la clase Empleados
Empleados::Empleados(MYSQL* con) : conector(con) {}

void Empleados::crearEmpleado(int idEmpleado, const string& nombres, const string& apellidos, const string& direccion, const string& telefono, const string& dpi, bool genero, const string& fecha_nacimiento, int idPuesto, const string& fecha_inicio_labores, const string& fechaingreso) {
    stringstream ss;
    ss << "INSERT INTO Empleados (idEmpleado, nombres, apellidos, direccion, telefono, dpi, genero, fecha_nacimiento, idPuesto, fecha_inicio_labores, fechaingreso) VALUES ("
        << idEmpleado << ", '" << nombres << "', '" << apellidos << "', '" << direccion << "', '" << telefono << "', '" << dpi << "', " << genero << ", '" << fecha_nacimiento << "', " << idPuesto << ", '" << fecha_inicio_labores << "', '" << fechaingreso << "')";
    string query = ss.str();
    if (mysql_query(conector, query.c_str()) == 0) {
        cout << "Empleado creado exitosamente." << endl;
    }
    else {
        cout << "Error al crear empleado: " << mysql_error(conector) << endl;
    }
}

void Empleados::mostrarEmpleados() {
    if (mysql_query(conector, "SELECT * FROM Empleados") == 0) {
        MYSQL_RES* res = mysql_store_result(conector);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {

            cout << " \nID: " << row[0] << endl;
            cout << " Nombres: " << row[1] << endl;
            cout << " Apellidos: " << row[2] << endl;
            cout << " Dirección: " << row[3] << endl;
            cout << " Telefono: " << row[4] << endl;
            cout << " DPI: " << row[5] << endl;
            cout << " Genero: " << row[6] << endl;
            cout << " Fecha de Nacimiento: " << row[7] << endl;
            cout << " ID Puesto: " << row[8] << endl;
            cout << " Fecha de Inicio Labores: " << row[9] << endl;
            cout << " Fecha de Ingreso: " << row[10] << endl;
            cout << "\n-----------------------------------------\n";
        }
        mysql_free_result(res);
    }
    else {
        cout << "Error al mostrar empleados: " << mysql_error(conector) << endl;
    }
}

void Empleados::modificarEmpleado(int idEmpleado, const string& nuevosNombres, const string& nuevosApellidos, const string& nuevaDireccion, const string& nuevoTelefono, const string& nuevoDpi, bool nuevoGenero, const string& nuevaFechaNacimiento, int nuevoIdPuesto, const string& nuevaFechaInicioLabores, const string& nuevaFechaIngreso) {
    stringstream ss;
    ss << "UPDATE Empleados SET nombres = '" << nuevosNombres << "', apellidos = '" << nuevosApellidos << "', direccion = '" << nuevaDireccion
        << "', telefono = '" << nuevoTelefono << "', dpi = '" << nuevoDpi << "', genero = " << nuevoGenero << ", fecha_nacimiento = '" << nuevaFechaNacimiento
        << "', idPuesto = " << nuevoIdPuesto << ", fecha_inicio_labores = '" << nuevaFechaInicioLabores << "', fecha_ingreso = '" << nuevaFechaIngreso
        << "' WHERE idEmpleado = " << idEmpleado;
    string query = ss.str();
    if (mysql_query(conector, query.c_str()) == 0) {
        cout << "Empleado modificado exitosamente." << endl;
    }
    else {
        cout << "Error al modificar empleado: " << mysql_error(conector) << endl;
    }
}

void Empleados::eliminarEmpleado(int idEmpleado) {
    stringstream ss;
    ss << "DELETE FROM Empleados WHERE idEmpleado = " << idEmpleado;
    string query = ss.str();
    if (mysql_query(conector, query.c_str()) == 0) {
        cout << "Empleado eliminado exitosamente." << endl;
    }
    else {
        cout << "Error al eliminar empleado: " << mysql_error(conector) << endl;
    }
}