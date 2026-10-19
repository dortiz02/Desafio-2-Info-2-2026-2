#include "album.h"
#include "cadena.h"

Album::Album() : nombre(""), ruta(""), privacidad(Privacidad::Publico), fotos() {
}

Album::Album(const std::string& nombre, const std::string& ruta, Privacidad privacidad)
    : nombre(nombre), ruta(ruta), privacidad(privacidad), fotos() {
}

Album::Album(const Album& otro)
    : nombre(otro.nombre), ruta(otro.ruta), privacidad(otro.privacidad), fotos(otro.fotos) {
}

Album& Album::operator=(const Album& otro) {
    if (this != &otro) {
        nombre = otro.nombre;
        ruta = otro.ruta;
        privacidad = otro.privacidad;
        fotos = otro.fotos;
    }
    return *this;
}

const std::string& Album::getNombre() const { return nombre; }
const std::string& Album::getRuta() const { return ruta; }
Privacidad Album::getPrivacidad() const { return privacidad; }
void Album::setNombre(const std::string& n) { nombre = n; }
void Album::setRuta(const std::string& r) { ruta = r; }
void Album::setPrivacidad(Privacidad p) { privacidad = p; }

void Album::reservarFotos(int n) {
    fotos.reservar(n);
}

void Album::agregarFoto(const Foto& foto) {
    fotos.agregar(foto);
}

int Album::cantidadFotos() const {
    return fotos.tamano();
}

Foto& Album::getFoto(int pos) {
    return fotos[pos];
}

const Foto& Album::getFoto(int pos) const {
    return fotos[pos];
}

bool Album::esVisibleParaAmigos() const {
    return privacidad == Privacidad::Publico || privacidad == Privacidad::Amigos;
}

unsigned long long Album::memoriaDinamica() const {
    unsigned long long total = Cadena::bytesDinamicos(nombre) + Cadena::bytesDinamicos(ruta);
    total += fotos.memoriaDinamica();
    for (int i = 0; i < fotos.tamano(); i++) {
        total += fotos[i].memoriaDinamica();
    }
    return total;
}

unsigned long long Album::bytesTotales() const {
    return sizeof(Album) + memoriaDinamica();
}

std::ostream& operator<<(std::ostream& salida, const Album& a) {
    salida << a.nombre << " [";
    if (a.privacidad == Privacidad::Publico) {
        salida << "publico";
    } else if (a.privacidad == Privacidad::Amigos) {
        salida << "amigos";
    } else {
        salida << "privado";
    }
    salida << "] - " << a.fotos.tamano() << " fotos";
    return salida;
}
