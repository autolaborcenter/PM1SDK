//
// Created by User on 2019/4/2.
//

#ifndef PM1_SDK_WEAK_SHARED_LOCK_HH
#define PM1_SDK_WEAK_SHARED_LOCK_HH


#include <shared_mutex>

class weak_shared_lock {
public:
	explicit weak_shared_lock(std::shared_mutex &core)
			: core(core), own(core.try_lock_shared()) {}
	
	~weak_shared_lock() {
		if (own) core.unlock_shared();
	}
	
	explicit operator bool() const {
		return own;
	}

private:
	bool              own;
	std::shared_mutex &core;
};


#endif //PM1_SDK_WEAK_SHARED_LOCK_HH