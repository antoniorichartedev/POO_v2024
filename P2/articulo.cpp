#include "articulo.hpp"
#include <iomanip> // std::fixed, std::setprecision


Articulo::Articulo(Cadena referencia, Cadena titulo, Fecha f_de_publi, double precio, unsigned int existencias):
    cod_referencia{referencia},
    titulo_{titulo},
    f_publi_{f_de_publi},
    precio_{precio},
    n_ejemplares{existencias}
{}

std::ostream& operator<<(std::ostream& os, const Articulo& art)
{
    os << "[" << art.referencia() <<"] \"" << art.titulo() << "\", " << art.f_publi()
    << ". " << std::fixed << std::setprecision(2) << art.precio() << " €" << std::endl;


    return os;
}

