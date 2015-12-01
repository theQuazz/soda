#ifndef __VENDINGMACHINE_H__
#define __VENDINGMACHINE_H__

#include "printer.h"
#include "nameserver.h"
#include "watcard.h"

_Task VendingMachine {
  public:
    enum Flavours {                         // flavours of soda (YOU DEFINE)
        Cola,
        RootBeer,
        GingerAle,
        CreamSoda,
        NUM_FLAVOURS
    };

  private:
    Printer &printer;
    unsigned int stock[NUM_FLAVOURS];
    const unsigned int id, sodaCost, maxStockPerFlavour;

    void main();

  public:
    _Event Funds {};                       // insufficient funds
    _Event Stock {};                       // out of stock for particular flavour
    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour );
    void buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
};

#endif
