#version 330 core

uniform mat4 vpMatrix;
uniform mat3 normalMatrix;
uniform mat4 transform;

uniform vec4 wireframeColor;
uniform bool drawWireframe;

struct MaterialProps
{
    vec4 Oa;
    vec4 Od;
    vec4 Os;
    float s;
};

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

uniform vec3 cameraPosition;
uniform int NL;//<=10
uniform vec4 ambientLight;
uniform LightProps lights[10];
uniform MaterialProps material;

layout(location=0) in vec4 position;
layout(location=1) in vec3 normal;

out vec4 vertexColor;

//compute light color at a point por each light
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


void main()
{
    gl_Position = vpMatrix*transform*position;
    //theres not much work for the fragment shader, so we just use p3.fs
    if(drawWireframe){//in case this object is selected
        vertexColor=wireframeColor;
    }else{

        vertexColor = material.Oa*ambientLight;//computes ambient light
    
        vec3 N = normalize(normalMatrix*normal);

        vec3 P = vec3(position);
        for(int i =0; i<NL; i++){//adds each light contribution to vertex color
            vec4 intensity=getIntensityAtPoint(P, lights[i], N);
            vec3 L = normalize(lights[i].position-P);
            vec3 R = reflect(L, N);
            vec3 V = normalize(P-cameraPosition);
            float diffuse = max(dot(N, L), 0);
            float specular = max(dot(R,V), 0);
            vertexColor+=material.Od*intensity*diffuse+material.Os*intensity*pow(specular, material.s);
        }
    }
}