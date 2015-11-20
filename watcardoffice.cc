#include "watcardoffice.h"


WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) :
    printer( prt ),
    bank( bank ),
    couriers( new Courier[numCouriers] ) {}

WATCardOffice::~WATCardOffice() {
    delete [] couriers;
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    return (WATCard::FWATCard)NULL;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
    return (WATCard::FWATCard)NULL;
}

void WATCardOffice::main() {
    printer.print( Printer::WATCardOffice, 'S' );
    
    _Accept( ~WATCardOffice ) {
        printer.print( Printer::WATCardOffice, 'F' );
    }
}
