#pragma once
#include <windows.h>
#include <cstdint>
#include <string>
#include <stdexcept>

#include "../../SDK/Classes/Convar.h"

// SDK Interfaces Includes
#include "../../SDK/Interfaces/IBaseClientDLL.h"
#include "../../SDK/Interfaces/IClientEntityList.h"
#include "../../SDK/Interfaces/INetChannel.h"
#include "../../SDK/Interfaces/IEngineClient.h"
#include "../../SDK/Interfaces/IEngineTrace.h"
#include "../../SDK/Interfaces/IInput.h"
#include "../../SDK/Interfaces/IVModelInfo.h"

class CInterfaceRegister
{
public:
	InstantiateInterfaceFn	pCreateFn;		//0x0
	const char* szName;			//0x4
	CInterfaceRegister* pNext;			//0x8
};

namespace Interfaces {
	bool Setup();	
	template <typename T>
	T* Capture(const std::string_view szModuleName, const std::string_view szInterface);

	inline IBaseClientDll*			Client;
	inline IClientEntityList*		EntityList;
	inline IEngineClient*			Engine;
	inline IEngineTrace*			EngineTrace;
	inline IInput*					Input;
	inline IVModelInfo*				ModelInfo;
}