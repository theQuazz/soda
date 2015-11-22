#include "truck.h"
#include "bottlingplant.h"

Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) :
    printer( prt ),
    nameServer( nameServer ),
    plant( plant ),
    numVendingMachines( numVendingMachines ),
    maxStockPerFlavour( maxStockPerFlavour ),
    nextMachine( 0 ) {}

void Truck::main() {
    printer.print( Printer::Truck, 'S' );

    machines = nameServer.getMachineList();

    for ( ;; ) {
        unsigned int delay = get_random()( 1, 10 );
        yield( delay );

        try {
            plant.getShipment( cargo );

            numSodas = 0;
            for ( unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++ ) {
                numSodas+= cargo[i];
            }

            printer.print( Printer::Truck, 'P', numSodas );
        }
        catch ( BottlingPlant::Shutdown ) {
            break;
        }

        for ( unsigned int i = 0; i < numVendingMachines && numSodas != 0; i++, nextMachine++ ) {
            if ( nextMachine == numVendingMachines ) {
                nextMachine = 0;
            }

            printer.print( Printer::BottlingPlant, 'd', nextMachine, numSodas );

            VendingMachine *machine = machines[nextMachine];
            unsigned int *inventory = machine->inventory();
            unsigned int unfilled = 0;

            for ( unsigned int j = 0; j < VendingMachine::NUM_FLAVOURS; j++ ) {
                unsigned int required = maxStockPerFlavour - inventory[j];
                unsigned int add = std::min( required, cargo[j] );

                inventory[j] += add;
                cargo[j] -= add;
                numSodas -= add;
                unfilled += required - add;
            }

            if ( unfilled ) {
                printer.print( Printer::BottlingPlant, 'U', nextMachine, unfilled );
            }
            printer.print( Printer::BottlingPlant, 'D', nextMachine, numSodas );

            machine->restocked();
        }
    }

    printer.print( Printer::Truck, 'F' );
}
