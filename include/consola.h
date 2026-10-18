#ifndef CONSOLA_H
#define CONSOLA_H

#include <string>
#include "redsocial.h"

class Consola {
private:
    RedSocial red;
    std::string rutaDatos;
    int usuarioActual;
    int mejoresAmigos[3];
    int cantidadMejores;
    bool yaActualizo;
    bool entradaCerrada;

    std::string leerLinea(const std::string& mensaje);
    int leerEntero(const std::string& mensaje, int minimo, int maximo);

    void mostrarMejoresAmigos();
    void mostrarResultadoSolicitud(int codigo);

    void iniciarSesion();
    void menuPrincipal();
    void menuAmistades();
    void enviarSolicitud();
    void aceptarSolicitudes();

public:
    Consola();
    Consola(const Consola& otra);
    Consola& operator=(const Consola& otra);

    void ejecutar(const std::string& ruta);
};

#endif
