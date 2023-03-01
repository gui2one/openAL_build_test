#include <iostream>
#include <AL/alc.h>
#include <AL/al.h>
int main(int argc, char** argv)
{
    std::cout << "OpenAL Test" << std::endl;
    
    ALCdevice* device;
    ALCcontext* context;

    device = alcOpenDevice(NULL);

    if(!device)
    {
        throw("failed to get device" );
    }

    context = alcCreateContext(device,nullptr);

    if(!context)
    {
        throw("Faild to create Sound Context");
    }

    if(!alcMakeContextCurrent(context))
    {
        throw("Faild to make contet current");
    }

    const ALchar* name = nullptr;
    if(alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
    {
        name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
    }

    if(!name || alcGetError(device) != AL_NO_ERROR)
    {
        name = alcGetString(device, ALC_DEVICE_SPECIFIER);
    }

    std::cout << "Device and Context Created :\n\t" << name<< std::endl;    
    return 0;
}