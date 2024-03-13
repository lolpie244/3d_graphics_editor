#pragma once
#include "stage/stage.h"
#include <stack>

namespace stage {
	class StageManager {
		void NextStage(Stage& new_stage);
		void PreviousStage();
	private:
		std::stack<Stage> stages;
	};
}
