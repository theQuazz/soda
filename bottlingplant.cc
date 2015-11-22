#include "bottlingplant.h"

BottlingPlant::BottlingPlant(
        Printer &prt,
        NameServer &nameServer,
        unsigned int numVendingMachines,
        unsigned int maxShippedPerFlavour,
        unsigned int maxStockPerFlavour,
        unsigned int timeBetweenShipments ) :
    truck( prt, nameServer, *this, numVendingMachines, maxStockPerFlavour ),
    printer( prt ),
    nameServer( nameServer ),
    numVendingMachines( numVendingMachines ),
    maxShippedPerFlavour( maxShippedPerFlavour ),
    maxStockPerFlavour( maxShippedPerFlavour ),
    timeBetweenShipments( timeBetweenShipments ),
    shutdown( false ) {}

void BottlingPlant::getShipment( unsigned int cargo[] ) {
    if ( shutdown ) throw Shutdown();

    for ( unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++ ) {
        cargo[i] = production[i];
    }
}

void BottlingPlant::main() {
    printer.print( Printer::BottlingPlant, 'S' );

    for ( ;; ) {
        yield( timeBetweenShipments );

        for ( unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++ ) {
            production[i] = get_random()( 0, maxShippedPerFlavour );
        }

        _Accept( ~BottlingPlant ) {
            shutdown = true;
            try {
                _Accept( getShipment );
            }
            catch ( uMutexFailure::RendezvousFailure ) {
            }
            break;
        }
        or _Accept( getShipment ) {}
    }

    printer.print( Printer::BottlingPlant, 'F' );
}
