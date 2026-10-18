#include <iostream>
#include "lista.h"
#include "fecha.h"
#include "usuario.h"

int main() {
    Lista<int> numeros;
    numeros.agregar(10);
    numeros.agregar(20);
    Fecha hoy(5, 10, 2026, 14, 30);
    Usuario u("ana0", Fecha(12, 5, 2004), "ana0@udea.edu.co",
              "/users/storage2026/ana0/albums/profile.png", "clave0",
              "Medellin", "Colombia", hoy);
    std::cout << "Prueba inicial Ude@Book: " << u << " | items=" << numeros.tamano() << "\n";
    return 0;
}
