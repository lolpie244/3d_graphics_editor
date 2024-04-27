#pragma once

#include <atomic>

class UUID {
   public:
    int Id() const { return id_; }

   private:
    int id_ = max_object_id_++;

    static std::atomic<int> max_object_id_;
};

inline std::atomic<int> UUID::max_object_id_{1};
