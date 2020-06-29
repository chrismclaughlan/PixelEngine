#pragma once
#include "hwindows.h"
#include "types.h"
#include <iostream>

#define THROW_EXCEPTION(msg)\
throw AppException(msg, __FILE__, __LINE__);\

class AppException : public std::exception
{
private:
    const char* errorLogFile = "errorlog.out";
	const char* info;
	const char* file;
	const int32 line;
    FILE* fLog = nullptr;
public:
	AppException(char const* const info, const char* file, const int32 line)
		: info(info), file(file), line(line) {}
	const char* what() const;
	const char* getFile() const;
	const int32 getLine() const;
	void logError();
};

//		const char* point;
//		if ((point = strrchr(fileName, '.')) != NULL) {
//			if (strcmp(point, ".out") != 0) {
//				// does not end with .out
//				printError("Error creating exception: file type must be .out", fileName);
//				return;
//			}
//		}