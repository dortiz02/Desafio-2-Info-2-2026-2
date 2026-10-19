#ifndef CADENA_H
#define CADENA_H

#include <string>

class Cadena {
public:
    Cadena();
    Cadena(const Cadena& otra);

    // Comparacion caracter por caracter
    static bool iguales(const std::string& a, const std::string& b);

    // true si el texto esta vacio o solo tiene espacios/tabs
    static bool esVacio(const std::string& texto);

    // Hash (tipo djb2) para la tabla de usuarios
    static unsigned long hash(const std::string& texto);

    // "Split" numerico: separa un texto como 12/05/2026 en enteros.
    // Devuelve cuantos numeros leyo o -1 si el formato es malo.
    static int dividirEnteros(const std::string& texto, char separador, int* valores, int maximo);

    // Convierte un texto de solo digitos a entero
    static bool aEntero(const std::string& texto, int& valor);

    // Bytes que ocupa en el heap el contenido del string (sin el objeto en si)
    static unsigned long long bytesDinamicos(const std::string& texto);
};

#endif
