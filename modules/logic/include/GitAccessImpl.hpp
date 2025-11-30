#pragma once

#include <include/GitRepositoryDetector.hpp>

namespace RaportPKUP::UI
{
class Application;

class GitRepositoryDetectorDI final : public GitRepositoryDetector
{
  public:
	GitRepositoryDetectorDI(std::weak_ptr<Application> app);

  private:
	const GitRepositoryAccessor& getAccessorImpl() const override
	{
		return *_accessor;
	}

	std::shared_ptr<GitRepositoryAccessor> _accessor;
};
} // namespace RaportPKUP::UI
