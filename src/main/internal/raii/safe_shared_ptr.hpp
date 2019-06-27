﻿//
// Created by User on 2019/4/10.
//

#ifndef PM1_SDK_SAFE_SHARED_PTR_HPP
#define PM1_SDK_SAFE_SHARED_PTR_HPP


#include <memory>
#include <functional>
#include <shared_mutex>
#include "weak_shared_lock.hpp"

template<class t>
class safe_shared_ptr {
    std::shared_ptr<t> ptr;
    std::shared_mutex  mutex;

public:
    using ptr_t = decltype(ptr);
    
    inline ptr_t operator()(ptr_t new_ptr) {
        std::unique_lock<decltype(mutex)> _(mutex);
        ptr.swap(new_ptr);
        return new_ptr;
    }
    
    template<class _t>
    inline _t read(const std::function<_t(ptr_t)> &block) {
        weak_shared_lock lock(mutex);
        if (!lock)
            throw std::logic_error("pointer is busy");
        if (!ptr)
            throw std::logic_error("pointer is null");
        
        return block(ptr);
    }
};


#endif //PM1_SDK_SAFE_SHARED_PTR_HPP