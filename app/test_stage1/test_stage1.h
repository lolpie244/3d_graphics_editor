#include <vector>
#include "events/event.h"
#include "stage/stage.h"

class TestStage1 : public stage::Stage {
   public:
	TestStage1();
    void Run() override;

	std::vector<std::unique_ptr<events::Event>> events;
};
