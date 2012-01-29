#include "Json.h"

long long Json::Json::ParseInt(std::wstring::const_iterator& Char)
{
	long long Int;
	return Int;
}

double Json::Json::ParseDouble(std::wstring::const_iterator& Char)
{
	double Double;
	return Double;
}

bool Json::Json::ParseBool(std::wstring::const_iterator& Char)
{
	bool Bool;
	return Bool;
}

std::wstring Json::Json::ParseString(std::wstring::const_iterator& Char)
{
	std::wstring String;
	return String;
}

bool Json::Json::ParseNull(std::wstring::const_iterator& Char)
{
	bool IsNull;
	return IsNull;
}

std::wstring Json::Json::ToString(long long Int)
{
	std::wstring ResultString;
	return ResultString;
}

std::wstring Json::Json::ToString(double Double)
{
	std::wstring ResultString;
	return ResultString;
}

std::wstring Json::Json::ToString(bool Bool)
{
	std::wstring ResultString;
	return ResultString;
}

std::wstring Json::Json::ToString(std::wstring String)
{
	std::wstring ResultString;
	return ResultString;
}

std::wstring Json::Json::ToString(void)
{
	std::wstring ResultString;
	return ResultString;
}

std::wstring Json::Json::Create(const Item* const& Root)
{
	std::stack<const Item* const> Level;
	std::wstring ResultString;
	return ResultString;
}

Json::Item* Json::Json::Parse(const std::wstring& JsonString)
{
	auto Char=JsonString.cbegin();
	std::stack<Item*> Level;
	Item* Root=nullptr;
	do{
		if(*Char==L'{') Root=new Item(Hash());
		else if(*Char==L'[') Root=new Item(Array());
		else if(*Char!=L' '&&*Char!=L'\t'&&*Char!=L'\n'&&*Char!=L'\r') throw std::exception("不正な文字が含まれています。");
	}while(Root==nullptr&&++Char!=JsonString.cend());
	Level.push(Root);
	do{
		Char++;
		if(*Char==L','||*Char==L' '||*Char==L'\t'||*Char==L'\n'||*Char==L'\r') continue;
		Type ObjType=Level.top()->Type();
		if(ObjType==Type::Hash){
			std::wstring Key;
			if(*Char==L'\"'){
				Key=ParseString(Char);
				for(;*Char!=L':';Char++) continue;
				for(Char++;*Char==L' '||*Char==L'\t'||*Char==L'\n'||*Char==L'\r';Char++) continue;
			}
			if(*Char==L'\"'){
				Level.top()->Hash().insert(std::make_pair(Key,new Item(ParseString(Char))));
			}else if(*Char==L'-'||(*Char>=L'0'&&*Char<=L'9')){
				auto Start=Char;
				for(;;Char++) if(*Char==L' '||*Char==L'\t'||*Char==L'\n'||*Char==L'\r'||*Char==L','||*Char==L'}'||*Char==L']') break;
				if(std::regex_match(Start,Char,std::wregex(L"-?\\d+"))) Level.top()->Hash().insert(std::make_pair(Key,new Item(ParseInt(Start))));
				else if(std::regex_match(Start,Char,std::wregex(L"-?\\d+\\.\\d+([eE][+-]\\d+)?"))) Level.top()->Hash().insert(std::make_pair(Key,new Item(ParseDouble(Start))));
				else throw std::exception("数字以外の文字が入っている、または不正な数値形式の文字列です。\nJSONの数値文字列は10進数で記述しなければなりません。");
				Char--;
			}else if(*Char==L't'||*Char==L'f'){
				Level.top()->Hash().insert(std::make_pair(Key,new Item(ParseBool(Char))));
			}else if(*Char==L'{'){
				Item* Obj=new Item(Hash());
				Level.top()->Hash().insert(std::make_pair(Key,Obj));
				Level.push(Obj);
			}else if(*Char==L'['){
				Item* Obj=new Item(Array());
				Level.top()->Hash().insert(std::make_pair(Key,Obj));
				Level.push(Obj);
			}else if(*Char==L'n'){
				Level.top()->Hash().insert(std::make_pair(Key,ParseNull(Char)?new Item():(throw std::exception(),nullptr)));
			}else if(*Char==L'}') Level.pop();
		}else if(ObjType==Type::Array){
			if(*Char==L'\"'){
				Level.top()->Array().push_back(new Item(ParseString(Char)));
			}else if(*Char==L'-'||(*Char>=L'0'&&*Char<=L'9')){
				auto Start=Char;
				for(;;Char++) if(*Char==L' '||*Char==L'\t'||*Char==L'\n'||*Char==L'\r'||*Char==L','||*Char==L'}'||*Char==L']') break;
				if(std::regex_match(Start,Char,std::wregex(L"-?\\d+"))) Level.top()->Array().push_back(new Item(ParseInt(Char)));
				else if(std::regex_match(Start,Char,std::wregex(L"-?\\d+\\.\\d+([eE][+-]\\d+)?"))) Level.top()->Array().push_back(new Item(ParseDouble(Char)));
				else throw std::exception("数字以外の文字が入っている、または不正な数値形式の文字列です。\nJSONの数値文字列は10進数で記述しなければなりません。");
				Char--;
			}else if(*Char==L't'||*Char==L'f'){
				Level.top()->Array().push_back(new Item(ParseBool(Char)));
			}else if(*Char==L'{'){
				Item* Obj=new Item(Hash());
				Level.top()->Array().push_back(Obj);
				Level.push(Obj);
			}else if(*Char==L'['){
				Item* Obj=new Item(Array());
				Level.top()->Array().push_back(Obj);
				Level.push(Obj);
			}else if(*Char==L'n'){
				Level.top()->Array().push_back(ParseNull(Char)?new Item():(throw std::exception(),nullptr));
			}else if(*Char==L']') Level.pop();
		}
	}while(Level.size()!=0);
	return Root;
}