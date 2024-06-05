#include "pedido-articulo.hpp"
#include <iomanip> // std::fixed, std::setprecision
#include "usuario.hpp"
#include "pedido.hpp"
#include "articulo.hpp"
#include "tarjeta.hpp"

/* *** Implementación de la clase LineaPedido *** */
LineaPedido::LineaPedido(double p_vent, size_t cant): precio_venta_{p_vent}, cantidad_{cant} {}

std::ostream& operator<<(std::ostream& os, const LineaPedido& lp)
{
    os << std::fixed << std::setprecision(2) << lp.precio_venta() << " €\t" << lp.cantidad() << std::endl;

    return os;
}

/* Implementación de la clase Pedido_Articulo */
bool Pedido_Articulo::OrdenaArticulos::operator() (const Articulo* art1, const Articulo* art2) const 
{
    return art1->referencia() < art2->referencia();
}

bool Pedido_Articulo::OrdenaPedidos::operator()(const Pedido* ped1, const Pedido* ped2) const
{
    return ped1->numero() < ped2->numero();
}

void Pedido_Articulo::pedir(Pedido& p, Articulo& art, double pr, unsigned int cantidad)
{
    ped_art[&p].insert(std::make_pair(&art, LineaPedido{pr, cantidad}));
    art_ped[&art].insert(std::make_pair(&p, LineaPedido(pr, cantidad)));
}

void Pedido_Articulo::pedir(Articulo& art, Pedido& p, double pr, unsigned int cantidad)
{
    pedir(p, art, pr, cantidad);
}

Pedido_Articulo::ItemsPedido Pedido_Articulo::detalle(Pedido& ped) const
{
    auto it = ped_art.find(&ped);

    if(it != ped_art.end())
        return it->second;
    else
        return Pedido_Articulo::ItemsPedido{};
}

Pedido_Articulo::Pedidos Pedido_Articulo::ventas(Articulo& art) const
{
    auto it = art_ped.find(&art);

    if(it != art_ped.end())
        return it->second;
    else
        return Pedido_Articulo::Pedidos{};
}

void Pedido_Articulo::mostrarDetallePedidos(std::ostream& os) const
{
    double precioTotal = 0.0;
    for(auto it = ped_art.begin(); it != ped_art.end(); it++)
    {
        os << "Pedido núm. " << it->first->numero() << std::endl;
        os << "Cliente: " << it->first->tarjeta()->titular() << "\tFecha: " << it->first->fecha() << std::endl;
        os << detalle(*(it->first)) << std::endl;
        precioTotal += it->first->total();
    }

    os << "TOTAL VENTAS\t\t" << precioTotal << " € " << std::endl;
}

void Pedido_Articulo::mostrarVentasArticulos(std::ostream& os) const
{
    for(auto it = art_ped.begin(); it != art_ped.end(); it++)
    {
        os << "Ventas de [" << it->first->referencia() << "\"" << it->first->titulo() << "\"" << std::endl;
        os << ventas(*(it->first)) << std::endl;
    }
}

std::ostream& operator<<(std::ostream& os, const Pedido_Articulo::ItemsPedido& itsped)
{
    os << "  PVP\tCantidad\tArtículo" << std::endl;
    os << "==================================================================" << std::endl;
    double cantidadTotal = 0.0;

    for(auto it = itsped.begin(); it != itsped.end(); it++)
    {
        os << std::fixed << std::setprecision(2) << it->second.precio_venta() << " € " << std::left << std::setw(16) << it->second.cantidad() << 
        "[" << it->first->referencia() << "] \"" << it->first->titulo() << "\"" << std::endl;
        cantidadTotal += it->second.precio_venta() * it->second.cantidad();
    }

    os << "==================================================================" << std::endl;
    os << std::left << std::setw(8) << "Total" << std::fixed << std::setprecision(2) << cantidadTotal << " €" << std::endl;

    return os;
}

std::ostream& operator<<(std::ostream& os, const Pedido_Articulo::Pedidos& peds)
{
    auto cantidad_de_pedidos = peds.size();
    double cantidadTotal = 0.0;
    unsigned int cantidad_de_arti = 0;
    os << "[Pedidos: " << cantidad_de_pedidos << "]" << std::endl;
    os << "==================================================================" << std::endl;
    os << "  PVP\tCantidad\tFecha de venta" << std::endl;
    os << "==================================================================" << std::endl;

    for(auto it = peds.begin(); it != peds.end(); it++)
    {
        os << std::fixed << std::setprecision(2) << it->second.precio_venta() << " € " << std::left << std::setw(16) << it->second.cantidad() << 
        it->first->fecha() << std::endl;
        cantidadTotal += it->second.precio_venta() * it->second.cantidad();
        cantidad_de_arti += it->second.cantidad();
    }

    os << cantidadTotal << " € " << cantidad_de_arti << std::endl;

    return os;
}

