#ifndef LISTA_H
#define LISTA_H

#include "medidor.h"

template <class T>
class Lista {
private:
    T* datos;
    int capacidad;
    int cantidad;

    void crecer(int minimo) {
        // Duplicamos la capacidad actual para redimensionar el arreglo dinamico
        int nuevaCapacidad = (capacidad == 0) ? 4 : capacidad * 2;
        if (nuevaCapacidad < minimo) {
            nuevaCapacidad = minimo;
        }
        T* nuevo = new T[nuevaCapacidad];
        for (int i = 0; i < cantidad; i++) {
            Medidor::contarIndirecta();
            nuevo[i] = datos[i];
        }
        delete[] datos;
        datos = nuevo;
        capacidad = nuevaCapacidad;
    }

public:
    Lista() : datos(nullptr), capacidad(0), cantidad(0) {
    }

    Lista(const Lista& otra) : datos(nullptr), capacidad(otra.capacidad), cantidad(otra.cantidad) {
        if (capacidad > 0) {
            datos = new T[capacidad];
            for (int i = 0; i < cantidad; i++) {
                Medidor::contarIndirecta();
                datos[i] = otra.datos[i];
            }
        }
    }

    ~Lista() {
        delete[] datos;
    }

    Lista& operator=(const Lista& otra) {
        if (this != &otra) {
            delete[] datos;
            capacidad = otra.capacidad;
            cantidad = otra.cantidad;
            datos = nullptr;
            if (capacidad > 0) {
                datos = new T[capacidad];
                for (int i = 0; i < cantidad; i++) {
                    Medidor::contarIndirecta();
                    datos[i] = otra.datos[i];
                }
            }
        }
        return *this;
    }

    void reservar(int n) {
        if (n > capacidad) {
            crecer(n);
        }
    }

    void agregar(const T& valor) {
        if (cantidad == capacidad) {
            crecer(cantidad + 1);
        }
        datos[cantidad] = valor;
        cantidad++;
    }

    void insertarOrdenado(const T& valor) {
        if (cantidad == capacidad) {
            crecer(cantidad + 1);
        }
        // Desplazamos los elementos mayores hacia la derecha para mantener el orden ascendente
        int pos = cantidad;
        while (pos > 0 && datos[pos - 1] > valor) {
            Medidor::contarIndirecta();
            datos[pos] = datos[pos - 1];
            pos--;
        }
        datos[pos] = valor;
        cantidad++;
    }

    void eliminar(int pos) {
        if (pos < 0 || pos >= cantidad) {
            return;
        }
        for (int i = pos; i < cantidad - 1; i++) {
            Medidor::contarIndirecta();
            datos[i] = datos[i + 1];
        }
        cantidad--;
    }

    int buscar(const T& valor) const {
        for (int i = 0; i < cantidad; i++) {
            Medidor::contarIndirecta();
            if (datos[i] == valor) {
                return i;
            }
        }
        return -1;
    }

    int buscarBinaria(const T& valor) const {
        // Busqueda binaria O(log n) sobre el arreglo ordenado
        int inicio = 0;
        int fin = cantidad - 1;
        while (inicio <= fin) {
            Medidor::contarIndirecta();
            int medio = (inicio + fin) / 2;
            if (datos[medio] == valor) {
                return medio;
            }
            if (datos[medio] < valor) {
                inicio = medio + 1;
            } else {
                fin = medio - 1;
            }
        }
        return -1;
    }

    T& operator[](int pos) {
        return datos[pos];
    }

    const T& operator[](int pos) const {
        return datos[pos];
    }

    int tamano() const {
        return cantidad;
    }

    int getCapacidad() const {
        return capacidad;
    }

    bool vacia() const {
        return cantidad == 0;
    }

    unsigned long long memoriaDinamica() const {
        return (unsigned long long)capacidad * sizeof(T);
    }
};

#endif
