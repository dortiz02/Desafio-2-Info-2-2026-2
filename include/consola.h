#ifndef CONSOLA_H
#define CONSOLA_H

#include <string>
#include "redsocial.h"
#include "album.h"
#include "mensaje.h"
#include "fecha.h"

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
    bool leerFecha(const std::string& mensaje, Fecha& fecha);

    void mostrarMetricas() const;
    void mostrarMejoresAmigos();
    void alternarMejoresAmigos();
    void mostrarMensaje(const Mensaje& m);
    void mostrarResultadoSolicitud(int codigo);

    void iniciarSesion();
    void menuPrincipal();
    void menuAmistades();
    void enviarSolicitud();
    void aceptarSolicitudes();
    void visitarPerfil();
    void recorrerAlbum(Album& album);
    void verSugerencias();
    void menuMensajes();
    void enviarMensaje();
    void leerMensajesNuevos();
    void buscarMensajesPorFecha();

public:
    Consola();
    Consola(const Consola& otra);
    Consola& operator=(const Consola& otra);

    void ejecutar(const std::string& ruta);
};

#endif
