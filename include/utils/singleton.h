#pragma once

template <typename Derived>
class Singleton {
   public:
    virtual ~Singleton() = default;

    static Derived& Instance() {
        static Derived instance;
        return instance;
    }

    Singleton& operator=(const Singleton&) = delete;
    Singleton(const Singleton&) = delete;

   protected:
    Singleton() = default;
};
