#include <mysql.h>
#include <iostream>
#include <string>

using namespace std;

class CompraMaestro {
private:
    MYSQL* con;
public:
    CompraMaestro(MYSQL* connection) : con(connection) {}

    void menuCompras(CompraMaestro& comprasmaestro) {
        int opc, idCompraDetalle, idCompra, idProducto, cantidad, noOrdenCompra, idProveedor;
        double precioCostoUnitario;
        string fechaOrden, fechaIngreso;

        while (true) {
            cout << "\n\t-------MENU COMPRAS-------";
            cout << "\n\t1. Crear Compra y Detalle";
            cout << "\n\t2. Mostrar Compras y Detalles";
            cout << "\n\t3. Modificar Compra y Detalle";
            cout << "\n\t4. Eliminar Compra y Detalle";
            cout << "\n\t5. Salir";
            cout << "\n\tIngrese una opcion: ";
            cin >> opc;
            switch (opc) {
            case 1:
                system("cls");
                cout << "Ingrese ID de Compra: ";
                cin >> idCompra;
                cout << "Ingrese Numero de Orden de Compra: ";
                cin >> noOrdenCompra;
                cout << "Ingrese ID de Proveedor: ";
                cin >> idProveedor;
                cout << "Ingrese Fecha de Orden (YYYY-MM-DD): ";
                cin >> fechaOrden;
                cout << "Ingrese Fecha de Ingreso (YYYY-MM-DD HH:MM:SS): ";
                cin.ignore();
                getline(cin, fechaIngreso);
                cout << "Ingrese ID de Detalle de Compra: ";
                cin >> idCompraDetalle;
                cout << "Ingrese ID de Producto: ";
                cin >> idProducto;
                cout << "Ingrese Cantidad: ";
                cin >> cantidad;
                cout << "Ingrese Precio Costo Unitario: ";
                cin >> precioCostoUnitario;
                comprasmaestro.crearCompraYDetalle(idCompra, noOrdenCompra, idProveedor, fechaOrden, fechaIngreso, idCompraDetalle, idProducto, cantidad, precioCostoUnitario);
                system("pause");
                system("cls");
                break;

            case 2:
                system("cls");
                comprasmaestro.mostrarComprasYDetalles();
                system("pause");
                system("cls");
                break;
            case 3:
                system("cls");
                int idCompra, noOrdenCompra, idProveedor, idCompraDetalle, idProducto, cantidad;
                double precioCostoUnitario;

                cout << "Ingrese ID de Compra a modificar: ";
                cin >> idCompra;
                cout << "Ingrese nuevo Numero de Orden de Compra: ";
                cin >> noOrdenCompra;
                cout << "Ingrese nuevo ID de Proveedor: ";
                cin >> idProveedor;
                cout << "Ingrese nueva Fecha de Orden (YYYY-MM-DD): ";
                cin >> fechaOrden;
                cout << "Ingrese nueva Fecha de Ingreso (YYYY-MM-DD HH:MM:SS): ";
                cin.ignore();
                getline(cin, fechaIngreso);
                cout << "Ingrese ID de Detalle de Compra a modificar: ";
                cin >> idCompraDetalle;
                cout << "Ingrese nuevo ID de Producto: ";
                cin >> idProducto;
                cout << "Ingrese nueva Cantidad: ";
                cin >> cantidad;
                cout << "Ingrese nuevo Precio Costo Unitario: ";
                cin >> precioCostoUnitario;
                comprasmaestro.modificarCompraYDetalle(idCompra, noOrdenCompra, idProveedor, fechaOrden, fechaIngreso,
                    idCompraDetalle, idProducto, cantidad, precioCostoUnitario);
                system("pause");
                system("cls");
                break;
            case 4:
                system("cls");
                cout << "Ingrese ID de Compra a eliminar: ";
                cin >> idCompra;
                comprasmaestro.eliminarCompraYDetalle(idCompra);
                system("pause");
                system("cls");
                break;

            case 5:
                system("cls");
                cout << "Saliendo..." << endl;
                system("pause");
                system("cls");
                return;
            default:
                cout << "Opcion no valida, intente de nuevo." << endl;
                break;
            }
        }
    }

    // Función para crear una compra y su detalle
    void crearCompraYDetalle(int idCompra, int noOrdenCompra, int idProveedor, const string& fechaOrden, const string& fechaIngreso,
        int idCompraDetalle, int idProducto, int cantidad, double precioCostoUnitario) {
        // Iniciar transacción
        mysql_autocommit(con, 0);
        bool success = true;

        // Insertar la compra
        string queryCompra = "INSERT INTO compras (idcompra, no_orden_compra, idproveedor, fecha_orden, fechaingreso) VALUES (" +
            to_string(idCompra) + ", " + to_string(noOrdenCompra) + ", " + to_string(idProveedor) + ", '" +
            fechaOrden + "', '" + fechaIngreso + "')";
        if (mysql_query(con, queryCompra.c_str())) {
            cerr << "Error al crear la compra: " << mysql_error(con) << endl;
            success = false;
        }

        // Insertar el detalle de la compra
        string queryCompraDetalle = "INSERT INTO compras_detalle (idcompra_detalle, idcompra, idproducto, cantidad, precio_costo_unitario) VALUES (" +
            to_string(idCompraDetalle) + ", " + to_string(idCompra) + ", " + to_string(idProducto) + ", " +
            to_string(cantidad) + ", " + to_string(precioCostoUnitario) + ")";
        if (mysql_query(con, queryCompraDetalle.c_str())) {
            cerr << "Error al crear el detalle de la compra: " << mysql_error(con) << endl;
            success = false;
        }

        // Realizar commit o rollback
        if (success) {
            mysql_commit(con);
            cout << "Compra y detalle de compra creados exitosamente." << endl;
        }
        else {
            mysql_rollback(con);
            cerr << "Error al crear la compra y/o su detalle, se realizo rollback." << endl;
        }
        mysql_autocommit(con, 1); // Restaurar autocommit
    }

    // Función para mostrar todas las compras y sus detalles
    void mostrarComprasYDetalles() {
        string query = "SELECT c.idcompra, c.no_orden_compra, c.idproveedor, c.fecha_orden, c.fechaingreso, "
            "cd.idcompra_detalle, cd.idproducto, cd.cantidad, cd.precio_costo_unitario "
            "FROM compras c INNER JOIN compras_detalle cd ON c.idcompra = cd.idcompra";

        if (mysql_query(con, query.c_str())) {
            cerr << "Error al mostrar las compras y sus detalles: " << mysql_error(con) << endl;
        }
        else {
            MYSQL_RES* result = mysql_store_result(con);
            MYSQL_ROW row;

            cout << "-------------------COMPRAS---------------------"<<endl;
            while ((row = mysql_fetch_row(result))) {

                cout << "\nID: " << row[0] << endl;
                cout << "No. Orden Compra: " << row[1] << endl;
                cout << "ID Proveedor: " << row[2] << endl;
                cout << "Fecha Orden: " << row[3] << endl;
                cout << "Fecha Ingreso: " << row[4] << endl;
                cout << "ID Compra Detalle: " << row[5] << endl;
                cout << "ID Producto: " << row[6] << endl;
                cout << "Cantidad: " << row[7] << endl;
                cout << "Precio Costo Unitario: " << row[8] << endl;
                cout << "\n-----------------------------------------\n";
            }
            mysql_free_result(result);
        }
    }

    // Función para modificar una compra y su detalle
    void modificarCompraYDetalle(int idCompra, int noOrdenCompra, int idProveedor, const string& fechaOrden, const string& fechaIngreso,
        int idCompraDetalle, int idProducto, int cantidad, double precioCostoUnitario) {
        // Iniciar transacción
        mysql_autocommit(con, 0);
        bool success = true;

        // Modificar la compra
        string queryCompra = "UPDATE compras SET no_orden_compra=" + to_string(noOrdenCompra) + ", idproveedor=" +
            to_string(idProveedor) + ", fecha_orden='" + fechaOrden + "', fechaingreso='" + fechaIngreso +
            "' WHERE idcompra=" + to_string(idCompra);
        if (mysql_query(con, queryCompra.c_str())) {
            cerr << "Error al modificar la compra: " << mysql_error(con) << endl;
            success = false;
        }

        // Modificar el detalle de la compra
        string queryCompraDetalle = "UPDATE compras_detalle SET idproducto=" + to_string(idProducto) + ", cantidad=" +
            to_string(cantidad) + ", precio_costo_unitario=" + to_string(precioCostoUnitario) +
            " WHERE idcompra_detalle=" + to_string(idCompraDetalle);
        if (mysql_query(con, queryCompraDetalle.c_str())) {
            cerr << "Error al modificar el detalle de la compra: " << mysql_error(con) << endl;
            success = false;
        }

        // Realizar commit o rollback
        if (success) {
            mysql_commit(con);
            cout << "Compra y detalle de compra modificados exitosamente." << endl;
        }
        else {
            mysql_rollback(con);
            cerr << "Error al modificar la compra y/o su detalle, se realizó rollback." << endl;
        }
        mysql_autocommit(con, 1); // Restaurar autocommit
    }

    // Función para eliminar una compra y su detalle
    void eliminarCompraYDetalle(int idCompra) {
        // Iniciar transacción
        mysql_autocommit(con, 0);
        bool success = true;

        // Eliminar el detalle de la compra
        string queryCompraDetalle = "DELETE FROM compras_detalle WHERE idcompra=" + to_string(idCompra);
        if (mysql_query(con, queryCompraDetalle.c_str())) {
            cerr << "Error al eliminar el detalle de la compra: " << mysql_error(con) << endl;
            success = false;
        }

        // Eliminar la compra
        string queryCompra = "DELETE FROM compras WHERE idcompra=" + to_string(idCompra);
        if (mysql_query(con, queryCompra.c_str())) {
            cerr << "Error al eliminar la compra: " << mysql_error(con) << endl;
            success = false;
        }

        // Realizar commit o rollback
        if (success) {
            mysql_commit(con);
            cout << "Compra y detalle de compra eliminados exitosamente." << endl;
        }
        else {
            mysql_rollback(con);
            cerr << "Error al eliminar la compra y/o su detalle, se realizó rollback." << endl;
        }
        mysql_autocommit(con, 1); // Restaurar autocommit
    }
};
