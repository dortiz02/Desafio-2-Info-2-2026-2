#ifndef REDSOCIAL_H
#define REDSOCIAL_H

#include <string>
#include <random>
#include "lista.h"
#include "usuario.h"
#include "mensaje.h"
#include "fecha.h"
#include "indiceusuarios.h"

class RedSocial {
private:
    Lista<Usuario*> usuarios;
    Lista<Mensaje*> mensajes;
    IndiceUsuarios indice;
    Fecha fechaActual;
    std::string rutaBase;
    std::mt19937 generador;

    void liberar();
    void copiarDe(const RedSocial& otra);

public:
    static const int SOL_OK = 0;
    static const int SOL_NO_EXISTE = 1;
    static const int SOL_MISMO_USUARIO = 2;
    static const int SOL_YA_AMIGOS = 3;
    static const int SOL_YA_ENVIADA = 4;
    static const int SOL_YA_RECIBIDA = 5;

    RedSocial();
    RedSocial(const RedSocial& otra);
    ~RedSocial();
    RedSocial& operator=(const RedSocial& otra);

    int agregarUsuario(Usuario* usuario);
    void construirIndice();
    bool crearAmistad(int a, int b);
    bool registrarSolicitud(int de, int para);
    int agregarMensaje(Mensaje* mensaje);

    int cantidadUsuarios() const;
    int cantidadMensajes() const;
    Usuario& getUsuario(int id);
    const Usuario& getUsuario(int id) const;
    Mensaje& getMensaje(int id);
    const Mensaje& getMensaje(int id) const;
    const Fecha& getFechaActual() const;
    void setFechaActual(const Fecha& f);
    const std::string& getRutaBase() const;
    void setRutaBase(const std::string& r);

    int buscarUsuario(const std::string& nombre) const;
    int autenticar(const std::string& nombre, const std::string& clave) const;
    bool sonAmigos(int a, int b) const;
    int amigosEnComun(int a, int b) const;

    int elegirMejoresAmigos(int idUsuario, int* resultado, const int* excluidos, int cantExcluidos);
    int enviarSolicitud(int idUsuario, const std::string& nombreDestino);
    bool aceptarSolicitud(int idUsuario, int posicion);
};

#endif
