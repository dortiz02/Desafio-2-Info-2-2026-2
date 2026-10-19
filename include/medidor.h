#ifndef MEDIDOR_H
#define MEDIDOR_H

class Medidor {
private:
    static unsigned long long directas;
    static unsigned long long indirectas;
    static unsigned long long bytesLocales;
    static unsigned long long llamadasExternas;

public:
    Medidor();
    Medidor(const Medidor& otro);

    static void reiniciar();
    static void contarDirecta();
    static void contarIndirecta();
    static void contarExterna();
    static void sumarLocales(unsigned long long bytes);

    static unsigned long long getDirectas();
    static unsigned long long getIndirectas();
    static unsigned long long getBytesLocales();

    static void mostrar(unsigned long long bytesEstructuras);
};

#endif
