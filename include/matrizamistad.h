#ifndef MATRIZAMISTAD_H
#define MATRIZAMISTAD_H

class MatrizAmistad {
private:
    bool* celdas;
    int n;

public:
    MatrizAmistad();
    explicit MatrizAmistad(int cantidadUsuarios);
    MatrizAmistad(const MatrizAmistad& otra);
    ~MatrizAmistad();
    MatrizAmistad& operator=(const MatrizAmistad& otra);

    void redimensionar(int cantidadUsuarios);
    void conectar(int a, int b);
    bool sonAmigos(int a, int b) const;
    int amigosEnComun(int a, int b) const;
};

#endif
