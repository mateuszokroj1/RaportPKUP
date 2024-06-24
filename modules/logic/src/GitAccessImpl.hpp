#pragma once

#include <include/GitRepositoryDetector.hpp>

namespace RaportPKUP
{
class GitRepositoryDetectorDI final : public GitRepositoryDetector
{
  public:
	GitRepositoryDetectorDI(std::weak_ptr<UI::Application> app);

  private:
	const GitRepositoryAccessor& getAccessorImpl() const override
	{
		return *_accessor;
	}

	std::shared_ptr<GitRepositoryAccessor> _accessor;
};
} // namespace RaportPKUP
