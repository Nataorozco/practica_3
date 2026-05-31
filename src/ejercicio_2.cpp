#include <iostream>
#include <cstring>

using std::cout;
using std::cin;
using std::endl;

//Implemente el algoritmo LZ78 utilizando un diccionario dińamico construido manualmente.
//Para esta parte:
//No se permite el uso de vector, map, unordered map ni de otros contenedores de la STL.
//No se permite almacenar frases completas en string.
//El diccionario debe implementarse mediante arreglos dińamicos y memoria dińamica.
//Cada entrada del diccionario debe representarse como un par (prefijo, caŕacter).
//El programa deber ́a:
//1. Construir el diccionario dińamicamente durante la compresíon.
//2. Generar como salida pares ( ́ındice, caŕacter).
//3. Implementar el proceso de descompresíon mediante la reconstruccíon del diccionario.
//4. Verificar que el texto reconstruido sea identico al original.
//Se puede usar como referencia el video Ejemplo codificaci ́on LZ78
// Estructura para entradas del diccionario LZ78: cada entrada es un par (índice del prefijo, carácter)

struct entrada {
    int prefijo;  // Índice del prefijo en el diccionario (0 si no hay prefijo)
    char caracter; // Carácter que completa la entrada
};

// Función para comprimir usando LZ78
// Construye el diccionario dinámicamente y genera pares (índice, carácter)
char* comprimirLZ78(const char* texto, int longitud) {
    // Inicializar diccionario dinámico (empezamos con tamaño pequeño, expandimos si es necesario)
    int capacidad = 10; // Capacidad inicial
    entrada* diccionario = new entrada[capacidad];
    int tam_dic = 0; // Tamaño actual del diccionario

    // Para almacenar la salida comprimida como cadena de caracteres (pares serializados)
    // Usamos un buffer dinámico para construir la cadena comprimida
    char* comprimido = new char[longitud * 10]; // Estimación de tamaño
    int pos_comp = 0; // Posición en el buffer comprimido

    int i = 0;
    while (i < longitud) {
        int mejor_indice = 0; // Índice del prefijo más largo encontrado (0 = vacío)
        int longitud_prefijo = 0; // Longitud del prefijo encontrado

        // Buscar el prefijo más largo en el diccionario
        for (int j = 0; j < tam_dic; ++j) {
            // Reconstruir la frase de la entrada j
            // Nota: Para simplicidad, asumimos frases cortas; en producción, optimizar
            char* frase_j = new char[longitud + 1];
            int len_j = 0;
            int idx = j;
            while (idx != 0) {
                frase_j[len_j++] = diccionario[idx].caracter;
                idx = diccionario[idx].prefijo;
            }
            // Invertir la frase (ya que se construyó al revés)
            for (int k = 0; k < len_j / 2; ++k) {
                char temp = frase_j[k];
                frase_j[k] = frase_j[len_j - 1 - k];
                frase_j[len_j - 1 - k] = temp;
            }
            frase_j[len_j] = '\0';

            // Verificar si coincide con el prefijo actual
            int match_len = 0;
            while (i + match_len < longitud && match_len < len_j && texto[i + match_len] == frase_j[match_len]) {
                ++match_len;
            }
            if (match_len == len_j && match_len > longitud_prefijo) {
                mejor_indice = j + 1; // Índices empiezan en 1
                longitud_prefijo = match_len;
            }
            delete[] frase_j;
        }

        // Si no hay prefijo, usar 0
        if (longitud_prefijo == 0) {
            mejor_indice = 0;
        }

        // El carácter actual
        char actual = texto[i + longitud_prefijo];

        // Agregar a la salida comprimida: (índice,carácter)
        pos_comp += sprintf(comprimido + pos_comp, "(%d,%c)", mejor_indice, actual);

        // Agregar nueva entrada al diccionario
        if (tam_dic >= capacidad) {
            // Expandir diccionario
            capacidad *= 2;
            entrada* nuevo_dic = new entrada[capacidad];
            for (int k = 0; k < tam_dic; ++k) {
                nuevo_dic[k] = diccionario[k];
            }
            delete[] diccionario;
            diccionario = nuevo_dic;
        }
        diccionario[tam_dic].prefijo = mejor_indice;
        diccionario[tam_dic].caracter = actual;
        ++tam_dic;

        // Avanzar en el texto
        i += longitud_prefijo + 1;
    }

    // Liberar diccionario
    delete[] diccionario;

    // Terminar la cadena comprimida
    comprimido[pos_comp] = '\0';
    return comprimido;
}

// Función para descomprimir usando LZ78
// Reconstruye el diccionario y genera el texto original
char* descomprimirLZ78(const char* comprimido, int longitud_comp) {
    // Buffer para descomprimido (estimación grande)
    char* descomprimido = new char[longitud_comp * 10];
    int pos_desc = 0;

    // Diccionario para descompresión
    int capacidad = 10;
    entrada* diccionario = new entrada[capacidad];
    int tam_dic = 0;

    // Parsear la cadena comprimida
    const char* ptr = comprimido;
    while (*ptr) {
        if (*ptr == '(') {
            ++ptr;
            int indice = 0;
            while (*ptr >= '0' && *ptr <= '9') {
                indice = indice * 10 + (*ptr - '0');
                ++ptr;
            }
            if (*ptr == ',') ++ptr;
            char caracter = *ptr;
            ++ptr;
            if (*ptr == ')') ++ptr;

            // Reconstruir la frase para este par
            char* frase = new char[longitud_comp + 1];
            int len_frase = 0;
            if (indice == 0) {
                frase[len_frase++] = caracter;
            } else {
                // Construir la cadena del prefijo
                int idx = indice - 1;
                while (true) {
                    frase[len_frase++] = diccionario[idx].caracter;
                    if (diccionario[idx].prefijo == 0) break;
                    idx = diccionario[idx].prefijo - 1;
                }
                // Invertir la frase (estaba al revés)
                for (int k = 0; k < len_frase / 2; ++k) {
                    char temp = frase[k];
                    frase[k] = frase[len_frase - 1 - k];
                    frase[len_frase - 1 - k] = temp;
                }
                frase[len_frase++] = caracter;
            }
            frase[len_frase] = '\0';

            // Agregar a descomprimido
            strcpy(descomprimido + pos_desc, frase);
            pos_desc += len_frase;

            // Agregar al diccionario
            if (tam_dic >= capacidad) {
                capacidad *= 2;
                entrada* nuevo_dic = new entrada[capacidad];
                for (int k = 0; k < tam_dic; ++k) {
                    nuevo_dic[k] = diccionario[k];
                }
                delete[] diccionario;
                diccionario = nuevo_dic;
            }
            diccionario[tam_dic].prefijo = indice;
            diccionario[tam_dic].caracter = caracter;
            ++tam_dic;

            delete[] frase;
        } else {
            ++ptr;
        }
    }

    delete[] diccionario;
    descomprimido[pos_desc] = '\0';
    return descomprimido;
}

void ejercicio_2() {
    // Definición de constante para evitar "números mágicos" y mejorar el mantenimiento
    const int MAX_INPUT = 1000; 
    char texto[MAX_INPUT];

    cout << "\n--- Ejercicio 2: Compresion LZ78 ---" << endl;
    cout << "Ingrese una cadena de texto: ";
    
    // Se usa cin.getline para permitir espacios y controlar el límite del arreglo
    cin.getline(texto, MAX_INPUT);

    // Uso de size_t para compatibilidad técnica con strlen y evitar desbordamientos
    size_t longitud = strlen(texto);

    // Bloque try-catch obligatorio para el manejo de excepciones [3]
    try {
        if (longitud == 0) {
            throw std::invalid_argument("La cadena no puede estar vacia.");
        }

        // Llamada al módulo de compresión
        char* comprimido = comprimirLZ78(texto, longitud);
        cout << "Texto comprimido: " << comprimido << endl;

        // Llamada al módulo de descompresión
        char* descomprimido = descomprimirLZ78(comprimido, strlen(comprimido));
        cout << "Texto descomprimido: " << descomprimido << endl;

        // Verificación: comparar con el original
        if (strcmp(texto, descomprimido) == 0) {
            cout << "Verificacion exitosa: el texto descomprimido coincide con el original." << endl;
        } else {
            cout << "Error: el texto descomprimido no coincide con el original." << endl;
        }

        // Liberar memoria
        delete[] comprimido;
        delete[] descomprimido;
        
    } catch (const std::exception& e) {
        cout << "Error detectado: " << e.what() << endl;
    }
}
