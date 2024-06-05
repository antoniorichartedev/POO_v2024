#include <iostream>
/* Clase Pedido */
class Pedido_Articulo;
class Usuario_Pedido;
class Usuario;
class Articulo;
class Tarjeta;
#include "../P1/fecha.hpp"

class Pedido
{
    public:
        friend class Pedido_Articulo;
        friend class Usuario_Pedido;

        Pedido(Usuario_Pedido& usu_ped, Pedido_Articulo& ped_art, Usuario& usu, const Tarjeta& tarj, const Fecha& fech_pedido = Fecha());

        
        // Clase de excepción Vacío, para evitar crear objetos Pedido vacíos.
        class Vacio
        {
            public: 
                Vacio(const Usuario& usuario): usu{&usuario} {}
                const Usuario& usuario() const { return *usu; }
            private:
                const Usuario* usu;
        };

        // Clase de excepción Impostor, en el caso de que el usuario compre con una tarjeta que no es suya.
        class Impostor
        {
            public:
                Impostor(const Usuario& usuario): usu{&usuario} {}
                const Usuario& usuario() const { return *usu; }
            private:
                const Usuario* usu;
        };

        // Clase de excepción SinStock. Antes de lanzar una excepción de este tipo, ¡¡¡¡VACIA EL CARRITO!!!!
        class SinStock
        {
            public:
                SinStock(Articulo& articulo): art_{&articulo} {}
                Articulo& articulo() const { return *art_; }
            private:
                Articulo* art_;
        };

        // Métodos observadores.
        int numero() const { return numped_; }
        const Tarjeta* tarjeta() const { return tarj_; }
        Fecha fecha() const { return f_ped_; }
        double total() const { return importeTotal_; }
        static int n_total_pedidos() { return ultPed_; }
    private:
        int numped_;
        const Tarjeta* tarj_;
        Fecha f_ped_;
        double importeTotal_;
        static int ultPed_;
};

// Operador de inserción en flujo para mostrar los pedidos.
std::ostream& operator<<(std::ostream& os, const Pedido& ped);

