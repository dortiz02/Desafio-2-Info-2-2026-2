#include "foto.h"
#include "cadena.h"

Foto::Foto() : fechaCaptura(), tamanoKB(0), ruta(""), likes(0) {
}

Foto::Foto(const Fecha& fechaCaptura, int tamanoKB, const std::string& ruta, int likes)
    : fechaCaptura(fechaCaptura), tamanoKB(tamanoKB), ruta(ruta), likes(likes) {
}

Foto::Foto(const Foto& otra)
    : fechaCaptura(otra.fechaCaptura), tamanoKB(otra.tamanoKB), ruta(otra.ruta), likes(otra.likes) {
}

Foto& Foto::operator=(const Foto& otra) {
    if (this != &otra) {
        fechaCaptura = otra.fechaCaptura;
        tamanoKB = otra.tamanoKB;
        ruta = otra.ruta;
        likes = otra.likes;
    }
    return *this;
}

const Fecha& Foto::getFechaCaptura() const { return fechaCaptura; }
int Foto::getTamanoKB() const { return tamanoKB; }
const std::string& Foto::getRuta() const { return ruta; }
int Foto::getLikes() const { return likes; }
void Foto::setFechaCaptura(const Fecha& f) { fechaCaptura = f; }
void Foto::setTamanoKB(int t) { tamanoKB = t; }
void Foto::setRuta(const std::string& r) { ruta = r; }
void Foto::setLikes(int l) { likes = l; }

void Foto::darLike() {
    likes++;
}

void Foto::quitarLike() {
    if (likes > 0) {
        likes--;
    }
}

bool Foto::operator==(const Foto& otra) const {
    return Cadena::iguales(ruta, otra.ruta);
}

std::ostream& operator<<(std::ostream& salida, const Foto& f) {
    salida << f.ruta << "\n   Capturada: " << f.fechaCaptura << " | Tamano: " << f.tamanoKB
           << " KB | Likes: " << f.likes;
    return salida;
}
