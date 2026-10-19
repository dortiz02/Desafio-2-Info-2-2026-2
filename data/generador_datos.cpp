#include <iostream>
#include <string>
#include <random>
#include "redsocial.h"
#include "usuario.h"
#include "album.h"
#include "foto.h"
#include "mensaje.h"
#include "fecha.h"
#include "almacenamiento.h"

const int NUM_USUARIOS = 1200;
const int TAM_GRUPO = 50;
const int NUM_MENSAJES = 15000;
const int NUM_SOLICITUDES = 400;

const int CANT_NOMBRES = 30;
const std::string NOMBRES[CANT_NOMBRES] = {
    "ana", "luis", "maria", "carlos", "laura", "juan", "sofia", "diego", "camila", "andres",
    "valentina", "felipe", "daniela", "sebastian", "paula", "mateo", "isabella", "santiago",
    "natalia", "julian", "carolina", "david", "mariana", "nicolas", "juliana", "jorge",
    "manuela", "esteban", "lucia", "tomas"};

const int CANT_CIUDADES = 10;
const std::string CIUDADES[CANT_CIUDADES] = {
    "Medellin", "Bogota", "Cali", "Barranquilla", "Cartagena",
    "Bucaramanga", "Pereira", "Manizales", "Cucuta", "Santa Marta"};

const int CANT_ALBUMES = 8;
const std::string NOMBRES_ALBUM[CANT_ALBUMES] = {
    "Vacaciones", "Familia", "Universidad", "Mascotas", "Cumpleanos", "Comida", "Viajes", "Amigos"};

const int CANT_FRASES = 12;
const std::string FRASES[CANT_FRASES] = {
    "Hola, como vas con el desafio de informatica?",
    "Nos vemos manana en la universidad",
    "Te mande las fotos del paseo, revisalas",
    "Acuerdate de la reunion del grupo de estudio",
    "Feliz cumpleanos, que la pases muy bien",
    "Oye, me prestas los apuntes de la clase?",
    "Quedamos para el almuerzo a las doce",
    "Ya termine el taller, quedo pendiente el informe",
    "Vamos a cine el fin de semana?",
    "Gracias por la ayuda con el codigo",
    "Que tal estuvo el examen?",
    "Llego tarde, empiecen sin mi"};

const double DENSIDADES[5] = {0.65, 0.80, 0.35, 0.50, 0.20};

void copiarTexto(char* destino, int& pos, const std::string& origen) {
    for (int i = 0; i < (int)origen.size(); i++) {
        destino[pos] = origen[i];
        pos++;
    }
}

void copiarNumero(char* destino, int& pos, int valor) {
    char digitos[12];
    int cuantos = 0;
    if (valor == 0) {
        digitos[0] = '0';
        cuantos = 1;
    }
    while (valor > 0) {
        digitos[cuantos] = (char)('0' + valor % 10);
        cuantos++;
        valor = valor / 10;
    }
    for (int i = cuantos - 1; i >= 0; i--) {
        destino[pos] = digitos[i];
        pos++;
    }
}

std::string unir(const std::string& a, const std::string& b) {
    char buffer[256];
    int pos = 0;
    copiarTexto(buffer, pos, a);
    copiarTexto(buffer, pos, b);
    return std::string(buffer, pos);
}

std::string unirConNumero(const std::string& a, int numero, const std::string& b) {
    char buffer[256];
    int pos = 0;
    copiarTexto(buffer, pos, a);
    copiarNumero(buffer, pos, numero);
    copiarTexto(buffer, pos, b);
    return std::string(buffer, pos);
}

std::string rutaPerfil(const std::string& base, const std::string& usuario) {
    char buffer[256];
    int pos = 0;
    copiarTexto(buffer, pos, "/users/");
    copiarTexto(buffer, pos, base);
    copiarTexto(buffer, pos, "/");
    copiarTexto(buffer, pos, usuario);
    copiarTexto(buffer, pos, "/albums/profile.png");
    return std::string(buffer, pos);
}

std::string rutaAlbum(const std::string& base, const std::string& usuario, const std::string& album) {
    char buffer[256];
    int pos = 0;
    copiarTexto(buffer, pos, "/users/");
    copiarTexto(buffer, pos, base);
    copiarTexto(buffer, pos, "/");
    copiarTexto(buffer, pos, usuario);
    copiarTexto(buffer, pos, "/albums/");
    copiarTexto(buffer, pos, album);
    return std::string(buffer, pos);
}

std::string rutaFoto(const std::string& rutaDelAlbum, int numero) {
    char buffer[256];
    int pos = 0;
    copiarTexto(buffer, pos, rutaDelAlbum);
    copiarTexto(buffer, pos, "/foto");
    copiarNumero(buffer, pos, numero);
    copiarTexto(buffer, pos, ".png");
    return std::string(buffer, pos);
}

int aleatorio(std::mt19937& gen, int minimo, int maximo) {
    std::uniform_int_distribution<int> dist(minimo, maximo);
    return dist(gen);
}

double probabilidad(std::mt19937& gen) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(gen);
}

void crearAlbum(RedSocial& red, std::mt19937& gen, int idUsuario, int indiceNombre, Privacidad priv, int numFotos) {
    Usuario& u = red.getUsuario(idUsuario);
    const std::string& nombreAlbum = NOMBRES_ALBUM[indiceNombre % CANT_ALBUMES];
    std::string ruta = rutaAlbum(red.getRutaBase(), u.getNombre(), nombreAlbum);
    Album* album = new Album(nombreAlbum, ruta, priv);
    album->reservarFotos(numFotos);
    for (int f = 0; f < numFotos; f++) {
        Fecha captura(aleatorio(gen, 1, 28), aleatorio(gen, 1, 12), aleatorio(gen, 2024, 2026),
                      aleatorio(gen, 0, 23), aleatorio(gen, 0, 59));
        if (captura.getAnio() == 2026 && captura.getMes() > 9) {
            captura.setMes(9);
        }
        album->agregarFoto(Foto(captura, aleatorio(gen, 300, 9000), rutaFoto(ruta, f + 1), aleatorio(gen, 0, 60)));
    }
    u.agregarAlbum(album);
}

int main() {
    std::mt19937 gen(2026);  
    RedSocial red;
    red.setFechaActual(Fecha(18, 10, 2026, 0, 0));
    red.setRutaBase("storage2026");

    for (int i = 0; i < NUM_USUARIOS; i++) {
        std::string nombre = unirConNumero(NOMBRES[i % CANT_NOMBRES], i, "");
        Fecha nacimiento(aleatorio(gen, 1, 28), aleatorio(gen, 1, 12), aleatorio(gen, 1985, 2007));
        Fecha inscripcion(aleatorio(gen, 1, 28), aleatorio(gen, 1, 12), aleatorio(gen, 2020, 2025));
        std::string correo = unir(nombre, "@udea.edu.co");
        std::string clave = unirConNumero("clave", i, "");
        std::string pais = (aleatorio(gen, 0, 19) == 0) ? "Mexico" : "Colombia";
        red.agregarUsuario(new Usuario(nombre, nacimiento, correo, rutaPerfil(red.getRutaBase(), nombre),
                                       clave, CIUDADES[aleatorio(gen, 0, CANT_CIUDADES - 1)], pais, inscripcion));
    }
    red.construirIndice();

    int grupos = NUM_USUARIOS / TAM_GRUPO;
    for (int g = 0; g < grupos; g++) {
        double densidad = DENSIDADES[g % 5];
        int inicio = g * TAM_GRUPO;
        for (int a = inicio; a < inicio + TAM_GRUPO; a++) {
            for (int b = a + 1; b < inicio + TAM_GRUPO; b++) {
                if (probabilidad(gen) < densidad) {
                    red.crearAmistad(a, b);
                }
            }
        }
    }
    // algunas amistades entre grupos distintos
    for (int a = 0; a < NUM_USUARIOS; a++) {
        for (int k = 0; k < 2; k++) {
            int b = aleatorio(gen, 0, NUM_USUARIOS - 1);
            if (b / TAM_GRUPO != a / TAM_GRUPO) {
                red.crearAmistad(a, b);
            }
        }
    }
    red.crearAmistad(0, 1);
    red.crearAmistad(0, 2);
    red.crearAmistad(0, 3);

    int creadas = 0;
    while (creadas < NUM_SOLICITUDES) {
        int a = aleatorio(gen, 0, NUM_USUARIOS - 1);
        int b = aleatorio(gen, 0, NUM_USUARIOS - 1);
        if (red.registrarSolicitud(a, b)) {
            creadas++;
        }
    }
    // el usuario 0 recibe varias solicitudes para probar la aceptacion
    int paraUsuario0 = 0;
    for (int emisor = 100; emisor < NUM_USUARIOS && paraUsuario0 < 4; emisor += 100) {
        if (red.registrarSolicitud(emisor, 0)) {
            paraUsuario0++;
        }
    }

    crearAlbum(red, gen, 1, 0, Privacidad::Publico, 5);
    crearAlbum(red, gen, 1, 1, Privacidad::Amigos, 5);
    crearAlbum(red, gen, 2, 2, Privacidad::Amigos, 5);
    crearAlbum(red, gen, 2, 3, Privacidad::Publico, 5);
    crearAlbum(red, gen, 3, 4, Privacidad::Privado, 5);
    crearAlbum(red, gen, 3, 5, Privacidad::Amigos, 5);
    for (int i = 0; i < NUM_USUARIOS; i++) {
        if (i >= 1 && i <= 3) {
            continue;
        }
        if (probabilidad(gen) < 0.35) {
            int cantidad = aleatorio(gen, 1, 3);
            for (int j = 0; j < cantidad; j++) {
                crearAlbum(red, gen, i, i + j * 3, (Privacidad)aleatorio(gen, 0, 2), aleatorio(gen, 3, 8));
            }
        }
    }

    Fecha momento(16, 8, 2026, 0, 0);
    int generados = 0;
    while (generados < NUM_MENSAJES) {
        momento.sumarMinutos(aleatorio(gen, 1, 11));
        if (momento.claveDia() >= red.getFechaActual().claveDia()) {
            break;
        }
        int emisor;
        int receptor;
        if (generados % 10 == 0) {
            // un 10% de los mensajes son para el usuario 0
            receptor = 0;
            const Lista<int>& amigos0 = red.getUsuario(0).getAmigos();
            emisor = amigos0[aleatorio(gen, 0, amigos0.tamano() - 1)];
        } else {
            emisor = aleatorio(gen, 0, NUM_USUARIOS - 1);
            const Lista<int>& amigos = red.getUsuario(emisor).getAmigos();
            if (amigos.tamano() == 0) {
                continue;
            }
            receptor = amigos[aleatorio(gen, 0, amigos.tamano() - 1)];
        }
        bool leido = true;
        if (momento.claveDia() >= 20261008 && probabilidad(gen) < 0.6) {
            leido = false;
        }
        red.agregarMensaje(new Mensaje(emisor, receptor, momento, leido, FRASES[aleatorio(gen, 0, CANT_FRASES - 1)]));
        generados++;
    }

    if (!Almacenamiento::guardar(red, "data/ude_datos.dat")) {
        std::cout << "No se pudo escribir data/ude_datos.dat (ejecute desde la carpeta raiz)\n";
        return 1;
    }

    long long totalAmistades = 0;
    for (int i = 0; i < NUM_USUARIOS; i++) {
        totalAmistades += red.getUsuario(i).getAmigos().tamano();
    }
    std::cout << "Archivo data/ude_datos.dat generado.\n";
    std::cout << "Usuarios: " << red.cantidadUsuarios() << "\n";
    std::cout << "Amistades: " << totalAmistades / 2 << "\n";
    std::cout << "Mensajes: " << red.cantidadMensajes() << "\n";
    std::cout << "Amigos del usuario ana0: " << red.getUsuario(0).getAmigos().tamano() << "\n";
    std::cout << "Usuario de prueba: ana0 / clave0\n";
    std::cout << "Amigos con 2 albumes de 5 fotos: luis1, maria2, carlos3\n";
    return 0;
}
