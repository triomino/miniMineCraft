#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    bool Enable;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    bool Enable;
};

#define NR_POINT_LIGHTS 4

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 color;

//uniform int 1;
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material[3];

uniform int faceFlag;
uniform int Choosed;

// Function prototypes
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
    
    // result += CalcSpotLight(spotLight, norm, FragPos, viewDir, 1);    
    
    if (Choosed == 1) {
        result /= 2.0f;
    }
    if (Choosed == 2){
        result = (result + vec3(1.0f)) / 2.0f;
    }
    color = vec4(result, 1.0f);
}

// Calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // Specular shading
    float spec;
    bool blinn;
    blinn = true;
    if (faceFlag == 1){
        if (blinn){
            vec3 halfwayDir = normalize(lightDir + viewDir);  
            spec = pow(max(dot(normal, halfwayDir), 0.0), material[1].shininess);
        }
        else {
            vec3 reflectDir = reflect(-lightDir, normal);
            spec = pow(max(dot(viewDir, reflectDir), 0.0), material[1].shininess);
        }
        
        // Combine results
        vec3 ambient = light.ambient * vec3(texture(material[1].diffuse, TexCoords));
        vec3 diffuse = light.diffuse * diff * vec3(texture(material[1].diffuse, TexCoords));
        vec3 specular = light.specular * spec * vec3(texture(material[1].specular, TexCoords));
        return (ambient + diffuse + specular);
    }
    else if (faceFlag == 2){
        if (blinn){
            vec3 halfwayDir = normalize(lightDir + viewDir);  
            spec = pow(max(dot(normal, halfwayDir), 0.0), material[2].shininess);
        }
        else {
            vec3 reflectDir = reflect(-lightDir, normal);
            spec = pow(max(dot(viewDir, reflectDir), 0.0), material[2].shininess);
        }
        
        // Combine results
        vec3 ambient = light.ambient * vec3(texture(material[2].diffuse, TexCoords));
        vec3 diffuse = light.diffuse * diff * vec3(texture(material[2].diffuse, TexCoords));
        vec3 specular = light.specular * spec * vec3(texture(material[2].specular, TexCoords));
        return (ambient + diffuse + specular);
    }
    else {
        if (blinn){
            vec3 halfwayDir = normalize(lightDir + viewDir);  
            spec = pow(max(dot(normal, halfwayDir), 0.0), material[0].shininess);
        }
        else {
            vec3 reflectDir = reflect(-lightDir, normal);
            spec = pow(max(dot(viewDir, reflectDir), 0.0), material[0].shininess);
        }
        
        // Combine results
        vec3 ambient = light.ambient * vec3(texture(material[0].diffuse, TexCoords));
        vec3 diffuse = light.diffuse * diff * vec3(texture(material[0].diffuse, TexCoords));
        vec3 specular = light.specular * spec * vec3(texture(material[0].specular, TexCoords));
        return (ambient + diffuse + specular);
    }
    
}

// Calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 result = vec3(0.0f, 0.0f, 0.0f);
    if (light.Enable){
        if (faceFlag == 0){
            vec3 lightDir = normalize(light.position - fragPos);
            // Diffuse shading
            float diff = max(dot(normal, lightDir), 0.0);
            // Specular shading
            float spec;
            bool blinn = true;
            if (blinn){
                vec3 halfwayDir = normalize(lightDir + viewDir);  
                spec = pow(max(dot(normal, halfwayDir), 0.0), material[0].shininess);
            }
            else {
                vec3 reflectDir = reflect(-lightDir, normal);
                spec = pow(max(dot(viewDir, reflectDir), 0.0), material[0].shininess);
            }        
            // Attenuation
            float distance = length(light.position - fragPos);
            float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
            // Combine results
            vec3 ambient = light.ambient * vec3(texture(material[0].diffuse, TexCoords));
            vec3 diffuse = light.diffuse * diff * vec3(texture(material[0].diffuse, TexCoords));
            vec3 specular = light.specular * spec * vec3(texture(material[0].specular, TexCoords));
            ambient *= attenuation;
            diffuse *= attenuation;
            specular *= attenuation;
            result = ambient + diffuse + specular;

        }
        else if (faceFlag == 1){
            vec3 lightDir = normalize(light.position - fragPos);
            // Diffuse shading
            float diff = max(dot(normal, lightDir), 0.0);
            // Specular shading
            float spec;
            bool blinn = true;
            if (blinn){
                vec3 halfwayDir = normalize(lightDir + viewDir);  
                spec = pow(max(dot(normal, halfwayDir), 0.0), material[1].shininess);
            }
            else {
                vec3 reflectDir = reflect(-lightDir, normal);
                spec = pow(max(dot(viewDir, reflectDir), 0.0), material[1].shininess);
            }        
            // Attenuation
            float distance = length(light.position - fragPos);
            float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
            // Combine results
            vec3 ambient = light.ambient * vec3(texture(material[1].diffuse, TexCoords));
            vec3 diffuse = light.diffuse * diff * vec3(texture(material[1].diffuse, TexCoords));
            vec3 specular = light.specular * spec * vec3(texture(material[1].specular, TexCoords));
            ambient *= attenuation;
            diffuse *= attenuation;
            specular *= attenuation;
            result = ambient + diffuse + specular;

        }
        else {
            vec3 lightDir = normalize(light.position - fragPos);
            // Diffuse shading
            float diff = max(dot(normal, lightDir), 0.0);
            // Specular shading
            float spec;
            bool blinn = true;
            if (blinn){
                vec3 halfwayDir = normalize(lightDir + viewDir);  
                spec = pow(max(dot(normal, halfwayDir), 0.0), material[2].shininess);
            }
            else {
                vec3 reflectDir = reflect(-lightDir, normal);
                spec = pow(max(dot(viewDir, reflectDir), 0.0), material[2].shininess);
            }        
            // Attenuation
            float distance = length(light.position - fragPos);
            float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
            // Combine results
            vec3 ambient = light.ambient * vec3(texture(material[2].diffuse, TexCoords));
            vec3 diffuse = light.diffuse * diff * vec3(texture(material[2].diffuse, TexCoords));
            vec3 specular = light.specular * spec * vec3(texture(material[2].specular, TexCoords));
            ambient *= attenuation;
            diffuse *= attenuation;
            specular *= attenuation;
            result = ambient + diffuse + specular;
        }
    }
    return result;
}