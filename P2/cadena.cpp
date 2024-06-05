#include "cadena.hpp"
#include <cstring>
#include <stdexcept>
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

// Inicializamos el atributo vacía.
char Cadena::vacia[1] = {'\0'};

Cadena::Cadena(size_t tama, char c) : tam_{tama}, s_{tam_ == 0 ? &vacia[0] : new char[tam_ + 1]}
{ 
    for(size_t i = 0; i < tam_; i++)
        s_[i] = c;

    s_[tam_] = '\0';
}

Cadena::Cadena(const Cadena &cad): tam_{cad.tam_}, s_{tam_ == 0 ? &vacia[0] : new char[tam_ + 1]}
{
    // simplemente copiamos el contenido de la cadena cad.
    if(tam_ != 0)
        std::strcpy(s_, cad.s_);
}

Cadena::Cadena(const char* cad) : tam_{std::strlen(cad)}, s_{ tam_ == 0 ? &vacia[0] : new char[tam_ + 1]}
{
    // simplemente copiamos el contenido de la cadena cad.
    if(tam_ != 0)
        std::strcpy(s_, cad);
}

Cadena::~Cadena()
{
    if(tam_ != 0)
        delete[] s_;
}

Cadena& Cadena::operator=(const Cadena& c2)
{
    if (this != &c2)
    {
        if (tam_ != 0)
            delete[] s_;

        tam_ = c2.tam_;
        s_ = tam_ == 0 ? vacia : new char[tam_ + 1];

        if(tam_ != 0)
            std::strcpy(s_, c2.s_);
    }

    return *this;
}

Cadena& Cadena::operator=(const char* cad)
{
    // Comprobamos que las cadenas sean distintas.
    if(s_ != cad)
    {
        // Comprobamos que la cadena implícita no está vacía. Si no lo está, la borramos.
        if(tam_ != 0)
            delete[] s_;
        
        tam_ = std::strlen(cad);
        s_ = tam_ == 0 ? &vacia[0] : new char[tam_ + 1];

        if(tam_ != 0)
            std::strcpy(s_, cad);
    }

    return *this;
}

Cadena& Cadena::operator+=(const Cadena& cad)
{ 
    // Creamos una cadena nueva con el tamaño especificado.
    const auto tam = tam_ + cad.tam_;
    const char* cadenita = s_;

    // Asignamos memoria suficiente para albergar la cadena concatenada resultante.
    s_ = new char[tam + 1]();

    // Copiamos la primera cadena en la cadena de caracteres que hemos creado previamente.
    std::strncpy(s_, cadenita, tam_);

    // Luego, copiamos la segunda cadena en cadenita.
    std::strncpy(s_ + tam_, cad.s_, cad.tam_);

    // Modificamos el tamaño de la cadena principal.
    tam_ = tam;

    // Liberamos a cadenita.
    delete[] cadenita;
    
    return *this;
}

Cadena operator+(const Cadena& cad1, const Cadena& cad2)
{
    // Creamos una cadena nueva con el tamaño especificado.
    const auto tam = cad1.length() + cad2.length();
    Cadena cadenita{tam};

    // Copiamos la primera cadena en la cadena de caracteres que hemos creado previamente.
    std::strncpy(&cadenita[0], (const char *)cad1, cad1.length());

    // Una vez copiada la primera cadena, concatenamos la segunda cadena.
    std::strcpy(&cadenita[0] + cad1.length(), (const char*)cad2);

    // Devolvemos la cadena resultante.
    return cadenita;
}

bool operator ==(const Cadena& c1, const Cadena& c2)
{
    return std::strcmp((const char *)c1, (const char*)c2) == 0;
}

bool operator !=(const Cadena& c1, const Cadena& c2)
{
    return !(c1 == c2);
}

bool operator <(const Cadena& c1, const Cadena& c2)
{
    return std::strcmp((const char *)c1, (const char*)c2) < 0;
}

bool operator >(const Cadena& c1, const Cadena& c2)
{
    return c2 < c1;
}

bool operator >=(const Cadena& c1, const Cadena& c2)
{
    return !(c1 < c2);
}

bool operator <=(const Cadena& c1, const Cadena& c2)
{
    return !(c2 < c1);
}

char& Cadena::at(size_t i) 
{
    if(i > tam_ - 1 or tam_ == 0)
        throw std::out_of_range("Te has pasado de los límites.");
    
    return s_[i];
}

char Cadena::at(size_t i) const
{
    if(i > tam_ - 1 or tam_ == 0)
        throw std::out_of_range("Te has pasado de los límites.");
    
    return s_[i];
}

Cadena Cadena::substr(size_t i, size_t tam) const
{   
    if(i > tam_ - 1 or tam > tam_ - i)
        throw std::out_of_range("Límites excedidos.");

    // Creamos un objeto cadena, pasandole la subcadena.
    Cadena cad(tam);

    // Obtenemos la subcadena.
    std::strncpy(&cad[0], s_ + i, tam);

    return cad;

}

std::ostream& operator<<(std::ostream& os, const Cadena& cad)
{
    // Insertamos la cadena en el flujo, sin ningún cambio.
    os << cad.c_str();

    return os;
}

std::istream& operator>>(std::istream& is, Cadena& cad)
{
    // Creamos una cadena de bajo nivel para meter la palabra.
    char cadenita[33] {};

    // Introducimos la palabra de la entrada estándar en la cadena de bajo nivel.
    is.width(33);
    is >> cadenita;

    // Guardamos la cadenita en la Cadena que se pasó por parámetro.
    cad = cadenita;

    return is;
}


/* ************ Definición del constructor y operador de asignación de movimiento ************ */

Cadena::Cadena(Cadena&& cad): tam_{cad.length()}, s_{cad.s_}
{
    // Lo ponemos apuntando a vacía, para dejarlo en el estado válido que se menciona.
    cad.s_ = &cad.vacia[0];

    // El tamaño lo ponemos a 0;
    cad.tam_ = 0;
}

Cadena& Cadena::operator=(Cadena&& cad)
{
    // Evitamos autoasignación.
    if(this != &cad)
    {
        if(tam_ != 0)
            delete[] s_;

        tam_ = cad.tam_;
        s_ = cad.s_;

        // Ponemos la cadena a copiar en un estado válido tras el movimiento.
        cad.s_ = &vacia[0];
        cad.tam_ = 0;
    }

    return *this;
}