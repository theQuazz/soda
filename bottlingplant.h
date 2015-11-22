#ifndef __BOTTLINGPLANT_H__
#define __BOTTLINGPLANT_H__

#include "MPRNG.h"
#include "truck.h"
#include "printer.h"
#include "nameserver.h"
#include "vendingmachine.h"

_Task BottlingPlant {
    Truck truck;
    Printer &printer;
    NameServer &nameServer;
    const unsigned int numVendingMachines;
    const unsigned int maxShippedPerFlavour;
    const unsigned int maxStockPerFlavour;
    const unsigned int timeBetweenShipments;
    bool shutdown;
    unsigned int production[VendingMachine::NUM_FLAVOURS];

    void main();
  public:
    _Event Shutdown {};                    // shutdown plant
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    void getShipment( unsigned int cargo[] );
};

#endif
