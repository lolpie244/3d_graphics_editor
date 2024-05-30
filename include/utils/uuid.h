#pragma once

#include <atomic>
#include <cstdint>
#include <cstdlib>
#include <limits>

class UUID {
   public:
    uint8_t Id() const { return id_; }
    void ForceSetId(uint8_t id) { id_ = id; }

   private:
    uint8_t id_ = rand() % std::numeric_limits<uint8_t>::max();
};
