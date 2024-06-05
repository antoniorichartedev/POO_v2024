#include "tarjeta.hpp"
#include "usuario.hpp"
#include <iostream>
#include <algorithm>
#include <cctype> // para isdigit().
#include <cstring>

// Añadimos la función luhn.
bool luhn(const Cadena&);

/* *** Implementación de la clase Numero *** */
Numero::Numero(const Cadena& cad)
{
    unsigned int tamaux = 0;

    // creamos una cadena auxiliar con el tamaño de la cadena que recibe
    // el constructor por parámetro.
    Cadena aux(cad.length());

    for(unsigned int i = 0; i < cad.length(); i++){
        // Si no es un espacio, seguimos.
        if(!isspace((unsigned char)cad[i])){

            // Si el caracter sobre el que estamos no es ni un dígito
            // ni un espacio, se lanza la excepción Incorrecto.
            if(!isdigit((unsigned char) cad[i])){
                throw Incorrecto{DIGITOS};
            }

            // Le asignamos el caracter actual a aux.
            aux[tamaux++] = cad[i];
        }
    }

    if(tamaux != cad.length()){
        // Utilizamos la función substr de Cadena para obtener el tamaño justo y necesario.
        aux = aux.substr(0, tamaux);
    }

    // Si la longitud de la tarjeta se pasa del rango, lanzamos la excepción correspondiente.
    if(aux.length() < 13 or aux.length() > 19){
        throw Incorrecto{LONGITUD};
    }

    // Si el algoritmo de luhn nos devuelve false, quiere decir que el número de la tarjeta
    // no es válido, por lo tanto, lanzamos la excepción correspondiente.
    if(!luhn(aux)){
        throw Incorrecto{NO_VALIDO};
    }

    // Si todo está correcto, guardamos el número.
    numero_ = aux;
}

bool operator <(const Numero& n1, const Numero& n2)
{
    return std::strcmp(n1, n2) < 0;
}

/* ---------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------- */

/* *** Implementación de la clase Tarjeta */

Tarjeta::Tarjeta(const Numero& n, Usuario& u, const Fecha& f): numero_{n}, user_{&u}, f_cad_{f}
{
    if(f_cad_ < Fecha()){
        throw Tarjeta::Caducada(f_cad_);
    }

    // Asignamos esta tarjeta a su usuario.
    u.es_titular_de(*this);

    // Recuerda el punto 5.6
    if(!numeros_tarjetas.insert(n).second){
        throw Num_duplicado{n};
    }

    // Activamos la tarjeta una vez que lleguemos a este punto.
    activa_ = true;
}



Tarjeta::Tipo Tarjeta::tipo() const noexcept
{
    if(numero_[0] == '3' && (numero_[1] == '4' || numero_[1] == '7'))
        return Tarjeta::Tipo{AmericanExpress};
    else if(numero_[0] == '3' && !(numero_[1] == '4' || numero_[1] == '7'))
        return Tarjeta::Tipo{JCB};
    else if(numero_[0] == '4')
        return Tarjeta::Tipo{VISA};
    else if(numero_[0] == '5')
        return Tarjeta::Tipo{Mastercard};
    else if(numero_[0] == '6')
        return Tarjeta::Tipo{Maestro};
    else
        return Tarjeta::Tipo{Otro};
    
}

bool Tarjeta::activa(bool estadoTarjeta)
{
    activa_ = estadoTarjeta;
    return activa_;
}

void Tarjeta::anula_titular()
{
    if(user_ != nullptr)
    {
        user_ = nullptr;

        // desactivamos la tarjeta.
        activa_ = false;
    }
}

Tarjeta::~Tarjeta()
{
    if(user_ != nullptr)
        const_cast<Usuario*>(user_)->no_es_titular_de(*this);

    numeros_tarjetas.erase(numero_);
}

bool operator <(const Tarjeta& t1, const Tarjeta& t2)
{
    return t1.numero() < t2.numero();
}

std::ostream& operator <<(std::ostream& os, const Tarjeta::Tipo& tip)
{
    static Cadena cadenas[] {"Otro", "VISA", "Mastercard", "Maestro", "JCB", "American Express"};

    return os << cadenas[tip];
}

std::ostream& operator <<(std::ostream& os, const Tarjeta& tarjeta)
{
    os << tarjeta.tipo() << std::endl;
    os << tarjeta.numero() << std::endl;

    Cadena nombre{tarjeta.titular()->nombre().c_str()}, apellidos{tarjeta.titular()->apellidos().c_str()};  //Guardamos las cadenas en cuestion
    for(auto i = nombre.begin(); i != nombre.end(); i++)
        *i = toupper((unsigned char) *i);
    for(auto i = apellidos.begin(); i != apellidos.end(); i++)
        *i = toupper((unsigned char) *i);

    os << nombre << " " << apellidos << std::endl;

    os << "Caduca: ";
    if(tarjeta.caducidad().mes() < 10)
        os << "0" << tarjeta.caducidad().mes();
    else
        os << tarjeta.caducidad().mes();
    if(tarjeta.caducidad().anno() % 1000 < 10)
        os << "/0" << tarjeta.caducidad().anno() % 1000 << std::endl;
    else
        os << "/" << tarjeta.caducidad().anno() % 1000 << std::endl;

    return os;
}