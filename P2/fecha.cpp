#include "fecha.hpp"

static int diasMes[12] = {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

bool bisiesto(int a)
{
    return (a % 4 == 0 && a % 100 != 0) || a % 400 == 0;
}

Fecha::Fecha(int d, int m, int a) : dia_{d}, mes_{m}, anno_{a}, actual{false}
{
    arreglarFecha(d, m, a);
    valida();
    
}

void Fecha::arreglarFecha(int d, int m, int a)
{
    std::time_t tiempo_calendario = std::time(nullptr);
    std::tm *tiempo_descompuesto = std::localtime(&tiempo_calendario);

    if(d == 0)
        dia_ = tiempo_descompuesto->tm_mday;
        
    if(m == 0)
        mes_ = tiempo_descompuesto->tm_mon + 1;      // tm_mon: 0 (ene)..11 (dic)

    if(a == 0)
        anno_ = tiempo_descompuesto->tm_year + 1900; // tm_year: años desde 1900

    
}

void Fecha::valida()
{
    if(dia_ < 1 or dia_ > diasMes[mes_ - 1])
        throw Invalida("Te has pasado de los límites en el día.");

    if(mes_ < 0 or mes_ > 12)
        throw Invalida("Te has pasado de los límtes del mes.");

    if(anno_ < AnnoMinimo or anno_ > AnnoMaximo)
        throw Invalida("Te has pasado de los límtes del año.");

    if(dia_ == 29 and mes_ == 2 and !bisiesto(anno_))
        throw Invalida("Este año no es bisiesto.");
}

Fecha::Fecha(const char* cadena)
{
    if(std::sscanf(cadena, "%d/%d/%d", &dia_, &mes_, &anno_) != 3)
        throw Invalida("La fecha introducida no cumple con el formato");

    arreglarFecha(dia_, mes_, anno_);
    valida();
    actual = false;
}

// Operadores de comparación.
bool operator <(const Fecha& f1, const Fecha& f2)
{
    bool menorque = false;

    if(f1.anno() < f2.anno())
        menorque = true;
    else
    {
        if(f1.anno() == f2.anno())
        {
            if(f1.mes() < f2.mes())
                menorque = true;
            else
            {
                if(f1.mes() == f2.mes())
                {
                    if(f1.dia() < f2.dia())
                        menorque = true;
                }
            }
        }  
    }

    return menorque;
}

bool operator >(const Fecha& f1, const Fecha& f2)
{
    return (f2 < f1);
}

bool operator <=(const Fecha& f1, const Fecha& f2)
{
    return !(f2 < f1);
}

bool operator >=(const Fecha& f1, const Fecha& f2)
{
    return !(f1 < f2);
}

bool operator ==(const Fecha& f1, const Fecha& f2)
{
    bool iguales = false;

    if(f1.anno() == f2.anno())
        if(f1.mes() == f2.mes())
            if(f1.dia() == f2.dia())
                iguales = true;
    
    return iguales;
}

bool operator !=(const Fecha& f1, const Fecha& f2)
{
    return !(f1 == f2);
}


// Operadores de asignación.

Fecha& Fecha::operator+=(int n)
{
    // Primero, sumamos n al día.
    dia_ += n;

    // Creamos la estructura tm.
    std::tm tm {};
    tm.tm_mday = dia_;
    tm.tm_mon = mes_ - 1;
    tm.tm_year = anno_ - 1900;

    // utilizamos mktime para normalizar la fecha.
    std::mktime(&tm);

    // Ahora, cada variable del tm los asignamos a cada una de las variables de la fecha.
    dia_ = tm.tm_mday;
    mes_ = tm.tm_mon + 1;
    anno_ = tm.tm_year + 1900;

    valida();
    actual = false;
    return *this;
}

Fecha& Fecha::operator-=(int n)
{
    *this += -n;
    valida();
    actual = false;
    return *this;
}

Fecha Fecha::operator+(int n) const
{
    Fecha t = *this;
    t += n;
    t.valida();
    t.actual = false;
    return t;
}

Fecha Fecha::operator-(int n) const
{
    Fecha t = *this;
    t += -n;
    t.valida();
    t.actual = false;
    return t;
}

Fecha& Fecha::operator++()
{
    *this += 1;
    valida();
    actual = false;
    return *this;
}

Fecha Fecha::operator++(int)
{
    Fecha t = *this;
    *this += 1;
    valida();
    actual = false;
    return t;
}

Fecha& Fecha::operator--()
{
    *this += -1;
    valida();
    actual = false;
    return *this;
}

Fecha Fecha::operator--(int) 
{
    Fecha t = *this;
    *this += -1;
    valida();
    t.actual = false;
    return t;
}

const char* Fecha::cadena() const
{
    if(!actual)
    { 
        // Creamos la estructura tm y ajustamos sus variables.
        std::tm tm{};
        tm.tm_mday = dia_;
        tm.tm_mon = mes_ - 1;
        tm.tm_year = anno_ - 1900;

        // Lo pasamos por mktime.
        std::mktime(&tm);

        // Ahora, vamos a meter la fecha como una cadena de caracteres en Crep.
        std::strftime(crep, 40, "%A %e de %B de %Y", &tm);

        // Ponemos actual a true, ya que tenemos la cadena actual introducida en crep.
        actual = true;
    }

    // Devolvemos Crep.
    return crep;

}

// Operador de insercción de flujo de salida.
std::ostream& operator<<(std::ostream& os, const Fecha& f1)
{
    // Basta con devolver la función cadena.
    os << f1.cadena();

    return os;
}

// Operador de extracción de flujo de entrada.
std::istream& operator>>(std::istream& is, Fecha& f1)
{
    // Reservamos memoria suficiente para almacenar la fecha en este formato "DD/MM/AAAA".
    char f[11] {};

    // Almacenamos la fecha en la variable f creada anteriormente.
    is >> f;
    
    // Probamos a crear la fecha.
    try{
        // Creamos tres enteros para almacenar el día, el mes y el año.
        int d = 0, m = 0, a = 0;

        // Si cumple con el formato, creamos la fecha.
        if(sscanf(f, "%2d/%2d/%4d", &d, &m, &a) == 3)
        {
            Fecha fec(d, m, a);

            // Si estamos en esta línea, quiere decir que hemos creado la fecha correctamente. Por lo tanto, la copiamos en f1.
            f1 = fec;
        }
        else{

            if(!is.fail())
                is.setstate(std::ios::failbit);

            throw Fecha::Invalida("Formato incorrecto");
        }
    }
    catch(const Fecha::Invalida &e)
    {
        // Si hay un error en la entrada estándar, se pone el estado de flujo en fail.
        if(!is.fail())
            is.setstate(std::ios::failbit);

        throw Fecha::Invalida("Fecha inválida.");
    }


    // Devolvemos la entrada estándar.
    return is;
}

/*

    // Si hay un error en la entrada estándar, se pone el estado de flujo en fail.
        if(!is.fail())
            is.setstate(std::ios::failbit);
    

*/