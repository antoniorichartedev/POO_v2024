#include <iostream>
#include "../P1/cadena.hpp"
#include <algorithm>

int main()
{
    Cadena cad1("hola soy una cadena");

    auto it = std::remove_if(cad1.begin(), cad1.end(), [](unsigned char c) { return c == ' '; });

    if(it != cad1.end())
    {
        *it = '\0';
        
    }

    auto tamcad1 = cad1.length();

    std::cout << "Cadena sin espacios en blanco -> " << cad1 << std::endl;
    std::cout << "Tamaño de la cadena -> " << tamcad1 << std::endl;

    Cadena aux{cad1.c_str()};
    cad1 = aux;
    tamcad1 = cad1.length();

    std::cout << std::endl;

    std::cout << "Cadena después de unos cambios -> " << cad1 << std::endl;
    std::cout << "Tamaño de la cadena tras los cambios -> " << tamcad1 << std::endl;

}