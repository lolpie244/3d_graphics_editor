#pragma once

#include <atomic>
#include <cstdint>
#include <cstdlib>
#include <limits>

class UUID {
   public:
    int Id() const { return id_; }
    void ForceSetId(int id) { id_ = id; }

   private:
    int id_ = rand() % std::numeric_limits<uint8_t>::max();
};
