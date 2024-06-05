#include <cstddef> // size_t
#include <iostream> // std::ostream

#include "../P1/cadena.hpp"
#include "../P1/fecha.hpp"



class Articulo
{
    public:
        // Ctor.
        Articulo (Cadena referencia, Cadena titulo, Fecha f_de_publi, double precio, unsigned int existencias = 0);

        // Métodos observadores.
        const Cadena referencia() const { return cod_referencia; }
        const Cadena titulo() const { return titulo_; }
        const Fecha f_publi() const { return f_publi_; }
        double precio() const { return precio_; }
        double& precio() { return precio_; }
        unsigned int stock() const { return n_ejemplares; }
        unsigned int& stock() { return n_ejemplares; }


    private:
        const Cadena cod_referencia, titulo_;
        const Fecha f_publi_;
        double precio_;
        unsigned int n_ejemplares;
};

// Operador de inserción en flujo de salida de Articulo.
std::ostream& operator<<(std::ostream& os, const Articulo& art);
