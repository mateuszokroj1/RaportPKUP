#pragma once

namespace RaportPKUP::UI
{
template <typename T> struct EmptyDeleter
{
	void operator()(T *)
	{
	}
};
} // namespace RaportPKUP::UI
