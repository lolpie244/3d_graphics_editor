#pragma once

#include <atomic>
#include <cstdlib>

class UUID {
   public:
    int Id() const { return id_; }

   private:
    int id_ = rand();
};
