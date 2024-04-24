#include <memory>
#include <vector>
#include "events/event.h"
#include "stage/stage.h"

class MenuStage : public stage::Stage {
   public:
	MenuStage();
    void Run() override;
};
