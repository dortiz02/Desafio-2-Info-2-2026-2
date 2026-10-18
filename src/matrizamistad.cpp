#include "matrizamistad.h"

MatrizAmistad::MatrizAmistad() : celdas(nullptr), n(0) {
}

MatrizAmistad::MatrizAmistad(int cantidadUsuarios) : celdas(nullptr), n(0) {
    redimensionar(cantidadUsuarios);
}

MatrizAmistad::MatrizAmistad(const MatrizAmistad& otra) : celdas(nullptr), n(otra.n) {
    if (n > 0) {
        int total = n * n;
        celdas = new bool[total];
        for (int i = 0; i < total; i++) {
            celdas[i] = otra.celdas[i];
        }
    }
}

MatrizAmistad::~MatrizAmistad() {
    delete[] celdas;
}

MatrizAmistad& MatrizAmistad::operator=(const MatrizAmistad& otra) {
    if (this != &otra) {
        delete[] celdas;
        celdas = nullptr;
        n = otra.n;
        if (n > 0) {
            int total = n * n;
            celdas = new bool[total];
            for (int i = 0; i < total; i++) {
                celdas[i] = otra.celdas[i];
            }
        }
    }
    return *this;
}

void MatrizAmistad::redimensionar(int cantidadUsuarios) {
    delete[] celdas;
    n = cantidadUsuarios;
    if (n <= 0) {
        celdas = nullptr;
        n = 0;
        return;
    }
    int total = n * n;
    celdas = new bool[total];
    for (int i = 0; i < total; i++) {
        celdas[i] = false;
    }
}

void MatrizAmistad::conectar(int a, int b) {
    if (a < 0 || b < 0 || a >= n || b >= n || a == b) {
        return;
    }
    celdas[a * n + b] = true;
    celdas[b * n + a] = true;
}

bool MatrizAmistad::sonAmigos(int a, int b) const {
    if (a < 0 || b < 0 || a >= n || b >= n) {
        return false;
    }
    return celdas[a * n + b];
}

int MatrizAmistad::amigosEnComun(int a, int b) const {
    if (a < 0 || b < 0 || a >= n || b >= n) {
        return 0;
    }
    int comunes = 0;
    for (int k = 0; k < n; k++) {
        if (celdas[a * n + k] && celdas[b * n + k]) {
            comunes++;
        }
    }
    return comunes;
}
