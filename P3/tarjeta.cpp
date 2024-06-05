#include "tarjeta.hpp"
#include "usuario.hpp"
#include <iostream>
#include <algorithm>
#include <functional> // para find_if()
#include <cctype> // para isdigit().
#include <cstring>

// Añadimos la función luhn.
bool luhn(const Cadena&);

/* *** Implementación de la clase Numero *** */
Numero::Numero(const Cadena& cad)
{

    // creamos una cadena auxiliar con el tamaño de la cadena que recibe
    // el constructor por parámetro.
    Cadena aux(cad);

    // Quitamos los espacios en blanco. La función remove_if lo que va haciendo es recorrer toda la Cadena en busca de
    // un caracter que sea un espacio en blanco. Cuando lo encuentra, no lo elimina, sino que reorganiza la cadena, de manera que
    // el espacio en blanco lo pone al final de la cadena. Si hay muchos espacios en blanco, a la izq de la cadena están todos los
    // caracteres que no sean espacios en blanco y a la derecha los espacios en blanco:
    /*
        "hola soy una cadena" ----------> "holasoyunacadena   "
    */
    auto it = std::remove_if(aux.begin(), aux.end(), EsBlanco{});
    
    // Luego, si el iterador no está al final de la cadena, quiere decir que había espacios en blanco, por lo que
    // justamente donde está el iterador ponemos el caracter terminador de la cadena.
    if(it != aux.end())
    {
        *it = '\0';

        // Creamos una nueva cadena para asignarle el tamaño justo y necesario a almacenar.
        Cadena aux_pero_con_tamanno_ajustado{aux.c_str()};
        aux = aux_pero_con_tamanno_ajustado;   
    }

    // Ahora, miramos si los caracteres de la cadena son todos dígitos.
    auto condicion = std::not1(EsDigito{});
    if(std::find_if(aux.begin(), aux.end(), condicion) != aux.end())
    {
        // Lanzamos excepción al haber encontrado un caracter que NO es un dígito.
        throw Incorrecto{DIGITOS};
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
        return Tarjeta::Tipo{AmericanExpress};
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