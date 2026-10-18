#ifndef LISTA_H
#define LISTA_H

template <class T>
class Lista {
private:
    T* datos;
    int capacidad;
    int cantidad;

    void crecer(int minimo) {
        int nuevaCapacidad = (capacidad == 0) ? 4 : capacidad * 2;
        if (nuevaCapacidad < minimo) {
            nuevaCapacidad = minimo;
        }
        T* nuevo = new T[nuevaCapacidad];
        for (int i = 0; i < cantidad; i++) {
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
                    datos[i] = otra.datos[i];
                }
            }
        }
        return *this;
    }

    void agregar(const T& valor) {
        if (cantidad == capacidad) {
            crecer(cantidad + 1);
        }
        datos[cantidad] = valor;
        cantidad++;
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

    bool vacia() const {
        return cantidad == 0;
    }
};

#endif
