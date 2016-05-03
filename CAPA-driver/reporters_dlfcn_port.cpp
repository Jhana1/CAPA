#include <dirent.h>
#include <dlfcn.h>
#include <iostream>

#include "CAPA/GenericException.h"
#include "CAPA/Options.h"

#include "reporters.h"

static CAPA::Reporter* selectedReporter = nullptr;

void loadReporter()
{
    selectedReporter = nullptr;
    std::string reportDirPath = CAPA::option::reporterPath();
    DIR *pDir = opendir(reportDirPath.c_str());
    if (pDir != nullptr)
    {
        struct dirent *dirp;
        while ((dirp = readdir(pDir)))
        {
            if (dirp->d_name[0] == '.')
            {
                continue;
            }
            std::string reporterPath = reportDirPath + "/" + std::string(dirp->d_name);
            void *reporterHandle = dlopen(reporterPath.c_str(), RTLD_LAZY);
            if (reporterHandle == nullptr)
            {
                std::cerr << dlerror() << std::endl;
                closedir(pDir);
                throw CAPA::GenericException("cannot open dynamic library: " + reporterPath);
            }
            CAPA::Reporter* (*createMethodPointer)();
            createMethodPointer = (CAPA::Reporter* (*)())dlsym(reporterHandle, "create");
            CAPA::Reporter* reporter = (CAPA::Reporter*)createMethodPointer();
            if (reporter->name() == CAPA::option::reportType())
            {
                selectedReporter = reporter;
                break;
            }
        }
        closedir(pDir);
    }
    if (selectedReporter == nullptr)
    {
        throw CAPA::GenericException(
            "cannot find dynamic library for report type: " + CAPA::option::reportType());
    }
}

CAPA::Reporter* reporter()
{
    return selectedReporter;
}
