#ifndef __TRUCK_H__
#define __TRUCK_H__

#include <algorithm>

#include "MPRNG.h"
#include "printer.h"
#include "nameserver.h"
#include "vendingmachine.h"

_Task BottlingPlant;

_Task Truck {
    Printer &printer;
    NameServer &nameServer;
    BottlingPlant &plant;
    VendingMachine **machines;
    const unsigned int numVendingMachines, maxStockPerFlavour;
    unsigned int cargo[VendingMachine::NUM_FLAVOURS], nextMachine, numSodas;

    void main();
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

#endif
