#include "test_stage1.h"

#include <GL/glew.h>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "gui/elements/buttons_list.h"
#include "render/mesh.h"
#include "render/render.h"
#include "stage/stage.h"
#include "utils/vector2.h"

TestStage1::TestStage1() {
    events.push_back(observer_.Bind(sf::Event::KeyPressed, [this](sf::Event event) {
        if (event.key.code == sf::Keyboard::Escape) {
            this->Stop(stage::StageState::Exit);
            return true;
        }
        return false;
    }));

    auto theme = utils::SvgTexture::loadFromFile("resources/theme.svg");
    shader.loadFromFile("shaders/texture_shader.vert", "shaders/texture_shader.frag");
    texture = theme->getElement("g587")->getTexture({28, 42});

    std::vector<render::Mesh::Vertex> vertices{
        {{-0.5, -0.5, 0}, {0, 0}}, {{0.5, -0.5, 0}, {1, 0}}, {{0.5, 0.5, 0}, {1, 1}}, {{-0.5, 0.5, 0}, {0, 1}}};

    std::vector<unsigned int> indices{0, 1, 2, 2, 3, 0};
    mesh = std::make_shared<render::Mesh>(vertices, indices);
	mesh->SetScale(1, 1);
	mesh->SetRotation(-90, render::Mesh::Z);
	mesh->Rotate(80, render::Mesh::X);
}

void TestStage1::Run() {
    PollEvents();

	shader.setUniform("u_Texture", texture);
    sf::Texture::bind(&texture);
    render::GL_render::Instance().Draw(*mesh, shader);
}
