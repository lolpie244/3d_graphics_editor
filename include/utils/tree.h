#pragma once

#include <functional>
#include <iostream>
#include <list>
#include <thread>
#include <unordered_set>

class Tree {
   public:
    void SetParent(Tree* new_parent) {
        new_parent->childs.insert(this);
        this->parent = new_parent;
    }
    void DeleteChild(Tree* child) {
        childs.erase(child);
        child->parent = nullptr;
    }

    template <typename Derived, typename Callable, typename... Args>
    void RunForChilds(Callable&& function, Derived* obj, Args&&... args) {
        if (this->parent)
            std::invoke(std::forward<Callable>(function), obj, std::forward<Args>(args)...);

        for (auto* child : childs) {
            auto next = dynamic_cast<Derived*>(child);
            if (next != nullptr)
                child->RunForChilds(std::forward<Callable>(function), dynamic_cast<Derived*>(child),
                                    std::forward<Args>(args)...);
        }
    }

    template <typename Derived, typename Callable, typename... Args>
    void RunForChilds(Callable&& function, const Derived* obj, Args&&... args) const {
        if (this->parent)
            std::invoke(std::forward<Callable>(function), obj, std::forward<Args>(args)...);

        for (auto* child : childs) {
            child->RunForChilds(std::forward<Callable>(function), static_cast<Derived*>(child),
                                std::forward<Args>(args)...);
        }
    }

    virtual ~Tree() {
        if (parent)
            parent->DeleteChild(this);

        for (auto& child : childs) { child->parent = nullptr; }
    }

   protected:
    std::unordered_set<Tree*> childs;
    Tree* parent = nullptr;
};
