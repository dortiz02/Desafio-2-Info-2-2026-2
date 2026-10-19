#ifndef ALMACENAMIENTO_H
#define ALMACENAMIENTO_H

#include <fstream>
#include <string>
#include "redsocial.h"
#include "fecha.h"

"""
Carga y guarda toda la red social en un unico archivo binario propio.
El archivo solo se usa al inicio (cargar) y al final (guardar); mientras
el programa corre todo se maneja en memoria.
El formato esta documentado en data/formato.txt
"""
class Almacenamiento {
private:
    static void escribirEntero(std::ofstream& archivo, int valor);
    static void escribirTexto(std::ofstream& archivo, const std::string& texto);
    static void escribirFecha(std::ofstream& archivo, const Fecha& fecha);
    static bool leerEntero(std::ifstream& archivo, int& valor);
    static bool leerTexto(std::ifstream& archivo, std::string& texto);
    static bool leerFecha(std::ifstream& archivo, Fecha& fecha);

public:
    Almacenamiento();
    Almacenamiento(const Almacenamiento& otro);

    static bool cargar(RedSocial& red, const std::string& ruta);
    static bool guardar(const RedSocial& red, const std::string& ruta);
};

#endif
