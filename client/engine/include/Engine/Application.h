#ifndef DYXIDE_APPLICATION_H
#define DYXIDE_APPLICATION_H

#include <Shared/Assert.h>

#include <string>

namespace dyxide
{
    struct ApplicationCommandLineArgs
    {
        int Count = 0;
        char** Args = nullptr;

        const char* operator[](int index) const
        {
            DYXIDE_ASSERT(index < Count);
            return Args[index];
        }
    };

    struct ApplicationSpecification
    {
        std::string Name = "Application";
        ApplicationCommandLineArgs CommandLineArgs;
    };

    class Application
    {
    public:
        Application(ApplicationSpecification specification);
        virtual ~Application();

        static Application& Get() { return *s_Instance; }

        const ApplicationSpecification& GetSpecification() { return m_Specification; }

    private:
        static Application* s_Instance;

        ApplicationSpecification m_Specification;
    };
}

#endif