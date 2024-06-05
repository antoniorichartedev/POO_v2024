#ifndef CADENA_HPP_
#define CADENA_HPP_

#include <cstddef> // size_t
#include <iterator> // Para los iteradores.

class Cadena
{
    public:
        // Ctor. de cadena. Son 3 ctores.
        // Cadena con dos parámetros.
        // Cadena con un solo parámetro.
        // Cadena sin parámetros.
        // Ponemos explicit en el constructor para evitar las conversiones implícitas.
        explicit Cadena(size_t tama = 0, char c = ' ');

        // Ctor. de copia.
        Cadena(const Cadena& cad);

        // Ctor. a partir de una cadena de caracteres.
        Cadena(const char* cad);

        // Ctor. de movimiento.
        Cadena(Cadena&& cad);

        // Operador de conversión const char*. Se le pone el explicit para evitar las conversiones implícitas.
        explicit operator const char* () const noexcept { return s_; }
        inline const char* c_str() const { return s_; }
        
        // Operador de asignación.
        Cadena& operator=(const Cadena& cad);
        Cadena& operator=(const char* cad);

        // Operador de asignación por movimiento.
        Cadena& operator=(Cadena&& cad);

        // Operadores.
        Cadena& operator+=(const Cadena& cad);

        // Operador de índice const y no-const.
        char operator[] (size_t i) const noexcept { return s_[i]; }
        char& operator[] (size_t i) noexcept { return s_[i]; }

        // Funciones observadoras.
        inline size_t length() const noexcept{ return tam_; } 

        // Función at(), que comprueba si el índice i está entre los límites.
        // Cosa que el operador de índice corchetes no hace.
        char& at(size_t i); 
        char at(size_t i) const;
        
        // Función que devuelve una subcadena.
        Cadena substr(size_t i, size_t t) const;

        // Iteradores - Tipos miembro de Cadena.
        typedef char* iterator;
        typedef const char* const_iterator;
        typedef std::reverse_iterator<iterator> reverse_iterator;
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

        // Iteradores - Funciones miembro de Cadena.
        iterator begin() noexcept;
        iterator end() noexcept;
        const_iterator begin() const noexcept;
        const_iterator end() const noexcept;
        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;
        reverse_iterator rbegin() noexcept;
        reverse_iterator rend() noexcept;
        const_reverse_iterator rbegin() const noexcept;
        const_reverse_iterator rend() const noexcept;
        const_reverse_iterator crbegin() const noexcept;
        const_reverse_iterator crend() const noexcept;
        
        
        // Destructor de Cadena. Necesario ya que se reserva memoria dinámica.
        ~Cadena();
    private:
        static char vacia[1];
        size_t tam_;
        char* s_;     
};

// Operador de suma de cadenas, tiene que ir fuera.
Cadena operator+(const Cadena& cad1, const Cadena& cad2);

// Operadores de comparación.
bool operator ==(const Cadena& c1, const Cadena& c2);
bool operator !=(const Cadena& c1, const Cadena& c2);
bool operator >=(const Cadena& c1, const Cadena& c2);
bool operator <=(const Cadena& c1, const Cadena& c2);
bool operator >(const Cadena& c1, const Cadena& c2);
bool operator <(const Cadena& c1, const Cadena& c2);


// Operador de extracción en flujo de entrada.
std::istream& operator>>(std::istream& is, Cadena& cad);

// Operador de insercción en flujo de salida.
std::ostream& operator<<(std::ostream& os, const Cadena& cad);


/* ************* Definición de iteradores ************* */

inline Cadena::iterator Cadena::begin() noexcept
{
    return s_;
}

inline Cadena::iterator Cadena::end() noexcept
{
    return s_ + tam_;
}

inline Cadena::const_iterator Cadena::begin() const noexcept
{
    return const_iterator(s_);
}

inline Cadena::const_iterator Cadena::cbegin() const noexcept
{
    return const_iterator(begin());
}

inline Cadena::const_iterator Cadena::cend() const noexcept
{
    return const_iterator(end());
}

inline Cadena::const_iterator Cadena::end() const noexcept
{
    return const_iterator(s_ + tam_);
}

inline Cadena::reverse_iterator Cadena::rbegin() noexcept
{
    return reverse_iterator(end());
}

inline Cadena::reverse_iterator Cadena::rend() noexcept
{
    return reverse_iterator(begin());
}

inline Cadena::const_reverse_iterator Cadena::rbegin() const noexcept
{
    return const_reverse_iterator(end());
}

inline Cadena::const_reverse_iterator Cadena::rend() const noexcept
{
    return const_reverse_iterator(begin());
}

inline Cadena::const_reverse_iterator Cadena::crbegin() const noexcept
{
    return const_reverse_iterator(end());
}

inline Cadena::const_reverse_iterator Cadena::crend() const noexcept
{
    return const_reverse_iterator(begin());
}

namespace std { // Estaremos dentro del espacio de nombres std.
template <> // Es una especializaci ón de una plantilla para Cadena.
struct hash<Cadena> { // Es una clase con solo un operador público.
size_t operator() (const Cadena& cad) const // El operador función.
{
hash<string> hs; // Creamos un objeto hash de string.
auto p{(const char*)(cad)}; // Convertimos Cadena a cadena de bajo nivel.
string s{p}; // Creamos un string desde la cadena de b. nivel .
size_t res{hs(s)}; // El hash del string . Como hs.operator()(s);
return res; // Devolvemos el hash del string.
// En forma abreviada:
// return hash<string>{}((const char*)(cad));
}
};
}

#endif // CADENA_HPP_