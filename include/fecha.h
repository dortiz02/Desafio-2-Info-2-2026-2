#ifndef FECHA_H
#define FECHA_H

#include <iostream>
#include <string>

class Fecha {
private:
    int dia;
    int mes;
    int anio;
    int hora;
    int minuto;

public:
    Fecha();
    Fecha(int dia, int mes, int anio, int hora = 0, int minuto = 0);
    Fecha(const Fecha& otra);
    Fecha& operator=(const Fecha& otra);

    int getDia() const;
    int getMes() const;
    int getAnio() const;
    int getHora() const;
    int getMinuto() const;
    void setDia(int d);
    void setMes(int m);
    void setAnio(int a);
    void setHora(int h);
    void setMinuto(int m);

    static int diasDelMes(int mes, int anio);
    bool esValida() const;

    int claveDia() const;
    long long claveCompleta() const;

    static bool desdeTexto(const std::string& texto, Fecha& resultado);

    bool operator==(const Fecha& otra) const;
    bool operator<(const Fecha& otra) const;
    friend std::ostream& operator<<(std::ostream& salida, const Fecha& f);
};

#endif
