#pragma once
#include <functional>
#include <memory>
#include <vector>

namespace cui
{

enum class CallbackResult
{
    Keep, Erase
};

struct Oneshot { explicit Oneshot() = default; };

enum class Nshot : size_t {};

namespace details
{

template<class Self>
std::shared_ptr<Self> lockIfWeak(std::weak_ptr<Self> const& self)
{
    return self.lock();
}

template<class Self>
Self const& lockIfWeak(Self const& self)
{
    return self;
}

template<class Self, class MemberFn>
auto bind(Self self, MemberFn memFn)
{
    return [self = std::move(self), memFn](auto&&... t) {
        auto const& ptr = lockIfWeak(self);
        if (ptr == nullptr) {
            return CallbackResult::Erase;
        }
        ((*ptr).*(memFn))(std::forward(t)...);
        return CallbackResult::Keep;
        };
}

template<class Self, class MemberFn>
auto bind(Self self, MemberFn memFn, Oneshot)
{
    return [self = std::move(self), memFn](auto&&... t) {
        auto const& ptr = lockIfWeak(self);
        if (ptr == nullptr) {
            return CallbackResult::Erase;
        }
        ((*ptr).*(memFn))(std::forward(t)...);
        return CallbackResult::Erase;
        };
}

template<class Self, class MemberFn>
auto bind(Self self, MemberFn memFn, Nshot n)
{
    return [self = std::move(self), memFn, n = static_cast<size_t>(n)](auto&&... t) mutable {
        if (n == 0) {
            return CallbackResult::Erase;
        }
        auto const& ptr = lockIfWeak(self);
        if (ptr == nullptr) {
            return CallbackResult::Erase;
        }
        ((*ptr).*(memFn))(std::forward(t)...);
        --n;
        return CallbackResult::Keep;
        };
}

}

template<class... T>
class Signal
{
public:
    using Functor = std::function<CallbackResult(T...)>;

    template<std::invocable Func>
    void connect(Func callback)
    {
        if constexpr (std::is_invocable_r_v<CallbackResult, Func, T...>) {
            callbacks.push_back(std::move(callback));
        }
        else {
            callbacks.push_back([callback = std::move(callback)](auto... t) mutable {
                callback(std::forward(t)...);
                return CallbackResult::Keep;
                });
        }
    }

    template<class Self, class MemberFn, class... Tag>
    void connect(Self self, MemberFn memFn, Tag... tag)
    {
        callbacks.push_back(details::bind(std::move(self), memFn, tag...));
    }

    void emit(T&&... t)
    {
        for (auto it = callbacks.begin(); it != callbacks.end();) {
            auto result = (*it)(t...);
            if (result == CallbackResult::Erase) {
                it = callbacks.erase(it);
            }
            else {
                ++it;
            }
        }
    }

private:
    std::vector<Functor> callbacks;
};

}