#version 330 core

#define MAX_LIGHTS_NUM 8

struct Light
{
    vec3 position;

	vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform int lightsNumber;
uniform Light lights[MAX_LIGHTS_NUM];
uniform vec3 viewPosition;

out vec4 FragColor;

in VS_OUT
{
    vec3 position;
    vec3 normal;
	vec2 textCoord;
} vertex;

uniform sampler2D u_Texture;

vec3 calculatePointLight(Light light, vec3 vertexPosition, vec3 vertexNormal, vec3 viewDirection)
{
    vec3 ambient = light.ambient;

	// diffuse
	vec3 lightDirection = normalize(vec3(light.position) - vertexPosition);
	float diffuseStength = max(0.0, dot(lightDirection, vertexNormal));
    vec3 diffuse = light.color * diffuseStength;

	// specular
    vec3 reflectDirection = reflect(-lightDirection, normalize(vertexNormal));
    float specularStength = max(0.0, dot(viewDirection, reflectDirection));
    vec3 specular = light.specular * pow(specularStength, 32.0);

    return (ambient + diffuse + specular) * light.color;
}

void main()
{
	vec4 texColor = texture(u_Texture, vertex.textCoord);
	vec3 viewDirection = normalize(viewPosition - vertex.position);

	vec3 lighting = vec3(0.0f);
	if (lightsNumber == 0) {
		lighting = vec3(1.0f);
	}

    for (int i = 0; i < lightsNumber; ++i)
    {
        lighting += calculatePointLight(lights[i], vertex.position, vertex.normal, viewDirection);
    }

	FragColor = texColor * vec4(lighting, 1.0f);
}

