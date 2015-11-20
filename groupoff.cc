#include "groupoff.h"

Groupoff::Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ) :
    printer( prt ),
    numStudents( numStudents ),
    sodaCost( sodaCost ),
    delay( groupoffDelay ) {}

WATCard::FWATCard Groupoff::giftCard() {
    return (WATCard::FWATCard)NULL;
}

void Groupoff::main() {
    printer.print( Printer::Groupoff, 'S' );

    for ( unsigned int i = 0; i < numStudents; i++ ) {
        _Accept( ~Groupoff );
    }

    for ( unsigned int i = 0; i < numStudents; i++ ) {
        yield( delay );

        // give a random gift card

        _Accept( ~Groupoff ) {
            printer.print( Printer::Groupoff, 'F' );
        }
        _Else {}
    }
}
