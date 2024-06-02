#include "EXCEPTIONS.h"

std::string exception_message(EXCEPTION_CODE error) {
    switch (error) {
    case EXCEPTION_CODE::FontNotFound:
        return "Font not found!";
    case EXCEPTION_CODE::FileForSavingNotFound:
        return "File for saving not found!";
    case EXCEPTION_CODE::ImageNotFound:
        return "Image not found!";
	case EXCEPTION_CODE::MemoryAllocationFailed:
		return "Memory allocation failed!";
	case EXCEPTION_CODE::FailedToReadPlayerName:
		return "Failed to read player name!";
    case EXCEPTION_CODE::FailedToReadScore:
		return "Failed to read score!";
	case EXCEPTION_CODE::FailedToOpenHighscoresFile:
		return "Failed to open highscores file!";
    default:
        return "Unknown error!";
    }
}
