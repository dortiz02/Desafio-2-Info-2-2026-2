#include "consola.h"
#include "almacenamiento.h"
#include "cadena.h"
#include "medidor.h"
#include "lista.h"
#include <iostream>

Consola::Consola()
    : red(), rutaDatos(""), usuarioActual(-1), cantidadMejores(0), yaActualizo(false), entradaCerrada(false) {
    mejoresAmigos[0] = -1;
    mejoresAmigos[1] = -1;
    mejoresAmigos[2] = -1;
}

Consola::Consola(const Consola& otra)
    : red(otra.red), rutaDatos(otra.rutaDatos), usuarioActual(otra.usuarioActual),
      cantidadMejores(otra.cantidadMejores), yaActualizo(otra.yaActualizo),
      entradaCerrada(otra.entradaCerrada) {
    for (int i = 0; i < 3; i++) {
        mejoresAmigos[i] = otra.mejoresAmigos[i];
    }
}

Consola& Consola::operator=(const Consola& otra) {
    if (this != &otra) {
        red = otra.red;
        rutaDatos = otra.rutaDatos;
        usuarioActual = otra.usuarioActual;
        cantidadMejores = otra.cantidadMejores;
        yaActualizo = otra.yaActualizo;
        entradaCerrada = otra.entradaCerrada;
        for (int i = 0; i < 3; i++) {
            mejoresAmigos[i] = otra.mejoresAmigos[i];
        }
    }
    return *this;
}


std::string Consola::leerLinea(const std::string& mensaje) {
    std::cout << mensaje;
    std::string linea;
    if (!std::getline(std::cin, linea)) {
        entradaCerrada = true;
        return "salir";
    }
    return linea;
}

int Consola::leerEntero(const std::string& mensaje, int minimo, int maximo) {
    while (true) {
        std::string linea = leerLinea(mensaje);
        if (entradaCerrada) {
            return minimo;
        }
        int valor = 0;
        if (Cadena::aEntero(linea, valor) && valor >= minimo && valor <= maximo) {
            return valor;
        }
        std::cout << "Opcion no valida, debe ser un numero entre " << minimo << " y " << maximo << ".\n";
    }
}

bool Consola::leerFecha(const std::string& mensaje, Fecha& fecha) {
    while (true) {
        std::string linea = leerLinea(mensaje + " (dd/mm/aaaa, 0 para cancelar): ");
        if (entradaCerrada || Cadena::iguales(linea, "0")) {
            return false;
        }
        if (Fecha::desdeTexto(linea, fecha)) {
            return true;
        }
        std::cout << "Fecha no valida.\n";
    }
}

void Consola::mostrarMetricas() const {
    // el programa tambien incluye lo que ocupa la propia consola
    unsigned long long extra = sizeof(Consola) - sizeof(RedSocial);
    Medidor::mostrar(red.memoriaTotal() + extra);
}

void Consola::mostrarMensaje(const Mensaje& m) {
    const Usuario& emisor = red.getUsuario(m.getIdEmisor());
    std::cout << m.getFecha() << " | De: " << emisor.getNombre() << " | " << m.getTexto();
    if (!m.estaLeido()) {
        std::cout << "  (no leido)";
    }
    std::cout << "\n";
}

void Consola::mostrarResultadoSolicitud(int codigo) {
    if (codigo == RedSocial::SOL_OK) {
        std::cout << "Solicitud de amistad enviada.\n";
    } else if (codigo == RedSocial::SOL_NO_EXISTE) {
        std::cout << "Ese usuario no existe.\n";
    } else if (codigo == RedSocial::SOL_MISMO_USUARIO) {
        std::cout << "No puedes enviarte una solicitud a ti mismo.\n";
    } else if (codigo == RedSocial::SOL_YA_AMIGOS) {
        std::cout << "Ya son amigos.\n";
    } else if (codigo == RedSocial::SOL_YA_ENVIADA) {
        std::cout << "Ya le enviaste una solicitud a esa persona.\n";
    } else {
        std::cout << "Esa persona ya te envio una solicitud, aceptala desde 'Aceptar solicitudes'.\n";
    }
}


void Consola::ejecutar(const std::string& ruta) {
    rutaDatos = ruta;

    // I. Carga de datos (no aparece en el menu del usuario)
    Medidor::reiniciar();
    if (!Almacenamiento::cargar(red, rutaDatos)) {
        std::cout << "No se pudo cargar el archivo de datos: " << rutaDatos << "\n";
        return;
    }
    std::cout << "=== Ude@Book ===\n";
    std::cout << "Datos cargados: " << red.cantidadUsuarios() << " usuarios, "
              << red.cantidadMensajes() << " mensajes.\n";
    mostrarMetricas();

    bool seguir = true;
    while (seguir) {
        std::cout << "\n--- Ingreso a la plataforma ---\n";
        std::string nombre = leerLinea("Usuario (escriba 'salir' para terminar): ");
        if (Cadena::iguales(nombre, "salir")) {
            seguir = false;
        } else {
            std::string clave = leerLinea("Contrasena: ");
            Medidor::reiniciar();
            int id = red.autenticar(nombre, clave);
            Medidor::sumarLocales(sizeof(int) + sizeof(std::string) * 2);
            if (id == -1) {
                std::cout << "Usuario o contrasena incorrectos.\n";
                mostrarMetricas();
            } else {
                usuarioActual = id;
                iniciarSesion();
                menuPrincipal();
                usuarioActual = -1;
            }
        }
    }

    Medidor::reiniciar();
    if (Almacenamiento::guardar(red, rutaDatos)) {
        std::cout << "\nCambios guardados en " << rutaDatos << "\n";
    } else {
        std::cout << "\nNo se pudieron guardar los cambios.\n";
    }
    mostrarMetricas();
}


void Consola::iniciarSesion() {
    yaActualizo = false;
    cantidadMejores = red.elegirMejoresAmigos(usuarioActual, mejoresAmigos, nullptr, 0);
    std::cout << "\n";
    mostrarMejoresAmigos();
    const Usuario& yo = red.getUsuario(usuarioActual);
    std::cout << "\nBienvenido a UdeaBook " << yo.getNombre() << " (" << yo.getCiudad() << ", " << yo.getPais() << ")\n";
    Medidor::sumarLocales(sizeof(void*));
    mostrarMetricas();
}

void Consola::mostrarMejoresAmigos() {
    if (cantidadMejores == 0) {
        std::cout << "Aun no tienes amigos para mostrar.\n";
        return;
    }
    for (int i = 0; i < cantidadMejores; i++) {
        Medidor::contarDirecta();
        const Usuario& amigo = red.getUsuario(mejoresAmigos[i]);
        std::cout << "Foto perfil de amigo " << (i + 1) << ": " << amigo.getRutaFotoPerfil()
                  << " (" << amigo.getNombre() << ", "
                  << red.amigosEnComun(usuarioActual, mejoresAmigos[i]) << " amigos en comun)\n";
    }
}

void Consola::alternarMejoresAmigos() {
    Medidor::reiniciar();
    if (yaActualizo) {
        std::cout << "Ya utilizaste la unica actualizacion permitida de fotos de perfil.\n";
    } else if (cantidadMejores == 0) {
        std::cout << "Aun no tienes amigos para mostrar.\n";
    } else {
        int anteriores[3];
        int cuantosAnteriores = cantidadMejores;
        for (int i = 0; i < 3; i++) {
            Medidor::contarDirecta();
            anteriores[i] = mejoresAmigos[i];
        }
        cantidadMejores = red.elegirMejoresAmigos(usuarioActual, mejoresAmigos, anteriores, cuantosAnteriores);
        yaActualizo = true;
        Medidor::sumarLocales(sizeof(int) * 5);
        std::cout << "\n";
        mostrarMejoresAmigos();
    }
    mostrarMetricas();
}

void Consola::menuPrincipal() {
    bool seguir = true;
    while (seguir && !entradaCerrada) {
        std::cout << "\nMenu de opciones:\n\n";
        std::cout << "1.- Administrar amistades\n";
        std::cout << "2.- Visitar perfil de un amigo\n";
        std::cout << "3.- Ver sugerencias de amistad\n";
        std::cout << "4.- Administrar mensajes\n\n";
        if (!yaActualizo && cantidadMejores > 0) {
            std::cout << ".- Ingrese 'N' o 'n' para alternar el grupo de fotos de perfil de amigos.\n";
        }
        std::cout << ".- Ingrese 'S' o 's' para cerrar su sesion.\n\n";
        std::string entrada = leerLinea("Su opcion: ");
        if (entradaCerrada || Cadena::iguales(entrada, "S") || Cadena::iguales(entrada, "s")
            || Cadena::iguales(entrada, "0")) {
            seguir = false;
        } else if (Cadena::iguales(entrada, "N") || Cadena::iguales(entrada, "n")) {
            alternarMejoresAmigos();
        } else if (Cadena::iguales(entrada, "1")) {
            menuAmistades();
        } else if (Cadena::iguales(entrada, "2")) {
            visitarPerfil();
        } else if (Cadena::iguales(entrada, "3")) {
            verSugerencias();
        } else if (Cadena::iguales(entrada, "4")) {
            menuMensajes();
        } else {
            std::cout << "Opcion no valida.\n";
        }
    }
}

void Consola::menuAmistades() {
    bool seguir = true;
    while (seguir) {
        std::cout << "\n--- Administrar amistades ---\n";
        std::cout << "1. Enviar solicitud de amistad\n";
        std::cout << "2. Aceptar solicitudes recibidas\n";
        std::cout << "0. Volver\n";
        int opcion = leerEntero("Opcion: ", 0, 2);
        if (opcion == 1) {
            enviarSolicitud();
        } else if (opcion == 2) {
            aceptarSolicitudes();
        } else {
            seguir = false;
        }
    }
}

void Consola::enviarSolicitud() {
    std::string destino = leerLinea("Nombre de usuario al que enviar la solicitud: ");
    Medidor::reiniciar();
    int codigo = red.enviarSolicitud(usuarioActual, destino);
    Medidor::sumarLocales(sizeof(int) + sizeof(std::string));
    mostrarResultadoSolicitud(codigo);
    mostrarMetricas();
}

void Consola::aceptarSolicitudes() {
    Medidor::reiniciar();
    bool seguir = true;
    while (seguir) {
        Medidor::contarDirecta();
        const Lista<int>& pendientes = red.getUsuario(usuarioActual).getSolicitudesRecibidas();
        if (pendientes.tamano() == 0) {
            std::cout << "No tienes solicitudes pendientes.\n";
            seguir = false;
        } else {
            std::cout << "\nSolicitudes recibidas:\n";
            for (int i = 0; i < pendientes.tamano(); i++) {
                Medidor::contarDirecta();
                std::cout << "  " << (i + 1) << ". " << red.getUsuario(pendientes[i]) << "\n";
            }
            int opcion = leerEntero("Numero de la solicitud a aceptar (0 para volver): ", 0, pendientes.tamano());
            if (opcion == 0) {
                seguir = false;
            } else {
                std::string nuevoAmigo = red.getUsuario(pendientes[opcion - 1]).getNombre();
                red.aceptarSolicitud(usuarioActual, opcion - 1);
                std::cout << "Ahora eres amigo de " << nuevoAmigo << ".\n";
            }
        }
    }
    Medidor::sumarLocales(sizeof(int) * 3 + sizeof(bool) + sizeof(void*) + sizeof(std::string));
    mostrarMetricas();
}

void Consola::visitarPerfil() {
    std::string nombre = leerLinea("Nombre de usuario del amigo: ");
    Medidor::reiniciar();
    int idAmigo = red.buscarUsuario(nombre);

    if (idAmigo == -1) {
        std::cout << "Ese usuario no existe.\n";
    } else if (!red.sonAmigos(usuarioActual, idAmigo)) {
        std::cout << "Solo puedes visitar perfiles de tus amigos.\n";
    } else {
        Usuario& amigo = red.getUsuario(idAmigo);
        std::cout << "\nPerfil de " << amigo << "\n";
        bool seguir = true;
        while (seguir) {
            Medidor::contarDirecta();
            int total = amigo.cantidadAlbumes();
            int* visibles = new int[total > 0 ? total : 1];
            int cantidadVisibles = 0;

            std::cout << "\nAlbumes disponibles:\n";
            for (int i = 0; i < total; i++) {
                Medidor::contarDirecta();
                if (amigo.getAlbum(i)->esVisibleParaAmigos()) {
                    visibles[cantidadVisibles] = i;
                    cantidadVisibles++;
                    std::cout << "  " << cantidadVisibles << ". " << *amigo.getAlbum(i) << "\n";
                }
            }

            if (cantidadVisibles == 0) {
                std::cout << "  (no tiene albumes visibles)\n";
                seguir = false;
            } else {
                int opcion = leerEntero("Elija un album (0 para volver): ", 0, cantidadVisibles);
                if (opcion == 0) {
                    seguir = false;
                } else {
                    recorrerAlbum(*amigo.getAlbum(visibles[opcion - 1]));
                }
            }
            Medidor::sumarLocales(sizeof(int) * 5 + sizeof(void*));
            delete[] visibles;
        }
    }
    Medidor::sumarLocales(sizeof(int) + sizeof(std::string) + sizeof(void*));
    mostrarMetricas();
}

void Consola::recorrerAlbum(Album& album) {
    int total = album.cantidadFotos();
    if (total == 0) {
        std::cout << "El album esta vacio.\n";
        return;
    }


    bool* leDiLike = new bool[total];
    for (int i = 0; i < total; i++) {
        Medidor::contarDirecta();
        leDiLike[i] = false;
    }

    std::cout << "\nAlbum: " << album.getNombre() << "\n";
    int posicion = 0;
    bool seguir = true;
    while (seguir) {
        Medidor::contarDirecta();
        std::cout << "\nFoto " << (posicion + 1) << " de " << total << ": " << album.getFoto(posicion) << "\n";
        std::cout << "1. Siguiente\n2. Previa\n3. Dar/quitar like\n0. Salir del album\n";
        int opcion = leerEntero("Opcion: ", 0, 3);
        if (opcion == 1) {
            posicion = (posicion + 1) % total;
        } else if (opcion == 2) {
            posicion = (posicion - 1 + total) % total;
        } else if (opcion == 3) {
            if (leDiLike[posicion]) {
                album.getFoto(posicion).quitarLike();
                leDiLike[posicion] = false;
                std::cout << "Quitaste tu like.\n";
            } else {
                album.getFoto(posicion).darLike();
                leDiLike[posicion] = true;
                std::cout << "Diste like.\n";
            }
        } else {
            seguir = false;
        }
    }
    Medidor::sumarLocales(sizeof(int) * 4 + sizeof(bool) + sizeof(void*) + (unsigned long long)total * sizeof(bool));
    delete[] leDiLike;
}

void Consola::verSugerencias() {
    int k = leerEntero("Cuantas sugerencias desea ver (1 a 20): ", 1, 20);
    Medidor::reiniciar();

    int* ids = new int[k];
    int* comunes = new int[k];
    int cantidad = red.sugerirAmigos(usuarioActual, k, ids, comunes);

    if (cantidad == 0) {
        std::cout << "No hay sugerencias disponibles por ahora.\n";
    } else {
        std::cout << "\nSugerencias de amistad:\n";
        for (int i = 0; i < cantidad; i++) {
            Medidor::contarDirecta();
            std::cout << "  " << (i + 1) << ". " << red.getUsuario(ids[i]).getNombre()
                      << " - " << comunes[i] << " amigos en comun\n";
        }
        int opcion = leerEntero("Numero al que enviar solicitud (0 para ninguno): ", 0, cantidad);
        if (opcion > 0) {
            int codigo = red.enviarSolicitud(usuarioActual, ids[opcion - 1]);
            mostrarResultadoSolicitud(codigo);
        }
    }
    Medidor::sumarLocales(sizeof(int) * 5 + sizeof(void*) * 2 + (unsigned long long)k * sizeof(int) * 2);
    delete[] ids;
    delete[] comunes;
    mostrarMetricas();
}

void Consola::menuMensajes() {
    bool seguir = true;
    while (seguir) {
        std::cout << "\n--- Administrar mensajes ---\n";
        std::cout << "1. Enviar mensaje\n";
        std::cout << "2. Leer mensajes nuevos\n";
        std::cout << "3. Buscar mensajes por rango de fecha\n";
        std::cout << "0. Volver\n";
        int opcion = leerEntero("Opcion: ", 0, 3);
        if (opcion == 1) {
            enviarMensaje();
        } else if (opcion == 2) {
            leerMensajesNuevos();
        } else if (opcion == 3) {
            buscarMensajesPorFecha();
        } else {
            seguir = false;
        }
    }
}

void Consola::enviarMensaje() {
    std::string destino = leerLinea("Nombre del amigo: ");
    std::string texto = leerLinea("Mensaje (maximo 512 caracteres): ");
    Medidor::reiniciar();
    int codigo = red.enviarMensaje(usuarioActual, destino, texto);
    Medidor::sumarLocales(sizeof(int) + sizeof(std::string) * 2);

    if (codigo == RedSocial::MSG_OK) {
        std::cout << "Mensaje enviado.\n";
    } else if (codigo == RedSocial::MSG_NO_EXISTE) {
        std::cout << "Ese usuario no existe.\n";
    } else if (codigo == RedSocial::MSG_NO_AMIGO) {
        std::cout << "Solo puedes enviar mensajes a tus amigos.\n";
    } else if (codigo == RedSocial::MSG_VACIO) {
        std::cout << "No se puede enviar un mensaje vacio.\n";
    } else {
        std::cout << "El mensaje supera los 512 caracteres.\n";
    }
    mostrarMetricas();
}

void Consola::leerMensajesNuevos() {
    Medidor::reiniciar();
    Lista<int> nuevos;
    red.recopilarNoLeidos(usuarioActual, nuevos);

    if (nuevos.tamano() == 0) {
        std::cout << "No tienes mensajes nuevos.\n";
    } else {
        std::cout << "\nMensajes nuevos (" << nuevos.tamano() << "):\n";
        for (int i = 0; i < nuevos.tamano(); i++) {
            Medidor::contarDirecta();
            mostrarMensaje(red.getMensaje(nuevos[i]));
        }
        red.marcarLeidos(nuevos);
        std::cout << "Los mensajes quedaron marcados como leidos.\n";
    }
    Medidor::sumarLocales(sizeof(Lista<int>) + nuevos.memoriaDinamica() + sizeof(int));
    mostrarMetricas();
}

void Consola::buscarMensajesPorFecha() {
    Fecha inicio;
    Fecha fin;
    if (!leerFecha("Fecha de inicio", inicio)) {
        return;
    }
    if (!leerFecha("Fecha de fin", fin)) {
        return;
    }

    Medidor::reiniciar();
    Lista<int> encontrados;
    if (!red.buscarPorRango(usuarioActual, inicio, fin, encontrados)) {
        std::cout << "La fecha de inicio no puede ser posterior a la de fin.\n";
    } else if (encontrados.tamano() == 0) {
        std::cout << "No hay mensajes recibidos en ese rango.\n";
    } else {
        std::cout << "\nMensajes recibidos entre " << inicio.getDia() << "/" << inicio.getMes() << "/"
                  << inicio.getAnio() << " y " << fin.getDia() << "/" << fin.getMes() << "/"
                  << fin.getAnio() << ": " << encontrados.tamano() << "\n";
        for (int i = 0; i < encontrados.tamano(); i++) {
            Medidor::contarDirecta();
            mostrarMensaje(red.getMensaje(encontrados[i]));
        }
    }
    Medidor::sumarLocales(sizeof(Fecha) * 2 + sizeof(Lista<int>) + encontrados.memoriaDinamica() + sizeof(int));
    mostrarMetricas();
}
