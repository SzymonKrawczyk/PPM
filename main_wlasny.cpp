//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//                   Klasy obraz, ppm (plik pokazujacy mozliwosci klas)                 //
//                                                                                      //
//  Autor: Szymon Krawczyk                                                              //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
// Changelog:                                                                           //
//  Data        |  Zmiany                                           | Przez kogo        //
// -------------|-------------------------------------------------- |------------------ //
//  06/12/2019  | Utworzenie programu                               | Szymon Krawczyk   //
//              |                                                   |                   //
//  07/12/2019  | Napisanie algorytmow liczenia kolorow             | Szymon Krawczyk   //
//              |                                                   |                   //
//  14/12/2019  | Dodanie komentarzy                                | Szymon Krawczyk   //
//              | Dodanie obslugi wlasnych wyjatkow                 |                   //
//              | Dodanie mierzenia czasu wykonywania algorytmow    |                   //
//              |                                                   |                   //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>           // Algorytm nr1
#include <unordered_set>    // Algorytm nr2
#include <time.h>           // Czas liczenia

#include "ppm.h"
#include "MyException_base.h"

using std::cout;
using std::cin;

using KrawczykS::ppm;
using KrawczykS::color;
using KrawczykS::obraz;

using KrawczykS::myException_Base;

unsigned long liczKolory (const obraz&);
unsigned long liczKolory2(const obraz&);

int main() {

    ppm dynamic(15, 11);                    // Wlasny plik .ppm utworzony w programie

    dynamic.setColor(0,  0,   0,   0,   0);
    dynamic.setColor(1,  1, 255,   0, 255);
    dynamic.setColor(2,  2,   0,   0,   0);
    dynamic.setColor(3,  3,   0, 255, 255);
    dynamic.setColor(4,  4,   0,   0,   0);
    dynamic.setColor(5,  5, 255, 255,   0);
    dynamic.setColor(6,  6,   0,   0,   0);
    dynamic.setColor(7,  7,   0,   0, 255);
    dynamic.setColor(8,  8,   0,   0,   0);
    dynamic.setColor(9,  9,   0,   0, 255);
    dynamic.setColor(8,  9,   0, 255,   0);
    dynamic.setColor(7,  9, 255,   0,   0);
    dynamic.setColor(9,  8,   0, 255,   0);
    dynamic.setColor(9,  7, 255,   0,   0);
    dynamic.setColor(0, 10, 128, 128, 128);
    try {
        dynamic.saveAs("myFile.ppm");

        cout << "Utworzono plik \"myFile.ppm\"!\n";
    }
    catch (const myException_Base& error) {
        std::clog << "\nOjej! " << error.what() << "\n";
    }

    cout << "\nOtwieranie pliku Lenna.ppm...\n";
    ppm Lenna("Lenna.ppm");

    cout << "Rozpoczecie liczenia unikalnych kolorow...\n";

    clock_t time0 = time(NULL);  // time(NULL) - aktualny czas

    cout << "Unikalne kolory policzone funkcja nr2 (na podstawie unordered set): " << liczKolory2(Lenna) << " | ";
    clock_t time1 = time(NULL);
    cout << "Czas liczenia: " << difftime(time1, time0) << "s\n";

    cout << "Unikalne kolory policzone funkcja nr1 (na podstawie vector):        " << liczKolory(Lenna)  << " | ";
    clock_t time2 = time(NULL);
    cout << "Czas liczenia: " << difftime(time2, time1) << "s\n";


    cout << "\n\n- - - Koniec - - -\n"
         << "<Enter> aby kontynuowac";
    cin.get();
    return 0;
}



unsigned long liczKolory(const obraz& obraz_) {

    // Dzialanie algorytmu:
    //
    //  Idz przez cala tablice kolorow. Na kazdym kroku sprawdz,
    //  czy danego koloru nie ma w tablicy kolorow. Jezeli nie ma,
    //  dodaj do tablicy. Jezeli jest, przerwij i przejdz do nastepnego koloru.
    //
    //  Na koniec zwroc ilosc elementow w tablicy unikalnych kolorow.

    std::vector<color> uniqueColors;

    unsigned int width  = obraz_.getWidth   ();
    unsigned int height = obraz_.getHeight  ();
           color ** tab = obraz_.getcolorTab();

    bool repetition = false;

    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {

            repetition = false;

            for (unsigned int index = 0; index < uniqueColors.size(); index++) {

                if (tab[j][i] == uniqueColors[index]) {

                    repetition = true;
                    break;  // Nie sprawdzaj dalej, skoro juz jest powtorzony (optymalizacja)
                }
            }
            if (!repetition) {

                uniqueColors.push_back(tab[j][i]);
            }

        }
    }

    return uniqueColors.size();
}


unsigned long liczKolory2(const obraz& obraz_) {

    // Dzialanie algorytmu:
    //
    //  Idz przez cala tablice kolorow. Na kazdym kroku sprobuj dodac
    //  kolor do kontenera unordered_set. Unordered_set pozwala przechowywac 
    //  tylko jedna unikalna wartosc, przez co dodanie wartosci, ktora juz
    //  sie znajduje w kontenerze nie doda jej; doda za to kazdy nowy unikalny kolor.
    //
    //  Na koniec zwroc ilosc elementow w kontenerze unikalnych kolorow.

    // Funkcja hashujaca zamienia klucz na wartosc indeksowa, tworzac kubelek, ktory zapewnia szybki dostep do danych.
    //  Im bardziej unikalny klucz (dzieki lepszej funkcji hashujacej), tym wiecej kubelkow oraz tym szybsze 
    //  wyszukiwanie danych w kontenerze.

    struct color_hash {

        std::size_t operator()(const color& C_) const {

            unsigned long Key = (C_.getR() << 16) | (C_.getG() << 8) | (C_.getB());         // Tworzenie klucza
                                                                                            // poprzez binarne przesuniecia wartosci kolorow
                                                                                            // (w ten sposob klucze sa unikalne, co maksymalizuje
                                                                                            //  ilosc kubelkow)
            return std::hash<unsigned long>()(Key);
        }
    };

    std::unordered_set<color, color_hash> uniqueColors;

    unsigned int width  = obraz_.getWidth   ();
    unsigned int height = obraz_.getHeight  ();
           color ** tab = obraz_.getcolorTab();

    for (unsigned int i = 0; i < height; i++) {
        for (unsigned int j = 0; j < width; j++) {
            
            uniqueColors.insert(tab[j][i]);
        }
    }

    return uniqueColors.size();
}

/*
    //  Funkcja oparta na unordered_set jest nieporownywalnie szybsza,
    //  podczas testu na obrazie 2250px x 4000px, wyniki byly nastepujace:
    //
    //      Funkcja nr1 (na podstawie vector)        :  2491s (~41.5min)
    //      Funkcja nr2 (na podstawie unordered set) :     1s
    //
    //  Przy kazdym innym pliku funkcja nr2 rowniez byla szybsza.
*/