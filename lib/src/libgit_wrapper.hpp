#pragma once

#include <memory>
#include <atomic>

#include <git2.h>

namespace RaportGen
{
    class GitAPI
    {
    private:
        GitAPI();

    public:
        ~GitAPI();

        static std::weak_ptr<GitAPI> getInstance();

private:
        static std::atomic<std::shared_ptr<GitAPI>> _instance;

        friend class std::shared_ptr<GitAPI>;
    };
}