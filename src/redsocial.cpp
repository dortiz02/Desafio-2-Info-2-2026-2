#include "redsocial.h"
#include "cadena.h"

RedSocial::RedSocial()
    : usuarios(), mensajes(), indice(), fechaActual(18, 10, 2026), rutaBase(""),
      generador(std::random_device()()) {
}

RedSocial::RedSocial(const RedSocial& otra)
    : usuarios(), mensajes(), indice(otra.indice), fechaActual(otra.fechaActual),
      rutaBase(otra.rutaBase), generador(otra.generador) {
    copiarDe(otra);
}

RedSocial::~RedSocial() {
    liberar();
}

RedSocial& RedSocial::operator=(const RedSocial& otra) {
    if (this != &otra) {
        liberar();
        indice = otra.indice;
        fechaActual = otra.fechaActual;
        rutaBase = otra.rutaBase;
        generador = otra.generador;
        copiarDe(otra);
    }
    return *this;
}

void RedSocial::liberar() {
    for (int i = 0; i < usuarios.tamano(); i++) {
        delete usuarios[i];
    }
    for (int i = 0; i < mensajes.tamano(); i++) {
        delete mensajes[i];
    }
    usuarios = Lista<Usuario*>();
    mensajes = Lista<Mensaje*>();
}

void RedSocial::copiarDe(const RedSocial& otra) {
    usuarios.reservar(otra.usuarios.tamano());
    for (int i = 0; i < otra.usuarios.tamano(); i++) {
        usuarios.agregar(new Usuario(*otra.usuarios[i]));
    }
    mensajes.reservar(otra.mensajes.tamano());
    for (int i = 0; i < otra.mensajes.tamano(); i++) {
        mensajes.agregar(new Mensaje(*otra.mensajes[i]));
    }
}

int RedSocial::agregarUsuario(Usuario* usuario) {
    usuarios.agregar(usuario);
    return usuarios.tamano() - 1;
}

void RedSocial::construirIndice() {
    indice.construir(usuarios);
}

bool RedSocial::crearAmistad(int a, int b) {
    if (a == b || usuarios[a]->esAmigo(b)) {
        return false;
    }
    usuarios[a]->agregarAmigo(b);
    usuarios[b]->agregarAmigo(a);
    return true;
}

bool RedSocial::registrarSolicitud(int de, int para) {
    if (de == para || usuarios[de]->esAmigo(para)) {
        return false;
    }
    if (usuarios[de]->tieneSolicitudEnviadaA(para) || usuarios[para]->tieneSolicitudEnviadaA(de)) {
        return false;
    }
    usuarios[de]->agregarSolicitudEnviada(para);
    usuarios[para]->agregarSolicitudRecibida(de);
    return true;
}

int RedSocial::agregarMensaje(Mensaje* mensaje) {
    mensajes.agregar(mensaje);
    int id = mensajes.tamano() - 1;
    usuarios[mensaje->getIdEmisor()]->agregarMensajeEnviado(id);
    usuarios[mensaje->getIdReceptor()]->agregarMensajeRecibido(id);
    return id;
}

int RedSocial::cantidadUsuarios() const { return usuarios.tamano(); }
int RedSocial::cantidadMensajes() const { return mensajes.tamano(); }
Usuario& RedSocial::getUsuario(int id) { return *usuarios[id]; }
const Usuario& RedSocial::getUsuario(int id) const { return *usuarios[id]; }
Mensaje& RedSocial::getMensaje(int id) { return *mensajes[id]; }
const Mensaje& RedSocial::getMensaje(int id) const { return *mensajes[id]; }
const Fecha& RedSocial::getFechaActual() const { return fechaActual; }
void RedSocial::setFechaActual(const Fecha& f) { fechaActual = f; }
const std::string& RedSocial::getRutaBase() const { return rutaBase; }
void RedSocial::setRutaBase(const std::string& r) { rutaBase = r; }

int RedSocial::buscarUsuario(const std::string& nombre) const {
    return indice.buscar(nombre, usuarios);
}

int RedSocial::autenticar(const std::string& nombre, const std::string& clave) const {
    int id = buscarUsuario(nombre);
    if (id == -1) {
        return -1;
    }
    if (!Cadena::iguales(usuarios[id]->getClave(), clave)) {
        return -1;
    }
    return id;
}

bool RedSocial::sonAmigos(int a, int b) const {
    return usuarios[a]->esAmigo(b);
}

// Mejorado: recorrido con dos punteros sobre las listas ordenadas O(da + db)
int RedSocial::amigosEnComun(int a, int b) const {
    const Lista<int>& la = usuarios[a]->getAmigos();
    const Lista<int>& lb = usuarios[b]->getAmigos();
    int i = 0;
    int j = 0;
    int comunes = 0;
    while (i < la.tamano() && j < lb.tamano()) {
        if (la[i] == lb[j]) {
            comunes++;
            i++;
            j++;
        } else if (la[i] < lb[j]) {
            i++;
        } else {
            j++;
        }
    }
    return comunes;
}

int RedSocial::elegirMejoresAmigos(int idUsuario, int* resultado, const int* excluidos, int cantExcluidos) {
    const Lista<int>& amigos = usuarios[idUsuario]->getAmigos();
    int n = amigos.tamano();
    if (n == 0) {
        return 0;
    }

    double* base = new double[n];
    double* actual = new double[n];
    bool* elegido = new bool[n];

    for (int i = 0; i < n; i++) {
        int comunes = amigosEnComun(idUsuario, amigos[i]);
        double peso = 1.0;
        if (comunes > 20) {
            peso = 3.5;
        } else if (comunes > 10) {
            peso = 2.5;
        }
        base[i] = peso;
        elegido[i] = false;

        bool excluido = false;
        for (int j = 0; j < cantExcluidos; j++) {
            if (excluidos[j] == amigos[i]) {
                excluido = true;
            }
        }
        actual[i] = excluido ? 0.0 : peso;
    }

    int cuantos = 0;
    int maximo = (n < 3) ? n : 3;
    bool restaurado = false;

    while (cuantos < maximo) {
        double total = 0.0;
        for (int i = 0; i < n; i++) {
            total += actual[i];
        }

        if (total <= 0.0) {
            if (restaurado) {
                break;
            }
            restaurado = true;
            for (int i = 0; i < n; i++) {
                if (!elegido[i]) {
                    actual[i] = base[i];
                }
            }
            continue;
        }

        std::uniform_real_distribution<double> distribucion(0.0, total);
        double r = distribucion(generador);

        double acumulado = 0.0;
        int posElegida = -1;
        for (int i = 0; i < n && posElegida == -1; i++) {
            if (actual[i] > 0.0) {
                acumulado += actual[i];
                if (r < acumulado) {
                    posElegida = i;
                }
            }
        }
        if (posElegida == -1) {
            for (int i = n - 1; i >= 0 && posElegida == -1; i--) {
                if (actual[i] > 0.0) {
                    posElegida = i;
                }
            }
        }

        resultado[cuantos] = amigos[posElegida];
        cuantos++;
        actual[posElegida] = 0.0;
        elegido[posElegida] = true;
    }

    delete[] base;
    delete[] actual;
    delete[] elegido;
    return cuantos;
}

int RedSocial::enviarSolicitud(int idUsuario, const std::string& nombreDestino) {
    int destino = buscarUsuario(nombreDestino);
    if (destino == -1) {
        return SOL_NO_EXISTE;
    }
    if (destino == idUsuario) {
        return SOL_MISMO_USUARIO;
    }
    if (usuarios[idUsuario]->esAmigo(destino)) {
        return SOL_YA_AMIGOS;
    }
    if (usuarios[idUsuario]->tieneSolicitudEnviadaA(destino)) {
        return SOL_YA_ENVIADA;
    }
    if (usuarios[idUsuario]->tieneSolicitudRecibidaDe(destino)) {
        return SOL_YA_RECIBIDA;
    }
    registrarSolicitud(idUsuario, destino);
    return SOL_OK;
}

bool RedSocial::aceptarSolicitud(int idUsuario, int posicion) {
    Usuario& yo = *usuarios[idUsuario];
    if (posicion < 0 || posicion >= yo.getSolicitudesRecibidas().tamano()) {
        return false;
    }
    int emisor = yo.getSolicitudesRecibidas()[posicion];
    yo.quitarSolicitudRecibida(posicion);
    usuarios[emisor]->quitarSolicitudEnviada(idUsuario);
    yo.agregarAmigo(emisor);
    usuarios[emisor]->agregarAmigo(idUsuario);
    return true;
}
