#version 330 core

struct LightProps
{
    int type;
    vec4 color;
    vec3 position;
    vec3 direction;
    float falloff;
    float spotlightAngleRadians;
    float radialFalloff;
};

struct MaterialProps
{
    vec4 Oa;
    vec4 Od;
    vec4 Os;
    float s;
};

uniform vec3 cameraPosition;
uniform vec4 ambientLight;
uniform int NL;//<=10
uniform LightProps lights[10];
uniform MaterialProps material;

in vec3 vertexPosition;
in vec3 vertexNormal;

out vec4 fragmentColor;

vec4 getIntensityAtPoint(vec3 point, LightProps light, vec3 normal) {
    vec4 intensity;
    if (light.type == 0) {//directional
        return light.color*max(dot(light.direction, -normal), 0);
    }else if (light.type == 1) {//point
        //ifs to avoid heavy calculations in some cases
        float distance = length(point - light.position); 
        if (light.falloff == 0) {
            return light.color;
        }
        else if (light.falloff == 1) {
            return light.color * (1/distance);
        }
        else {
            return light.color * pow(distance, -light.falloff);
        }
    }else if (light.type == 2) {//spot
        //ifs to avoid heavy calculations in some cases
        vec3 distanceVector = light.position - point;
        float projectionLenght = dot(-distanceVector, light.direction);
        float psi = acos(projectionLenght/length(distanceVector));
        if (psi > light.spotlightAngleRadians) {
            return vec4(0, 0, 0, 0);
        }
        if (light.falloff == 0) {
            return light.color * pow(projectionLenght/length(distanceVector), light.radialFalloff);
        }
        else if (light.falloff == 1) {
            return light.color * (1 / length(distanceVector)) * pow(projectionLenght/length(distanceVector), light.radialFalloff);
        }
        else {
            return light.color * pow(length(distanceVector), -light.falloff) * pow(projectionLenght/length(distanceVector), light.radialFalloff);
        }
    }
}

vec4 phong(vec3 P, vec3 N)
{
    vec4 color = material.Oa*ambientLight;
    
    for(int i =0; i<NL; i++){
        vec4 intensity=getIntensityAtPoint(P, lights[i], N);
        vec3 L = normalize(lights[i].position-P);
        vec3 R = reflect(L, N);
        vec3 V = normalize(P-cameraPosition);
        float diffuse = max(dot(N, L), 0);
        float specular = max(dot(R,V), 0);
        color+=material.Od*intensity*diffuse+material.Os*intensity*pow(specular, material.s);
    }
    return color;
}

void main()
{
    fragmentColor = phong(vertexPosition, normalize(vertexNormal));
}
