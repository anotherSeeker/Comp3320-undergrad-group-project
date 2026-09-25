//#version 460 core
#version 300 es

in vec3 aPos;
in float currentTime;

struct waveData 
{
    float frequency;
    float amplitude;
    float speed;
    vec2 direction;
    float sharpness; //used for steep sine
}

vec3 sumOfSines(vec3 pos)
{
    
}

float WaveSine(vec3 pos, float time, waveData w)
{
    //we only care about the xz of the world position
    vec2 xz = pos.xz;

    //based on https://developer.nvidia.com/gpugems/gpugems/part-i-natural-effects/chapter-1-effective-water-simulation-physical-models
    //wavelength = 2/frequency
    float wavelength = 2/w.frequency;

    //speed expressed as a phase. S * 2/wavelength, where S is the distance traveled by the wave in one second
    float phase = w.speed * (2/frequency);

    float height = w.amplitude * sin(dot(w.direction, xz) * wavelength + time * phase); 

    return height;
}

float WaveSteepSine(vec3 pos, float time, waveData w)
{
    //we only care about the xz of the world position
    vec2 xz = pos.xz;

    //based on https://developer.nvidia.com/gpugems/gpugems/part-i-natural-effects/chapter-1-effective-water-simulation-physical-models
    //wavelength = 2/frequency
    float wavelength = 2/w.frequency;

    //speed expressed as a phase. S * 2/wavelength, where S is the distance traveled by the wave in one second
    float phase = w.speed * (2/frequency);

    //2*amplitude * ( (sin(direction dot position) * wavelength + phase) + 1)/2 ^ steepness
    float wave = 2*w.amplitude * pow( (sin(dot(w.direction, xz) * wavelength + time * phase) + 1) /2, steepness); 



}

