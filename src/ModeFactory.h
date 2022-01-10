#define ARM 0x11
#define AUTO 0x001
#define MANUAL 0x11

#include "Modes/ArmMode.h"
#include "Modes/AutonomousMode.h"
#include "Modes/DriveMode.h"

/**
Mode Factory is built to differentiate the frame values 
from the can and return a performable mode that will be used during the next phase
**/
class ModeFactory 
{
public:
    static Mode * buildMode(int frame);
}