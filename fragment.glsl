#version 330 core
out vec4 FragColor;
in vec2 fragPos;

uniform float uTime;
uniform vec2 uResolution;
uniform vec2 uC;

void main() {
    // Map from [-1,1] quad to complex plane
    vec2 z = fragPos * 2.0; // scale to [-2,2]
    int maxIter = 300;
    int i;
    for(i = 0; i < maxIter; i++) {
        float x = (z.x * z.x - z.y * z.y) + uC.x;
        float y = (2.0 * z.x * z.y) + uC.y;
        z = vec2(x, y);
        if(dot(z, z) > 4.0) break;
    }

    float t = float(i) + 1.0 - log2(log(length(z)) / log(2.0));
    t = 5 * t / float(maxIter);
    FragColor = vec4(t, t, t, 1.0);
}
