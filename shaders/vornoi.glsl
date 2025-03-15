
#version 330 core

// will be set from the C code
#define NUM_POINTS <num-points>


uniform vec2 points[NUM_POINTS];


void main() {
    vec3 colors[7] = {
        vec3(0.8000,0.1411,0.1137),
        vec3(0.5960,0.5921,0.1019),
        vec3(0.8431,0.6000,0.1294),
        vec3(0.2705,0.5215,0.5333),
        vec3(0.6941,0.3843,0.5254),
        vec3(0.4078,0.6156,0.4156),
        vec3(0.6588,0.6000,0.5176),
    };

    vec2 pos = gl_FragCoord.xy;

    int closestIdx = -1;
    float closestDist = 1000.0;

    for (int i = 0; i < NUM_POINTS; i++) {
        float dist = distance(pos, points[i]);
        if (dist < closestDist) {
            closestDist = dist;
            closestIdx = i;
        }
    }

    vec3 color = colors[closestIdx % 7];
    color = mix(color, color / 2.0f, closestDist / (12.8f * 7.2f));
    gl_FragColor = vec4(color, 1.0);
}
