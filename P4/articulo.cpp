#include "articulo.hpp"
#include <iomanip> // std::fixed, std::setprecision

Autor::Autor(const Cadena& nombre, const Cadena& apellidos, const Cadena& direccion):
    nombre_{nombre},
    apellidos_{apellidos},
    direccion_{direccion}
{}


// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

Articulo::Articulo(Autores& aut, Cadena referencia, Cadena titulo, Fecha f_de_publi, double precio, unsigned int existencias):
    cod_referencia{referencia},
    titulo_{titulo},
    f_publi_{f_de_publi},
    precio_{precio},
    n_ejemplares{existencias},
    autores_{aut}
{
    // Si no tenemos autores, ya que el enunciado nos dice que tenemos que tener al menos uno, lanzamos la excepción.
    if(autores_.empty())
        throw Autores_vacios{};
}

std::ostream& operator<<(std::ostream& os, const Articulo& art)
{
    os << "[" << art.referencia() <<"] \"" << art.titulo() << "\", de ";
    
    const auto& autores_arts = art.autores();
    for(auto it = autores_arts.begin(); it != autores_arts.end(); it++)
    {
        // La función std::next() nos permite saber el iterador siguiente donde está ubicado el iterador.
        if(std::next(it) != autores_arts.end())
        {
            os << (*it)->apellidos() << ", ";
        }
        else
        {
            os << (*it)->apellidos() << ". ";
        }
    }
    
    os << art.f_publi().anno() << ". " << std::fixed << std::setprecision(2) << art.precio() << " €" << std::endl;
    os << "\t";

    // Llamamos a impresion_especifica y dependiendo el tipo de objeto, imprimirá una cosa u otra.
    art.impresion_especifica(os);

    return os;
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

ArticuloAlmacenable::ArticuloAlmacenable(Autores& aut, Cadena referencia, Cadena titulo, Fecha f_de_publi, double precio, unsigned int stock):
    Articulo{aut, referencia, titulo, f_de_publi, precio, stock}, stock_{stock} {}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

Libro::Libro(Autores& aut, Cadena referencia, Cadena titulo, Fecha f_de_publi, double precio, unsigned int numpag, unsigned int stock):
    ArticuloAlmacenable{aut, referencia, titulo, f_de_publi, precio, stock}, n_pag_{numpag} {}

void Libro::impresion_especifica(std::ostream& os) const
{
    os << n_pag_ << " págs., " << stock() << " unidades.";
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

Revista::Revista(Autores& aut, Cadena referencia, Cadena titulo, Fecha f_de_publi, double precio, int numero, int periodicidad, unsigned int stock):
    ArticuloAlmacenable{aut, referencia, titulo, f_de_publi, precio, stock}, periodicidad_{periodicidad}, numero_{numero} {}

void Revista::impresion_especifica(std::ostream& os) const
{
    os << "Número: " << numero_ << ", Periodicidad: " << periodicidad_ << " días." << std::endl;

    // Calculamos la próxima fecha.
    Fecha proxima_periodicidad(f_publi() + periodicidad_);

    os << "\tPróximo número a partir de: " << proxima_periodicidad << ".";
}

// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------

LibroDigital::LibroDigital(Autores& aut, Cadena referencia, Cadena titulo, Fecha f_de_publi,  double precio, Fecha f_expiracion):
    Articulo{aut, referencia, titulo, f_de_publi, precio}, f_expir_{f_expiracion} {}

void LibroDigital::impresion_especifica(std::ostream& os) const 
{
    os << "A la venta hasta el " << f_expir_ << ".";
}