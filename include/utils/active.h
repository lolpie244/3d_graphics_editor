#pragma once

class Activatable {
   public:
	virtual ~Activatable() = default;

    virtual void Enable() { is_active_ = true; }
    virtual void Disable() { is_active_ = false; }
    virtual bool IsActive() const { return is_active_; }

   protected:
    bool is_active_ = true;
};

#define ReturnOnDisable(x) \
	if (!IsActive()) \
		return x;
