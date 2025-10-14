#include <utility>

namespace my
{
constexpr struct nullopt_t
{
} nullopt;

template <typename _T>
class optional final
{
private:
    _T* val;

public:
    optional(nullopt_t = nullopt) : val{nullptr} {};
    optional(_T&& val);
    template <typename... _Args>
    optional(_Args&&... args);
    optional(const optional<_T>& other)
        : val{new _T(std::forward<_T>(other.value()))} {};
    optional(optional<_T>&& other) noexcept { swap(other); };

    optional<_T>& operator=(const optional<_T>& other);
    optional<_T>&& operator=(optional<_T>&& other) noexcept;

    _T& operator=(const _T& value);
    _T&& operator=(_T&& value) noexcept;

    ~optional() noexcept { reset(); };

    bool has_value() const noexcept { return val; }
    operator bool() const noexcept { return has_value(); }

    _T& value() & noexcept { return *val; }
    const _T& value() const& noexcept { return *val; }

    _T&& value() && noexcept { return *val; }
    const _T&& value() const&& noexcept { return *val; }

    _T& operator*() & noexcept { return value(); }
    const _T& operator*() const& noexcept { return value(); }

    _T&& operator*() && noexcept { return value(); }
    const _T&& operator*() const&& noexcept { return value(); }

    _T* operator->() noexcept { return val; }
    const _T* operator->() const noexcept { return val; }

    void reset() noexcept;
    void swap(optional& other) & noexcept { std::swap(this->val, other.val); }
    void swap(optional&& other) && noexcept { std::swap(this->val, other.val); }

    template <typename... _Args>
    void emplace(_Args&&... args);
};

template <typename _T>
optional<_T>::optional(_T&& val)
{
    val = new _T(std::forward<_T>(val));
}

template <typename _T>
inline optional<_T>& optional<_T>::operator=(const optional<_T>& other)
{
    reset();
    val = new _T(other.value());
    return *this;
}

template <typename _T>
inline optional<_T>&& optional<_T>::operator=(optional<_T>&& other) noexcept
{
    swap(other);
    return std::move(*this);
}

template <typename _T>
inline _T& optional<_T>::operator=(const _T& value)
{
    *val = value;
    return *val;
}

template <typename _T>
inline _T&& optional<_T>::operator=(_T&& value) noexcept
{
    std::swap(*val, value);
    return std::move<_T>(*val);
}

template <typename _T>
inline void optional<_T>::reset() noexcept
{
    delete val;
    val = nullptr;
}

template <typename _T>
template <typename... _Args>
inline optional<_T>::optional(_Args&&... args)
{
    val = new _T(std::forward<_Args>(args)...);
}

template <typename _T>
template <typename... _Args>
void optional<_T>::emplace(_Args&&... args)
{
    reset();
    val = new _T(std::forward<_Args>(args)...);
}

} // namespace my

template <typename _T>
struct std::hash<my::optional<_T>>
{
    std::size_t operator()(const my::optional<_T>& obj) noexcept
    {
        return std::hash<_T>::operator()(obj);
    }
};

template <typename _T>
bool operator==(const my::optional<_T>& obj, const my::nullopt_t) noexcept
{
    return !obj.has_value();
}

template <typename _T>
bool operator!=(const my::optional<_T>& obj, const my::nullopt_t) noexcept
{
    return obj.has_value();
}
