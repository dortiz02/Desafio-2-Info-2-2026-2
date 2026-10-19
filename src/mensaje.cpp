#include "mensaje.h"
#include "cadena.h"

Mensaje::Mensaje() : idEmisor(-1), idReceptor(-1), fecha(), leido(false), texto("") {
}

Mensaje::Mensaje(int idEmisor, int idReceptor, const Fecha& fecha, bool leido, const std::string& texto)
    : idEmisor(idEmisor), idReceptor(idReceptor), fecha(fecha), leido(leido), texto(texto) {
}

Mensaje::Mensaje(const Mensaje& otro)
    : idEmisor(otro.idEmisor), idReceptor(otro.idReceptor), fecha(otro.fecha),
      leido(otro.leido), texto(otro.texto) {
}

Mensaje& Mensaje::operator=(const Mensaje& otro) {
    if (this != &otro) {
        idEmisor = otro.idEmisor;
        idReceptor = otro.idReceptor;
        fecha = otro.fecha;
        leido = otro.leido;
        texto = otro.texto;
    }
    return *this;
}

int Mensaje::getIdEmisor() const { return idEmisor; }
int Mensaje::getIdReceptor() const { return idReceptor; }
const Fecha& Mensaje::getFecha() const { return fecha; }
bool Mensaje::estaLeido() const { return leido; }
const std::string& Mensaje::getTexto() const { return texto; }
void Mensaje::setLeido(bool l) { leido = l; }
void Mensaje::setTexto(const std::string& t) { texto = t; }

void Mensaje::marcarLeido() {
    leido = true;
}

unsigned long long Mensaje::memoriaDinamica() const {
    return Cadena::bytesDinamicos(texto);
}

unsigned long long Mensaje::bytesTotales() const {
    return sizeof(Mensaje) + memoriaDinamica();
}

bool Mensaje::operator<(const Mensaje& otro) const {
    return fecha < otro.fecha;
}

std::ostream& operator<<(std::ostream& salida, const Mensaje& m) {
    salida << "[" << m.fecha << "] " << m.texto;
    return salida;
}
