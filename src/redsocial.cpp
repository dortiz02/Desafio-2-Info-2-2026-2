#include "redsocial.h"
#include "cadena.h"
#include "medidor.h"

RedSocial::RedSocial()
    : usuarios(), mensajes(), indice(), fechaActual(18, 10, 2026), rutaBase(""), enviosSesion(0),
      generador(std::random_device()()) {
}

RedSocial::RedSocial(const RedSocial& otra)
    : usuarios(), mensajes(), indice(otra.indice), fechaActual(otra.fechaActual),
      rutaBase(otra.rutaBase), enviosSesion(otra.enviosSesion), generador(otra.generador) {
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
        enviosSesion = otra.enviosSesion;
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

int RedSocial::amigosEnComun(int a, int b) const {
    const Lista<int>& la = usuarios[a]->getAmigos();
    const Lista<int>& lb = usuarios[b]->getAmigos();
    int i = 0;
    int j = 0;
    int comunes = 0;
    while (i < la.tamano() && j < lb.tamano()) {
        Medidor::contarIndirecta();
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

    double* base = new double[n];       // peso original de cada amigo
    double* actual = new double[n];     // peso con el que participa en el sorteo
    bool* elegido = new bool[n];

    for (int i = 0; i < n; i++) {
        Medidor::contarDirecta();
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
            Medidor::contarDirecta();
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
        Medidor::contarDirecta();
        double total = 0.0;
        for (int i = 0; i < n; i++) {
            Medidor::contarDirecta();
            total += actual[i];
        }

        if (total <= 0.0) {
            // ya no quedan amigos nuevos, se devuelven los excluidos al sorteo
            if (restaurado) {
                break;
            }
            restaurado = true;
            for (int i = 0; i < n; i++) {
                Medidor::contarDirecta();
                if (!elegido[i]) {
                    actual[i] = base[i];
                }
            }
            continue;
        }

        std::uniform_real_distribution<double> distribucion(0.0, total);
        double r = distribucion(generador);
        Medidor::contarExterna();

        double acumulado = 0.0;
        int posElegida = -1;
        for (int i = 0; i < n && posElegida == -1; i++) {
            Medidor::contarDirecta();
            if (actual[i] > 0.0) {
                acumulado += actual[i];
                if (r < acumulado) {
                    posElegida = i;
                }
            }
        }
        if (posElegida == -1) {
            // por redondeo puede no entrar: se toma el ultimo con peso
            for (int i = n - 1; i >= 0 && posElegida == -1; i--) {
                Medidor::contarDirecta();
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

    Medidor::sumarLocales(sizeof(int) * 7 + sizeof(double) * 3 + sizeof(bool) * 2
                          + sizeof(void*) * 6 + (sizeof(double) * 2 + sizeof(bool)) * n);

    delete[] base;
    delete[] actual;
    delete[] elegido;
    return cuantos;
}

int RedSocial::enviarSolicitud(int idUsuario, const std::string& nombreDestino) {
    int destino = buscarUsuario(nombreDestino);
    Medidor::sumarLocales(sizeof(int) * 2 + sizeof(void*));
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
    Medidor::sumarLocales(sizeof(int) * 3 + sizeof(void*));
    return true;
}

int RedSocial::sugerirAmigos(int idUsuario, int k, int* ids, int* comunes) const {
    int total = usuarios.tamano();
    const Usuario& yo = *usuarios[idUsuario];
    int* contador = new int[total];
    for (int i = 0; i < total; i++) {
        Medidor::contarDirecta();
        contador[i] = 0;
    }

    // -1 marca a quienes NO se pueden sugerir (yo, amigos, solicitudes pendientes)
    contador[idUsuario] = -1;
    for (int i = 0; i < yo.getAmigos().tamano(); i++) {
        Medidor::contarDirecta();
        contador[yo.getAmigos()[i]] = -1;
    }
    for (int i = 0; i < yo.getSolicitudesRecibidas().tamano(); i++) {
        Medidor::contarDirecta();
        contador[yo.getSolicitudesRecibidas()[i]] = -1;
    }
    for (int i = 0; i < yo.getSolicitudesEnviadas().tamano(); i++) {
        Medidor::contarDirecta();
        contador[yo.getSolicitudesEnviadas()[i]] = -1;
    }

    Lista<int> candidatos;
    for (int i = 0; i < yo.getAmigos().tamano(); i++) {
        Medidor::contarDirecta();
        const Lista<int>& amigosDelAmigo = usuarios[yo.getAmigos()[i]]->getAmigos();
        for (int j = 0; j < amigosDelAmigo.tamano(); j++) {
            Medidor::contarDirecta();
            int candidato = amigosDelAmigo[j];
            if (contador[candidato] >= 0) {
                if (contador[candidato] == 0) {
                    candidatos.agregar(candidato);
                }
                contador[candidato]++;
            }
        }
    }

    int cantidad = 0;
    for (int i = 0; i < candidatos.tamano(); i++) {
        Medidor::contarDirecta();
        int candidato = candidatos[i];
        int cant = contador[candidato];
        int pos = cantidad;
        while (pos > 0 && comunes[pos - 1] < cant) {
            Medidor::contarDirecta();
            pos--;
        }
        if (pos < k) {
            int fin = (cantidad < k) ? cantidad : k - 1;
            for (int q = fin; q > pos; q--) {
                Medidor::contarDirecta();
                comunes[q] = comunes[q - 1];
                ids[q] = ids[q - 1];
            }
            comunes[pos] = cant;
            ids[pos] = candidato;
            if (cantidad < k) {
                cantidad++;
            }
        }
    }

    Medidor::sumarLocales(sizeof(int) * 11 + sizeof(void*) * 4 + sizeof(Lista<int>)
                          + (unsigned long long)total * sizeof(int) + candidatos.memoriaDinamica());
    delete[] contador;
    return cantidad;
}

int RedSocial::enviarMensaje(int idEmisor, const std::string& nombreDestino, const std::string& texto) {
    int destino = buscarUsuario(nombreDestino);
    Medidor::sumarLocales(sizeof(int) * 3 + sizeof(void*) + sizeof(Fecha));
    if (destino == -1) {
        return MSG_NO_EXISTE;
    }
    if (!usuarios[idEmisor]->esAmigo(destino)) {
        return MSG_NO_AMIGO;
    }
    if (Cadena::esVacio(texto)) {
        return MSG_VACIO;
    }
    if ((int)texto.size() > Mensaje::LIMITE_TEXTO) {
        return MSG_MUY_LARGO;
    }
    Fecha ahora = fechaActual;
    ahora.setHora(8 + (enviosSesion / 60) % 16);
    ahora.setMinuto(enviosSesion % 60);
    enviosSesion++;
    if (mensajes.tamano() > 0) {
        const Fecha& ultima = mensajes[mensajes.tamano() - 1]->getFecha();
        if (ahora < ultima) {
            ahora = ultima;
        }
    }

    agregarMensaje(new Mensaje(idEmisor, destino, ahora, false, texto));
    return MSG_OK;
}

void RedSocial::recopilarNoLeidos(int idUsuario, Lista<int>& salida) const {
    const Lista<int>& recibidos = usuarios[idUsuario]->getMensajesRecibidos();
    for (int i = 0; i < recibidos.tamano(); i++) {
        Medidor::contarDirecta();
        if (!mensajes[recibidos[i]]->estaLeido()) {
            salida.agregar(recibidos[i]);
        }
    }
    Medidor::sumarLocales(sizeof(int) + sizeof(void*) * 2);
}

void RedSocial::marcarLeidos(const Lista<int>& ids) {
    for (int i = 0; i < ids.tamano(); i++) {
        Medidor::contarDirecta();
        mensajes[ids[i]]->marcarLeido();
    }
    Medidor::sumarLocales(sizeof(int) + sizeof(void*));
}


int RedSocial::posicionLimite(const Lista<int>& ids, int claveDia, bool estricto) const {
    int inicio = 0;
    int fin = ids.tamano();
    while (inicio < fin) {
        Medidor::contarIndirecta();
        int medio = (inicio + fin) / 2;
        int claveMedio = mensajes[ids[medio]]->getFecha().claveDia();
        if (claveMedio < claveDia || (estricto && claveMedio == claveDia)) {
            inicio = medio + 1;
        } else {
            fin = medio;
        }
    }
    return inicio;
}


bool RedSocial::buscarPorRango(int idUsuario, const Fecha& inicio, const Fecha& fin, Lista<int>& salida) const {
    if (inicio.claveDia() > fin.claveDia()) {
        return false;
    }
    const Lista<int>& recibidos = usuarios[idUsuario]->getMensajesRecibidos();
    int desde = posicionLimite(recibidos, inicio.claveDia(), false);
    int hasta = posicionLimite(recibidos, fin.claveDia(), true);
    for (int i = desde; i < hasta; i++) {
        Medidor::contarDirecta();
        salida.agregar(recibidos[i]);
    }
    Medidor::sumarLocales(sizeof(int) * 3 + sizeof(void*) * 2);
    return true;
}

unsigned long long RedSocial::memoriaTotal() const {
    unsigned long long total = sizeof(RedSocial);
    total += Cadena::bytesDinamicos(rutaBase);
    total += indice.memoriaDinamica();
    total += usuarios.memoriaDinamica();
    for (int i = 0; i < usuarios.tamano(); i++) {
        total += usuarios[i]->bytesTotales();
    }
    total += mensajes.memoriaDinamica();
    for (int i = 0; i < mensajes.tamano(); i++) {
        total += mensajes[i]->bytesTotales();
    }
    return total;
}
