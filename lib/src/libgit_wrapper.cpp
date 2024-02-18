#include "libgit_wrapper.hpp"

namespace RaportGen
{
    std::atomic<std::shared_ptr<GitAPI>> GitAPI::_instance{};

    GitAPI::GitAPI()
    {
        git_libgit2_init();
    }

    GitAPI::~GitAPI()
    {
        git_libgit2_shutdown();
    }

    std::weak_ptr<GitAPI> GitAPI::getInstance()
    {
        if (const auto ptr = _instance.load())
            return ptr;
        else
        {
            _instance.store(std::shared_ptr<GitAPI>());
            return _instance.load();
        }
    }
}