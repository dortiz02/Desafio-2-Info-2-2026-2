#ifndef MEDIDOR_H
#define MEDIDOR_H

// Primera version del medidor de recursos: cuenta iteraciones directas e indirectas.
class Medidor {
private:
    static unsigned long long directas;
    static unsigned long long indirectas;

public:
    Medidor();
    Medidor(const Medidor& otro);

    static void reiniciar();
    static void contarDirecta();
    static void contarIndirecta();

    static unsigned long long getDirectas();
    static unsigned long long getIndirectas();
    static void mostrar(unsigned long long bytesEstructuras);
};

#endif
