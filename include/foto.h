#ifndef FOTO_H
#define FOTO_H

#include <iostream>
#include <string>
#include "fecha.h"

class Foto {
private:
    Fecha fechaCaptura;
    int tamanoKB;
    std::string ruta;
    int likes;

public:
    Foto();
    Foto(const Fecha& fechaCaptura, int tamanoKB, const std::string& ruta, int likes);
    Foto(const Foto& otra);
    Foto& operator=(const Foto& otra);

    const Fecha& getFechaCaptura() const;
    int getTamanoKB() const;
    const std::string& getRuta() const;
    int getLikes() const;
    void setFechaCaptura(const Fecha& f);
    void setTamanoKB(int t);
    void setRuta(const std::string& r);
    void setLikes(int l);

    void darLike();
    void quitarLike();

    // bytes dinamicos que usa la foto (el texto de la ruta)
    unsigned long long memoriaDinamica() const;

    bool operator==(const Foto& otra) const;
    friend std::ostream& operator<<(std::ostream& salida, const Foto& f);
};

#endif
