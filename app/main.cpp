#include <GL/glew.h>
#include <GL/glext.h>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Glsl.hpp>

#include "SFML/Graphics.hpp"
#include "renderer/vertex_array.h"
#include "renderer/vertex_buffer.h"
#include "utils/opengl_error.h"
#include "utils/texture.h"

int main() {
    // створення вікна (а отже і OpenGl контексту)
    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML WORK!");

	window.setFramerateLimit(60);

    // ініціалізація glew (має відбуватись після створення контексту!!!)
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        return 1;
    }

#if DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(GLDebugMessageCallback, 0);
#endif

    auto obj = sf::CircleShape(100);

    float positions[]{
        -0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f, 
		-0.5f,  0.5f,
    };

    unsigned int indices[]{0, 1, 2, 2, 3, 0};

    // визначення сорс коду шейдерів
    const std::string vertex_shader =
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}";
    const std::string fragment_shader =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
		"uniform vec4 u_Color;\n" // uniform, атрибут який передаємо з cpu
        "void main()\n"
        "{\n"
        "    FragColor = u_Color;\n"
        "}";

    sf::Shader shader;
    shader.loadFromMemory(vertex_shader, fragment_shader);

	renderer::VertexArray vao;
	renderer::VertexBuffer vbo(GL_ARRAY_BUFFER, positions, sizeof(positions));
	renderer::VertexBuffer ibo(GL_ELEMENT_ARRAY_BUFFER, indices, sizeof(indices));

	renderer::VertexLayout layout;
	layout.Add<float>(2);

	vao.SetBuffer(vbo, ibo, layout);
	vao.Bind();

    bool running = true;
	int r = 0;
	int step = -1;

    while (running) {
        // simple event poll
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                running = false;
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sf::Shader::bind(&shader);
		if (r >= 255 || r <= 0)
			step *= -1;

		r += step;

		shader.setUniform("u_Color", (sf::Glsl::Vec4)sf::Color(r, 30, 10, 255));

		// glUniform4f("u_Color");
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);  // малюємо збінджений буфер

        window.pushGLStates();
        // window.draw(obj);               // не відображається
        window.popGLStates();

        window.display();
    }
}
