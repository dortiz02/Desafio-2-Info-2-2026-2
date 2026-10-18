#ifndef REDSOCIAL_H
#define REDSOCIAL_H

#include <string>
#include "lista.h"
#include "usuario.h"
#include "mensaje.h"
#include "fecha.h"

class RedSocial {
private:
    Lista<Usuario*> usuarios;
    Lista<Mensaje*> mensajes;
    Fecha fechaActual;
    std::string rutaBase;

    void liberar();
    void copiarDe(const RedSocial& otra);

public:
    RedSocial();
    RedSocial(const RedSocial& otra);
    ~RedSocial();
    RedSocial& operator=(const RedSocial& otra);

    int agregarUsuario(Usuario* usuario);
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
};

#endif
