#include "pedido.hpp"
#include "pedido-articulo.hpp"
#include "usuario-pedido.hpp"
#include "articulo.hpp"
#include "usuario.hpp"
#include "tarjeta.hpp"

#include <iomanip>

std::ostream& operator<<(std::ostream& os, const Pedido& ped)
{
    os << "Núm. pedido: " << ped.numero() << std::endl;
    os << "Fecha:\t" << ped.fecha() << std::endl;
    os << "Pagado con:   " ;

    if(ped.tarjeta()->tipo() == Tarjeta::Otro){
        os << "Tipo indeterminado";
    }else{
        os << ped.tarjeta()->tipo();
    }

    os << " n.º: " << ped.tarjeta()->numero() << std::endl;
    os << std::left << std::setw(13) << "Importe:" << std::fixed << std::setprecision(2) << ped.total() << " €" << std::endl; 

    return os;
}

// Inicializamos el número de pedidos a 1.
int Pedido::ultPed_{0};

Pedido::Pedido(Usuario_Pedido& usu_ped, Pedido_Articulo& ped_art, Usuario& usu, const Tarjeta& tarj, const Fecha& fech_pedido): 
    numped_{++ultPed_},
    tarj_{&tarj},
    f_ped_{fech_pedido},
    importeTotal_{0}
{
    // Vamos a realizar las comprobaciones pertinentes.

    // Primero, si el carrito no tiene ningún artículo, lanzamos la excepción Vacio.
    if(usu.compra().empty())
    {
        ultPed_--; 
        throw Vacio{usu};
    }

    // Segundo, comprobamos que el titular de la tarjeta se corresponde con el usuario que realiza el pedido.
    if(!(usu.nombre() == tarj.titular()->nombre() && usu.apellidos() == tarj.titular()->apellidos()))
    {
        ultPed_--; 
        throw Impostor{usu};
    }

    // Tercero, si la cantidad de alguno de los artículos del carrito supera las existencias actuales, lanzamos una excepción del tipo SinStock.
    // Buscamos el artículo en el carrito del usuario.
    for(auto it = usu.compra().begin(); it != usu.compra().end(); it++)
    {
        if(it->first->stock() < it->second)
        {
            // Como no se puede hacer el pedido, decrementamos la variable de pedidos totales hasta ahora.
            ultPed_--;

            // Lanzamos la excepción.
            auto art_sin_stock = it->first;

            // Vaciamos el carrito.
            usu.vaciar_carro();

            throw SinStock{*art_sin_stock};
        }
    }

    // Cuarto, si la fecha de caducidad de la tarjeta es anterior a la fecha actual en la que se hace el pedido, lanzamos la excepción Tarjeta::Caducada.
    if(tarj_->caducidad() < f_ped_)
    {
        ultPed_--; 
        throw Tarjeta::Caducada{tarj_->caducidad()};
    }

    // Quinto, si la tarjeta no está activa, lanzamos la excepción Tarjeta::Desactivada.
    if(!tarj_->activa())
    {
        ultPed_--; 
        throw Tarjeta::Desactivada{};
    }

    // Tras las comprobaciones, si todo está bien, asociamos el pedido actual con el usuario y con los artículos.
    // Primero, asociamos el usuario.
    usu_ped.asocia(*this, usu);

    // Ahora, asociamos todos los artículos con el pedido.
    for(auto it = usu.compra().begin(); it != usu.compra().end(); it++)
    {
        // Decrementamos el stock solicitado.
        it->first->stock() -= it->second;

        // Realizamos la asociación.
        ped_art.pedir(*(it->first), *this, it->first->precio(), it->second);

        importeTotal_ += it->first->precio() * it->second;
    }

    // Luego de haber hecho el pedido, vaciamos el carrito.
    usu.vaciar_carro();
}       