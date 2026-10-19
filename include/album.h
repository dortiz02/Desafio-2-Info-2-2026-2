#ifndef ALBUM_H
#define ALBUM_H

#include <iostream>
#include <string>
#include "lista.h"
#include "foto.h"

enum class Privacidad {
    Publico = 0,
    Amigos = 1,
    Privado = 2
};

class Album {
private:
    std::string nombre;
    std::string ruta;
    Privacidad privacidad;
    Lista<Foto> fotos;

public:
    Album();
    Album(const std::string& nombre, const std::string& ruta, Privacidad privacidad);
    Album(const Album& otro);
    Album& operator=(const Album& otro);

    const std::string& getNombre() const;
    const std::string& getRuta() const;
    Privacidad getPrivacidad() const;
    void setNombre(const std::string& n);
    void setRuta(const std::string& r);
    void setPrivacidad(Privacidad p);

    void reservarFotos(int n);
    void agregarFoto(const Foto& foto);
    int cantidadFotos() const;
    Foto& getFoto(int pos);
    const Foto& getFoto(int pos) const;

    bool esVisibleParaAmigos() const;

    unsigned long long memoriaDinamica() const;
    unsigned long long bytesTotales() const;

    friend std::ostream& operator<<(std::ostream& salida, const Album& a);
};

#endif
