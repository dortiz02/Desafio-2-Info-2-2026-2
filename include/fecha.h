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

    void avanzarDia();

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

    static bool esBisiesto(int anio);
    static int diasDelMes(int mes, int anio);
    bool esValida() const;

    // Clave numerica que permite comparar solo por dia (aaaammdd)
    int claveDia() const;
    // Clave numerica con hora y minuto (aaaammddhhmm)
    long long claveCompleta() const;

    void sumarMinutos(int minutos);

    // Lee un texto con formato dd/mm/aaaa. Devuelve false si no es valido.
    static bool desdeTexto(const std::string& texto, Fecha& resultado);

    bool operator==(const Fecha& otra) const;
    bool operator!=(const Fecha& otra) const;
    bool operator<(const Fecha& otra) const;
    bool operator<=(const Fecha& otra) const;
    bool operator>(const Fecha& otra) const;
    bool operator>=(const Fecha& otra) const;

    friend std::ostream& operator<<(std::ostream& salida, const Fecha& f);
};

#endif
