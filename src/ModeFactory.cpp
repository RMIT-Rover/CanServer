#include "ModeFactory.h"
ModeFactory::buildMode(int frame_type, frame)
{
    if(frame == ARM)
    {
        return new ArmMode(frame)
    }
    if(frame == AUTO)
    {
        return new AutonomousMode(frame)
    }
    if(frame == MANUAL)
    {
        return new DriveMode(frame)
    }
}