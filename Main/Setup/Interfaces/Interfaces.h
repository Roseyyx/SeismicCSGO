#pragma once
#include <windows.h>
#include <cstdint>
#include <string>
#include <stdexcept>

#include "../../SDK/Classes/Convar.h"

// SDK Interfaces Includes
#include "../../SDK/Interfaces/ibaseclientdll.h"
#include "../../SDK/Interfaces/icliententitylist.h"
#include "../../SDK/Interfaces/isurface.h"
#include "../../SDK/Interfaces/iclientmodeshared.h"
#include "../../SDK/Interfaces/iconvar.h"
//#include "../../SDK/Interfaces/ieffects.h"
#include "../../SDK/Interfaces/inetchannel.h"
#include "../../SDK/Interfaces/iengineclient.h"
#include "../../SDK/Interfaces/ienginetrace.h"
//#include "../../SDK/Interfaces/ienginevgui.h"
//#include "../../SDK/Interfaces/ienginesound.h"
//#include "../../SDK/Interfaces/igameconsole.h"
//#include "../../SDK/Interfaces/igameeventmanager.h"
//#include "../../SDK/Interfaces/ivpanel.h"
//#include "../../SDK/Interfaces/igameui.h"
#include "../../SDK/Interfaces/IGlobalsVarsBase.h"
//#include "../../SDK/Interfaces/iservergamedll.h"
#include "../../SDK/Interfaces/iinput.h"
//#include "../../SDK/Interfaces/iinputsystem.h"
//#include "../../SDK/Interfaces/ilocalize.h"
//#include "../../SDK/Interfaces/imatchframework.h"
#include "../../SDK/Interfaces/igametypes.h"
#include "../../SDK/Interfaces/imaterialsystem.h"
//#include "../../SDK/Interfaces/imdlcache.h"
//#include "../../SDK/Interfaces/imemalloc.h"
//#include "../../SDK/Interfaces/inetworkstring.h"
#include "../../SDK/Interfaces/iphysicssurfaceprops.h"
//#include "../../SDK/Interfaces/iprediction.h"
//#include "../../SDK/Interfaces/isteamclient.h"
//#include "../../SDK/Interfaces/isteamgamecoordinator.h"
//#include "../../SDK/Interfaces/istudiorender.h"
//#include "../../SDK/Interfaces/ivdebugoverlay.h"
//#include "../../SDK/Interfaces/ivengineeffects.h"
//#include "../../SDK/Interfaces/iglowmanager.h"
//#include "../../SDK/Interfaces/iviewrender.h"
//#include "../../SDK/Interfaces/iviewrenderbeams.h"
#include "../../SDK/Interfaces/ivmodelinfo.h"
//#include "../../SDK/Interfaces/ivmodelrender.h"
//#include "../../SDK/Interfaces/ivrenderview.h"
#include "../../SDK/Interfaces/iweaponsystem.h"
//#include "../../SDK/Interfaces/ikeyvaluessystem.h"

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
	inline IClientModeShared*		ClientMode;
	inline IEngineTrace*			EngineTrace;
	inline IInput*					Input;
	inline IVModelInfo*				ModelInfo;
	inline IConVar*					Convar;
	inline ISurface*				Surface;
	inline IPhysicsSurfaceProps*	PhysicsProps;
	inline IMaterialSystem*			MaterialSystem;
	inline IGlobalVarsBase*			Globals;
	inline IGameTypes*				GameTypes;
	inline IWeaponSystem*			WeaponSystem;
}