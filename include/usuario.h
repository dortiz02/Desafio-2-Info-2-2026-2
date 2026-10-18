#ifndef USUARIO_H
#define USUARIO_H

#include <iostream>
#include <string>
#include "fecha.h"
#include "lista.h"
#include "album.h"

class Usuario {
private:
    std::string nombre;
    Fecha fechaNacimiento;
    std::string correo;
    std::string rutaFotoPerfil;
    std::string clave;
    std::string ciudad;
    std::string pais;
    Fecha fechaInscripcion;

    Lista<int> amigos;
    Lista<int> solicitudesRecibidas;
    Lista<int> solicitudesEnviadas;
    Lista<int> mensajesRecibidos;
    Lista<int> mensajesEnviados;
    Lista<Album*> albumes;

    void liberarAlbumes();
    void copiarAlbumes(const Usuario& otro);

public:
    Usuario();
    Usuario(const std::string& nombre, const Fecha& fechaNacimiento, const std::string& correo,
            const std::string& rutaFotoPerfil, const std::string& clave, const std::string& ciudad,
            const std::string& pais, const Fecha& fechaInscripcion);
    Usuario(const Usuario& otro);
    ~Usuario();
    Usuario& operator=(const Usuario& otro);

    const std::string& getNombre() const;
    const Fecha& getFechaNacimiento() const;
    const std::string& getCorreo() const;
    const std::string& getRutaFotoPerfil() const;
    const std::string& getClave() const;
    const std::string& getCiudad() const;
    const std::string& getPais() const;
    const Fecha& getFechaInscripcion() const;

    void setCorreo(const std::string& c);
    void setRutaFotoPerfil(const std::string& r);
    void setClave(const std::string& c);
    void setCiudad(const std::string& c);
    void setPais(const std::string& p);

    const Lista<int>& getAmigos() const;
    void agregarAmigo(int idAmigo);
    bool esAmigo(int idUsuario) const;

    const Lista<int>& getSolicitudesRecibidas() const;
    const Lista<int>& getSolicitudesEnviadas() const;
    void agregarSolicitudRecibida(int idEmisor);
    void agregarSolicitudEnviada(int idReceptor);
    void quitarSolicitudRecibida(int posicion);
    void quitarSolicitudEnviada(int idReceptor);
    bool tieneSolicitudRecibidaDe(int idUsuario) const;
    bool tieneSolicitudEnviadaA(int idUsuario) const;

    const Lista<int>& getMensajesRecibidos() const;
    const Lista<int>& getMensajesEnviados() const;
    void agregarMensajeRecibido(int idMensaje);
    void agregarMensajeEnviado(int idMensaje);

    void agregarAlbum(Album* album);
    int cantidadAlbumes() const;
    Album* getAlbum(int pos) const;

    bool operator==(const Usuario& otro) const;
    friend std::ostream& operator<<(std::ostream& salida, const Usuario& u);
};

#endif
