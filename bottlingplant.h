#ifndef __BOTTLINGPLANT_H__
#define __BOTTLINGPLANT_H__

#include "printer.h"
#include "nameserver.h"

_Task BottlingPlant {
    Printer &printer;
    NameServer &nameServer;
    const unsigned int numVendingMachines;
    const unsigned int maxShippedPerFlavour;
    const unsigned int maxStockPerFlavour;
    const unsigned int timeBetweenShipments;

    void main();
  public:
    _Event Shutdown {};                    // shutdown plant
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    void getShipment( unsigned int cargo[] );
};

#endif
