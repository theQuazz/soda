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

    // get machine list from nameserver
    machines = nameServer.getMachineList();

    for ( ;; ) { // main loop
        unsigned int delay = get_random()( 1, 10 );
        yield( delay );

        try {
            // get shipment from plant
            plant.getShipment( cargo );

            // calculate total sodas
            numSodas = 0;
            for ( unsigned int i = 0; i < VendingMachine::NUM_FLAVOURS; i++ ) {
                numSodas+= cargo[i];
            }

            printer.print( Printer::Truck, 'P', numSodas );
        }
        catch ( BottlingPlant::Shutdown ) {
            // got shutdown from plant, end this task
            break;
        }

        // Deliver to each vending machine
        for ( unsigned int i = 0; i < numVendingMachines && numSodas != 0; i++, nextMachine++ ) {
            if ( nextMachine == numVendingMachines ) {
                nextMachine = 0;
            }

            VendingMachine *machine = machines[nextMachine];
            printer.print( Printer::BottlingPlant, 'd', machine->getId(), numSodas );

            unsigned int *inventory = machine->inventory();
            unsigned int unfilled = 0;

            for ( unsigned int j = 0; j < VendingMachine::NUM_FLAVOURS; j++ ) {
                // restock according to existing inventory
                unsigned int required = maxStockPerFlavour - inventory[j];
                unsigned int add = std::min( required, cargo[j] );

                inventory[j] += add;
                cargo[j] -= add;
                numSodas -= add;
                unfilled += required - add;
            }

            // print according to result of restocking
            if ( unfilled ) {
                printer.print( Printer::BottlingPlant, 'U', nextMachine, unfilled );
            }
            printer.print( Printer::BottlingPlant, 'D', nextMachine, numSodas );

            // allow machine to resume operation
            machine->restocked();
        }
    }

    printer.print( Printer::Truck, 'F' );
}
