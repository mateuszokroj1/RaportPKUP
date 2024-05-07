#pragma once

namespace RaportPKUP::UI
{
template <typename T, T start_value, T end_value> class SessionSetter final
{
  public:
	SessionSetter(T *value_ptr) : _ptr(value_ptr)
	{
		if (value_ptr)
			*value_ptr = start_value;
	}

	~SessionSetter()
	{
		if (_ptr)
			*_ptr = end_value;
	}

  private:
	T *_ptr = nullptr;
};
} // namespace RaportPKUP::UI
