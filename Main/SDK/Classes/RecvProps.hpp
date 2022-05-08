#pragma once

class DVariant;
class RecvTable;
class RecvProp;
class RecvProxyData;

using RecvProxyFN = void(*)(const RecvProxyData* data, void* struct_ptr, void* out_ptr);
using ArrayLengthRecvProxyFN = void(*)(void* struct_ptr, int object_id, int current_array_length);
using DataTableRecvProxyFN = void(*)(const RecvProp * prop, void** out_ptr, void* data_ptr, int object_id);

enum SendPropType {
	_int = 0,
	_float,
	_vec,
	_vec_xy,
	_string,
	_array,
	_data_table,
	_int_64,
};
class DVariant {
public:
	union {
		float mFloat;
		long mInt;
		char* mString;
		void* mData;
		float mVector[3];
		__int64 mInt64;
	};
	SendPropType Type;
};
class RecvProxyData {
public:
	const RecvProp* RecvProp;
	DVariant Value;
	int ElementIndex;
	int ObjectID;
};
class RecvProp {
public:
	char* PropName;
	SendPropType PropType;
	int PropFlags;
	int BufferFlags;
	int IsInsideArray;
	const void* ExtraDataPtr;
	RecvProp* ArrayProp;
	ArrayLengthRecvProxyFN ArrayLengthProxy;
	RecvProxyFN ProxyFN;
	DataTableRecvProxyFN DataTableProxy;
	RecvTable* DataTable;
	int Offset;
	int ElementStride;
	int ElementsCount;
	const char* ParentArrayPropName;
};
class RecvTable {
public:
	RecvProp* Props;
	int PropsCount;
	void* DecoderPtr;
	char* TableName;
	bool IsInitialized;
	bool IsInMainList;
};