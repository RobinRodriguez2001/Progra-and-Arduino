#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <mysql.h>

using namespace std;

class VentaMaestro {
private:
    MYSQL* con;
    int ultimaFactura; // Último número de factura usado
    char ultimaSerie;  // Última serie usada

public:
    // Constructor
    VentaMaestro(MYSQL* connection) : con(connection), ultimaFactura(0), ultimaSerie('A') {
        obtenerUltimaFacturaYSerie(); // Obtener el último número de factura y serie usados
    }

    void menuVentaMaestro(VentaMaestro& ventaMaestro) {
        int opc, idVenta, idEmpleado;
        string fechaIngreso, nitCliente, cantidad;
        double precioUnitario;

        while (true) {
            cout << "\n\t-------MENU VENTA-------";
            cout << "\n\t1. Crear Venta y Detalle";
            cout << "\n\t2. Mostrar Ventas y Detalles";
            cout << "\n\t3. Eliminar Venta y Detalle";
            cout << "\n\t4. Salir";
            cout << "\n\tIngrese una opcion: ";
            cin >> opc;

            switch (opc) {
            case 1:
                cout << "Ingrese NIT del Cliente: ";
                cin >> nitCliente;
                cout << "Ingrese ID de Empleado: ";
                cin >> idEmpleado;
                fechaIngreso = "NOW()"; // Fecha de ingreso se maneja internamente en la clase
                ventaMaestro.crearVentaYDetalle(nitCliente, idEmpleado, fechaIngreso);
                break;
            case 2:
                ventaMaestro.mostrarVentasYDetalles();
                break;
            case 3:
                cout << "Ingrese ID de Venta a eliminar: ";
                cin >> idVenta;
                ventaMaestro.eliminarVentaYDetalle(idVenta);
                break;
            case 4:
                cout << "Saliendo..." << endl;
                return;
            default:
                cout << "Opción no valida, intente de nuevo." << endl;
                break;
            }
        }
    }

    // Función para crear una venta y su detalle
    void crearVentaYDetalle(const string& nitCliente, int idEmpleado, const string& fechaIngreso) {
        // Iniciar transacción
        mysql_autocommit(con, 0);
        bool success = true;

        // Obtener o registrar el cliente
        int idCliente = obtenerORegistrarCliente(nitCliente);
        if (idCliente == -1) {
            mysql_rollback(con);
            cerr << "Error al registrar el cliente." << endl;
            mysql_autocommit(con, 1); // Restaurar autocommit
            return;
        }
        else if (idCliente == -2) {
            // Es un consumidor final, no se registra en la base de datos
            idCliente = 0; // O utiliza un valor especial según tu diseño
        }

        // Generar automáticamente la serie y número de factura
        string serie = string(1, ultimaSerie); // Convertir char a string
        int noFactura = ultimaFactura++;       // Incrementar el número de factura

        // Si la serie llega a 'Z', reiniciar a 'A' y aumentar el número de factura
        if (ultimaSerie == 'Z') {
            ultimaSerie = 'A';
            ultimaFactura++;
        }
        else {
            ultimaSerie++;
        }

        // Modificar la venta
        ostringstream queryVenta;
        queryVenta << "INSERT INTO ventas (idventa, nofactura, serie, fechafactura, idcliente, idempleado, fechaingreso) "
            << "VALUES (NULL, " << noFactura << ", '" << serie << "', NOW(), " << idCliente << ", " << idEmpleado << ", NOW())";

        if (mysql_query(con, queryVenta.str().c_str())) {
            cerr << "Error al crear la venta: " << mysql_error(con) << endl;
            success = false;
        }

        // Obtener el ID de la venta insertada
        int idVenta = mysql_insert_id(con);

        // Realizar commit o rollback
        if (!success) {
            mysql_rollback(con);
            cerr << "Error al crear la venta, se realizó rollback." << endl;
            mysql_autocommit(con, 1); // Restaurar autocommit
            return;
        }

        // Agregar productos al detalle de la venta
        bool agregarMasProductos = true;
        while (agregarMasProductos) {
            int idProducto;
            string cantidadStr;
            double precioUnitario;

            cout << "Ingrese el ID del producto: ";
            cin >> idProducto;
            cout << "Ingrese la cantidad: ";
            cin >> cantidadStr;
            cout << "Ingrese el precio unitario: ";
            cin >> precioUnitario;

            // Modificar el detalle de la venta
            ostringstream queryVentaDetalle;
            queryVentaDetalle << "INSERT INTO ventas_detalle (idventa_detalle, idventa, idproducto, cantidad, precio_unitario) "
                << "VALUES (NULL, " << idVenta << ", " << idProducto << ", '" << cantidadStr << "', " << precioUnitario << ")";

            if (mysql_query(con, queryVentaDetalle.str().c_str())) {
                cerr << "Error al crear el detalle de la venta: " << mysql_error(con) << endl;
                success = false;
            }

            cout << "¿Desea agregar otro producto? (S/N): ";
            char respuesta;
            cin >> respuesta;
            if (toupper(respuesta) != 'S') {
                agregarMasProductos = false;
            }
        }

        // Realizar commit o rollback
        if (success) {
            mysql_commit(con);
            cout << "Venta y detalle de venta creados exitosamente." << endl;
            imprimirFactura(idVenta, nitCliente);
        }
        else {
            mysql_rollback(con);
            cerr << "Error al crear la venta y/o su detalle, se realizó rollback." << endl;
        }
        mysql_autocommit(con, 1); // Restaurar autocommit
    }

    // Función para eliminar una venta y su detalle
    void eliminarVentaYDetalle(int idVenta) {
        // Iniciar transacción
        mysql_autocommit(con, 0);
        bool success = true;

        // Eliminar el detalle de la venta
        ostringstream queryVentaDetalle;
        queryVentaDetalle << "DELETE FROM ventas_detalle WHERE idventa=" << idVenta;
        if (mysql_query(con, queryVentaDetalle.str().c_str())) {
            cerr << "Error al eliminar el detalle de la venta: " << mysql_error(con) << endl;
            success = false;
        }

        // Eliminar la venta
        ostringstream queryVenta;
        queryVenta << "DELETE FROM ventas WHERE idventa=" << idVenta;
        if (mysql_query(con, queryVenta.str().c_str())) {
            cerr << "Error al eliminar la venta: " << mysql_error(con) << endl;
            success = false;
        }

        // Realizar commit o rollback
        if (success) {
            mysql_commit(con);
            cout << "Venta y detalle de venta eliminados exitosamente." << endl;
        }
        else {
            mysql_rollback(con);
            cerr << "Error al eliminar la venta y/o su detalle, se realizó rollback." << endl;
        }
        mysql_autocommit(con, 1); // Restaurar autocommit
    }

    // Función para mostrar todas las ventas y sus detalles
    void mostrarVentasYDetalles() {
        string query = "SELECT v.idventa, v.nofactura, v.serie, DATE_FORMAT(v.fechafactura, '%d/%m/%Y'), c.nit, CONCAT(c.nombres, ' ', c.apellidos) AS nombre_completo, "
            "vd.idventa_detalle, vd.idproducto, p.idMarca, vd.cantidad, vd.precio_unitario "
            "FROM ventas v "
            "INNER JOIN ventas_detalle vd ON v.idventa = vd.idventa "
            "LEFT JOIN clientes c ON v.idcliente = c.idcliente "
            "LEFT JOIN productos p ON vd.idproducto = p.idproducto";

        if (mysql_query(con, query.c_str())) {
            cerr << "Error al mostrar las ventas y sus detalles: " << mysql_error(con) << endl;
        }
        else {
            MYSQL_RES* result = mysql_store_result(con);
            if (result == nullptr) {
                cerr << "Error al obtener el resultado de la consulta." << endl;
                return;
            }

            MYSQL_ROW row;
            while ((row = mysql_fetch_row(result))) {
                cout << "------------------------------------------------------------" << endl;
                cout << "No Factura: " << row[1] << "  Fecha: " << row[3] << endl;
                cout << "Nit: " << row[4];
                if (row[5] != nullptr) {
                    cout << "  Cliente: " << row[5] << endl;
                }
                else {
                    cout << "  Cliente: Consumidor Final" << endl;
                }
                cout << "Dirección: Ciudad" << endl;
                cout << "------------------------------------------------------------" << endl;

                cout << left << setw(10) << "Código"
                    << left << setw(30) << "Descripción"
                    << left << setw(10) << "Cantidad"
                    << right << setw(10) << "Precio" << endl;
                cout << "------------------------------------------------------------" << endl;

                mostrarDetalleVenta(stoi(row[0])); // Mostrar detalle de la venta
            }
            mysql_free_result(result); // Liberar el resultado después de usarlo
        }
    }

private:
    // Función para mostrar el detalle de una venta específica
    void mostrarDetalleVenta(int idVenta) {
        string query = "SELECT vd.idproducto, p.idMarca, vd.cantidad, vd.precio_unitario "
            "FROM ventas_detalle vd "
            "INNER JOIN productos p ON vd.idproducto = p.idproducto "
            "WHERE vd.idventa = " + to_string(idVenta);

        if (mysql_query(con, query.c_str())) {
            cerr << "Error al obtener el detalle de la venta: " << mysql_error(con) << endl;
            return;
        }

        MYSQL_RES* result = mysql_store_result(con);
        if (result == nullptr) {
            cerr << "Error al obtener el resultado de la consulta." << endl;
            return;
        }

        // Imprimir detalle de los productos
        while (MYSQL_ROW row = mysql_fetch_row(result)) {
            cout << left << setw(10) << row[0]   // ID del producto
                << left << setw(30) << obtenerNombreProducto(stoi(row[0]))   // Nombre del producto
                << left << setw(10) << row[2]   // Cantidad
                << right << setw(10) << "Q " << fixed << setprecision(2) << stod( row[3]) << endl; // Precio unitario
        }

        mysql_free_result(result); // Liberar el resultado después de usarlo
    }

    // Función para obtener el nombre de un producto dado su ID
    string obtenerNombreProducto(int idProducto) {
        string query = "SELECT producto FROM productos WHERE idproducto = " + to_string(idProducto);
        if (mysql_query(con, query.c_str())) {
            cerr << "Error al obtener el nombre del producto: " << mysql_error(con) << endl;
            return "Nombre no encontrado";
        }

        MYSQL_RES* result = mysql_store_result(con);
        if (result == nullptr) {
            cerr << "Error al obtener el resultado de la consulta." << endl;
            return "Nombre no encontrado";
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        if (row == nullptr) {
            cerr << "Producto no encontrado." << endl;
            return "Nombre no encontrado";
        }

        string nombre = row[0];
        mysql_free_result(result); // Liberar el resultado después de usarlo
        return nombre;
    }

    // Función para obtener o registrar un cliente por su NIT
    int obtenerORegistrarCliente(const string& nitCliente) {
        // Verificar si el cliente existe
        string query = "SELECT idcliente FROM clientes WHERE nit = '" + nitCliente + "'";
        if (mysql_query(con, query.c_str())) {
            cerr << "Error al verificar si el cliente existe: " << mysql_error(con) << endl;
            return -1;
        }

        MYSQL_RES* result = mysql_store_result(con);
        if (result == nullptr) {
            cerr << "Error al obtener el resultado de la consulta." << endl;
            return -1;
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        if (row != nullptr) {
            int idCliente = stoi(row[0]);
            mysql_free_result(result); // Liberar el resultado después de usarlo
            return idCliente;
        }

        mysql_free_result(result); // Liberar el resultado después de usarlo

        // Si no existe, registrar un nuevo cliente (opcional)
        char respuesta;
        cout << "El cliente no está registrado. ¿Desea registrarlo? (S/N): ";
        cin >> respuesta;

        if (toupper(respuesta) == 'S') {
            string nombres, apellidos;
            cin.ignore(); // Limpiar el buffer del teclado
            cout << "Ingrese los nombres del cliente: ";
            getline(cin, nombres);
            cout << "Ingrese los apellidos del cliente: ";
            getline(cin, apellidos);

            // Insertar el nuevo cliente en la base de datos
            ostringstream queryInsertar;
            queryInsertar << "INSERT INTO clientes (idcliente, nit, nombres, apellidos) "
                << "VALUES (NULL, '" << nitCliente << "', '" << nombres << "', '" << apellidos << "')";

            if (mysql_query(con, queryInsertar.str().c_str())) {
                cerr << "Error al insertar el nuevo cliente: " << mysql_error(con) << endl;
                return -1;
            }

            int idCliente = mysql_insert_id(con);
            return idCliente;
        }

        // No registrar al cliente
        cout << "El cliente no será registrado en la base de datos." << endl;
        return -2; // Indicar que no se registra en la base de datos
    }

    // Función para imprimir la factura de una venta
    void imprimirFactura(int idVenta, const string& nitCliente) {
        string query = "SELECT v.nofactura, v.serie, DATE_FORMAT(v.fechafactura, '%d/%m/%Y'), c.nit, CONCAT(c.nombres, ' ', c.apellidos) AS nombre_completo, "
            "p.producto, vd.cantidad, vd.precio_unitario, (vd.cantidad * vd.precio_unitario) AS total "
            "FROM ventas v "
            "INNER JOIN ventas_detalle vd ON v.idventa = vd.idventa "
            "LEFT JOIN clientes c ON v.idcliente = c.idcliente "
            "LEFT JOIN productos p ON vd.idproducto = p.idproducto "
            "WHERE v.idventa = " + to_string(idVenta);

        if (mysql_query(con, query.c_str())) {
            cerr << "Error al obtener la factura: " << mysql_error(con) << endl;
            return;
        }

        MYSQL_RES* result = mysql_store_result(con);
        if (result == nullptr) {
            cerr << "Error al obtener el resultado de la consulta." << endl;
            return;
        }

        // Imprimir la cabecera de la factura
        MYSQL_ROW row = mysql_fetch_row(result);
        cout << "------------------------------------------------------------" << endl;
        cout << "Factura No.: " << row[0] << " Serie: " << row[1] << " Fecha: " << row[2] << endl;
        cout << "Nit: " << row[3];
        if (row[4] != nullptr) {
            cout << "  Cliente: " << row[4] << endl;
        }
        else {
            cout << "  Cliente: Consumidor Final" << endl;
        }
        cout << "Dirección: Ciudad" << endl;
        cout << "------------------------------------------------------------" << endl;

        cout << left << setw(10) << "Código"
            << left << setw(30) << "Descripción"
            << left << setw(10) << "Cantidad"
            << right << setw(10) << "Precio" << endl;
        cout << "------------------------------------------------------------" << endl;

        // Imprimir el detalle de la factura
        do {
            cout << left << setw(10) << row[5]     // Nombre del producto
                << left << setw(30) << row[6]     // Cantidad
                << left << setw(10) << row[7]     // Precio unitario
                << right << setw(10) << row[8] << endl; // Total
        } while (row = mysql_fetch_row(result));

        cout << "------------------------------------------------------------" << endl;

        mysql_free_result(result); // Liberar el resultado después de usarlo
    }

    // Función para obtener el último número de factura y serie usados
    void obtenerUltimaFacturaYSerie() {
        string query = "SELECT nofactura, serie FROM ventas ORDER BY idventa DESC LIMIT 1";

        if (mysql_query(con, query.c_str())) {
            cerr << "Error al obtener la última factura y serie: " << mysql_error(con) << endl;
            return;
        }

        MYSQL_RES* result = mysql_store_result(con);
        if (result == nullptr) {
            cerr << "Error al obtener el resultado de la consulta." << endl;
            return;
        }

        MYSQL_ROW row = mysql_fetch_row(result);
        if (row != nullptr) {
            ultimaFactura = stoi(row[0]);
            ultimaSerie = row[1][0]; // La serie es un char, se toma el primer caracter
        }

        mysql_free_result(result); // Liberar el resultado después de usarlo
    }
};
