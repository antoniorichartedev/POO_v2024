
#include "usuario.hpp"
#include "tarjeta.hpp"
#include "articulo.hpp"
#include <cstring> // std::strlen
#include <unistd.h> // crypt()
#include <random>  
#include <iomanip> // std::setprecision, std::fixed.
using std::cout;
using std::endl;

/* *** Implementación clase Clave *** */
Clave::Clave(const char* password)
{
    if(std::strlen(password) < 5)
        throw Incorrecta(Razon::CORTA);

    const char* caracteres = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789./";

    // Obtenemos una semilla para generar un número aleatorio.
    std::random_device rd;

    // Generamos un índice aleatorio dentro del rango especificado.
    int n = strlen(caracteres);
    std::uniform_int_distribution<int> generar_indice(0, n);

    // Metemos dos caracteres cualesquiera en la salt.
    char salt[2] = {caracteres[generar_indice(rd)], caracteres[generar_indice(rd)]};

    // Pasamos la clave por crypt().
    auto clavecifrada = crypt(password, salt);

    if(clavecifrada == NULL)
        throw Incorrecta(Razon::ERROR_CRYPT);

    // Guardamos la clave cifrada.
    clave_ = clavecifrada;
}

bool Clave::verifica(const char* cad) const
{
    // Luego, la ciframos.
    char* clave_nueva_cifrada = crypt(cad, clave_.c_str());

    return std::strcmp(clave_nueva_cifrada, clave_.c_str()) == 0;
}

/* ----------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------- */

/* Implementación clase Usuario */

Usuario::Usuario(const Cadena& id, const Cadena& nomb, const Cadena& apel, const Cadena& dire, const Clave& clv): 
    identificador_{id}, nombre_{nomb}, apellidos_{apel}, direccion_{dire}, password_{clv} 
{
    if(!ids.insert(id).second)
    {
        throw Id_duplicado(id);
    }
}

void Usuario::es_titular_de(Tarjeta& tarj)
{
    // Insertamos la tarjeta en el conjunto de tarjetas del usuario.
    if(tarj.titular() == this)
        tarjetas_[tarj.numero()] = &tarj;
}

void Usuario::no_es_titular_de(Tarjeta& tarj)
{
    auto it = tarjetas_.find(tarj.numero());

    if(it != tarjetas_.end())
    {
        tarjetas_.erase(it);
        tarj.anula_titular();
    }
    else
        std::cout << "tarjeta no encontrada." << std::endl;
    
}

Usuario::~Usuario()
{
    for(const auto& tarjetita : tarjetas_)
    {
        if(tarjetita.second->titular() == this)
            tarjetita.second->anula_titular();
    }

    // Borramos al usuario registrado.
    ids.erase(identificador_);
}

void Usuario::compra(Articulo& art, unsigned int cantidad)
{
    if(cantidad == 0)
    {
        articulos_.erase(&art);
    }
    else
    {
        articulos_[&art] = cantidad;
    }
}

void Usuario::vaciar_carro()
{
    articulos_.clear();
}

std::ostream& operator<<(std::ostream& os, const Usuario& us)
{
    os << us.identificador_ << " [" << us.password_.clave() << "] " << us.nombre_ << " " << us.apellidos_ << endl;
    os << us.direccion_ << endl << "Tarjetas:" << endl;
    for(auto it = us.tarjetas_.begin(); it != us.tarjetas_.end(); it++)
    {
        os << *it->second << endl;
    }

    return os;
}

void mostrar_carro(std::ostream& os, const Usuario& us)
{
    os << "Carrito de compra de " << us.id() << " [Artículos: " << us.n_articulos() << "]" << endl;
    os << " Cant. Artículo" << endl << "===========================================================" << endl;
    
    for(auto it = us.compra().begin(); it != us.compra().end(); it++)
    {
        os << "   " << it->second << "     [" << (*it->first).referencia() << "] " << "\"" << (*it->first).titulo() << "\", " << 
        (*it->first).f_publi().anno() << ". " << std::fixed << std::setprecision(2) << (*it->first).precio() << " €" << endl;
    }
}