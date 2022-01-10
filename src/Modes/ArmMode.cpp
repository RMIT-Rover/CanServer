#include "ArmMode.h"

void ArmMode::perform(frame)
{
    if(frame.data[0] == 0b10000100)
        {
            if(frame.data[1] & (1<<0))
            {

                printf("first bit is a 0\r\n");\
                // use a socket to send shit to ARM.
            }
        }
}