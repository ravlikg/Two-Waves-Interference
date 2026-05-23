#version 130

uniform float t;
uniform float amp1;
uniform float freq1;
uniform float phase1;
uniform float amp2;
uniform float freq2;
uniform float phase2;
uniform vec2 source1;
uniform vec2 source2;
uniform int theme;

void main() {
    vec2 pixelPos = gl_FragCoord.xy;

    float r1 = distance(pixelPos, source1);
    float r2 = distance(pixelPos, source2);

    float wave1 = amp1 * sin(r1 * freq1 - t + phase1);
    float wave2 = amp2 * sin(r2 * freq2 - t + phase2);
    float totalWave = wave1 + wave2;

    vec4 finalColor = vec4(0.0, 0.0, 0.0, 1.0);

    if (theme == 0) {
        // 0: Deep Ocean
        float intensity = tanh(totalWave * 0.02);
        if (intensity > 0.0) {
            finalColor.r = intensity * 0.1;
            finalColor.g = intensity * 0.6;
            finalColor.b = intensity * 0.9;
        } else {
            finalColor.r = 0.0;
            finalColor.g = -intensity * 0.1;
            finalColor.b = -intensity * 0.4;
        }
    }
    else if (theme == 1) {
        // 1: Cinematic Monochrome
        float intensity = tanh(abs(totalWave) * 0.03);
        finalColor = vec4(vec3(intensity * 0.85), 1.0);
    }
    else if (theme == 2) {
        // 2: Cyberpunk Magma
        float intensity = tanh(totalWave * 0.025);
        if (intensity > 0.0) {
            finalColor.r = intensity * 0.2;
            finalColor.g = intensity * 0.4;
            finalColor.b = intensity * 0.95;
        } else {
            finalColor.r = -intensity * 0.9;
            finalColor.g = -intensity * 0.25;
            finalColor.b = -intensity * 0.05;
        }
    }
    else if (theme == 3) {
        // 3: Thermal Plasma
        float nWave = tanh(totalWave * 0.02) * 0.5 + 0.5;
        finalColor.r = nWave;
        finalColor.g = sin(nWave * 3.1415);
        finalColor.b = 1.0 - nWave;
        finalColor.a = 1.0;
    }

    gl_FragColor = finalColor;
}