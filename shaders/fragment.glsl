#version 330 core
precision highp float;
out vec4 FragColor;
in vec2 fragPos;

uniform vec2 uC;
uniform int maxIter;

const float PI = 3.14159265359;

vec2 complexPow(vec2 z, float p) {
    if (p == 1) {
      return z;
    }
    // z^p = (r*exp(i*theta))^p = r^p(cos(theta*p) + i sin(theta * p))
    float r = length(z);
    float theta = atan(z.y, z.x);
    float rp = pow(r, p);
    float angle = theta * p;
    return vec2(rp * cos(angle), rp * sin(angle));
}

vec3 palette(float t) {
    vec3 a = vec3(0.5);
    vec3 b = vec3(0.5);
    vec3 c = vec3(1.0);
    vec3 d = vec3(0.0, 0.33, 0.67);
    return a + b * cos(2.0 * PI * (c * t + d));
}

float juliaFunction() {
    vec2 z = fragPos * 2.0;
    int i;
    for (i = 0; i < maxIter; i++) {
        z = complexPow(z, -2) + uC;
        if(dot(z, z) > 4.0) break;
    }

    float t = float(i) + 1.0 - log2(log(length(z)) / log(2.0));
    t = 5 * t / float(maxIter);
    return t;
}

void main() {
    float t = juliaFunction();

    // vec3 rgb = vec3(t/1,t/1,t/1);
    vec3 rgb = palette(t);
    FragColor = vec4(rgb, 1.0);
}
