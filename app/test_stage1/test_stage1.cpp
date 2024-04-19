#include "test_stage1.h"

#include <GL/glew.h>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "data/mesh.h"
#include "data/texture.h"
#include "events/propterties/scaleable.h"
#include "gui/opengl_context.h"
#include "gui/sprite_element.h"
#include "math/points_cast.h"
#include "math/transform.h"
#include "math/vector2.h"
#include "render/mesh.h"
#include "render/renderer.h"
#include "stage/stage.h"
#include "stage/stage_manager.h"

TestStage1::TestStage1() {
    events.push_back(observer_.Bind(sf::Event::KeyPressed, [this](sf::Event event) {
        if (event.key.code == sf::Keyboard::Escape) {
            this->Stop(stage::StageState::Exit);
            return true;
        }
        return false;
    }));

    auto theme = data::SvgTexture::loadFromFile("resources/theme.svg");
    shader.loadFromFile("shaders/texture_shader.vert", "shaders/texture_shader.frag");
	point_shader.loadFromFile("shaders/texture_shader.vert", "shaders/point_shader.frag");
    texture = theme->getElement("g546")->getTexture({400, 400});

	background = std::make_unique<gui::SpriteGuiElement>(glm::vec3{600, 600, -1}, math::Vector2f{1200, 1200});
	background->SetTexture(theme->getElement("g71"));

	opengl_context = std::make_unique<gui::OpenglContext>();
	opengl_context->SetLeftCorner(50, 0);
	opengl_context->Resize(math::Vector2f(1700, 1080));
	opengl_context->SetScaleMethod<events::DefaultScale>();
	opengl_context->BindScale(observer_);

    mesh2 = data::Mesh::loadFromFile("resources/plane.obj");

    mesh2->Move(0, 0, -2.0f);
    mesh2->Scale(0.5, 0.5, 0.5);
	mesh2->texture = texture;
    mesh2->Rotate(90, math::Transform::X);


	opengl_context->BindDrag(observer_, [this](sf::Event event, math::Vector2f moved){
		auto move = math::to_ndc(stage::StageManager::Instance().windowSize() / 2.0f + moved);
        stage::StageManager::Instance().Camera()->Move(move.x, -1 * move.y);
		return true;
	});
}

void TestStage1::Run() {
    PollEvents();
    // mesh2->Rotate(1, math::Transform::Y);

	window_->pushGLStates();
    window_->draw(*background);
    window_->popGLStates();


	point_shader.setUniform("u_Color", sf::Color::Black);
    render::GL_render::Instance().Draw(*mesh2, point_shader, GL_POINTS);
    render::GL_render::Instance().Draw(*mesh2, shader);
}
