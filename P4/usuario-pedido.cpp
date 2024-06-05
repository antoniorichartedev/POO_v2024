#include "usuario-pedido.hpp"

void Usuario_Pedido::asocia(Usuario& us, Pedido& ped)
{
    // Para insertar en un diccionario de un puntero a un objeto y un conjunto de punteros a objetos.
    // std::map<Usuario*, std::set<Pedido*>>
    usu_ped[&us].insert(&ped);
    
    // Para insertar en un diccionario de un puntero a un objeto y a otro.
    ped_usu.insert(std::make_pair(&ped, &us)); 
}

void Usuario_Pedido::asocia(Pedido& ped, Usuario& us)
{
    asocia(us, ped);
}

Usuario_Pedido::Pedidos Usuario_Pedido::pedidos(Usuario& us) const
{
    auto it_usuario = usu_ped.find(&us);

    if(it_usuario != usu_ped.end())
        return it_usuario->second;
    else
        return Pedidos{};
}

Usuario* Usuario_Pedido::cliente(Pedido& ped) const
{
    auto it_pedido = ped_usu.find(&ped);

    if(it_pedido != ped_usu.end())
        return it_pedido->second;
    else
        return nullptr;
}

