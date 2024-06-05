#ifndef CADENA_HPP_
#define CADENA_HPP_

#include <cstddef> // size_t

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

        // Operador de conversión const char*. Se le pone el explicit para evitar las conversiones implícitas.
        explicit operator const char* () const noexcept { return s_; }

        // Operador de asignación.
        Cadena& operator=(const Cadena& cad);
        Cadena& operator=(const char* cad);

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

#endif // CADENA_HPP_