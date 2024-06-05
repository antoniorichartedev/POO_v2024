#include <iostream>
#include <map>
#include <set>

class Articulo;
class Pedido;

class LineaPedido
{
    public:
        explicit LineaPedido(double p_vent = 0.0, size_t cant = 1);

        // Métodos observadores.
        const double& precio_venta() const { return precio_venta_; }
        const size_t& cantidad() const { return cantidad_; }

    private:
        double precio_venta_;
        size_t cantidad_;
};

// Operador de inserción en flujo de salida para objetos de tipo LineaPedido.
std::ostream& operator<<(std::ostream& os, const LineaPedido& lp);

/* Clase de asociación Pedido_Articulo */
class Pedido_Articulo
{
    public:
        
        // Definición de OrdenaArticulos.
        struct OrdenaArticulos
        {
            bool operator ()(const Articulo* art1, const Articulo* art2) const;
        };

        // Definición de OrdenaPedidos
        struct OrdenaPedidos
        {
            bool operator()(const Pedido* ped1, const Pedido* ped2) const;
        };

        // Definición de las asociaciones.
        typedef std::map<Articulo*, LineaPedido, OrdenaArticulos> ItemsPedido;
        typedef std::map<Pedido*, ItemsPedido, OrdenaPedidos> Pedidos_Articulos;
        typedef std::map<Pedido*, LineaPedido, OrdenaPedidos> Pedidos;
        typedef std::map<Articulo*, Pedidos, OrdenaArticulos> Articulos_Pedidos;

        // Métodos para crear las asociaciones.
        void pedir(Pedido& p, Articulo& art, double pr, unsigned int cantidad = 1);
        void pedir(Articulo& art, Pedido& p, double pr, unsigned int cantidad = 1);

        // Métodos observadores.
        ItemsPedido detalle(Pedido& ped) const;
        Pedidos ventas(Articulo& art) const;
        void mostrarDetallePedidos(std::ostream& os) const;
        void mostrarVentasArticulos(std::ostream& os) const;
        
    private:
        Pedidos_Articulos ped_art;
        Articulos_Pedidos art_ped;
};

// Operadores de inserción en flujo de salida.
std::ostream& operator<<(std::ostream& os, const Pedido_Articulo::ItemsPedido& itsped);
std::ostream& operator<<(std::ostream& os, const Pedido_Articulo::Pedidos& peds);