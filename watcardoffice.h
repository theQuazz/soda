#ifndef __WATCARDOFFICE_H__
#define __WATCARDOFFICE_H__

#include <list>

#include "MPRNG.h"
#include "watcard.h"
#include "printer.h"
#include "bank.h"

_Task WATCardOffice {
    struct Args {
        unsigned int sid, amount;
        WATCard *curr;
    };
    struct Job {                           // marshalled arguments and return future
        Args args;                         // call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;          // return future
        Job( Args args ) : args( args ) {}
    };
    _Task Courier {                        // communicates with bank
        Printer &printer;
        WATCardOffice &office;
        Bank &bank;
        const unsigned int id;

        void main();
      public:
        Courier( Printer& printer, WATCardOffice &office, Bank &bank, unsigned int id );
    };

    unsigned int numCouriers;
    Printer &printer;
    Bank &bank;
    Courier **couriers;
    std::list<Job *> jobs;

    void main();
  public:
    _Event Lost {};                        // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCardOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
};

#endif
