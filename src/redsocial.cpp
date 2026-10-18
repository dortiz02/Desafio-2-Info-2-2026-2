#include "redsocial.h"
#include "cadena.h"

RedSocial::RedSocial()
    : usuarios(), mensajes(), fechaActual(18, 10, 2026), rutaBase("") {
}

RedSocial::RedSocial(const RedSocial& otra)
    : usuarios(), mensajes(), fechaActual(otra.fechaActual), rutaBase(otra.rutaBase) {
    copiarDe(otra);
}

RedSocial::~RedSocial() {
    liberar();
}

RedSocial& RedSocial::operator=(const RedSocial& otra) {
    if (this != &otra) {
        liberar();
        fechaActual = otra.fechaActual;
        rutaBase = otra.rutaBase;
        copiarDe(otra);
    }
    return *this;
}

void RedSocial::liberar() {
    for (int i = 0; i < usuarios.tamano(); i++) {
        delete usuarios[i];
    }
    for (int i = 0; i < mensajes.tamano(); i++) {
        delete mensajes[i];
    }
    usuarios = Lista<Usuario*>();
    mensajes = Lista<Mensaje*>();
}

void RedSocial::copiarDe(const RedSocial& otra) {
    usuarios.reservar(otra.usuarios.tamano());
    for (int i = 0; i < otra.usuarios.tamano(); i++) {
        usuarios.agregar(new Usuario(*otra.usuarios[i]));
    }
    mensajes.reservar(otra.mensajes.tamano());
    for (int i = 0; i < otra.mensajes.tamano(); i++) {
        mensajes.agregar(new Mensaje(*otra.mensajes[i]));
    }
}

int RedSocial::agregarUsuario(Usuario* usuario) {
    usuarios.agregar(usuario);
    return usuarios.tamano() - 1;
}

bool RedSocial::crearAmistad(int a, int b) {
    if (a == b || usuarios[a]->esAmigo(b)) {
        return false;
    }
    usuarios[a]->agregarAmigo(b);
    usuarios[b]->agregarAmigo(a);
    return true;
}

bool RedSocial::registrarSolicitud(int de, int para) {
    if (de == para || usuarios[de]->esAmigo(para)) {
        return false;
    }
    if (usuarios[de]->tieneSolicitudEnviadaA(para) || usuarios[para]->tieneSolicitudEnviadaA(de)) {
        return false;
    }
    usuarios[de]->agregarSolicitudEnviada(para);
    usuarios[para]->agregarSolicitudRecibida(de);
    return true;
}

int RedSocial::agregarMensaje(Mensaje* mensaje) {
    mensajes.agregar(mensaje);
    int id = mensajes.tamano() - 1;
    usuarios[mensaje->getIdEmisor()]->agregarMensajeEnviado(id);
    usuarios[mensaje->getIdReceptor()]->agregarMensajeRecibido(id);
    return id;
}

int RedSocial::cantidadUsuarios() const { return usuarios.tamano(); }
int RedSocial::cantidadMensajes() const { return mensajes.tamano(); }
Usuario& RedSocial::getUsuario(int id) { return *usuarios[id]; }
const Usuario& RedSocial::getUsuario(int id) const { return *usuarios[id]; }
Mensaje& RedSocial::getMensaje(int id) { return *mensajes[id]; }
const Mensaje& RedSocial::getMensaje(int id) const { return *mensajes[id]; }
const Fecha& RedSocial::getFechaActual() const { return fechaActual; }
void RedSocial::setFechaActual(const Fecha& f) { fechaActual = f; }
const std::string& RedSocial::getRutaBase() const { return rutaBase; }
void RedSocial::setRutaBase(const std::string& r) { rutaBase = r; }

// Version inicial: busqueda secuencial sobre todos los usuarios
int RedSocial::buscarUsuario(const std::string& nombre) const {
    for (int i = 0; i < usuarios.tamano(); i++) {
        if (Cadena::iguales(usuarios[i]->getNombre(), nombre)) {
            return i;
        }
    }
    return -1;
}

int RedSocial::autenticar(const std::string& nombre, const std::string& clave) const {
    int id = buscarUsuario(nombre);
    if (id == -1) {
        return -1;
    }
    if (!Cadena::iguales(usuarios[id]->getClave(), clave)) {
        return -1;
    }
    return id;
}

bool RedSocial::sonAmigos(int a, int b) const {
    return usuarios[a]->esAmigo(b);
}

// Version inicial: doble ciclo para contar amigos en comun
int RedSocial::amigosEnComun(int a, int b) const {
    const Lista<int>& la = usuarios[a]->getAmigos();
    const Lista<int>& lb = usuarios[b]->getAmigos();
    int comunes = 0;
    for (int i = 0; i < la.tamano(); i++) {
        for (int j = 0; j < lb.tamano(); j++) {
            if (la[i] == lb[j]) {
                comunes++;
            }
        }
    }
    return comunes;
}
