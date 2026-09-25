#version 460 core

in vec3 aPos;

struct waveData 
{

}

vec3 sumOfSines(vec3 worldPos)
{
    
}

float WaveSine(vec3 pos, float time, waveData w)
{
    //we only care about the xz of the world position
    vec2 xz = pos.xz;

    //based on https://developer.nvidia.com/gpugems/gpugems/part-i-natural-effects/chapter-1-effective-water-simulation-physical-models
    //Amplitude * sin(direction . position * frequency + time * speed)  
    float frequency = 2/w.wavelength;
    //speed expressed as phase. S * 2/wavelength, where S is the distance traveled by the wave in one second
    float phase = w.speed * (2/frequency);

    float offset = w.amplitude * sin(dot(w.direction, xz) * frequency + time * phase); 

    return offset;
}

