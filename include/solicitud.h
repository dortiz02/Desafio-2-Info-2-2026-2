#ifndef SOLICITUD_H
#define SOLICITUD_H

class Solicitud {
private:
    int idEmisor;
    int idReceptor;
    bool activa;

public:
    Solicitud();
    Solicitud(int idEmisor, int idReceptor);
    Solicitud(const Solicitud& otra);
    Solicitud& operator=(const Solicitud& otra);

    int getIdEmisor() const;
    int getIdReceptor() const;
    bool estaActiva() const;
    void setActiva(bool a);

    bool operator==(const Solicitud& otra) const;
};

#endif
