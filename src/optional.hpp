#include <iostream>
#include <source_location>
#include <utility>

namespace my
{
#define TEST

#ifndef TEST
#define DEBUG_MSG
#else
#define DEBUG_MSG                                                              \
    std::clog << std::source_location::current().line() << ','                 \
              << std::source_location::current().function_name() << "\n";
#endif

constexpr struct nullopt_t
{
} nullopt;

template <typename _T>
class optional final
{
private:
    alignas(_T) char buf[sizeof(_T)];
    _T* val;

public:
    optional(nullopt_t = nullopt) : val{nullptr} {DEBUG_MSG};
    optional(_T&& val);
    template <typename... _Args>
    optional(_Args&&... args);
    optional(const optional<_T>& other)
        : val{new(buf) _T(std::forward<_T>(other.value()))} {DEBUG_MSG};
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
        std::swap(this->buf, other.buf);
    }
    void swap(optional&& other) && noexcept
    {
        DEBUG_MSG std::swap(this->val, other.val);
        std::swap(this->buf, other.buf);
    }

    template <typename... _Args>
    void emplace(_Args&&... args);
};

template <typename _T>
optional<_T>::optional(_T&& val)
{
    DEBUG_MSG
    this->val = new (buf) _T(std::forward<_T>(val));
}

template <typename _T>
inline optional<_T>& optional<_T>::operator=(const optional<_T>& other)
{
    DEBUG_MSG
    reset();
    val = new (buf) _T(other.value());
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
    if (!val)
        val = new (buf) _T(value);
    else
        *val = value;

    return *val;
}

template <typename _T>
inline _T&& optional<_T>::operator=(_T&& value) noexcept
{
    DEBUG_MSG
    if (!val)
        val = new (buf) _T(std::forward<_T>(value));
    else
        std::swap(*val, value);

    return std::move(*val);
}

template <typename _T>
inline void optional<_T>::reset() noexcept
{
    DEBUG_MSG
    if (val)
    {
        val->~_T();
        val = nullptr;
    }
}

template <typename _T>
template <typename... _Args>
inline optional<_T>::optional(_Args&&... args)
{
    DEBUG_MSG
    val = new (buf) _T(std::forward<_Args>(args)...);
}

template <typename _T>
template <typename... _Args>
void optional<_T>::emplace(_Args&&... args)
{
    DEBUG_MSG
    reset();
    val = new (buf) _T(std::forward<_Args>(args)...);
}

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

template <typename _T>
bool operator==(const my::nullopt_t, const my::optional<_T>& obj) noexcept
{
    DEBUG_MSG
    return !obj.has_value();
}

template <typename _T>
bool operator!=(const my::nullopt_t, const my::optional<_T>& obj) noexcept
{
    DEBUG_MSG
    return obj.has_value();
}
} // namespace my

template <typename _T>
struct std::hash<my::optional<_T>>
{
    std::size_t operator()(const my::optional<_T>& obj) noexcept
    {
        DEBUG_MSG
        return std::hash<_T>::operator()(obj.value());
    }
};
