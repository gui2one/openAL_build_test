#include <iostream>
#include <AL/alc.h>
#include <AL/al.h>

#include <system_error>

#include "wave/file.h"
#include "openal_debug.h"

struct SOUND_DATA
{
    uint16_t channel_number;
    uint16_t bits_per_sample;
    uint32_t sample_rate;
    std::vector<int16_t> data;
};


SOUND_DATA read_wav_file(char* path)
{
    // read file's content
    SOUND_DATA sound_data;
    wave::File read_file;
    wave::Error err = read_file.Open(path, wave::kIn);
    if (err) {
        std::cout << "Something went wrong in in open" << std::endl;

    }
    std::vector<float> content;
    err = read_file.Read(&content);
    if (err) 
    {
        std::cout << "Something went wrong in read" << std::endl;
    }

    std::vector<int16_t> samples;
    // samples.reserve(content.size());
    for(const auto& float_val : content)
    {
        // std::cout << float_val << std::endl;
        
        samples.push_back((int16_t) (float_val * 32767));
    }

    std::cout << "File Read successfully : "  << path << std::endl;

    sound_data.bits_per_sample = read_file.bits_per_sample();
    sound_data.channel_number = read_file.channel_number();
    sound_data.sample_rate = read_file.sample_rate();
    sound_data.data = samples;
    std::cout << "channel number    : " << (int)sound_data.channel_number << std::endl;
    std::cout << "bits per sample   : " <<  (int)sound_data.bits_per_sample << std::endl;
    std::cout << "number of samples : " << sound_data.data.size() << std::endl;
    
    return sound_data;
}

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

    if(!alcMakeContextCurrent(context))
    {
        std::cout << "ERROR making openAL context currrent" << std::endl;
        
    }

    auto wav = read_wav_file("C:/gui2one/CODE/wasm_orbitals/Resources/Audio/orgue_2_Chords.wav");



    ALenum format;
    if(wav.channel_number == 1 && wav.bits_per_sample == 8)
        format = AL_FORMAT_MONO8;
    else if(wav.channel_number == 1 && wav.bits_per_sample == 16)
        format = AL_FORMAT_MONO16;
    else if(wav.channel_number == 2 && wav.bits_per_sample == 8)
        format = AL_FORMAT_STEREO8;
    else if(wav.channel_number == 2 && wav.bits_per_sample == 16)
        format = AL_FORMAT_STEREO16;
    else
    {
        std::cerr
            << "ERROR: unrecognised wave format: "
            << wav.channel_number << " channels, "
            << wav.bits_per_sample << " bps" << std::endl;
        return 0;
    }


    ALuint buffer_id;

    alGenBuffers(1, &buffer_id);

    alBufferData(buffer_id, format, wav.data.data(), (ALsizei)wav.data.size() * wav.channel_number, (ALsizei)wav.sample_rate);
    wav.data.clear();

    ALuint source_id;
    alGenSources(1, &source_id);
    alGenSources( 1, &source_id);
    alSourcef( source_id, AL_PITCH, 1);
    alSourcef( source_id, AL_GAIN, 0.5f);
    alSource3f( source_id, AL_POSITION, 0, 0, 0);
    alSource3f( source_id, AL_VELOCITY, 0, 0, 0);
    alSourcei( source_id, AL_LOOPING, AL_FALSE);
    alSourcei( source_id, AL_BUFFER, buffer_id);


    alSourcePlay(source_id);

    ALint state = AL_PLAYING;

    

    while(state == AL_PLAYING)
    {
        alGetSourcei(source_id, AL_SOURCE_STATE, &state);
    }

    alDeleteBuffers(1, &buffer_id);
    alDeleteSources(1, &source_id);
    return 0;
}