#include <iostream>
#include <mysql.h>
#include <sstream>
#include <string>

using namespace std;

class Productos {
private:
    MYSQL* conector;

public:
    Productos(MYSQL* con);
    void crearProducto(int idProducto, const string& producto, int idMarca, const string& descripcion, const string& imagen, double precioCosto, double precioVenta, int existencia, const string& fechaIngreso);
    void mostrarProductos();
    void modificarProducto(int idProducto, const string& producto, int idMarca, const string& descripcion, const string& imagen, double precioCosto, double precioVenta, int existencia, const string& fechaIngreso);
    void eliminarProducto(int idProducto);
    void menuProductos(Productos& productos) {
        int opc, idProducto, idMarca, existencia;
        string producto, descripcion, imagen, fechaIngreso;
        double precioCosto, precioVenta;

        while (true) {
            cout << "\n\t-------MENU PRODUCTOS-------";
            cout << "\n\t1. Crear Producto";
            cout << "\n\t2. Mostrar Productos";
            cout << "\n\t3. Modificar Producto";
            cout << "\n\t4. Eliminar Producto";
            cout << "\n\t5. Salir";
            cout << "\n\n\tSeleccione una opcion: ";
            cin >> opc;

            switch (opc) {
            case 1:
                cout << "Digite el ID del Producto: ";
                cin >> idProducto;
                cout << "Digite el nombre del Producto: ";
                cin.ignore();
                getline(cin, producto);
                cout << "Digite la descripcion del Producto: ";
                getline(cin, descripcion);
                cout << "Digite la Ruta de la Imagen: ";
                getline(cin, imagen);
                cout << "Digite el ID de la Marca: ";
                cin >> idMarca;
                cout << "Digite el precio de costo del Producto: ";
                cin >> precioCosto;
                cout << "Digite el precio de venta del Producto: ";
                cin >> precioVenta;
                cout << "Digite la existencia del Producto: ";
                cin >> existencia;
                fflush(stdin);
                productos.crearProducto(idProducto, producto, idMarca, descripcion, imagen, precioCosto, precioVenta, existencia, fechaIngreso);
                system("pause");
                system("cls");
                break;
            case 2:
                productos.mostrarProductos();
                system("pause");
                system("cls");
                break;
            case 3:
                cout << "Digite el ID del Producto a modificar: ";
                cin >> idProducto;
                cout << "Digite el nuevo nombre del Producto: ";
                cin.ignore();
                getline(cin, producto);
                cout << "Digite el nuevo ID de la Marca: ";
                cin >> idMarca;
                cout << "Digite la nueva descripcion del Producto: ";
                getline(cin, descripcion);
                cout << "Digite la Ruta de la Imagen: ";
                getline(cin, imagen);
                cout << "Digite el nuevo precio de costo del Producto: ";
                cin >> precioCosto;
                cout << "Digite el nuevo precio de venta del Producto: ";
                cin >> precioVenta;
                cout << "Digite la nueva existencia del Producto: ";
                cin >> existencia;
                cout << "Digite la nueva fecha de ingreso del Producto (YYYY-MM-DD): ";
                getline(cin, fechaIngreso);
                productos.modificarProducto(idProducto, producto, idMarca, descripcion, imagen, precioCosto, precioVenta, existencia, fechaIngreso);
                system("pause");
                system("cls");
                break;
            case 4:
                cout << "Digite el ID del Producto a eliminar: ";
                cin >> idProducto;
                productos.eliminarProducto(idProducto);
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

Productos::Productos(MYSQL* con) : conector(con) {}

void Productos::crearProducto(int idProducto, const string& producto, int idMarca, const string& descripcion, const string& imagen, double precioCosto, double precioVenta, int existencia, const string& fechaIngreso) {
    stringstream ss;
    ss << "INSERT INTO productos (idProducto, producto, idMarca, descripcion, imagen, precio_costo, precio_venta, existencia, fecha_ingreso) VALUES ("
       << idProducto << ", '" << producto << "', " << idMarca << ", '" << descripcion << "', '" << imagen << "', "
       << precioCosto << ", " << precioVenta << ", " << existencia << ",  NOW())";
    string query = ss.str();
    if (mysql_query(conector, query.c_str()) == 0) {
        cout << "Producto creado exitosamente." << endl;
    }
    else {
        cout << "Error al crear producto: " << mysql_error(conector) << endl;
    }
}

void Productos::mostrarProductos() {
    if (mysql_query(conector, "SELECT * FROM productos") == 0) {
        MYSQL_RES* res = mysql_store_result(conector);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << " \n\tID: " << row[0] << endl;
            cout << "\tProducto: " << row[1] << endl;
            cout << "\tMarca ID: " << row[2] << endl;
            cout << "\tDescripcion: " << row[3] << endl;
            cout << "\tImagen: " << row[4] << endl;
            cout << "\tPrecio Costo: " << row[5] << endl;
            cout << "\tPrecio Venta: " << row[6] << endl;
            cout << "\tExistencia: " << row[7] << endl;
            cout << "\tFecha Ingreso: " << row[8] << endl;
            cout << "\n-----------------------------------------\n";
        }
        mysql_free_result(res);
    }
    else {
        cout << "Error al mostrar productos: " << mysql_error(conector) << endl;
    }
}

void Productos::modificarProducto(int idProducto, const string& producto, int idMarca, const string& descripcion, const string& imagen, double precioCosto, double precioVenta, int existencia, const string& fechaIngreso) {
    stringstream ss;
    ss << "UPDATE productos SET producto = '" << producto << "', idMarca = " << idMarca << ", descripcion = '" << descripcion
       << "', imagen = '" << imagen << "', precio_costo = " << precioCosto << ", precio_venta = " << precioVenta
       << ", existencia = " << existencia << ", fecha_ingreso = '" << fechaIngreso << "' WHERE idProducto = " << idProducto;
    string query = ss.str();
    if (mysql_query(conector, query.c_str()) == 0) {
        cout << "Producto modificado exitosamente." << endl;
    }
    else {
        cout << "Error al modificar producto: " << mysql_error(conector) << endl;
    }
}

void Productos::eliminarProducto(int idProducto) {
    stringstream ss;
    ss << "DELETE FROM productos WHERE idProducto = " << idProducto;
    string query = ss.str();
    if (mysql_query(conector, query.c_str()) == 0) {
        cout << "Producto eliminado exitosamente." << endl;
    }
    else {
        cout << "Error al eliminar producto: " << mysql_error(conector) << endl;
    }
}