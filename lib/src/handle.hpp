#pragma once

#include <windows.h>

namespace RaportGen
{
	class Handle
	{
	private:

		HANDLE _val = nullptr;

	public:
		Handle() = default;
		~Handle() noexcept
		{
			release();
		}

		void release() noexcept
		{
			if (_val)
			{
				CloseHandle(_val);
				_val = nullptr;
			}
		}

		HANDLE* ptr()
		{
			return &_val;
		}

		operator HANDLE() noexcept { return _val; }
	};
}