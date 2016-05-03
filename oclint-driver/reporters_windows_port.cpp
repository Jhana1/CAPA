#include <dirent.h>
#include <windows.h>
#include <iostream>

#include "CAPA/GenericException.h"
#include "CAPA/Options.h"

#include "reporters.h"

static CAPA::Reporter *selectedReporter = NULL;

void loadReporter()
{
    selectedReporter = NULL;
    std::string reportDirPath = CAPA::option::reporterPath();
    DIR *pDir = opendir(reportDirPath.c_str());
    if (pDir != NULL)
    {
        struct dirent *dirp;
        while ((dirp = readdir(pDir)))
        {
            if (dirp->d_name[0] == '.')
            {
                continue;
            }
            std::string reporterPath = reportDirPath + "/" + std::string(dirp->d_name);
            HMODULE reporterHandle = LoadLibrary(reporterPath.c_str());
            if (reporterHandle == NULL)
            {
                std::cerr << GetLastError() << std::endl;
                closedir(pDir);
                throw CAPA::GenericException("cannot open dynamic library: " + reporterPath);
            }
            typedef CAPA::Reporter* (*CreateReporterFunc)();
            CreateReporterFunc createMethodPointer;
            createMethodPointer = (CreateReporterFunc) GetProcAddress(reporterHandle, "create");
            CAPA::Reporter* reporter = (CAPA::Reporter*)createMethodPointer();
            if (reporter->name() == CAPA::option::reportType())
            {
                selectedReporter = reporter;
                break;
            }
        }
        closedir(pDir);
    }
    if (selectedReporter == NULL)
    {
        throw CAPA::GenericException(
            "cannot find dynamic library for report type: " + CAPA::option::reportType());
    }
}

CAPA::Reporter* reporter()
{
    return selectedReporter;
}
