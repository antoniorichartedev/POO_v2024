#include <cstddef> // size_t
#include <iostream> // std::ostream
#include <set>
#include "../P1/cadena.hpp"
#include "../P1/fecha.hpp"

class Autor
{
    public:
        Autor(const Cadena& nombre, const Cadena& apellidos, const Cadena& direccion);

        // Métodos observadores.
        const Cadena nombre() const noexcept{ return nombre_; }
        const Cadena apellidos() const noexcept { return apellidos_; }
        const Cadena direccion() const noexcept { return direccion_; }

    private:
        Cadena nombre_, apellidos_, direccion_;
};

class Articulo
{
    public:
        // Declaración del tipo Autores.
        typedef std::set<Autor*> Autores;

        // Ctor.
        Articulo (Autores& aut, Cadena referencia, Cadena titulo, Fecha f_de_publi, double precio, unsigned int existencias = 0);

        // Métodos observadores.
        const Cadena referencia() const { return cod_referencia; }
        const Cadena titulo() const { return titulo_; }
        const Fecha f_publi() const { return f_publi_; }
        double precio() const { return precio_; }
        double& precio() { return precio_; }
        unsigned int stock() const { return n_ejemplares; }
        unsigned int& stock() { return n_ejemplares; }
        Autores autores() const { return autores_; }
        // Clase de excepción Autores_vacios
        class Autores_vacios
        {
            public:

            private:

        };

        // Método virtal puro con el que mostraremos la información de las clases derivadas de esta.
        virtual void impresion_especifica(std::ostream& os) const = 0;

        // Destructor virtual.
        virtual ~Articulo() {};
    private:
        const Cadena cod_referencia, titulo_;
        const Fecha f_publi_;
        double precio_;
        unsigned int n_ejemplares;
        const Autores autores_;
};

// Operador de inserción en flujo de salida de Articulo.
std::ostream& operator<<(std::ostream& os, const Articulo& art);

// -------------------------------------------------------------------------------------
// ------------------------  Clases derivadas de Articulo  -----------------------------
// -------------------------------------------------------------------------------------

class ArticuloAlmacenable: public Articulo
{
    public:
        ArticuloAlmacenable(Autores& aut, Cadena referencia, Cadena titulo, Fecha f_de_publi, double precio, unsigned int stock = 0);

        // Método virtal puro con el que mostraremos la información de las clases derivadas de esta.
        virtual void impresion_especifica(std::ostream& os) const = 0;

    private:
        unsigned int stock_;

};

// Le indicamos final porque a partir de él, no hay más clases derivadas, por lo que se lo avisamos al compilador.
class Libro final: public ArticuloAlmacenable
{
    public:
        Libro(Autores& aut, Cadena referencia, Cadena titulo, Fecha f_de_publi, double precio, unsigned int numpag, unsigned int stock = 0);

        // Métodos observadores.
        unsigned int n_pag() const { return n_pag_; }

        // Método de impresión específica, aquí si que hay que implementarla.
        void impresion_especifica(std::ostream& os) const;

    private:
        const unsigned int n_pag_;

};

class Revista: public ArticuloAlmacenable
{
    public:
        Revista(Autores& aut, Cadena referencia, Cadena titulo, Fecha f_de_publi, double precio, int numero, int periodicidad, unsigned int stock = 0);

        // Métodos observadores.
        int periodicidad() const { return periodicidad_; }
        int numero() const { return numero_; }

        // Método de impresión específica, aquí si que hay que implementarla.
        void impresion_especifica(std::ostream& os) const;

    private:
        const int periodicidad_, numero_;
};

class LibroDigital: public Articulo
{
    public:
        LibroDigital(Autores& aut, Cadena referencia, Cadena titulo, Fecha f_de_publi, double precio, Fecha f_expiracion);

        // Métodos observadores.
        Fecha f_expir() const { return f_expir_; }

        // Método de impresión específica, aquí si que hay que implementarla.
        void impresion_especifica(std::ostream& os) const;

    private:
        const Fecha f_expir_;
};