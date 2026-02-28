#version 330

in vec3 fragPosition;
in vec3 fragNormal;
in vec2 fragTexCoord;

out vec4 finalColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec4 ambient;
uniform vec3 viewPos;

// Структура света (должна совпадать с rlights.h)
#define MAX_LIGHTS 4

struct Light {
    int enabled;
    int type;
    vec3 position;
    vec3 target;
    vec4 color;
};

uniform Light lights[MAX_LIGHTS];

void main()
{
    vec4 texelColor = texture(texture0, fragTexCoord);
    vec3 normal = normalize(fragNormal);
    vec3 viewDir = normalize(viewPos - fragPosition);
    
    vec3 lightResult = vec3(0.0);
    
    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        if (lights[i].enabled == 1)
        {
            vec3 lightDir = vec3(0.0);
            
            if (lights[i].type == 0) // Точечный свет
            {
                lightDir = normalize(lights[i].position - fragPosition);
            }
            else if (lights[i].type == 1) // Направленный свет
            {
                lightDir = normalize(-lights[i].target); // или position->target
            }
            
            // Диффузное освещение (основной свет)
            float diff = max(dot(normal, lightDir), 0.0);
            vec3 diffuse = lights[i].color.rgb * diff;
            
            // Блики
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
            vec3 specular = lights[i].color.rgb * spec;
            
            lightResult += diffuse + specular;
        }
    }
    
    // Добавляем фоновый свет + результат от лампочек
    vec3 final = (ambient.rgb + lightResult) * texelColor.rgb * colDiffuse.rgb;
    
    finalColor = vec4(final, texelColor.a * colDiffuse.a);
}