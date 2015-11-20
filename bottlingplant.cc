#include "bottlingplant.h"

BottlingPlant::BottlingPlant(
        Printer &prt,
        NameServer &nameServer,
        unsigned int numVendingMachines,
        unsigned int maxShippedPerFlavour,
        unsigned int maxStockPerFlavour,
        unsigned int timeBetweenShipments ) :
    printer( prt ),
    nameServer( nameServer ),
    numVendingMachines( numVendingMachines ),
    maxShippedPerFlavour( maxShippedPerFlavour ),
    maxStockPerFlavour( maxShippedPerFlavour ),
    timeBetweenShipments( timeBetweenShipments ) {}

void BottlingPlant::getShipment( unsigned int cargo[] ) {
}

void BottlingPlant::main() {
}
