#include "NetvarManager.h"

#include "../../Setup/Config/Config.h"

#include "../../Setup/Interfaces/Interfaces.h"

bool CNetvarManager::Setup(const std::string_view szDumpFileName) {
	// clear values
	mapProps.clear();
	iStoredProps = 0;
	iStoredTables = 0;

	// format time
	const std::string szTime = std::format("[{:%d-%m-%Y %X}] ", std::chrono::system_clock::now());

	// open our dump file to write in (here is not exception handle because dump is not critical)
	fsDumpFile.open(Config::GetWorkingPath().append(szDumpFileName), std::ios::out | std::ios::trunc);

	if (fsDumpFile.good())
		// write current date, time and info
		fsDumpFile << szTime << "Seismic | Netvar Dumps\n\n";

	for (auto pClass = Interfaces::Client->GetAllClasses(); pClass != nullptr; pClass = pClass->pNext)
	{
		if (pClass->pRecvTable == nullptr)
			continue;

		StoreProps(pClass->szNetworkName, pClass->pRecvTable, 0U, 0);
	}

	// close dump file
	fsDumpFile.close();

	return !mapProps.empty();
}

void CNetvarManager::StoreProps(const char* szClassName, RecvTable_t* pRecvTable, const std::uintptr_t uOffset, const int iDepth)
{
	std::string szDepth = { };

	for (int i = 0; i < iDepth; i++)
		szDepth.append("\t");

	if (fsDumpFile.good())
		fsDumpFile << std::format("{0}[{1}]\n", szDepth, pRecvTable->szNetTableName);

	for (int i = 0; i < pRecvTable->nProps; ++i)
	{
		const auto pCurrentProp = &pRecvTable->pProps[i];

		// base tables filter
		if (pCurrentProp == nullptr || isdigit(pCurrentProp->szVarName[0]))
			continue;

		// skip baseclass
		if (FNV1A::Hash(pCurrentProp->szVarName) == FNV1A::HashConst("baseclass"))
			continue;

		// has child table
		if (const auto pChildTable = pCurrentProp->pDataTable; pChildTable != nullptr &&
			// has props
			pChildTable->nProps > 0 &&
			// first char is 'D' ("DT" - "DataTable")
			pChildTable->szNetTableName[0] == 'D' &&
			// type is data table
			pCurrentProp->iRecvType == DPT_DATATABLE)
			// recursively get props in all child tables
			StoreProps(szClassName, pChildTable, static_cast<std::uintptr_t>(pCurrentProp->iOffset) + uOffset, iDepth + 1);

		// make own netvar storing format
		const FNV1A_t uHash = FNV1A::Hash(std::format("{}->{}", szClassName, pCurrentProp->szVarName).c_str());
		const std::uintptr_t uTotalOffset = static_cast<std::uintptr_t>(pCurrentProp->iOffset) + uOffset;

		// check does we have already grabbed property pointer and offset
		if (!mapProps[uHash].uOffset)
		{
			if (fsDumpFile.good())
				fsDumpFile << std::format("{0}\t{1} {2} = 0x{3:04X};\n", szDepth, GetPropertyType(pCurrentProp), pCurrentProp->szVarName, uTotalOffset);

			// write values to map entry
			mapProps[uHash] = { pCurrentProp, uTotalOffset };

			// count total stored props
			iStoredProps++;
		}
	}

	// count total stored tables
	iStoredTables++;
}

std::string CNetvarManager::GetPropertyType(const RecvProp_t* pRecvProp) const
{
	static CStandartRecvProxies* pStandartRecvProxies = Interfaces::Client->GetStandardRecvProxies();

	if (pRecvProp == nullptr)
		return "";

	RecvVarProxyFn pProxyFn = pRecvProp->oProxyFn;

	switch (pRecvProp->iRecvType)
	{
	case DPT_INT:
		// @credits: hinnie
		if (pProxyFn == pStandartRecvProxies->pInt32ToInt8)
			return "byte";
		else if (pProxyFn == pStandartRecvProxies->pInt32ToInt16)
			return "short";

		return "int";
	case DPT_FLOAT:
		return "float";
	case DPT_VECTOR:
		return "Vector";
	case DPT_VECTOR2D:
		return "Vector2D";
	case DPT_STRING:
		return std::format("char[{}]", pRecvProp->nStringBufferSize);
	case DPT_ARRAY:
		return std::format("array[{}]", pRecvProp->iElements);
	case DPT_DATATABLE:
		return "void*";
	case DPT_INT64:
		return "std::int64_t";
	default:
		break;
	}

	return "";
}

std::uintptr_t CNetvarManager::FindInDataMap(DataMap_t* pMap, const FNV1A_t uFieldHash)
{
	while (pMap != nullptr)
	{
		for (int i = 0; i < pMap->nDataFields; i++)
		{
			if (pMap->pDataDesc[i].szFieldName == nullptr)
				continue;

			if (FNV1A::Hash(pMap->pDataDesc[i].szFieldName) == uFieldHash)
				return pMap->pDataDesc[i].iFieldOffset[TD_OFFSET_NORMAL];

			if (pMap->pDataDesc[i].iFieldType == FIELD_EMBEDDED)
			{
				if (pMap->pDataDesc[i].pTypeDescription != nullptr)
				{
					if (const auto uOffset = FindInDataMap(pMap->pDataDesc[i].pTypeDescription, uFieldHash); uOffset != 0U)
						return uOffset;
				}
			}
		}

		pMap = pMap->pBaseMap;
	}

	return 0U;
}