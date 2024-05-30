#pragma once

#include <atomic>
#include <cstdlib>

class UUID {
   public:
    int Id() const { return id_; }
    void ForceSetId(int id) { id_ = id; }

   private:
    int id_ = rand();
};
