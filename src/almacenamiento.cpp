#include "almacenamiento.h"
#include "usuario.h"
#include "album.h"
#include "foto.h"
#include "mensaje.h"
#include "medidor.h"

Almacenamiento::Almacenamiento() {
}

Almacenamiento::Almacenamiento(const Almacenamiento&) {
}

// ---------------- escritura ----------------

void Almacenamiento::escribirEntero(std::ofstream& archivo, int valor) {
    archivo.write(reinterpret_cast<const char*>(&valor), sizeof(int));
}

void Almacenamiento::escribirTexto(std::ofstream& archivo, const std::string& texto) {
    int largo = (int)texto.size();
    escribirEntero(archivo, largo);
    archivo.write(texto.data(), largo);
}

void Almacenamiento::escribirFecha(std::ofstream& archivo, const Fecha& fecha) {
    escribirEntero(archivo, fecha.getDia());
    escribirEntero(archivo, fecha.getMes());
    escribirEntero(archivo, fecha.getAnio());
    escribirEntero(archivo, fecha.getHora());
    escribirEntero(archivo, fecha.getMinuto());
}

// ---------------- lectura ----------------

bool Almacenamiento::leerEntero(std::ifstream& archivo, int& valor) {
    archivo.read(reinterpret_cast<char*>(&valor), sizeof(int));
    return (bool)archivo;
}

bool Almacenamiento::leerTexto(std::ifstream& archivo, std::string& texto) {
    int largo = 0;
    if (!leerEntero(archivo, largo)) {
        return false;
    }
    if (largo < 0 || largo > 100000) {
        return false;
    }
    char* buffer = new char[largo + 1];
    archivo.read(buffer, largo);
    if (!archivo) {
        delete[] buffer;
        return false;
    }
    texto = std::string(buffer, largo);
    delete[] buffer;
    return true;
}

bool Almacenamiento::leerFecha(std::ifstream& archivo, Fecha& fecha) {
    int d, m, a, h, mi;
    if (!leerEntero(archivo, d) || !leerEntero(archivo, m) || !leerEntero(archivo, a)
        || !leerEntero(archivo, h) || !leerEntero(archivo, mi)) {
        return false;
    }
    fecha = Fecha(d, m, a, h, mi);
    return fecha.esValida();
}

// ---------------- guardar ----------------

bool Almacenamiento::guardar(const RedSocial& red, const std::string& ruta) {
    std::ofstream archivo(ruta, std::ios::binary);
    if (!archivo.is_open()) {
        return false;
    }

    int totalUsuarios = red.cantidadUsuarios();

    // 1. cabecera
    archivo.write("UDEB", 4);
    escribirEntero(archivo, 1);
    escribirFecha(archivo, red.getFechaActual());
    escribirTexto(archivo, red.getRutaBase());

    // 2. usuarios
    escribirEntero(archivo, totalUsuarios);
    for (int i = 0; i < totalUsuarios; i++) {
        const Usuario& u = red.getUsuario(i);
        escribirTexto(archivo, u.getNombre());
        escribirFecha(archivo, u.getFechaNacimiento());
        escribirTexto(archivo, u.getCorreo());
        escribirTexto(archivo, u.getRutaFotoPerfil());
        escribirTexto(archivo, u.getClave());
        escribirTexto(archivo, u.getCiudad());
        escribirTexto(archivo, u.getPais());
        escribirFecha(archivo, u.getFechaInscripcion());
    }

    // 3. amistades: cada pareja (a,b) se guarda una sola vez con a < b
    int aristas = 0;
    for (int i = 0; i < totalUsuarios; i++) {
        const Lista<int>& amigos = red.getUsuario(i).getAmigos();
        for (int j = 0; j < amigos.tamano(); j++) {
                if (amigos[j] > i) {
                aristas++;
            }
        }
    }
    escribirEntero(archivo, aristas);
    for (int i = 0; i < totalUsuarios; i++) {
        const Lista<int>& amigos = red.getUsuario(i).getAmigos();
        for (int j = 0; j < amigos.tamano(); j++) {
                if (amigos[j] > i) {
                escribirEntero(archivo, i);
                escribirEntero(archivo, amigos[j]);
            }
        }
    }

    // 4. solicitudes pendientes (quien envia, quien recibe)
    int solicitudes = 0;
    for (int i = 0; i < totalUsuarios; i++) {
        solicitudes += red.getUsuario(i).getSolicitudesEnviadas().tamano();
    }
    escribirEntero(archivo, solicitudes);
    for (int i = 0; i < totalUsuarios; i++) {
        const Lista<int>& enviadas = red.getUsuario(i).getSolicitudesEnviadas();
        for (int j = 0; j < enviadas.tamano(); j++) {
                escribirEntero(archivo, i);
            escribirEntero(archivo, enviadas[j]);
        }
    }

    // 5. albumes con sus fotos
    int albumes = 0;
    for (int i = 0; i < totalUsuarios; i++) {
        albumes += red.getUsuario(i).cantidadAlbumes();
    }
    escribirEntero(archivo, albumes);
    for (int i = 0; i < totalUsuarios; i++) {
        const Usuario& u = red.getUsuario(i);
        for (int j = 0; j < u.cantidadAlbumes(); j++) {
                const Album* a = u.getAlbum(j);
            escribirEntero(archivo, i);
            escribirTexto(archivo, a->getNombre());
            escribirTexto(archivo, a->getRuta());
            escribirEntero(archivo, (int)a->getPrivacidad());
            escribirEntero(archivo, a->cantidadFotos());
            for (int k = 0; k < a->cantidadFotos(); k++) {
                        const Foto& f = a->getFoto(k);
                escribirFecha(archivo, f.getFechaCaptura());
                escribirEntero(archivo, f.getTamanoKB());
                escribirTexto(archivo, f.getRuta());
                escribirEntero(archivo, f.getLikes());
            }
        }
    }

    // 6. mensajes (en orden de fecha)
    escribirEntero(archivo, red.cantidadMensajes());
    for (int i = 0; i < red.cantidadMensajes(); i++) {
        const Mensaje& m = red.getMensaje(i);
        escribirEntero(archivo, m.getIdEmisor());
        escribirEntero(archivo, m.getIdReceptor());
        escribirFecha(archivo, m.getFecha());
        escribirEntero(archivo, m.estaLeido() ? 1 : 0);
        escribirTexto(archivo, m.getTexto());
    }

    archivo.close();
    return true;
}

// ---------------- cargar ----------------

bool Almacenamiento::cargar(RedSocial& red, const std::string& ruta) {
    std::ifstream archivo(ruta, std::ios::binary);
    if (!archivo.is_open()) {
        return false;
    }

    // 1. cabecera
    char magia[4];
    archivo.read(magia, 4);
    if (!archivo || magia[0] != 'U' || magia[1] != 'D' || magia[2] != 'E' || magia[3] != 'B') {
        return false;
    }
    int version = 0;
    if (!leerEntero(archivo, version) || version != 1) {
        return false;
    }
    Fecha fechaActual;
    std::string rutaBase;
    if (!leerFecha(archivo, fechaActual) || !leerTexto(archivo, rutaBase)) {
        return false;
    }
    red.setFechaActual(fechaActual);
    red.setRutaBase(rutaBase);

    // 2. usuarios
    int totalUsuarios = 0;
    if (!leerEntero(archivo, totalUsuarios) || totalUsuarios < 0 || totalUsuarios > 1000000) {
        return false;
    }
    for (int i = 0; i < totalUsuarios; i++) {
        std::string nombre, correo, rutaFoto, clave, ciudad, pais;
        Fecha nacimiento, inscripcion;
        if (!leerTexto(archivo, nombre) || !leerFecha(archivo, nacimiento)
            || !leerTexto(archivo, correo) || !leerTexto(archivo, rutaFoto)
            || !leerTexto(archivo, clave) || !leerTexto(archivo, ciudad)
            || !leerTexto(archivo, pais) || !leerFecha(archivo, inscripcion)) {
            return false;
        }
        red.agregarUsuario(new Usuario(nombre, nacimiento, correo, rutaFoto, clave, ciudad, pais, inscripcion));
    }
    red.construirIndice();

    // 3. amistades
    int aristas = 0;
    if (!leerEntero(archivo, aristas) || aristas < 0) {
        return false;
    }
    for (int i = 0; i < aristas; i++) {
        int a, b;
        if (!leerEntero(archivo, a) || !leerEntero(archivo, b)) {
            return false;
        }
        if (a < 0 || b < 0 || a >= totalUsuarios || b >= totalUsuarios) {
            return false;
        }
        red.crearAmistad(a, b);
    }

    // 4. solicitudes
    int solicitudes = 0;
    if (!leerEntero(archivo, solicitudes) || solicitudes < 0) {
        return false;
    }
    for (int i = 0; i < solicitudes; i++) {
        int de, para;
        if (!leerEntero(archivo, de) || !leerEntero(archivo, para)) {
            return false;
        }
        if (de < 0 || para < 0 || de >= totalUsuarios || para >= totalUsuarios) {
            return false;
        }
        red.registrarSolicitud(de, para);
    }

    // 5. albumes
    int albumes = 0;
    if (!leerEntero(archivo, albumes) || albumes < 0) {
        return false;
    }
    for (int i = 0; i < albumes; i++) {
        int duenio, privacidad, fotos;
        std::string nombre, rutaAlbum;
        if (!leerEntero(archivo, duenio) || !leerTexto(archivo, nombre) || !leerTexto(archivo, rutaAlbum)
            || !leerEntero(archivo, privacidad) || !leerEntero(archivo, fotos)) {
            return false;
        }
        if (duenio < 0 || duenio >= totalUsuarios || privacidad < 0 || privacidad > 2 || fotos < 0) {
            return false;
        }
        Album* album = new Album(nombre, rutaAlbum, (Privacidad)privacidad);
        red.getUsuario(duenio).agregarAlbum(album);
        album->reservarFotos(fotos);
        for (int j = 0; j < fotos; j++) {
                Fecha captura;
            int tamano, likes;
            std::string rutaFoto;
            if (!leerFecha(archivo, captura) || !leerEntero(archivo, tamano)
                || !leerTexto(archivo, rutaFoto) || !leerEntero(archivo, likes)) {
                return false;
            }
            album->agregarFoto(Foto(captura, tamano, rutaFoto, likes));
        }
    }

    // 6. mensajes
    int totalMensajes = 0;
    if (!leerEntero(archivo, totalMensajes) || totalMensajes < 0) {
        return false;
    }
    for (int i = 0; i < totalMensajes; i++) {
        int emisor, receptor, leido;
        Fecha fecha;
        std::string texto;
        if (!leerEntero(archivo, emisor) || !leerEntero(archivo, receptor) || !leerFecha(archivo, fecha)
            || !leerEntero(archivo, leido) || !leerTexto(archivo, texto)) {
            return false;
        }
        if (emisor < 0 || receptor < 0 || emisor >= totalUsuarios || receptor >= totalUsuarios) {
            return false;
        }
        red.agregarMensaje(new Mensaje(emisor, receptor, fecha, leido == 1, texto));
    }

    archivo.close();
    return true;
}
