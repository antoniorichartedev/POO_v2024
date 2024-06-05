#include <iostream>
#include <set>
#include <map>
class Usuario;
class Pedido;

/* Clase de asociación Usuario_Pedido */
class Usuario_Pedido
{
    public:

        // Miembro público de la asociación. Sentido Usuario --> Pedido
        typedef std::set<Pedido*> Pedidos;

        // Método con el que realizamos la asociación.
        void asocia(Usuario& us, Pedido& ped);
        void asocia(Pedido& ped, Usuario& us);

        // Métodos observadores.
        Pedidos pedidos(Usuario& us) const;
        Usuario* cliente(Pedido& ped) const;

        
    private:
        std::map<Usuario*, Pedidos> usu_ped;

        // En el sentido contrario.
        std::map<Pedido*, Usuario*> ped_usu; 
};