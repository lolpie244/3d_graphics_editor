#include <unordered_set>

template<typename Derived>
class Tree {
   public:
    void SetParent(Tree* new_parent) {
        if (new_parent != nullptr)
            new_parent->DeleteChild(this);

        new_parent->AddChild(this);
    }
    void DeleteChild(Tree* child) {
        childs.erase(child);
        child->parent = nullptr;
    }

	virtual ~Tree() {
		for (auto &child : childs) {
			child->parent = nullptr;
		}

		parent->DeleteChild(this);
	}

   private:
    void AddChild(Tree* new_child) { childs.insert(new_child); }

    std::unordered_set<Tree*> childs;
    Tree* parent = nullptr;
};
