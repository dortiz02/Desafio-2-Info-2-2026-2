#include "cadena.h"
#include "medidor.h"

Cadena::Cadena() {
}

Cadena::Cadena(const Cadena&) {
}

bool Cadena::iguales(const std::string& a, const std::string& b) {
    int n = (int)a.size();
    if (n != (int)b.size()) {
        return false;
    }
    for (int i = 0; i < n; i++) {
        Medidor::contarIndirecta();
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}

bool Cadena::esVacio(const std::string& texto) {
    int n = (int)texto.size();
    for (int i = 0; i < n; i++) {
        Medidor::contarIndirecta();
        char c = texto[i];
        if (c != ' ' && c != '\t' && c != '\r' && c != '\n') {
            return false;
        }
    }
    return true;
}

unsigned long Cadena::hash(const std::string& texto) {
    unsigned long h = 5381;
    int n = (int)texto.size();
    for (int i = 0; i < n; i++) {
        Medidor::contarIndirecta();
        h = h * 33 + (unsigned char)texto[i];
    }
    return h;
}

int Cadena::dividirEnteros(const std::string& texto, char separador, int* valores, int maximo) {
    int cuantos = 0;
    int actual = 0;
    bool hayDigitos = false;
    int n = (int)texto.size();

    for (int i = 0; i < n; i++) {
        Medidor::contarIndirecta();
        char c = texto[i];
        if (c >= '0' && c <= '9') {
            actual = actual * 10 + (c - '0');
            hayDigitos = true;
            if (actual > 99999999) {
                return -1;
            }
        } else if (c == separador) {
            if (!hayDigitos || cuantos >= maximo) {
                return -1;
            }
            valores[cuantos] = actual;
            cuantos++;
            actual = 0;
            hayDigitos = false;
        } else if (c == ' ' || c == '\r') {
        } else {
            return -1;
        }
    }

    if (!hayDigitos || cuantos >= maximo) {
        return -1;
    }
    valores[cuantos] = actual;
    cuantos++;
    return cuantos;
}

bool Cadena::aEntero(const std::string& texto, int& valor) {
    int numeros[1];
    int cuantos = dividirEnteros(texto, '/', numeros, 1);
    if (cuantos != 1) {
        return false;
    }
    valor = numeros[0];
    return true;
}

unsigned long long Cadena::bytesDinamicos(const std::string& texto) {

    if (texto.size() <= 15) {
        return 0;
    }
    return (unsigned long long)texto.capacity() + 1;
}
