#pragma once

#include <functional>
#include <iostream>
#include <list>
#include <thread>
#include <unordered_set>

class Tree {
   public:
    void SetParent(Tree* new_parent) {
        new_parent->childs.push_back(this);
        this->parent = new_parent;
    }
    void DeleteChild(Tree* child) {
        child->parent = nullptr;
    }

    template <typename Derived, typename Callable, typename... Args>
    void RunForChilds(Callable&& function, Derived* obj, Args&&... args) {
        if (running_)
            return;
        running_ = true;

        for (auto* child : childs) {
            auto next = dynamic_cast<Derived*>(child);
            if (next != nullptr)
				std::invoke(std::forward<Callable>(function), next, std::forward<Args>(args)...);
        }
        running_ = false;
    }

    template <typename Derived, typename Callable, typename... Args>
    void RunForChilds(Callable&& function, const Derived* obj, Args&&... args) const {
        if (running_)
            return;
        running_ = true;

        for (auto* child : childs) {
            auto next = dynamic_cast<Derived*>(child);
			if (next != nullptr)
				std::invoke(std::forward<Callable>(function), next, std::forward<Args>(args)...);
        }
        running_ = false;
    }

    virtual ~Tree() {
        if (parent)
            parent->DeleteChild(this);

        for (auto& child : childs) { child->parent = nullptr; }
    }

   protected:
    std::list<Tree*> childs;
    Tree* parent = nullptr;
    mutable bool running_ = false;
};
