#include <iostream>
#include <stdexcept>
#include <limits>

using std::cout;
using std::cin;
using std::endl;

// ─────────────────────────────────────────────────────────────────
//  Rotacion circular de bits a la IZQUIERDA (1 byte)
//  Ejemplo con n=2: 01000001 → 00000101
//  Los bits que "salen" por la izquierda entran por la derecha.
// ─────────────────────────────────────────────────────────────────
unsigned char rotarIzquierda(unsigned char byte, int n) {
    return (byte << n) | (byte >> (8 - n));
}

// ─────────────────────────────────────────────────────────────────
//  Rotacion circular de bits a la DERECHA (1 byte)
//  Es el proceso inverso de rotarIzquierda.
// ─────────────────────────────────────────────────────────────────
unsigned char rotarDerecha(unsigned char byte, int n) {
    return (byte >> n) | (byte << (8 - n));
}

// ─────────────────────────────────────────────────────────────────
//  Encriptar: recibe un buffer de bytes, su tamaño, n y K.
//  Devuelve un nuevo buffer con los bytes encriptados (heap).
//  El llamador es responsable de liberar con delete[].
//
//  Por cada byte:
//    1. rotacion izquierda n bits
//    2. XOR con K
// ─────────────────────────────────────────────────────────────────
unsigned char* encriptar(const unsigned char* datos, int tamanio, int n, unsigned char K) {
    if (datos == nullptr || tamanio <= 0) {
        throw std::invalid_argument("Encriptar: buffer nulo o tamanio invalido.");
    }
    if (n <= 0 || n >= 8) {
        throw std::invalid_argument("Encriptar: n debe estar entre 1 y 7.");
    }

    unsigned char* resultado = new unsigned char[tamanio];

    for (int i = 0; i < tamanio; ++i) {
        unsigned char rotado = rotarIzquierda(datos[i], n);
        resultado[i] = rotado ^ K;
    }

    return resultado;
}

// ─────────────────────────────────────────────────────────────────
//  Desencriptar: proceso exactamente inverso a encriptar.
//  Por cada byte:
//    1. XOR con K  (deshace el XOR)
//    2. rotacion derecha n bits  (deshace la rotacion)
// ─────────────────────────────────────────────────────────────────
unsigned char* desencriptar(const unsigned char* datos, int tamanio, int n, unsigned char K) {
    if (datos == nullptr || tamanio <= 0) {
        throw std::invalid_argument("Desencriptar: buffer nulo o tamanio invalido.");
    }
    if (n <= 0 || n >= 8) {
        throw std::invalid_argument("Desencriptar: n debe estar entre 1 y 7.");
    }

    unsigned char* resultado = new unsigned char[tamanio];

    for (int i = 0; i < tamanio; ++i) {
        unsigned char sinXor = datos[i] ^ K;
        resultado[i] = rotarDerecha(sinXor, n);
    }

    return resultado;
}

// ─────────────────────────────────────────────────────────────────
//  ejercicio_3(): funcion que se llama desde main.cpp
// ─────────────────────────────────────────────────────────────────
void ejercicio_3() {
    cout << "\n--- Ejercicio 3: Encriptacion y Desencriptacion ---" << endl;

    // Texto de prueba
    const int MAX_INPUT = 1000;
    char texto[MAX_INPUT];

    cout << "Ingrese una cadena de texto: ";
    cin.getline(texto, MAX_INPUT);

    // Pedir n
    int n = 0;
    while (n <= 0 || n >= 8) {
        cout << "Ingrese el numero de rotaciones de bits n (1-7): ";
        cin >> n;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (n <= 0 || n >= 8) {
            cout << "Valor invalido. n debe estar entre 1 y 7." << endl;
        }
    }

    // Pedir K
    int kTemp = -1;
    while (kTemp < 0 || kTemp > 255) {
        cout << "Ingrese la clave K (0-255): ";
        cin >> kTemp;
        cin.clear();
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (kTemp < 0 || kTemp > 255) {
            cout << "Valor invalido. K debe estar entre 0 y 255." << endl;
        }
    }
    unsigned char K = static_cast<unsigned char>(kTemp);

    try {
        int tamanio = 0;
        while (texto[tamanio] != '\0') ++tamanio; // longitud sin strlen

        if (tamanio == 0) {
            throw std::invalid_argument("La cadena no puede estar vacia.");
        }

        // Convertir texto a buffer de bytes
        const unsigned char* buffer = reinterpret_cast<const unsigned char*>(texto);

        // Encriptar
        unsigned char* encriptado = encriptar(buffer, tamanio, n, K);

        cout << "\nTexto original   : " << texto << endl;
        cout << "Bytes encriptados: ";
        for (int i = 0; i < tamanio; ++i) {
            // Imprimir en hexadecimal para visualizar la transformacion
            cout << std::hex << std::uppercase
                 << (int)encriptado[i] << " ";
        }
        cout << std::dec << endl;

        // Desencriptar
        unsigned char* desencriptado = desencriptar(encriptado, tamanio, n, K);

        // Convertir de vuelta a texto para mostrar
        char* textoRecuperado = reinterpret_cast<char*>(desencriptado);
        textoRecuperado[tamanio] = '\0';

        cout << "Texto recuperado : " << textoRecuperado << endl;

        // Verificar integridad
        bool iguales = true;
        for (int i = 0; i < tamanio; ++i) {
            if (texto[i] != textoRecuperado[i]) {
                iguales = false;
                break;
            }
        }

        if (iguales) {
            cout << "Verificacion exitosa: el texto recuperado coincide con el original." << endl;
        } else {
            cout << "Error: el texto recuperado no coincide con el original." << endl;
        }

        delete[] encriptado;
        delete[] desencriptado;

    } catch (const std::exception& e) {
        cout << "Error detectado: " << e.what() << endl;
    }
}
