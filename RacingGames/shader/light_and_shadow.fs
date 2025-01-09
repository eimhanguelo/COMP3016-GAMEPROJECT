#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define MAX_NR_POINT_LIGHTS 4  
uniform PointLight pointLights[MAX_NR_POINT_LIGHTS];
uniform int numPointLight;


uniform vec3 lightDirection;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    projCoords = projCoords * 0.5 + 0.5;
  
    float closestDepth = texture(shadowMap, projCoords.xy).r; 

    float currentDepth = projCoords.z;
    

    vec3 normal = normalize(fs_in.Normal);
    float bias = max(0.001 * (1.0 - dot(normal, lightDirection)), 0.00001);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}


vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    float diff = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = 0.0;

    float distance    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * distance +
                 light.quadratic * (distance * distance));

    vec3 ambient  = light.ambient  * vec3(texture(diffuseTexture, fs_in.TexCoords));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(diffuseTexture, fs_in.TexCoords));
    vec3 specular = light.specular * spec;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

void main()
{
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    vec3 lightColor = vec3(0.8);

    vec3 ambient = 0.3 * color;

    float diff = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = diff * color;  

    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDirection, normal);
    
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDirection + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;

    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
    for(int i = 0; i < numPointLight; i++)
        lighting += CalcPointLight(pointLights[i], normal, fs_in.FragPos, viewDir);
    
    FragColor = vec4(lighting, 1.0);
}
