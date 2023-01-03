#version 330 core

struct Light {
    vec3  position;
    vec3  direction;
    vec3  ambient;
    vec3  diffuse;
    vec3  specular;
    float innerCutOff;
    float outerCutOff;
    float c0;
    float c1;
    float c2;
};

struct Material {
    vec4  ambient;
    vec4  diffuse;
    vec4  specular;
    vec4  emissive;
    float shininess;
};

struct Textures {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emissive;
    sampler2D normal;
    float     shininess;
};

#define NLD 1
#define NLP 1
#define NLF 2

uniform Light    ulightG;
uniform Light    ulightD[NLD];
uniform Light    ulightP[NLP];
uniform Light    ulightF[NLF];
uniform Material umaterial;
uniform Textures utextures;
uniform bool     uWithMaterials;
uniform vec3     ueye;

in  vec3 vnor;
in  vec3 vpos;
in  vec2 vtex;

out vec4 outColor;

vec3 funDirectional(Light light, Material material, vec3 N, vec3 V);
vec3 funPositional (Light light, Material material, vec3 N, vec3 V);
vec3 funFocal      (Light light, Material material, vec3 N, vec3 V);

void main() {

    vec3 N = normalize(vnor);
    vec3 V = normalize(ueye - vpos);

    Material material;

    if(uWithMaterials) {
        material.ambient   = umaterial.ambient;
        material.diffuse   = umaterial.diffuse;
        material.specular  = umaterial.specular;
        material.emissive  = umaterial.emissive;
        material.shininess = umaterial.shininess;
    }
    else {
        material.ambient   = texture(utextures.diffuse ,vtex)*0.3;
        material.diffuse   = texture(utextures.diffuse ,vtex);
        material.specular  = texture(utextures.specular,vtex);
        material.emissive  = texture(utextures.emissive,vtex);
        material.shininess = utextures.shininess;
    }

    vec3 color = material.emissive.rgb + ulightG.ambient * material.ambient.rgb;
    for(int i=0; i<NLD; i++) color += funDirectional(ulightD[i],material,N,V);
    for(int i=0; i<NLP; i++) color += funPositional (ulightP[i],material,N,V);
    for(int i=0; i<NLF; i++) color += funFocal      (ulightF[i],material,N,V);

    outColor = vec4(color, material.diffuse.a);

}

vec3 funDirectional(Light light, Material material, vec3 N, vec3 V) {

    vec3  L = normalize(-light.direction);
    vec3  R = normalize(reflect(-L,N));

    float dotLN = dot(L,N);
    float dotRV = 0.0;
    if(dotLN<0.0) dotLN = 0.0;
    else          dotRV = max(dot(R,V), 0.0);

    vec3  ambient  = light.ambient  * material.ambient.rgb;
    vec3  diffuse  = light.diffuse  * material.diffuse.rgb  * dotLN;
    vec3  specular = light.specular * material.specular.rgb * pow(dotRV,material.shininess);

    vec3 color = ambient + diffuse + specular;

    return color;

}

vec3 funPositional(Light light, Material material, vec3 N, vec3 V) {

    vec3  L = normalize(light.position - vpos);
    vec3  R = normalize(reflect(-L,N));

    float dotLN = dot(L,N);
    float dotRV = 0.0;
    if(dotLN<0.0) dotLN = 0.0;
    else          dotRV = max(dot(R,V), 0.0);

    vec3  ambient  = light.ambient  * material.ambient.rgb;
    vec3  diffuse  = light.diffuse  * material.diffuse.rgb  * dotLN;
    vec3  specular = light.specular * material.specular.rgb * pow(dotRV,material.shininess);

    float distance    = length(light.position - vpos);
    float attenuation = 1.0/(light.c0 + light.c1*distance + light.c2*pow(distance,2));

    vec3 color = attenuation*(ambient + diffuse + specular);

    return color;

}

vec3 funFocal(Light light, Material material, vec3 N, vec3 V) {

    vec3  L = normalize(light.position - vpos);
    vec3  R = normalize(reflect(-L,N));

    float dotLN = dot(L,N);
    float dotRV = 0.0;
    if(dotLN<0.0) dotLN = 0.0;
    else          dotRV = max(dot(R,V), 0.0);

    vec3  ambient  = light.ambient  * material.ambient.rgb;
    vec3  diffuse  = light.diffuse  * material.diffuse.rgb  * dotLN;
    vec3  specular = light.specular * material.specular.rgb * pow(dotRV,material.shininess);

    float distance    = length(light.position - vpos);
    float attenuation = 1.0/(light.c0 + light.c1*distance + light.c2*pow(distance,2));

    vec3  direction   = normalize(-light.direction);
    float intensity   = clamp((light.outerCutOff - dot(L,direction))/(light.outerCutOff - light.innerCutOff), 0.0, 1.0);

    vec3 color = attenuation*(ambient + intensity*(diffuse + specular));

    return color;

}