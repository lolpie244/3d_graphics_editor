#include <memory>
#include <vector>
#include "events/event.h"
#include "stage/stage.h"

class TestStage : public stage::Stage {
   public:

	TestStage();
    void Run() override;

	std::vector<std::unique_ptr<events::Event>> events;
};
