#ifndef FN_DETAIL_SMALL_UNIQUE_PTR_HPP
#define FN_DETAIL_SMALL_UNIQUE_PTR_HPP

#include <cstddef>

#include <memory>
#include <type_traits>
#include <utility>

namespace fn {
namespace detail {

class Storage {
public:
	void* get() noexcept {
		return reinterpret_cast<void*>(std::addressof(buffer_));
	}

	const void* get() const noexcept {
		return reinterpret_cast<const void*>(std::addressof(buffer_));
	}

	template <typename T>
	T* reinterpret() noexcept {
		return reinterpret_cast<T*>(get());
	}

	template <typename T>
	const T* reinterpret() const noexcept {
		return reinterpret_cast<const T*>(get());
	}

private:
	alignas(std::max_align_t) char buffer_[sizeof(void*) * 4];
};

template <typename T>
struct ValueTag { };

template <typename T>
class SmallUniquePtr {
public:
	template <
		typename ...As,
		std::enable_if_t<
			std::is_constructible<T, As&&...>::value
			&& sizeof(T) <= sizeof(Storage) && alignof(T) <= alignof(Storage),
			int
		> = 0
	>
	explicit SmallUniquePtr(As &&...args)
	noexcept(std::is_nothrow_constructible<T, As&&...>::value) {
		new(storage_.get()) T(std::forward<As>(args)...);
	}

	template <
		typename ...As,
		std::enable_if_t<
			std::is_constructible<T, As&&...>::value
			&& (sizeof(T) > sizeof(Storage) || alignof(T) > alignof(Storage)),
			int
		> = 0
	>
	explicit SmallUniquePtr(As &&...args)
	noexcept(std::is_nothrow_constructible<T, As&&...>::value)
	: owned_{ new T(std::forward<As>(args)...) } { }

	template <
		typename U,
		typename ...As,
		std::enable_if_t<
			std::is_base_of<T, U>::value
			&& std::is_constructible<U, As&&...>::value
			&& sizeof(U) <= sizeof(Storage) && alignof(U) <= alignof(Storage),
			int
		> = 0
	>
	explicit SmallUniquePtr(ValueTag<U>, As &&...args)
	noexcept(std::is_nothrow_constructible<U, As&&...>::value) {
		new(storage_.get()) U(std::forward<As>(args)...);
	}

	template <
		typename U,
		typename ...As,
		std::enable_if_t<
			std::is_base_of<T, U>::value
			&& std::is_constructible<U, As&&...>::value
			&& (sizeof(U) > sizeof(Storage) || alignof(U) > alignof(Storage)),
			int
		> = 0
	>
	explicit SmallUniquePtr(ValueTag<U>, As &&...args)
	noexcept(std::is_nothrow_constructible<U, As&&...>::value)
	: owned_{ new U(std::forward<As>(args)...) } { }

	~SmallUniquePtr() {
		if (owned_) {
			delete owned_;
		} else {
			storage_.template reinterpret<T>()->T::~T();
		}
	}

	T& operator*() noexcept {
		if (owned_) {
			return *owned_;
		} else {
			return *storage_.template reinterpret<T>();
		}
	}

	const T& operator*() const noexcept {
		if (owned_) {
			return *owned_;
		} else {
			return *storage_.template reinterpret<T>();
		}
	}

	T* operator->() noexcept {
		return get();
	}

	const T* operator->() const noexcept {
		return get();
	}

	T* get() noexcept {
		return std::addressof(**this);
	}

	const T* get() const noexcept {
		return std::addressof(**this);
	}

private:
	Storage storage_;
	T *owned_ = nullptr;
};

} // namespace detail
} // namespace fn

#endif
