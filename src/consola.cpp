#include "consola.h"
#include "almacenamiento.h"
#include "cadena.h"
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
    if (!Almacenamiento::cargar(red, rutaDatos)) {
        std::cout << "No se pudo cargar el archivo de datos: " << rutaDatos << "\n";
        return;
    }
    std::cout << "=== Ude@Book ===\n";
    std::cout << "Datos cargados: " << red.cantidadUsuarios() << " usuarios, "
              << red.cantidadMensajes() << " mensajes.\n";

    bool seguir = true;
    while (seguir) {
        std::cout << "\n--- Ingreso a la plataforma ---\n";
        std::string nombre = leerLinea("Usuario (escriba 'salir' para terminar): ");
        if (Cadena::iguales(nombre, "salir")) {
            seguir = false;
        } else {
            std::string clave = leerLinea("Contrasena: ");
            int id = red.autenticar(nombre, clave);
            if (id == -1) {
                std::cout << "Usuario o contrasena incorrectos.\n";
            } else {
                usuarioActual = id;
                iniciarSesion();
                menuPrincipal();
                usuarioActual = -1;
            }
        }
    }

    if (Almacenamiento::guardar(red, rutaDatos)) {
        std::cout << "\nCambios guardados en " << rutaDatos << "\n";
    }
}

void Consola::iniciarSesion() {
    const Usuario& yo = red.getUsuario(usuarioActual);
    std::cout << "\nBienvenido, " << yo.getNombre() << " (" << yo.getCiudad() << ", " << yo.getPais() << ")\n";

    yaActualizo = false;
    cantidadMejores = red.elegirMejoresAmigos(usuarioActual, mejoresAmigos, nullptr, 0);
    mostrarMejoresAmigos();

    if (cantidadMejores > 0) {
        int opcion = leerEntero("Desea actualizar estas fotos? Solo se puede una vez (1 = si, 0 = no): ", 0, 1);
        if (opcion == 1) {
            int anteriores[3];
            int cuantosAnteriores = cantidadMejores;
            for (int i = 0; i < 3; i++) {
                anteriores[i] = mejoresAmigos[i];
            }
            cantidadMejores = red.elegirMejoresAmigos(usuarioActual, mejoresAmigos, anteriores, cuantosAnteriores);
            yaActualizo = true;
            mostrarMejoresAmigos();
        }
    }
}

void Consola::mostrarMejoresAmigos() {
    if (cantidadMejores == 0) {
        std::cout << "Aun no tienes amigos para mostrar.\n";
        return;
    }
    std::cout << "\nFotos de perfil de tus amigos:\n";
    for (int i = 0; i < cantidadMejores; i++) {
        const Usuario& amigo = red.getUsuario(mejoresAmigos[i]);
        std::cout << "  " << (i + 1) << ". " << amigo.getNombre() << " -> " << amigo.getRutaFotoPerfil()
                  << " (" << red.amigosEnComun(usuarioActual, mejoresAmigos[i]) << " amigos en comun)\n";
    }
}

void Consola::menuPrincipal() {
    bool seguir = true;
    while (seguir) {
        std::cout << "\n===== Menu principal (" << red.getUsuario(usuarioActual).getNombre() << ") =====\n";
        std::cout << "1. Administrar amistades\n";
        std::cout << "0. Cerrar sesion\n";
        int opcion = leerEntero("Opcion: ", 0, 1);
        if (opcion == 1) {
            menuAmistades();
        } else {
            seguir = false;
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
    int codigo = red.enviarSolicitud(usuarioActual, destino);
    mostrarResultadoSolicitud(codigo);
}

void Consola::aceptarSolicitudes() {
    bool seguir = true;
    while (seguir) {
        const Lista<int>& pendientes = red.getUsuario(usuarioActual).getSolicitudesRecibidas();
        if (pendientes.tamano() == 0) {
            std::cout << "No tienes solicitudes pendientes.\n";
            seguir = false;
        } else {
            std::cout << "\nSolicitudes recibidas:\n";
            for (int i = 0; i < pendientes.tamano(); i++) {
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
}
