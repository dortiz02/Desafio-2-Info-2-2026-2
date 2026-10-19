#include "fecha.h"
#include "cadena.h"

Fecha::Fecha() : dia(1), mes(1), anio(2026), hora(0), minuto(0) {
}

Fecha::Fecha(int dia, int mes, int anio, int hora, int minuto)
    : dia(dia), mes(mes), anio(anio), hora(hora), minuto(minuto) {
}

Fecha::Fecha(const Fecha& otra)
    : dia(otra.dia), mes(otra.mes), anio(otra.anio), hora(otra.hora), minuto(otra.minuto) {
}

Fecha& Fecha::operator=(const Fecha& otra) {
    dia = otra.dia;
    mes = otra.mes;
    anio = otra.anio;
    hora = otra.hora;
    minuto = otra.minuto;
    return *this;
}

int Fecha::getDia() const { return dia; }
int Fecha::getMes() const { return mes; }
int Fecha::getAnio() const { return anio; }
int Fecha::getHora() const { return hora; }
int Fecha::getMinuto() const { return minuto; }
void Fecha::setDia(int d) { dia = d; }
void Fecha::setMes(int m) { mes = m; }
void Fecha::setAnio(int a) { anio = a; }
void Fecha::setHora(int h) { hora = h; }
void Fecha::setMinuto(int m) { minuto = m; }

bool Fecha::esBisiesto(int anio) {
    if (anio % 400 == 0) {
        return true;
    }
    if (anio % 100 == 0) {
        return false;
    }
    return anio % 4 == 0;
}

int Fecha::diasDelMes(int mes, int anio) {
    if (mes == 2) {
        return esBisiesto(anio) ? 29 : 28;
    }
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        return 30;
    }
    return 31;
}

bool Fecha::esValida() const {
    if (anio < 1900 || anio > 2100) return false;
    if (mes < 1 || mes > 12) return false;
    if (dia < 1 || dia > diasDelMes(mes, anio)) return false;
    if (hora < 0 || hora > 23) return false;
    if (minuto < 0 || minuto > 59) return false;
    return true;
}

int Fecha::claveDia() const {
    return anio * 10000 + mes * 100 + dia;
}

long long Fecha::claveCompleta() const {
    return (long long)claveDia() * 10000 + hora * 100 + minuto;
}

void Fecha::avanzarDia() {
    dia++;
    if (dia > diasDelMes(mes, anio)) {
        dia = 1;
        mes++;
        if (mes > 12) {
            mes = 1;
            anio++;
        }
    }
}

void Fecha::sumarMinutos(int minutos) {
    minuto += minutos;
    hora += minuto / 60;
    minuto = minuto % 60;
    int dias = hora / 24;
    hora = hora % 24;
    for (int i = 0; i < dias; i++) {
        avanzarDia();
    }
}

bool Fecha::desdeTexto(const std::string& texto, Fecha& resultado) {
    int partes[3];
    int cuantos = Cadena::dividirEnteros(texto, '/', partes, 3);
    if (cuantos != 3) {
        return false;
    }
    Fecha candidata(partes[0], partes[1], partes[2]);
    if (!candidata.esValida()) {
        return false;
    }
    resultado = candidata;
    return true;
}

bool Fecha::operator==(const Fecha& otra) const {
    return claveCompleta() == otra.claveCompleta();
}

bool Fecha::operator!=(const Fecha& otra) const {
    return !(*this == otra);
}

bool Fecha::operator<(const Fecha& otra) const {
    return claveCompleta() < otra.claveCompleta();
}

bool Fecha::operator<=(const Fecha& otra) const {
    return claveCompleta() <= otra.claveCompleta();
}

bool Fecha::operator>(const Fecha& otra) const {
    return claveCompleta() > otra.claveCompleta();
}

bool Fecha::operator>=(const Fecha& otra) const {
    return claveCompleta() >= otra.claveCompleta();
}

std::ostream& operator<<(std::ostream& salida, const Fecha& f) {
    if (f.dia < 10) salida << '0';
    salida << f.dia << '/';
    if (f.mes < 10) salida << '0';
    salida << f.mes << '/' << f.anio << ' ';
    if (f.hora < 10) salida << '0';
    salida << f.hora << ':';
    if (f.minuto < 10) salida << '0';
    salida << f.minuto;
    return salida;
}
