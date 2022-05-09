#pragma once

// used: windows import
#include "../../Includes/MainIncludes.h"

// Singleton Implementation
// Restricts instantiation to one object
template <typename T>
class CSingleton
{
protected:
	CSingleton() { }
	~CSingleton() { }

	CSingleton(const CSingleton&) = delete;
	CSingleton& operator=(const CSingleton&) = delete;

	CSingleton(CSingleton&&) = delete;
	CSingleton& operator=(CSingleton&&) = delete;
public:
	static T& Get()
	{
		static T pInstance{ };
		return pInstance;
	}
};

// Memory Management functions
// Used to allocate and deallocate memory and call virtual functions
namespace Memory {
	template <typename T, std::size_t S>
	std::vector<T> GetFilledVector(const T& fill)
	{
		std::vector<T> vecTemp(S);
		std::fill(vecTemp.begin(), vecTemp.begin() + S, fill);
		return vecTemp;
	}
	/// @returns : virtual function pointer of specified class at given index
	template <typename T = void*>
	constexpr T GetVFunc(void* thisptr, std::size_t nIndex)
	{
		return (*static_cast<T**>(thisptr))[nIndex];
	}
	// call virtual function of specified class at given index
	// @note: references should be wrapped with std::ref call!
	template <typename T, typename ... Args_t>
	constexpr T CallVFunc(void* thisptr, std::size_t nIndex, Args_t... argList)
	{
		using VirtualFn = T(__thiscall*)(void*, decltype(argList)...);
		return (*static_cast<VirtualFn**>(thisptr))[nIndex](thisptr, argList...);
	}
}