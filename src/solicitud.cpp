#include "solicitud.h"

Solicitud::Solicitud() : idEmisor(-1), idReceptor(-1), activa(false) {
}

Solicitud::Solicitud(int idEmisor, int idReceptor)
    : idEmisor(idEmisor), idReceptor(idReceptor), activa(true) {
}

Solicitud::Solicitud(const Solicitud& otra)
    : idEmisor(otra.idEmisor), idReceptor(otra.idReceptor), activa(otra.activa) {
}

Solicitud& Solicitud::operator=(const Solicitud& otra) {
    if (this != &otra) {
        idEmisor = otra.idEmisor;
        idReceptor = otra.idReceptor;
        activa = otra.activa;
    }
    return *this;
}

int Solicitud::getIdEmisor() const { return idEmisor; }
int Solicitud::getIdReceptor() const { return idReceptor; }
bool Solicitud::estaActiva() const { return activa; }
void Solicitud::setActiva(bool a) { activa = a; }

bool Solicitud::operator==(const Solicitud& otra) const {
    return idEmisor == otra.idEmisor && idReceptor == otra.idReceptor;
}
