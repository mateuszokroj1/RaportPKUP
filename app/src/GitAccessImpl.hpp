#pragma once

#include <include/GitRepositoryDetector.hpp>

namespace RaportPKUP
{
class GitRepositoryDetectorDI final : public GitRepositoryDetector
{
  public:
	GitRepositoryDetectorDI(UI::Application&);

  private:
	const GitRepositoryAccessor& getAccessorImpl() const override
	{
		return *_accessor;
	}

	std::shared_ptr<GitRepositoryAccessor> _accessor;
};
} // namespace RaportPKUP
