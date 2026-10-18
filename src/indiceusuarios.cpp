#include "indiceusuarios.h"
#include "cadena.h"

IndiceUsuarios::IndiceUsuarios() : tabla(nullptr), capacidad(0), cantidad(0) {
}

IndiceUsuarios::IndiceUsuarios(const IndiceUsuarios& otro)
    : tabla(nullptr), capacidad(otro.capacidad), cantidad(otro.cantidad) {
    if (capacidad > 0) {
        tabla = new int[capacidad];
        for (int i = 0; i < capacidad; i++) {
            tabla[i] = otro.tabla[i];
        }
    }
}

IndiceUsuarios::~IndiceUsuarios() {
    delete[] tabla;
}

IndiceUsuarios& IndiceUsuarios::operator=(const IndiceUsuarios& otro) {
    if (this != &otro) {
        delete[] tabla;
        tabla = nullptr;
        capacidad = otro.capacidad;
        cantidad = otro.cantidad;
        if (capacidad > 0) {
            tabla = new int[capacidad];
            for (int i = 0; i < capacidad; i++) {
                tabla[i] = otro.tabla[i];
            }
        }
    }
    return *this;
}

void IndiceUsuarios::insertar(int id, const Lista<Usuario*>& usuarios) {
    int pos = (int)(Cadena::hash(usuarios[id]->getNombre()) % (unsigned long)capacidad);
    while (tabla[pos] != -1) {
        pos = (pos + 1) % capacidad;
    }
    tabla[pos] = id;
    cantidad++;
}

void IndiceUsuarios::construir(const Lista<Usuario*>& usuarios) {
    delete[] tabla;
    capacidad = usuarios.tamano() * 2 + 1;
    tabla = new int[capacidad];
    cantidad = 0;
    for (int i = 0; i < capacidad; i++) {
        tabla[i] = -1;
    }
    for (int i = 0; i < usuarios.tamano(); i++) {
        insertar(i, usuarios);
    }
}

int IndiceUsuarios::buscar(const std::string& nombre, const Lista<Usuario*>& usuarios) const {
    if (capacidad == 0) {
        return -1;
    }
    int pos = (int)(Cadena::hash(nombre) % (unsigned long)capacidad);
    while (tabla[pos] != -1) {
        if (Cadena::iguales(usuarios[tabla[pos]]->getNombre(), nombre)) {
            return tabla[pos];
        }
        pos = (pos + 1) % capacidad;
    }
    return -1;
}
