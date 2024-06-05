#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include "../P1/cadena.hpp"

class Articulo;
class Numero;
class Tarjeta; 

/* Clase Clave */
class Clave
{
    public:

        // Ctor.
        Clave(const char* password);

        // Enum para clase Incorrecta.
        enum Razon{CORTA, ERROR_CRYPT};

        // Clase para excepciones.
        class Incorrecta
        {
            public:
                Incorrecta(Razon r): razon_{r} {}
                Razon razon() const { return razon_; }
            private:
                Razon razon_;
        };

        // Método observador.
        const Cadena& clave() const { return clave_; }

        // Método verifica() -> true si cad corresponde con la clave almacenada y 
        // false lo contrario.
        bool verifica(const char* cad) const;
    private:
        Cadena clave_;
};

/* ******************************************************************************* */
/* ******************************************************************************* */
/* ******************************************************************************* */


/* *** Clase Usuario *** */
class Usuario
{
    public:
        // Sinónimo público Tarjetas, se define aquí.
        typedef std::map<Numero, Tarjeta*> Tarjetas;

        // Sinónimo público Articulos, el unsigned int es el atributo de enlace de 
        // la asociación Usuario-Articulo.
        typedef std::unordered_map<Articulo*, unsigned int> Articulos;

        // Ctor. Comprueba que el usuario que se vaya a construir sea correcto (id no repetido).
        Usuario(const Cadena& id, const Cadena& nomb, const Cadena& apel, const Cadena& dire, const Clave& clv);

        // Clase de excepción Id_duplicado.
        class Id_duplicado
        {
            public:
                Id_duplicado(const Cadena& cad): id_dupli{cad} {}
                const Cadena& idd() const { return id_dupli; }
            private:
                Cadena id_dupli;
        };

        // Ctor de copia. Lo ponemos en delete porque está PROHIBIDA la copia de usuarios.
        Usuario(const Usuario& usu) = delete;

        // Lo mismo pasa con la asignación.
        Usuario& operator=(const Usuario& usu) = delete;

        // Métodos observadores.
        const Cadena& id() const { return identificador_; }
        const Cadena& nombre() const { return nombre_; }
        const Cadena& apellidos() const { return apellidos_; }
        const Cadena& direccion() const { return direccion_; }
        const Tarjetas& tarjetas() const { return tarjetas_; }
        const Articulos& compra() const { return articulos_; }
        size_t n_articulos() const { return articulos_.size(); }
        
        // Asociaciones.
        void es_titular_de(Tarjeta& tarj);
        void no_es_titular_de(Tarjeta& tarj);
        void compra(Articulo& art, unsigned int cantidad = 1);

        friend std::ostream& operator <<(std::ostream& os, const Usuario& us);

        void vaciar_carro();

        // Destructor.
        ~Usuario();
    private:
        const Cadena identificador_, nombre_, apellidos_, direccion_;
        Clave password_;
        Tarjetas tarjetas_;
        Articulos articulos_;

        inline static std::unordered_set<Cadena> ids;
        typedef std::unordered_set<Cadena>::iterator tipoIt;
        
};

void mostrar_carro(std::ostream& os, const Usuario& us);