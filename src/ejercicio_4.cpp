#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <string>
#include <limits>

using std::cout;
using std::cin;
using std::endl;

// ═══════════════════════════════════════════════════════════════════
//  Declaraciones de funciones de ejercicios anteriores
//  (ya implementadas en ejercicio_1.cpp y ejercicio_2.cpp)
// ═══════════════════════════════════════════════════════════════════
std::string comprimirRLE(const std::string& texto);
std::string descomprimirRLE(const std::string& comprimido);

char* comprimirLZ78(const char* texto, int longitud);
char* descomprimirLZ78(const char* comprimido, int longitud_comp);

unsigned char* encriptar(const unsigned char* datos, int tamanio, int n, unsigned char K);
unsigned char* desencriptar(const unsigned char* datos, int tamanio, int n, unsigned char K);

// ═══════════════════════════════════════════════════════════════════
//  Leer archivo de texto completo
//  Devuelve el contenido como char* (heap). El llamador libera con delete[].
// ═══════════════════════════════════════════════════════════════════
char* leerArchivo(const std::string& ruta, int& tamanio) {
    std::ifstream archivo(ruta);
    if (!archivo.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + ruta);
    }

    // Medir tamaño
    archivo.seekg(0, std::ios::end);
    tamanio = static_cast<int>(archivo.tellg());
    archivo.seekg(0, std::ios::beg);

    if (tamanio <= 0) {
        throw std::runtime_error("El archivo esta vacio: " + ruta);
    }

    char* contenido = new char[tamanio + 1];
    archivo.read(contenido, tamanio);
    contenido[tamanio] = '\0';
    return contenido;
}

// ═══════════════════════════════════════════════════════════════════
//  Escribir texto en un archivo
// ═══════════════════════════════════════════════════════════════════
void escribirArchivo(const std::string& ruta, const char* contenido) {
    std::ofstream archivo(ruta);
    if (!archivo.is_open()) {
        throw std::runtime_error("No se pudo crear el archivo: " + ruta);
    }
    archivo << contenido;
}

// ═══════════════════════════════════════════════════════════════════
//  Flujo con RLE
// ═══════════════════════════════════════════════════════════════════
void flujoRLE(const char* textoOriginal, int tamanio,
              int n, unsigned char K,
              const std::string& archivoSalida) {

    cout << "\n[RLE] Comprimiendo..." << endl;
    std::string textoStr(textoOriginal, tamanio);
    std::string comprimido = comprimirRLE(textoStr);
    cout << "  Tamanio original   : " << tamanio << " bytes" << endl;
    cout << "  Tamanio comprimido : " << comprimido.size() << " bytes" << endl;

    // ── Encriptar ─────────────────────────────────────────────────
    cout << "[RLE] Encriptando..." << endl;
    const unsigned char* bufComp =
        reinterpret_cast<const unsigned char*>(comprimido.c_str());
    int szComp = static_cast<int>(comprimido.size());

    unsigned char* encriptado = encriptar(bufComp, szComp, n, K);
    cout << "  Encriptacion completada." << endl;

    // ── Desencriptar ──────────────────────────────────────────────
    cout << "[RLE] Desencriptando..." << endl;
    unsigned char* desencriptado = desencriptar(encriptado, szComp, n, K);
    delete[] encriptado;
    cout << "  Desencriptacion completada." << endl;

    // ── Descomprimir ──────────────────────────────────────────────
    cout << "[RLE] Descomprimiendo..." << endl;
    std::string compRecuperado(reinterpret_cast<char*>(desencriptado), szComp);
    delete[] desencriptado;
    std::string textoFinal = descomprimirRLE(compRecuperado);

    // ── Verificar ─────────────────────────────────────────────────
    bool iguales = (textoFinal == textoStr);
    if (iguales) {
        cout << "  Verificacion exitosa: el texto recuperado es identico al original." << endl;
    } else {
        throw std::runtime_error("flujoRLE: el texto recuperado NO coincide con el original.");
    }

    // ── Escribir en archivo de salida ─────────────────────────────
    escribirArchivo(archivoSalida, textoFinal.c_str());
    cout << "  Resultado escrito en: " << archivoSalida << endl;
}

// ═══════════════════════════════════════════════════════════════════
//  Flujo con LZ78
// ═══════════════════════════════════════════════════════════════════
void flujoLZ78(const char* textoOriginal, int tamanio,
               int n, unsigned char K,
               const std::string& archivoSalida) {

    cout << "\n[LZ78] Comprimiendo..." << endl;
    char* comprimido = comprimirLZ78(textoOriginal, tamanio);
    int szComp = static_cast<int>(strlen(comprimido));
    cout << "  Tamanio original   : " << tamanio << " bytes" << endl;
    cout << "  Tamanio comprimido : " << szComp << " bytes" << endl;

    // ── Encriptar ─────────────────────────────────────────────────
    cout << "[LZ78] Encriptando..." << endl;
    const unsigned char* bufComp =
        reinterpret_cast<const unsigned char*>(comprimido);

    unsigned char* encriptado = encriptar(bufComp, szComp, n, K);
    delete[] comprimido;
    cout << "  Encriptacion completada." << endl;

    // ── Desencriptar ──────────────────────────────────────────────
    cout << "[LZ78] Desencriptando..." << endl;
    unsigned char* desencriptado = desencriptar(encriptado, szComp, n, K);
    delete[] encriptado;
    cout << "  Desencriptacion completada." << endl;

    // ── Descomprimir ──────────────────────────────────────────────
    cout << "[LZ78] Descomprimiendo..." << endl;
    char* compRecuperado = reinterpret_cast<char*>(desencriptado);
    compRecuperado[szComp] = '\0';

    char* textoFinal = descomprimirLZ78(compRecuperado, szComp);
    delete[] desencriptado;

    // ── Verificar ─────────────────────────────────────────────────
    bool iguales = (strcmp(textoOriginal, textoFinal) == 0);
    if (iguales) {
        cout << "  Verificacion exitosa: el texto recuperado es identico al original." << endl;
    } else {
        delete[] textoFinal;
        throw std::runtime_error("flujoLZ78: el texto recuperado NO coincide con el original.");
    }

    // ── Escribir en archivo de salida ─────────────────────────────
    escribirArchivo(archivoSalida, textoFinal);
    cout << "  Resultado escrito en: " << archivoSalida << endl;

    delete[] textoFinal;
}

// ═══════════════════════════════════════════════════════════════════
//  ejercicio_4(): funcion que se llama desde main.cpp
// ═══════════════════════════════════════════════════════════════════
void ejercicio_4() {
    cout << "\n--- Ejercicio 4: Integracion ---" << endl;

    try {
        // ── Archivo de entrada ────────────────────────────────────
        std::string archivoEntrada;
        cout << "Ingrese la ruta del archivo de entrada: ";
        std::getline(cin >> std::ws, archivoEntrada);

        int tamanio = 0;
        char* textoOriginal = leerArchivo(archivoEntrada, tamanio);
        cout << "Texto leido (" << tamanio << " bytes): " << textoOriginal << endl;

        // ── Metodo de compresion ──────────────────────────────────
        int metodo = 0;
        while (metodo != 1 && metodo != 2) {
            cout << "\nSeleccione metodo de compresion:" << endl;
            cout << "  1) RLE" << endl;
            cout << "  2) LZ78" << endl;
            cout << "Opcion: ";
            cin >> metodo;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (metodo != 1 && metodo != 2) {
                cout << "Opcion invalida." << endl;
            }
        }

        // ── Parametros de encriptacion ────────────────────────────
        int n = 0;
        while (n <= 0 || n >= 8) {
            cout << "Ingrese rotacion de bits n (1-7): ";
            cin >> n;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (n <= 0 || n >= 8) cout << "Valor invalido. n debe ser 1-7." << endl;
        }

        int kTemp = -1;
        while (kTemp < 0 || kTemp > 255) {
            cout << "Ingrese clave XOR K (0-255): ";
            cin >> kTemp;
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (kTemp < 0 || kTemp > 255) cout << "Valor invalido. K debe ser 0-255." << endl;
        }
        unsigned char K = static_cast<unsigned char>(kTemp);

        // ── Archivo de salida ─────────────────────────────────────
        std::string archivoSalida;
        cout << "Ingrese la ruta del archivo de salida: ";
        std::getline(cin >> std::ws, archivoSalida);

        // ── Ejecutar flujo ────────────────────────────────────────
        if (metodo == 1) {
            flujoRLE(textoOriginal, tamanio, n, K, archivoSalida);
        } else {
            flujoLZ78(textoOriginal, tamanio, n, K, archivoSalida);
        }

        delete[] textoOriginal;

    } catch (const std::exception& e) {
        cout << "Error detectado: " << e.what() << endl;
    }
}
