#ifndef CADENA_H
#define CADENA_H

#include <string>

// Utilidades de texto propias recorriendo caracter por caracter.
class Cadena {
public:
    Cadena();
    Cadena(const Cadena& otra);

    static bool iguales(const std::string& a, const std::string& b);
    static bool esVacio(const std::string& texto);
    static int dividirEnteros(const std::string& texto, char separador, int* valores, int maximo);
    static bool aEntero(const std::string& texto, int& valor);
};

#endif
