#include "test_stage1.h"

#include <GL/glew.h>

#include "data/model_loader.h"
#include "math/points_cast.h"

TestStage1::TestStage1() {
    events.push_back(observer_.Bind(sf::Event::KeyPressed, [this](sf::Event event) {
        if (event.key.code == sf::Keyboard::Escape) {
            this->Stop(stage::StageState::Exit);
            return true;
        }
        return false;
    }));

    auto theme = data::SvgTexture::loadFromFile("resources/theme.svg");
    shader.loadFromFile("shaders/texture.vert", "shaders/texture.frag");
    point_shader.loadFromFile("shaders/texture.vert", "shaders/point.frag");
    picking_shader.loadFromFile("shaders/texture.vert", "shaders/picking.frag");
    gizmo_shader.loadFromFile("shaders/gizmo.vert", "shaders/gizmo.frag");
    gizmo_picking.loadFromFile("shaders/gizmo.vert", "shaders/picking.frag");

    texture = theme->getElement("g546")->getTexture({400, 400});

    background = std::make_unique<gui::SpriteGuiElement>(glm::vec3{600, 600, -1}, math::Vector2f{1200, 1200});
    background->SetTexture(theme->getElement("g71"));

    opengl_context = std::make_unique<gui::OpenglContext>();
    opengl_context->SetLeftCorner(50, 0);
    opengl_context->Resize(math::Vector2f(1700, 1080));
    opengl_context->SetScaleMethod<events::DefaultScale>();
    opengl_context->BindScale(observer_);

    camera_->SetOrigin(0, 0, 0);

	opengl_context->BindPress(observer_, [this](sf::Event event){
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            this->pixel = picking.ReadPixel(event.mouseButton.x, 800 - event.mouseButton.y);
            return true;
        }
		return false;
	});

    opengl_context->BindDrag(observer_, [this](sf::Event event, math::Vector2f moved) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			auto move = math::to_ndc(stage::StageManager::Instance().windowSize() / 2.0f + moved);
            stage::StageManager::Instance().Camera()->Move(-move.x, move.y);
            return true;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
            stage::StageManager::Instance().Camera()->Rotate(moved.x * 0.5, math::Y);
            stage::StageManager::Instance().Camera()->Rotate(-moved.y * 0.5, math::X);
            return true;
        }
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->pixel.ObjectID) {
			auto old = model->Vertex(this->pixel.VertexId);
			auto coord = math::to_world_coords(stage::StageManager::Instance().windowSize() / 2.0f + moved);
			model->SetVertexPosition(this->pixel.VertexId, old.position + coord * 4.0f);
			return true;
			// auto scale = coord * math::to_vector(pixel.VertexId);
			// std::cout << scale.x << ' ' << scale.y << ' ' << scale.z << '\n';
			// std::cout << math::to_vector(pixel.VertexId).x << ' ' << math::to_vector(pixel.VertexId).y << ' ' << math::to_vector(pixel.VertexId).z << '\n';
			//
			// model->Move(scale.x, scale.y, scale.z);
		}

        return false;
    });

    opengl_context->BindScroll(observer_, [this](sf::Event event) {
        stage::StageManager::Instance().Camera()->Move(0, 0, -event.mouseWheelScroll.delta);
        return true;
    });

    camera_->Move(0, 0, 3.0f);
    camera_->Rotate(-40, math::X);
    ///////////////////////////////////////////
    model = data::loadModel("resources/cube.obj", render::MeshChange::Enable);

    model->Scale(0.5, 0.5, 0.5);
    model->texture = data::PngTexture::loadFromFile("resources/cube.png")->getTexture({0, 0});
	///////////////////////////////////////////
	gizmo = data::loadGizmo("resources/gizmo/cube.obj");
}

void TestStage1::Run() {
    // window_->pushGLStates();
    // window_->draw(*background);
    // window_->popGLStates();

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		picking.Bind();
		model->DrawPoints(picking_shader);
		gizmo->Draw(gizmo_picking, model.get());
		picking.Unbind();
    }
    PollEvents();
    model->Draw(shader);
	gizmo->Draw(gizmo_shader, model.get());
    FrameEnd();
}
