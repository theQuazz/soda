#include "groupoff.h"

Groupoff::Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay ) :
    printer( prt ),
    numStudents( numStudents ),
    sodaCost( sodaCost ),
    delay( groupoffDelay ),
    fcards( new WATCard::FWATCard[numStudents] ),
    cards( new WATCard[numStudents] ) {}

Groupoff::~Groupoff() {
    // Clean up arrays
    delete [] fcards;
    delete [] cards;
}

WATCard::FWATCard Groupoff::giftCard() {
    // Give the next gift card & imcrement pointer
    return curFCard;
}

void Groupoff::main() {
    printer.print( Printer::Groupoff, 'S' );

    for ( unsigned int i = 0; i < numStudents; i++ ) {
        // Pass fwatcard to giftCard method, and allow it to proceed
        curFCard = fcards[i];
        _Accept( giftCard );
    }

    for ( unsigned int i = 0; i < numStudents; i++ ) {
        yield( delay );

        // give card to a random student
        cards[i].deposit( sodaCost );

        // max = last possible index
        unsigned int max = numStudents - i - 1;
        // choose random student from 0 to max inclusive
        unsigned int rand = get_random()( 0, max );

        // grab associated card
        WATCard::FWATCard fcard = fcards[rand];
        // replace taken card with last card in current scope
        // next time, the scope will be shortened by 1 to excluce the duplicate entry
        fcards[rand] = fcards[max];
        // deliver the card
        fcard.delivery( &cards[i] );

        printer.print( Printer::Groupoff, 'D', sodaCost );

        // check for destructor call
        _Accept( ~Groupoff ) {
            // end task
            break;
        }
        _Else {}
    }

    printer.print( Printer::Groupoff, 'F' );
}
