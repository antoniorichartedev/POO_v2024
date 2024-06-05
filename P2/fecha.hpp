#ifndef FECHA_HPP_
#define FECHA_HPP_

#include <iostream>
#include <cstddef>
#include <ctime> // Para obtener la fecha de hoy.
#include <cstring>
#include <cstdio>
#include <ios>

bool bisiesto(int a);   // Función para saber si un año es bisiesto.

class Fecha{
    public:
        // Ctor. de Fecha
        explicit Fecha(int d = 0, int m = 0, int a = 0);

        // Ctor. de Fecha a partir de una cadena de caracteres.
        Fecha(const char* cadena);

        // Definición de constantes.
        static const int AnnoMinimo = 1902;
        static const int AnnoMaximo = 2037;

        // Clase de excepción.
        class Invalida
        {
            public:
                Invalida(const char* f): fallo{f} {}
                const char* por_que() const { return fallo; }
            private:
                const char* fallo;
        };

        // Operadores.
        Fecha& operator++(); // Incremento prefijo.
        Fecha operator++(int); // Incremento sufijo.
        Fecha& operator--(); // Decremento prefijo.
        Fecha operator--(int); // Decremento sufijo.
        Fecha operator+(int n) const; // Suma de días a la fecha, pero no modifica la original.
        Fecha operator-(int n) const; // Resta de días a la fecha, pero no modifica la original.
        Fecha& operator+=(int n); // Suma con asignación a la fecha, modifica a la original.
        Fecha& operator-=(int n); // Resta con asignación a la fecha, modifica a la original.
        
        // Métodos observadores.
        inline int dia() const noexcept { return dia_; }
        inline int mes() const noexcept { return mes_; }
        inline int anno() const noexcept { return anno_; }

        // Antiguo operador de conversión a const char*. Ahora es una función llamada cadena.
        const char* cadena() const;

        
    private:
        int dia_, mes_, anno_;
        mutable char crep[40];
        mutable bool actual;
        void valida(); // Para validar la fecha.
        void arreglarFecha(int d, int m, int a); // Para arreglar la fecha en el caso de que nos falle algo de la misma.
        
};

// Operadores de comparación.
bool operator <(const Fecha& f1, const Fecha& f2);
bool operator >(const Fecha& f1, const Fecha& f2);
bool operator <=(const Fecha& f1, const Fecha& f2);
bool operator >=(const Fecha& f1, const Fecha& f2);
bool operator ==(const Fecha& f1, const Fecha& f2);
bool operator !=(const Fecha& f1, const Fecha& f2);

// Operadores de insercción y extracción de flujo.
std::ostream& operator<<(std::ostream& os, const Fecha& f1);
std::istream& operator>>(std::istream& is, Fecha& f1);

#endif // FECHA_HPP_