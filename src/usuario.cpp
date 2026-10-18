#include "usuario.h"
#include "cadena.h"

Usuario::Usuario()
    : nombre(""), fechaNacimiento(), correo(""), rutaFotoPerfil(""), clave(""), ciudad(""),
      pais(""), fechaInscripcion(), amigos(), solicitudesRecibidas(), solicitudesEnviadas(),
      mensajesRecibidos(), mensajesEnviados(), albumes() {
}

Usuario::Usuario(const std::string& nombre, const Fecha& fechaNacimiento, const std::string& correo,
                 const std::string& rutaFotoPerfil, const std::string& clave, const std::string& ciudad,
                 const std::string& pais, const Fecha& fechaInscripcion)
    : nombre(nombre), fechaNacimiento(fechaNacimiento), correo(correo),
      rutaFotoPerfil(rutaFotoPerfil), clave(clave), ciudad(ciudad), pais(pais),
      fechaInscripcion(fechaInscripcion), amigos(), solicitudesRecibidas(), solicitudesEnviadas(),
      mensajesRecibidos(), mensajesEnviados(), albumes() {
}

Usuario::Usuario(const Usuario& otro)
    : nombre(otro.nombre), fechaNacimiento(otro.fechaNacimiento), correo(otro.correo),
      rutaFotoPerfil(otro.rutaFotoPerfil), clave(otro.clave), ciudad(otro.ciudad),
      pais(otro.pais), fechaInscripcion(otro.fechaInscripcion), amigos(otro.amigos),
      solicitudesRecibidas(otro.solicitudesRecibidas), solicitudesEnviadas(otro.solicitudesEnviadas),
      mensajesRecibidos(otro.mensajesRecibidos), mensajesEnviados(otro.mensajesEnviados), albumes() {
    copiarAlbumes(otro);
}

Usuario::~Usuario() {
    liberarAlbumes();
}

Usuario& Usuario::operator=(const Usuario& otro) {
    if (this != &otro) {
        liberarAlbumes();
        nombre = otro.nombre;
        fechaNacimiento = otro.fechaNacimiento;
        correo = otro.correo;
        rutaFotoPerfil = otro.rutaFotoPerfil;
        clave = otro.clave;
        ciudad = otro.ciudad;
        pais = otro.pais;
        fechaInscripcion = otro.fechaInscripcion;
        amigos = otro.amigos;
        solicitudesRecibidas = otro.solicitudesRecibidas;
        solicitudesEnviadas = otro.solicitudesEnviadas;
        mensajesRecibidos = otro.mensajesRecibidos;
        mensajesEnviados = otro.mensajesEnviados;
        copiarAlbumes(otro);
    }
    return *this;
}

void Usuario::liberarAlbumes() {
    for (int i = 0; i < albumes.tamano(); i++) {
        delete albumes[i];
    }
    albumes = Lista<Album*>();
}

void Usuario::copiarAlbumes(const Usuario& otro) {
    albumes.reservar(otro.albumes.tamano());
    for (int i = 0; i < otro.albumes.tamano(); i++) {
        albumes.agregar(new Album(*otro.albumes[i]));
    }
}

const std::string& Usuario::getNombre() const { return nombre; }
const Fecha& Usuario::getFechaNacimiento() const { return fechaNacimiento; }
const std::string& Usuario::getCorreo() const { return correo; }
const std::string& Usuario::getRutaFotoPerfil() const { return rutaFotoPerfil; }
const std::string& Usuario::getClave() const { return clave; }
const std::string& Usuario::getCiudad() const { return ciudad; }
const std::string& Usuario::getPais() const { return pais; }
const Fecha& Usuario::getFechaInscripcion() const { return fechaInscripcion; }

void Usuario::setCorreo(const std::string& c) { correo = c; }
void Usuario::setRutaFotoPerfil(const std::string& r) { rutaFotoPerfil = r; }
void Usuario::setClave(const std::string& c) { clave = c; }
void Usuario::setCiudad(const std::string& c) { ciudad = c; }
void Usuario::setPais(const std::string& p) { pais = p; }

const Lista<int>& Usuario::getAmigos() const {
    return amigos;
}

// Version inicial: agrega al final y busca linealmente
void Usuario::agregarAmigo(int idAmigo) {
    amigos.agregar(idAmigo);
}

bool Usuario::esAmigo(int idUsuario) const {
    return amigos.buscar(idUsuario) != -1;
}

const Lista<int>& Usuario::getSolicitudesRecibidas() const {
    return solicitudesRecibidas;
}

const Lista<int>& Usuario::getSolicitudesEnviadas() const {
    return solicitudesEnviadas;
}

void Usuario::agregarSolicitudRecibida(int idEmisor) {
    solicitudesRecibidas.agregar(idEmisor);
}

void Usuario::agregarSolicitudEnviada(int idReceptor) {
    solicitudesEnviadas.agregar(idReceptor);
}

void Usuario::quitarSolicitudRecibida(int posicion) {
    solicitudesRecibidas.eliminar(posicion);
}

void Usuario::quitarSolicitudEnviada(int idReceptor) {
    int pos = solicitudesEnviadas.buscar(idReceptor);
    if (pos != -1) {
        solicitudesEnviadas.eliminar(pos);
    }
}

bool Usuario::tieneSolicitudRecibidaDe(int idUsuario) const {
    return solicitudesRecibidas.buscar(idUsuario) != -1;
}

bool Usuario::tieneSolicitudEnviadaA(int idUsuario) const {
    return solicitudesEnviadas.buscar(idUsuario) != -1;
}

const Lista<int>& Usuario::getMensajesRecibidos() const {
    return mensajesRecibidos;
}

const Lista<int>& Usuario::getMensajesEnviados() const {
    return mensajesEnviados;
}

void Usuario::agregarMensajeRecibido(int idMensaje) {
    mensajesRecibidos.agregar(idMensaje);
}

void Usuario::agregarMensajeEnviado(int idMensaje) {
    mensajesEnviados.agregar(idMensaje);
}

void Usuario::agregarAlbum(Album* album) {
    albumes.agregar(album);
}

int Usuario::cantidadAlbumes() const {
    return albumes.tamano();
}

Album* Usuario::getAlbum(int pos) const {
    return albumes[pos];
}

bool Usuario::operator==(const Usuario& otro) const {
    return Cadena::iguales(nombre, otro.nombre);
}

std::ostream& operator<<(std::ostream& salida, const Usuario& u) {
    salida << u.nombre << " (" << u.ciudad << ", " << u.pais << ")";
    return salida;
}
