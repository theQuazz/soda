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
    Job *job = new Job( { sid, amount, card } );
    jobs.push_back( job );
    return job->result;
}

WATCardOffice::Job *WATCardOffice::requestWork() {
    if ( jobs.empty() ) bench.wait();
    Job *job = jobs.front();
    jobs.pop_front();
    if (job != NULL) {
        printer.print( Printer::WATCardOffice, 'W' );
    }
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

    printer.print( Printer::Courier, id, 'F' );
}

void WATCardOffice::main() {
    printer.print( Printer::WATCardOffice, 'S' );

    for ( ;; ) {
        _Accept( ~WATCardOffice ) {
            for ( Job *job : jobs ) {
                delete job;
            }

            for ( unsigned int i = 0; i < numCouriers; i++ ) {
                jobs.push_back( NULL );
                bench.signal();
            }

            printer.print( Printer::WATCardOffice, 'F' );
            break;
        }
        or _Accept( create ) {
            printer.print( Printer::WATCardOffice, 'C', jobs.back()->args.sid, jobs.back()->args.amount );
            bench.signalBlock();
        }
        or _Accept( transfer ) {
            printer.print( Printer::WATCardOffice, 'T', jobs.back()->args.sid, jobs.back()->args.amount );
            bench.signalBlock();
        }
        or _Accept( requestWork ) {}
    }
}
