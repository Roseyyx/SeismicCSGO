#include "Interfaces.h"


#define Interface(Value, Type, ModuleName, InterfaceName) Value = Interfaces::GetInterface<Type*>((ModuleName), (InterfaceName));

bool Interfaces::Setup() {
	Interface(Client, IBaseClientDLL, "client.dll", "VClient018");
	return true;
}

#undef Interface