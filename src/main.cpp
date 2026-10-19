#include <fstream>
#include <string>
#include "consola.h"

std::string buscarArchivoDatos() {
    const int CANTIDAD = 5;
    const std::string candidatas[CANTIDAD] = {
        "data/ude_datos.dat",
        "../data/ude_datos.dat",
        "../../data/ude_datos.dat",
        "../../../data/ude_datos.dat",
        "../../../../data/ude_datos.dat"};

    for (int i = 0; i < CANTIDAD; i++) {
        std::ifstream prueba(candidatas[i], std::ios::binary);
        if (prueba.is_open()) {
            return candidatas[i];
        }
    }
    return candidatas[0];
}

int main() {
    Consola consola;
    consola.ejecutar(buscarArchivoDatos());
    return 0;
}
