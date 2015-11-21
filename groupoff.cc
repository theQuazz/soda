#include "groupoff.h"

Groupoff::Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ) :
    printer( prt ),
    numStudents( numStudents ),
    sodaCost( sodaCost ),
    delay( groupoffDelay ),
    cardIndex( 0 ),
    fcards( new WATCard::FWATCard[numStudents] ),
    cards( new WATCard[numStudents] ) {}

Groupoff::~Groupoff() {
    delete [] fcards;
    delete [] cards;
}

WATCard::FWATCard Groupoff::giftCard() {
    return fcards[cardIndex++];
}

void Groupoff::main() {
    printer.print( Printer::Groupoff, 'S' );

    for ( unsigned int i = 0; i < numStudents; i++ ) {
        _Accept( giftCard );
    }

    for ( unsigned int i = 0; i < numStudents; i++ ) {
        yield( delay );

        // give card to a random student
        cards[i].deposit( sodaCost );

        unsigned int max = numStudents - i - 1;
        unsigned int rand = get_random()( 0, max );

        WATCard::FWATCard fcard = fcards[rand];
        fcards[rand] = fcards[max];
        fcard.delivery( &cards[i] );

        printer.print( Printer::Groupoff, 'D', sodaCost );

        _Accept( ~Groupoff ) {
            break;
        }
        _Else {}
    }

    printer.print( Printer::Groupoff, 'F' );
}
