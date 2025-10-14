#include <initializer_list>
#include <iostream>
#include <utility>

namespace my
{
#define TEST

#ifndef TEST
#define DEBUG_MSG
#else
#ifdef _WIN32
#define DEBUG_MSG std::clog << __FUNCTION__ << "\n";
#else
#define DEBUG_MSG std::clog << __PRETTY_FUNCTION__ << "\n";
#endif
#endif
constexpr struct nullopt_t
{
} nullopt;

template <typename _T>
class optional final
{
private:
    _T* val;

public:
    optional(nullopt_t = nullopt) : val{nullptr} { DEBUG_MSG };
    optional(_T&& val);
    template <typename... _Args>
    optional(_Args&&... args);
    optional(const optional<_T>& other)
        : val{new _T(std::forward<_T>(other.value()))} {
              DEBUG_MSG
          };
    optional(optional<_T>&& other) noexcept { DEBUG_MSG swap(other); };

    optional<_T>& operator=(const optional<_T>& other);
    optional<_T>&& operator=(optional<_T>&& other) noexcept;

    _T& operator=(const _T& value);
    _T&& operator=(_T&& value) noexcept;

    ~optional() noexcept { DEBUG_MSG reset(); };

    bool has_value() const noexcept { DEBUG_MSG return val; }
    operator bool() const noexcept { DEBUG_MSG return has_value(); }

    _T& value() & noexcept { DEBUG_MSG return *val; }
    const _T& value() const& noexcept { DEBUG_MSG return *val; }

    _T&& value() && noexcept { DEBUG_MSG return *val; }
    const _T&& value() const&& noexcept { DEBUG_MSG return *val; }

    _T& operator*() & noexcept { DEBUG_MSG return value(); }
    const _T& operator*() const& noexcept { DEBUG_MSG return value(); }

    _T&& operator*() && noexcept { DEBUG_MSG return value(); }
    const _T&& operator*() const&& noexcept { DEBUG_MSG return value(); }

    _T* operator->() noexcept { DEBUG_MSG return val; }
    const _T* operator->() const noexcept { DEBUG_MSG return val; }

    void reset() noexcept;
    void swap(optional& other) & noexcept
    {
        DEBUG_MSG std::swap(this->val, other.val);
    }
    void swap(optional&& other) && noexcept
    {
        DEBUG_MSG std::swap(this->val, other.val);
    }

    template <typename... _Args>
    void emplace(_Args&&... args);
};

template <typename _T>
optional<_T>::optional(_T&& val)
{
    DEBUG_MSG
    this->val = new _T(std::forward<_T>(val));
}

template <typename _T>
inline optional<_T>& optional<_T>::operator=(const optional<_T>& other)
{
    DEBUG_MSG
    reset();
    val = new _T(other.value());
    return *this;
}

template <typename _T>
inline optional<_T>&& optional<_T>::operator=(optional<_T>&& other) noexcept
{
    DEBUG_MSG
    swap(other);
    return std::move(*this);
}

template <typename _T>
inline _T& optional<_T>::operator=(const _T& value)
{
    DEBUG_MSG
    *val = value;
    return *val;
}

template <typename _T>
inline _T&& optional<_T>::operator=(_T&& value) noexcept
{
    DEBUG_MSG
    std::swap(*val, value);
    return std::move(*val);
}

template <typename _T>
inline void optional<_T>::reset() noexcept
{
    DEBUG_MSG
    delete val;
    val = nullptr;
}

template <typename _T>
template <typename... _Args>
inline optional<_T>::optional(_Args&&... args)
{
    DEBUG_MSG
    val = new _T(std::forward<_Args>(args)...);
}

template <typename _T>
template <typename... _Args>
void optional<_T>::emplace(_Args&&... args)
{
    DEBUG_MSG
    reset();
    val = new _T(std::forward<_Args>(args)...);
}

template <typename _T>
struct std::hash<my::optional<_T>>
{
    std::size_t operator()(const my::optional<_T>& obj) noexcept
    {
        DEBUG_MSG
        return std::hash<_T>::operator()(obj.value());
    }
};

template <typename _T>
bool operator==(const my::optional<_T>& obj, const my::nullopt_t) noexcept
{
    DEBUG_MSG
    return !obj.has_value();
}

template <typename _T>
bool operator!=(const my::optional<_T>& obj, const my::nullopt_t) noexcept
{
    DEBUG_MSG
    return obj.has_value();
}
} // namespace my
