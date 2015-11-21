#include "vendingmachine.h"

VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour ) :
    printer( prt ),
    id( id ),
    sodaCost( sodaCost ),
    maxStockPerFlavour( maxStockPerFlavour )
{
    for ( unsigned int i = 0; i < NUM_FLAVOURS; i++ ) {
        stock[i] = maxStockPerFlavour;
    }

    nameServer.VMregister( this );
}

void VendingMachine::buy( Flavours flavour, WATCard &card ) {
    if ( card.getBalance() < sodaCost ) {
        _Throw Funds();
    }

    if ( ! stock[flavour] ) {
        _Throw Stock();
    }

    stock[flavour] -= 1;

    card.withdraw( sodaCost );

    printer.print( Printer::Vending, id, 'B', flavour, stock[flavour] );
}

unsigned int *VendingMachine::inventory() {
    printer.print( Printer::Vending, id, 'r', sodaCost );
    return stock;
}

void VendingMachine::restocked() {
    printer.print( Printer::Vending, id, 'R', sodaCost );
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
        for ( ;; ) {
            _Accept( ~VendingMachine ) {
                printer.print( Printer::Vending, id, 'F' );
                break;
            }
            or _Accept( buy ) {
            }
            or _Accept( inventory ) {
              _Accept( restocked );
            }
        }
    }
    _CatchResume( uMutexFailure::RendezvousFailure ) {
    }
}
