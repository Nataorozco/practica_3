#include <iostream>
#include <string>
#include <cctype> //Básicamente, te permite hacerle "preguntas" a un carácter (¿es una letra?, ¿es un número?) o cambiar su formato (convertirlo a mayúscula o minúscula).
#include <stdexcept> //Su función principal es proporcionarte objetos de error "listos para usar" que puedes lanzar con throw y capturar con catch para evitar que el programa se cierre inesperadamente.

//Implemente el algoritmo de Run-Length Encoding (RLE) para comprimir y descomprimir una cadena
//de texto.
//Para esta parte:
//Se permite el uso de la clase string.
//Se pueden utilizar operaciones propias de la clase string para recorrer y manipular texto.
//No se permite el uso de librer´ıas de compresi´on externas.
//El programa deber´a:
//1. Recibir una cadena de texto.
//2. Generar su versi´on comprimida mediante R
//3. Implementar el proceso inverso para recuperar la cadena original.
//4. Verificar que la descompresi´on produzca exactamente el texto inicial.
//Se puede usar como referencia el video Ejemplo codificaci´on RLE


using std::cout;
using std::cin;
using std::endl;
using std::string; 

std::string comprimirRLE(const std::string& texto); // declaraciones o prototipos de funciones
std::string descomprimirRLE(const std::string& comprimido);

void ejercicio_1() {
    cout << "Ejercicio 1" << endl;
    // aqui el programa recibe una cadena de texto y la comprime usando el algoritmo RLE (Run Length Encoding)
    
    std::string texto;
    cout << "Ingrese una cadena de texto: ";
    std::getline(cin, texto); // se usa getline para permitir espacios en la entrada
    
    try { // Se intenta comprimir y descomprimir el texto, y se maneja cualquier error que pueda ocurrir durante el proceso
        std::string comprimido = comprimirRLE(texto);
        cout << "Comprimido: " << comprimido << endl;
        
        std::string descomprimido = descomprimirRLE(comprimido);
        cout << "Descomprimido: " << descomprimido << endl;
        
        if (texto == descomprimido) {
            cout << "Verificación exitosa: el texto original coincide." << endl;
        } else {
            cout << "Error: el texto descomprimido no coincide." << endl;
        }
    } catch (const std::exception& e) { // Manejo de errores en caso de formato RLE inválido
        cout << "Error: " << e.what() << endl; // Imprime el mensaje de error
    }
}

std::string comprimirRLE(const std::string& texto) {
    if (texto.empty()) return ""; // Si la cadena está vacía, retorna una cadena vacía

    std::string salida;
    char actual = texto[0];
    int contador = 1;

    for (size_t i = 1; i < texto.size(); ++i) {
        if (texto[i] == actual) {
            contador++;
        } else {
            salida += std::to_string(contador);
            salida.push_back(actual);
            actual = texto[i];
            contador = 1;
        }
    }
    salida += std::to_string(contador);
    salida.push_back(actual);
    return salida;
}

std::string descomprimirRLE(const std::string& comprimido) {
    if (comprimido.empty()) return "";

    std::string salida;
    size_t i = 0;
    while (i < comprimido.size()) {
        std::string numStr;

        while (i < comprimido.size() && std::isdigit(comprimido[i])) {

            if (i + 1 < comprimido.size() && !std::isdigit(comprimido[i + 1])) {
                break;
            }
            numStr += comprimido[i];
            i++;
        }

        if (numStr.empty()) {
            throw std::invalid_argument("Formato RLE inválido: falta número");
        }

        int contador = std::stoi(numStr);

        if (i >= comprimido.size()) {
            throw std::invalid_argument("Formato RLE inválido: falta carácter");
        }

        char caracter = comprimido[i];
        i++;

        // Evitar desbordamientos por números absurdamente grandes
        if (contador > 100000) throw std::out_of_range("Contador demasiado grande");

        for (int j = 0; j < contador; ++j) {
            salida.push_back(caracter);
        }
    }
    return salida;
}

