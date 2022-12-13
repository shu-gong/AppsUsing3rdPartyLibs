#version 330 core
out vec4 FragColor;


struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform Light light;

void main()
{    
    //light.ambient = 10.0;
    // ambient
    vec3 ambient = light.ambient * texture(texture_diffuse1, TexCoords).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(texture_diffuse1, TexCoords).rgb;  
    
    vec3 result = ambient + diffuse;

    FragColor = vec4(result, 1.0);
}
