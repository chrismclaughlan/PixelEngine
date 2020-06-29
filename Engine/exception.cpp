#include "exception.h"

#define fprintfErrorLog(file)\
fprintf(file, "[ERROR] %s\n[LINE] %d\n[FILE] %s\n", what(), getLine(), getFile());\

void AppException::logError()
{
#ifndef _DEBUG
    fprintf(stdout, "[ERROR] %s\n", what());
#else

    // Reassign "stderr" to "freopen.out":
    errno_t err = freopen_s(&fLog, errorLogFile, "w", stderr);

    if (err != 0)
    {
        // Print error to console
        fprintfErrorLog(stdout);
        fprintf(stdout, "Error logging error in %s\n", errorLogFile);
    }
    else
    {
        // Log error in file
        fprintfErrorLog(fLog);
        fclose(fLog);
        std::string s = "type " + std::string(errorLogFile);
        system(s.c_str());

        fprintf(stdout, "Successfully logged error in %s\n", errorLogFile);
        fflush(stdout);
    }
#endif
}

const char* AppException::what() const
{
    return info;
}
const char* AppException::getFile() const
{
    return file;
}
const int32 AppException::getLine() const
{
    return line;
}