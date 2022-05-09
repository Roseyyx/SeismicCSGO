#pragma once

// used : std::ifstream
#include <fstream>

// used : std::unordered_map
#include <unordered_map>

// used : winapi includes
#include "../../Includes/MainIncludes.h"

// Hash Includes

// FNV1A
#include "../../SDK/Hashes/FNV1A.h"

// Datatable
// used: data table, prop, data type
#include "../../SDK/DataTypes/datamap.h"

// Datamap
#include "../../SDK/DataTypes/datatable.h"

#pragma region NetverDefinitons
#define NetvarVariable(Type, szFunctionName, szNetvarName) NetvarOffset(Type, szFunctionName, szNetVarName, 0U)
#pragma endregion

class CRecvPropHook {
public:
	CRecvPropHook(RecvProp_t* pRecvProp, const RecvVarProxyFn pNewProxyFn) :
		pRecvProp(pRecvProp), pOriginalFn(pRecvProp->oProxyFn)
	{
		SetProxy(pNewProxyFn);
	}
	// Get
	/* replace with our function */
	void Replace(RecvProp_t* pRecvProp)
	{
		this->pRecvProp = pRecvProp;
		this->pOriginalFn = pRecvProp->oProxyFn;
	}

	/* restore original function */
	void Restore() const
	{
		if (this->pOriginalFn != nullptr)
			this->pRecvProp->oProxyFn = this->pOriginalFn;
	}

	void SetProxy(const RecvVarProxyFn pNewProxyFn) const
	{
		this->pRecvProp->oProxyFn = pNewProxyFn;
	}

	RecvVarProxyFn GetOriginal() const
	{
		return this->pOriginalFn;
	}

private:
	// Values
	/* in future that is being modified and replace the original prop */
	RecvProp_t* pRecvProp = nullptr;
	/* original proxy function to make able to restore it later */
	RecvVarProxyFn pOriginalFn = nullptr;
};

class CNetvarManager : public CSingleton<CNetvarManager> {
public:
	struct NetvarObject_t
	{
		RecvProp_t* pRecvProp = nullptr;
		std::uintptr_t uOffset = 0U;
	};

	// Get
	/* fill map with netvars and also dump it to given file */
	bool Setup(const std::string_view szDumpFileName);
	/*
	 * stores the variables of objects in the hierarchy
	 * used to iterate through an object's data descriptions from data map
	 */
	std::uintptr_t FindInDataMap(DataMap_t* pMap, const FNV1A_t uFieldHash);

	// Values
	/* logging counters */
	int iStoredProps = 0;
	int iStoredTables = 0;
	/* networkable properties map */
	std::unordered_map<FNV1A_t, NetvarObject_t> mapProps = { };

private:
	/*
	 * recursively stores networked properties info from data tables in our map
	 * and also format our dump and write values to file
	 */
	void StoreProps(const char* szClassName, RecvTable_t* pRecvTable, const std::uintptr_t uOffset, const int iDepth);

	// Extra
	std::string GetPropertyType(const RecvProp_t* pRecvProp) const;

	// Values
	/* output file */
	std::ofstream fsDumpFile = { };
};