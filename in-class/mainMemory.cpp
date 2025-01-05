/*#include <iostream>
#include "memory.h"

int main()
{
    memory m;

    m.store(0, 72);     //H
    m.store(1, 69);     //E
    m.store(2, 76);     //L
    m.store(3, 76);     //L
    m.store(4, 79);     //O

    for(uint8_t i = 0; i < 5; i++)
        std::cout << static_cast<char>(m.load(i));
    std::cout << std::endl;

    process p1, p2;
    vector<process> processes;

    p1.base_address = 192000;
    p1.bound = 64000;               //end is the base_address + bound, tells us where end of memory address is
    p1.heap_base_address = 198000;
    p1.heap_bound = 19200;

    p2.base_address = 320000;
    p2.bound = 64000;
    //load or store -- add the base_address + address before execution

    return EXIT_SUCCESS;
}*/

#include <iostream>
#include <vector>
#include "mmu.h"
#include "process.h"

int main()
{
    mmu m;

    m.store( 0, 72 );
    m.store( 1, 69 );
    m.store( 2, 76 );
    m.store( 3, 76 );
    m.store( 4, 79 );

    for( uint8_t i=0; i < 5; i++)
        std::cout << static_cast<char>( m.load(i) );

    std::cout << std::endl;

    m.dump();

    return EXIT_SUCCESS;
}
