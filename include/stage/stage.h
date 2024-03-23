#pragma once

#include "events/observer.h"
namespace stage {

class Stage {
   public:
	   virtual void Run();
	   virtual void Stop();
   private:
    events::Observer observer_;
    bool exit_;
};
}  // namespace stage
