#include <iostream>
#include <cstring>
#include <cctype> // Para usar isspace
#include <cstddef> // size_t
using namespace std;



int main()
{
    char* cadenita = new char[33];
    char* cadenitasinespacios = new char[33];
    
    // metemos la cadena de la entrada estándar en la cadenita.
    cout << "Introduce la cadena con espacios, para luego quitárselos -> ";
    cin.getline(cadenita, 33);

    // contador para la nueva cadena.
    int j = 0;

    // buscamos el primer carácter que no sea un espacio en blanco.
    while(j < strlen(cadenita) and isspace(static_cast<unsigned char>(cadenita[j])))
        j++;

    for(size_t i = 0; i < strlen(cadenita) and !isspace(static_cast<unsigned char>(cadenita[j])); i++, j++)
    {
        cadenitasinespacios[i] = cadenita[j];
    }

    // añadimos el caracter nulo al final de la cadena y la mostramos.
    cadenitasinespacios[j] = '\0';

    cout << "Cadenita sin espacios al principio -> " << cadenitasinespacios << endl;
    cout << "Cadenita con espacios -> " << cadenita << endl;

    // Liberamos memoria.
    delete[] cadenita;
    delete[] cadenitasinespacios;

    return 0;

}