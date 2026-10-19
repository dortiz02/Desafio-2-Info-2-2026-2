#include "medidor.h"
#include <iostream>

unsigned long long Medidor::directas = 0;
unsigned long long Medidor::indirectas = 0;
unsigned long long Medidor::bytesLocales = 0;
unsigned long long Medidor::llamadasExternas = 0;

Medidor::Medidor() {
}

Medidor::Medidor(const Medidor&) {
}

void Medidor::reiniciar() {
    directas = 0;
    indirectas = 0;
    bytesLocales = 0;
    llamadasExternas = 0;
}

void Medidor::contarDirecta() {
    directas++;
}

void Medidor::contarIndirecta() {
    indirectas++;
}

void Medidor::contarExterna() {
    llamadasExternas++;
}

void Medidor::sumarLocales(unsigned long long bytes) {
    bytesLocales += bytes;
}

unsigned long long Medidor::getDirectas() {
    return directas;
}

unsigned long long Medidor::getIndirectas() {
    return indirectas;
}

unsigned long long Medidor::getBytesLocales() {
    return bytesLocales;
}

void Medidor::mostrar(unsigned long long bytesEstructuras) {
    std::cout << "\n------ Consumo de recursos ------\n";
    std::cout << "Iteraciones directas   : " << directas << "\n";
    std::cout << "Iteraciones indirectas : " << indirectas << "\n";
    std::cout << "Iteraciones totales    : " << (directas + indirectas) << "\n";
    std::cout << "Componente externo     : <random> (" << llamadasExternas << " llamadas al generador)\n";
    std::cout << "Memoria estructuras    : " << bytesEstructuras << " bytes\n";
    std::cout << "Memoria variables local: " << bytesLocales << " bytes\n";
    std::cout << "Memoria total en uso   : " << (bytesEstructuras + bytesLocales) << " bytes\n";
    std::cout << "---------------------------------\n";
}
