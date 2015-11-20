#include "vendingmachine.h"

VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost, unsigned int maxStockPerFlavour ) :
    printer( prt ),
    id( id ),
    sodaCost( sodaCost ),
    maxStockPerFlavour( maxStockPerFlavour ),
    restocking( false )
{
    nameServer.VMregister( this );
}

void VendingMachine::buy( Flavours flavour, WATCard &card ) {
}

unsigned int *VendingMachine::inventory() {
    restocking = true;
    return NULL;
}

void VendingMachine::restocked() {
    restocking = false;
}

_Nomutex unsigned int VendingMachine::cost() {
    return sodaCost;
}

_Nomutex unsigned int VendingMachine::getId() {
    return id;
}

void VendingMachine::main() {
    for ( ;; ) {
        _Accept( ~VendingMachine ) {
        }
        or _When( ! restocking ) _Accept( buy ) {
        }
        or _Accept( inventory ) {
        }
        or _Accept( restocked ) {
        }
    }
}
