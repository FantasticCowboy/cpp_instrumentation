#include "Instrument.h"


int main(){
    Instrument i;

    i.IncrementKey("hello");

    i.dumpStats();
    i.dumpStats();
    i.dumpStats();
    i.IncrementKey("hello");
    i.dumpStats();
    i.dumpStats();
    
}