#include "watcardoffice.h"

#include <iostream>

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) :
    numCouriers( numCouriers ),
    printer( prt ),
    bank( bank ),
    couriers( new Courier*[numCouriers] )
{
    for ( unsigned int i = 0; i < numCouriers; i++ ) {
        couriers[i] = new Courier( printer, *this, bank, i );
    }
}

WATCardOffice::~WATCardOffice() {
    for ( unsigned int i = 0; i < numCouriers; i++ ) {
        delete couriers[i];
    }
    delete [] couriers;
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    return transfer( sid, amount, NULL );
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
    job = new Job( { sid, amount, card } );
    return job->result;
}

WATCardOffice::Job *WATCardOffice::requestWork() {
    return job;
}

WATCardOffice::Courier::Courier( Printer &printer, WATCardOffice &office, Bank &bank, unsigned int id ) :
    printer( printer ),
    office( office ),
    bank( bank ),
    id( id ) {}

void WATCardOffice::Courier::main() {
    printer.print( Printer::Courier, id, 'S' );

    for ( ;; ) {
        Job *job = office.requestWork();

        if ( job == NULL ) {
            break;
        }

        printer.print( Printer::Courier, id, 't', job->args.sid, job->args.amount );

        bank.withdraw( job->args.sid, job->args.amount );

        if ( get_random()( 1, 6 ) == 1 ) {
            if ( job->args.curr ) {
                delete job->args.curr;
            }
            job->result.exception( new Lost() );
        }
        else {
            if ( ! job->args.curr ) {
                job->args.curr = new WATCard();
            }
            job->args.curr->deposit( job->args.amount );
            job->result.delivery( job->args.curr );
        }

        printer.print( Printer::Courier, id, 'T', job->args.sid, job->args.amount );

        delete job;
    }
}

void WATCardOffice::main() {
    printer.print( Printer::WATCardOffice, 'S' );
    
    for ( ;; ) {
        _Accept( ~WATCardOffice ) {
            std::cout << job << std::endl;
            job = NULL;
            for ( unsigned int i = 0; i < numCouriers; i++ ) {
                _Accept( requestWork );
            }
            printer.print( Printer::WATCardOffice, 'F' );
            break;
        }
        or _Accept( create ) {
            printer.print( Printer::WATCardOffice, 'C', job->args.sid, job->args.amount );
            _Accept( requestWork );
        }
        or _Accept( transfer ) {
            printer.print( Printer::WATCardOffice, 'T', job->args.sid, job->args.amount );
            _Accept( requestWork );
        }
    }
}
