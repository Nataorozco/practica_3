#include "enlace.h"
#include <iostream>
#include <conio.h>

using std::cout;
using std::cin;
using std::endl;

void imprimirMenu();
void ejecutar_ejercicio(int entrada);
void salir();

int main(){
    int entrada = 0;

    do {
        if (entrada != 0){
            cout << "Presione cualquier tecla para continuar..." << endl;
            _getch();
            system("cls");
        }
        imprimirMenu();
        cin >> entrada;
        cin.clear();
        ejecutar_ejercicio(entrada);
    } while (true);
    return 0;
}

void ejecutar_ejercicio(int entrada){

    switch (entrada) {
    case 1:
        ejercicio_1();
        cout << "\033[32mEjercicio 1 ejecutado.\033[0m" << endl;
        break;
    case 2:
        ejercicio_2();
        cout << "\033[32mEjercicio 2 ejecutado.\033[0m" << endl;
        break;
    case 3:
        ejercicio_3();
        cout << "\033[32mEjercicio 3 ejecutado.\033[0m" << endl;
        break;
    case 4:
        ejercicio_4();
        cout << "\033[32mEjercicio 4 ejecutado.\033[0m" << endl;
        break;
    case 5:
        ejercicio_5();
        cout << "\033[32mEjercicio 5 ejecutado.\033[0m" << endl;
        break;
    case 0:
        salir();
        break;
    default:
        cout << "\033[31mEntrada no valida.\033[0m" << endl;
        break;
    }
}

void imprimirMenu(){
    cout << "|-----------------------------------|" << endl;
    cout << "|         Ingrese un numero         |" << endl;
    cout << "| 1 - 5 para ejecutar un ejercicio  |" << endl;
    cout << "|            0 para salir           |" << endl;
    cout << "|-----------------------------------|" << endl;
    cout << "=> ";
}

void salir() {
    cout << "Saliendo..." << endl;
    exit(0);
}
