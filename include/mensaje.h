#ifndef MENSAJE_H
#define MENSAJE_H

#include <iostream>
#include <string>
#include "fecha.h"

class Mensaje {
private:
    int idEmisor;
    int idReceptor;
    Fecha fecha;
    bool leido;
    std::string texto;

public:
    static const int LIMITE_TEXTO = 512;

    Mensaje();
    Mensaje(int idEmisor, int idReceptor, const Fecha& fecha, bool leido, const std::string& texto);
    Mensaje(const Mensaje& otro);
    Mensaje& operator=(const Mensaje& otro);

    int getIdEmisor() const;
    int getIdReceptor() const;
    const Fecha& getFecha() const;
    bool estaLeido() const;
    const std::string& getTexto() const;
    void setLeido(bool l);
    void setTexto(const std::string& t);

    void marcarLeido();

    unsigned long long memoriaDinamica() const;
    unsigned long long bytesTotales() const;

    // Compara por fecha de envio
    bool operator<(const Mensaje& otro) const;
    friend std::ostream& operator<<(std::ostream& salida, const Mensaje& m);
};

#endif
