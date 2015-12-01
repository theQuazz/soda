#include "vendingmachine.h"

VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour ) :
    printer( prt ),
    id( id ),
    sodaCost( sodaCost ),
    maxStockPerFlavour( maxStockPerFlavour )
{
    // Set up stock values
    for ( unsigned int i = 0; i < NUM_FLAVOURS; i++ ) {
        stock[i] = 0;
    }

    // Register with nameserver
    nameServer.VMregister( this );
}

void VendingMachine::buy( Flavours _flavour, WATCard &_card ) {
    // Pass arguments into main task
    flavour = _flavour;
    card = &_card;
    // Wait for main task to process
    bench.wait();
    // Main process finished, check for & throw exception if necessary
    if (exc) {
        if (exc_type) {
            _Throw Funds();
        } else {
            _Throw Stock();
        }
    }
}

unsigned int *VendingMachine::inventory() {
    printer.print( Printer::Vending, id, 'r' );
    return stock;
}

void VendingMachine::restocked() {
    printer.print( Printer::Vending, id, 'R' );
}

_Nomutex unsigned int VendingMachine::cost() {
    return sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() {
    return id;
}

void VendingMachine::main() {
    printer.print( Printer::Vending, id, 'S', sodaCost );

    try {
        // Main loop - wait for buys, inventory, or destruction
        for ( ;; ) {
            _Accept( ~VendingMachine ) {
                printer.print( Printer::Vending, id, 'F' );
                // End task
                break;
            }
            or _Accept( buy ) {
                // Check balance & stock
                if ( card->getBalance() < sodaCost ) {
                    exc = true;
                    exc_type = true;
                } else if ( ! stock[flavour] ) {
                    exc = true;
                    exc_type = false;
                } else {
                    // Value & stock are ok, process transaction
                    exc = false;

                    stock[flavour] -= 1;

                    // Wait for available funds
                    card->withdraw( sodaCost );

                    printer.print( Printer::Vending, id, 'B', flavour, stock[flavour] );
                }
                // transaction complete, return control flow to method
                bench.signalBlock();
            }
            or _Accept( inventory ) {
                // Block until restock complete
                _Accept( restocked );
            }
        }
    }
    // Ignore exception since it is handled by caller to buy
    _CatchResume( uMutexFailure::RendezvousFailure ) {
    }
}
