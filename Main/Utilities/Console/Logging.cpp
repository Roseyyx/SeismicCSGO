#include "Logging.h"

bool Console::Attach(const char* szConsoleTitle) {
	if (!AllocConsole())
		return false;

	AttachConsole(ATTACH_PARENT_PROCESS);

	if (freopen_s(&pStream, "CONOUT$", "w", stdout) != 0)
		return false;

	// set console window title
	if (!SetConsoleTitle(szConsoleTitle))
		return false;

	return true;
}

void Console::Detach()
{
	// close write console stream
	fclose(pStream);

	// free allocated memory
	FreeConsole();

	// get console window
	if (const auto hConsoleWnd = GetConsoleWindow(); hConsoleWnd != nullptr)
		// close console window
		PostMessageW(hConsoleWnd, WM_CLOSE, 0U, 0L);
}