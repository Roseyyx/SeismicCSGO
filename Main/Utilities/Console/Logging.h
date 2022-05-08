#pragma once
// used: std::format
#include <format>
// used: std::ofstream
#include <fstream>
// used: std::cout, std::endl
#include <iostream>
// used: std::chrono::system_clock
#include <chrono>

#include "../../Includes/MainIncludes.h"

#pragma region logging_console_colors
#define FOREGROUND_WHITE		    (FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_YELLOW       	(FOREGROUND_RED | FOREGROUND_GREEN)
#define FOREGROUND_CYAN		        (FOREGROUND_BLUE | FOREGROUND_GREEN)
#define FOREGROUND_MAGENTA	        (FOREGROUND_RED | FOREGROUND_BLUE)
#define FOREGROUND_BLACK		    0

#define FOREGROUND_INTENSE_RED		(FOREGROUND_RED | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_GREEN	(FOREGROUND_GREEN | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_BLUE		(FOREGROUND_BLUE | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_WHITE	(FOREGROUND_WHITE | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_YELLOW	(FOREGROUND_YELLOW | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_CYAN		(FOREGROUND_CYAN | FOREGROUND_INTENSITY)
#define FOREGROUND_INTENSE_MAGENTA	(FOREGROUND_MAGENTA | FOREGROUND_INTENSITY)
#pragma endregion

#pragma region logging_exception_handling
#ifdef _DEBUG
#define SEH_CATCH _RPT0(_CRT_ERROR, ex.what());
#else
#define SEH_CATCH std::abort();
#endif

#define SEH_START try {
#define SEH_END } catch (const std::exception& ex) {		\
	Console::PushConsoleColor(FOREGROUND_INTENSE_RED);			\
	Console::Print(std::format("[error] {}", ex.what()));	\
	Console::PopConsoleColor();									\
	SEH_CATCH }
#pragma endregion


namespace Console {
	inline FILE* pStream;
	// current color of console text
	inline std::uint16_t wConsoleColor = FOREGROUND_WHITE;
	// current file used for file-logging
	inline std::ofstream ofsFile;

	/*
	 * @section: get
	 */
	 // attach console to current window with write permission and given title
	bool Attach(const char* szConsoleTitle);
	// close write streams and detach console from current window
	void Detach();
	// prints given text to the console/file
	template <typename ... Args_t>
	void Print(const std::string Attribute, const std::string_view szText, const Args_t& ... argList)
	{

		// print to console
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSE_GREEN);
		std::cout << "[" + Attribute + "] ";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wConsoleColor);
		if constexpr (sizeof...(argList) > 0)
			std::cout << std::vformat(szText, std::make_format_args(argList...)) << std::endl;
		else
			std::cout << szText << std::endl;
	}

	/*
	 * @section: options stack
	 */
	 // set given color to console
	inline void PushConsoleColor(const std::uint16_t wColor)
	{
		wConsoleColor = wColor;
	}
	// restore console color
	inline void PopConsoleColor()
	{
		wConsoleColor = FOREGROUND_WHITE;
	}
}
