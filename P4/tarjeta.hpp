#include <set>
#include "../P1/cadena.hpp"
#include "../P1/fecha.hpp"

class Usuario;

/* *** Clase Número *** */
class Numero
{
    public:
        Numero(const Cadena& num);

        // Enumeración para saber por qué un número no es válido.
        enum Razon{LONGITUD, DIGITOS, NO_VALIDO};

        // Clase de excepción Incorrecto.
        class Incorrecto
        {
            public:
                Incorrecto(const Razon& r): razon_{r} {}
                Razon razon() const { return razon_; }
            private:
                Razon razon_;
        };

        // Operador de conversión a cadena de caracteres.
        operator const char*() const { return numero_.c_str(); }
        
        // Clase EsDigito, para saber si un caracter de la cadena es un dígito o no.
        struct EsDigito : public std::unary_function<char,bool>
        {
            bool operator()(char c) const
            {
                // Comprueba que es un dígito.
                return isdigit(c);
            }
        };

        struct EsBlanco: public std::unary_function<char,bool>
        {
            bool operator()(char c) const
            {
                // Si lo quisieramos poner como una función lambda (es decir, una función anónima) -> [](unsigned char c) { return c == ' '; }
                return isspace(c);
            }
        };

    private:
        Cadena numero_;
};

// Operador < para dos objetos número.
bool operator <(const Numero& n1, const Numero& n2);


/* *** Clase Tarjeta *** */
class Tarjeta
{
    public:
        friend class Usuario;
        // Enumeración pública de tipos de Tarjetas llamada Tipo.
        enum Tipo{Otro, VISA, Mastercard, Maestro, JCB, AmericanExpress};

        // Ctor.
        Tarjeta(const Numero& n, Usuario& u, const Fecha& f);

        // Clases de excepción.
        class Num_duplicado
        {
            public:
                Num_duplicado(const Numero& d): dupli_{d} {}
                const Numero& que() const { return dupli_; }
            private:
                Numero dupli_;
        };

        class Caducada
        {
            public:
                Caducada(const Fecha& fcad): caducada_{fcad} {}
                const Fecha& cuando() const { return caducada_; }
            private:
                Fecha caducada_;
        };

        class Desactivada {};
        
        // Ctor de copia, movimiento y operadores asociados.
        Tarjeta(const Tarjeta& t) = delete;
        Tarjeta(Tarjeta&& t) = delete;

        // Métodos observadores y modificadores.
        const Numero& numero() const noexcept { return numero_; };
        const Usuario* titular() const noexcept { return user_; };
        const Fecha& caducidad() const noexcept { return f_cad_; };
        bool activa() const noexcept { return activa_; };
        bool activa(bool estadoTarjeta); 
        Tipo tipo() const noexcept;

        // Destructor.
        ~Tarjeta();

    private:    
        const Numero numero_;
        const Usuario* user_;
        const Fecha f_cad_;
        bool activa_;
        void anula_titular();

        inline static std::set<Numero> numeros_tarjetas;
        typedef std::set<Numero>::iterator tipoIt_Tarjetas;
};

// Operador menor-que para ordenar tarjetas.
bool operator <(const Tarjeta& t1, const Tarjeta& t2);

// Operadores de flujo de salida de Tarjetas.
std::ostream& operator <<(std::ostream& os, const Tarjeta& tarj);
std::ostream& operator <<(std::ostream& os, const Tarjeta::Tipo& tip);

