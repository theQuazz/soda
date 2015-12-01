#ifndef __GROUPOFF_H__
#define __GROUPOFF_H__

#include "MPRNG.h"
#include "printer.h"
#include "watcard.h"

_Task Groupoff {
    Printer &printer;
    const unsigned int numStudents, sodaCost, delay;
    WATCard::FWATCard curFCard;
    WATCard::FWATCard *fcards;
    WATCard *cards;

    void main();
  public:
    Groupoff( Printer &prt, unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    ~Groupoff();
    WATCard::FWATCard giftCard();
};

#endif
