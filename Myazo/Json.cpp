#include "Json.h"

Json::Item::Item(const Item&)
{
	return;
}

Json::Item::Item(void)
{
	return;
}

Json::Item::Item(long long& Int)
{
	return;
}

Json::Item::Item(double& Double)
{
	return;
}

Json::Item::Item(bool& Bool)
{
	return;
}

Json::Item::Item(std::wstring& String)
{
	return;
}

Json::Item::Item(std::map<std::wstring,Item*>& Hash)
{
	return;
}

Json::Item::Item(std::vector<Item*>& Array)
{
	return;
}

Json::Item::~Item(void)
{
	return;
}

bool Json::Item::IsNull(void)const
{
	return IsNull_Value;
}

long long& Json::Item::Int(void)
{
	return *Int_Value;
}

double& Json::Item::Double(void)
{
	return *Double_Value;
}

bool& Json::Item::Bool(void)
{
	return *Bool_Value;
}

std::map<std::wstring,Json::Item*>& Json::Item::Hash(void)
{
	return *Hash_Value;
}

std::vector<Json::Item*>& Json::Item::Array(void)
{
	return *Array_Value;
}