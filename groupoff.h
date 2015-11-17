#ifndef __GROUPOFF_H__
#define __GROUPOFF_H__

_Task Groupoff {
    void main();
  public:
    Groupoff( unsigned int numStudents, unsigned int sodaCost, unsigned int groupoffDelay );
    WATCard::FWATCard giftCard();
};

#endif
