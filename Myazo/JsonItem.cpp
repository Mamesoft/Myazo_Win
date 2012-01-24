#include "Json.h"

Json::Item::Item(const Item&)
{
	return;
}

Json::Item::Item(void)
{
	Clear();
	IsNull_Value=true;
	Type_Value=Type::Null;
	return;
}

void Json::Item::Clear(void)
{
	Int_Value=nullptr;
	Double_Value=nullptr;
	Bool_Value=nullptr;
	String_Value=nullptr;
	Hash_Value=nullptr;
	Array_Value=nullptr;
	return;
}

Json::Item::Item(const long long& Int)
{
	Clear();
	Int_Value=new long long(Int);
	Type_Value=Type::Int;
	return;
}

Json::Item::Item(const double& Double)
{
	Clear();
	Double_Value=new double(Double);
	Type_Value=Type::Double;
	return;
}

Json::Item::Item(const bool& Bool)
{
	Clear();
	Bool_Value=new bool(Bool);
	Type_Value=Type::Bool;
	return;
}

Json::Item::Item(const std::wstring& String)
{
	Clear();
	String_Value=new std::wstring(String);
	Type_Value=Type::String;
	return;
}

Json::Item::Item(const std::map<std::wstring,Item*>& Hash)
{
	Clear();
	Hash_Value=new std::map<std::wstring,Item*>(Hash);
	Type_Value=Type::Hash;
	return;
}

Json::Item::Item(const std::vector<Item*>& Array)
{
	Clear();
	Array_Value=new std::vector<Item*>(Array);
	Type_Value=Type::Array;
	return;
}

Json::Item::Item(long long&& Int)
{
	Clear();
	Int_Value=new long long(std::move(Int));
	Type_Value=Type::Int;
	return;
}

Json::Item::Item(double&& Double)
{
	Clear();
	Double_Value=new double(std::move(Double));
	Type_Value=Type::Double;
	return;
}

Json::Item::Item(bool&& Bool)
{
	Clear();
	Bool_Value=new bool(std::move(Bool));
	Type_Value=Type::Bool;
	return;
}

Json::Item::Item(std::wstring&& String)
{
	Clear();
	String_Value=new std::wstring(std::move(String));
	Type_Value=Type::String;
	return;
}

Json::Item::Item(std::map<std::wstring,Item*>&& Hash)
{
	Clear();
	Hash_Value=new std::map<std::wstring,Item*>(std::move(Hash));
	Type_Value=Type::Hash;
	return;
}

Json::Item::Item(std::vector<Item*>&& Array)
{
	Clear();
	Array_Value=new std::vector<Item*>(std::move(Array));
	Type_Value=Type::Array;
	return;
}

Json::Item::~Item(void)
{
	if(IsNull_Value) return;
	if(Int_Value) delete Int_Value;
	else if(Double_Value) delete Double_Value;
	else if(Bool_Value) delete Bool_Value;
	else if(String_Value) delete String_Value;
	else if(Hash_Value) delete Hash_Value;
	else if(Array_Value) delete Array_Value;
	return;
}

Json::Type Json::Item::Type(void)
{
	return Type_Value;
}

bool Json::Item::IsNull(void)const
{
	return IsNull_Value;
}

long long& Json::Item::Int(void)
{
	if(Int_Value) return *Int_Value;
	else throw std::exception();
}

double& Json::Item::Double(void)
{
	if(Double_Value) return *Double_Value;
	else throw std::exception();
}

bool& Json::Item::Bool(void)
{
	if(Bool_Value) return *Bool_Value;
	else throw std::exception();
}

std::wstring& Json::Item::String(void)
{
	if(String_Value) return *String_Value;
	else throw std::exception();
}

std::map<std::wstring,Json::Item*>& Json::Item::Hash(void)
{
	if(Hash_Value) return *Hash_Value;
	else throw std::exception();
}

std::vector<Json::Item*>& Json::Item::Array(void)
{
	if(Array_Value) return *Array_Value;
	else throw std::exception();
}