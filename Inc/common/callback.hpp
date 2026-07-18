#pragma once

namespace callback {

class Callback {
public:
	using FuncPtr = void(*)(void* context);

	constexpr Callback() :
			func_(nullptr), context_(nullptr) {
	}
	constexpr Callback(FuncPtr func, void *context = nullptr) :
			func_(func), context_(context) {
	}

	void operator()() const {
		if (func_) {
			func_(context_);
		}
	}

	explicit operator bool() const {
		return func_ != nullptr;
	}

private:
	FuncPtr func_;
	void *context_;
};

} // namespace callback
