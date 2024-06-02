#pragma once
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <iostream>
#include "API.h"

enum class EXPORT_API EXCEPTION_CODE {
    FontNotFound,
    FileForSavingNotFound,
    ImageNotFound,
	MemoryAllocationFailed,
	FailedToReadPlayerName,
	FailedToReadScore,
	FailedToOpenHighscoresFile
};

std::string EXPORT_API exception_message(EXCEPTION_CODE error);

#endif // EXCEPTIONS_H
