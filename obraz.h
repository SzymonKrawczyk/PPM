//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//                              Klasa obraz (plik .h)                                   //
//                                                                                      //
//  Klasa sluzy jako baza dla klas reprezentujacych poszczegolne formaty plikow         //
//  graficznych. Przechowuje wymiary, glebie kolorow i ich tablice oraz zarzadza nia.   //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//   Dostepna funkcjonalnosc:                                                           //
//                                                                                      //
//    (R, G, B, A to unsigned char, A w przedziale 0-100)                               //
//    (W, H, D to kolejno: szerokosc, wysokosc i glebia kolorow [unsigned int])         //
//                                                                                      //
//                                                                                      //
//    obraz.setColor(W, H, color)               // Ustawia kolor na podanej pozycji     //
//                                                                                      //
//    obraz.setColor(W, H, R, G, B, A = 100)    // Ustawia kolor na podanej pozycji     //
//                                                                                      //
//    obraz.getColor(W, H) const                // Pobiera kolor z podanej pozycji      //
//                                                                                      //
//                                                                                      //
//    obraz.setWidth   (W)              // Ustawia i zwraca szerokosc                   //
//                                                                                      //
//    obraz.setHeight  (H)              // Ustawia i zwraca wysokosc                    //
//                                                                                      //
//    obraz.setDepth   (D)              // Ustawia i zwraca glebie                      //
//                                                                                      //
//                                                                                      //
//    obraz.getWidth   () const         // Zwraca szerokosc                             //
//                                                                                      //
//    obraz.getHeight  () const         // Zwraca wysokosc                              //
//                                                                                      //
//    obraz.getDepth   () const         // Zwraca glebie                                //
//                                                                                      //
//                                                                                      //
//   Metody potencjalnie niebezpieczne:                                                 //
//                                                                                      //
//    obraz.deleteColorTab()        // Usuwa obecna tablice kolorow                     //
//                                                                                      //
//    obraz.createEmptyColorTab()   // Tworzy nowa tablice kolorow na podstawie         //
//                                  // obecnych wymiarow                                //
//                                                                                      //
//  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //
//                                                                                      //
//  Funkcjonalnosc niepubliczna:                                                        //
//                                                                                      //
//    obraz.setcolorTab(color**)    // Usuwa obecna tablice kolorow i ustawia nowa      //
//                                  // (kopia powierzchowna)                            //
//                                                                                      //
//    obraz.getcolorTab() const     // Pobiera wskaznik na obecna tablice kolorow       //
//                                                                                      //
//                                                                                      //
//  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //  //
//                                                                                      //
//  Potrzeba zdefiniowania w klasach pochodnych:                                        //
//                                                                                      //
//    obraz.loadFromFile(const char*)   // Odczyt z pliku                               //
//                                                                                      //
//    obraz.saveAs(const char*) const   // Zapis do pliku                               //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
//  Autor: Szymon Krawczyk                                                              //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////
//                                                                                      //
// Changelog:                                                                           //
//  Data        |  Zmiany                                           | Przez kogo        //
// -------------|-------------------------------------------------- |------------------ //
//  06/12/2019  | Utworzenie klasy                                  | Szymon Krawczyk   //
//              |                                                   |                   //
//  14/12/2019  | Dodanie komentarzy                                | Szymon Krawczyk   //
//              | Usuniecie niepotrzebnych plikow naglowkowych      |                   //
//              |                                                   |                   //
//                                                                                      //
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef OBRAZ_H
#define OBRAZ_H

#include "color.h"
#include <fstream>  // odczyt, zapis

namespace KrawczykS {

    class obraz {

    private:

        unsigned int _width;
        unsigned int _height;
        unsigned int _depth;
             color** _colorTab;

    protected:

        void deleteColorTab();          // Metody pomocnicze, nieprzeznaczone do uzytku publicznego
        void createEmptyColorTab();

    public: 

        obraz();
        obraz(const obraz&);
        virtual ~obraz();       // Destruktor wirtualny zapewnia wywolanie destruktorow klas pochodnych
                                //  dzieki temu zapobieagmy mozliwym bledom, 
                                //  poniewaz niewywolanie destruktora klasy pochodnej moze prowadzic do np.: wyciekow pamieci

        virtual void  setColor(unsigned int, unsigned int, const color&);
        virtual void  setColor(unsigned int, unsigned int, unsigned char, unsigned char, unsigned char, unsigned char = 100);

        virtual color getColor(unsigned int, unsigned int) const;

        unsigned int setWidth (unsigned int);
        unsigned int setHeight(unsigned int);
        unsigned int setDepth (unsigned int);

        unsigned int getWidth () const;
        unsigned int getHeight() const;
        unsigned int getDepth () const;

        virtual void loadFromFile(const char*)       = 0;
        virtual void saveAs      (const char*) const = 0;


        void    setcolorTab(color**);
        color** getcolorTab()  const;
    };
}

#endif