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
    if ( shutdown ) throw Shutdown(); // end task

    // copy production into cargo
    for ( unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++ ) {
        cargo[i] = production[i];
    }
}

void BottlingPlant::main() {
    printer.print( Printer::BottlingPlant, 'S' );

    // main task loop
    for ( ;; ) {
        yield( timeBetweenShipments );

        unsigned int numProduced = 0;
        // produce flavours for next shipment
        for ( unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++ ) {
            production[i] = get_random()( 0, maxShippedPerFlavour );
            numProduced += production[i];
        }

        printer.print( Printer::BottlingPlant, 'G', numProduced );

        _Accept( ~BottlingPlant ) {
            // throw shutdown to truck to shut it down, then shut down this task
            shutdown = true;
            try {
                _Accept( getShipment );
            }
            catch ( uMutexFailure::RendezvousFailure ) {
            }
            break;
        }
        or _Accept( getShipment ) {
            printer.print( Printer::BottlingPlant, 'P' );
        }
    }

    printer.print( Printer::BottlingPlant, 'F' );
}
