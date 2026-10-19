#ifndef INDICEUSUARIOS_H
#define INDICEUSUARIOS_H

#include <string>
#include "lista.h"
#include "usuario.h"

class IndiceUsuarios {
private:
    int* tabla;        
    int capacidad;
    int cantidad;

    void insertar(int id, const Lista<Usuario*>& usuarios);

public:
    IndiceUsuarios();
    IndiceUsuarios(const IndiceUsuarios& otro);
    ~IndiceUsuarios();
    IndiceUsuarios& operator=(const IndiceUsuarios& otro);

    void construir(const Lista<Usuario*>& usuarios);
    int buscar(const std::string& nombre, const Lista<Usuario*>& usuarios) const;

    unsigned long long memoriaDinamica() const;
};

#endif
