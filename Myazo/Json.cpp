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
	std::wostringstream Result;
	Result<<Int;
	return Result.str();
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

std::wstring Json::Json::Create(const Item& Root)
{
	union Iterator
	{
		Hash::const_iterator& Hash;
		Array::const_iterator& Array;
	};
	std::stack<Item&> Level;
	std::stack<Iterator> IteratorLevel;
	std::wstring JsonString;
	std::wostringstream Converter;
	Converter.setf(std::ios::scientific);
	JsonString.reserve(1000);
	IteratorLevel.push(Iterator());
	*Root->Hash().cbegin();
	if(Root!=nullptr){
		if(Root->Type()==Type::Hash){
			JsonString+=L'{';
			IteratorLevel.top().Hash=Root->Hash().cbegin();
		}else if(Root->Type()==Type::Array){
			JsonString+=L'[';
			IteratorLevel.top().Array=Root->Array().cbegin();
		}else throw std::exception("配列又は連想配列を表す型は、Json::Array\n又はJson::Hashでなければなりません。");
		Level.push(Root);
		while(Level.size()>0){
			Type ObjType=Level.top()->Type();
			if(ObjType==Type::Hash){
				if(IteratorLevel.top().Hash!=Level.top()->Hash().cend()){
					JsonString+=L'\"';
					JsonString+=(*IteratorLevel.top().Hash).first;
					JsonString+=L"\":";
					Item& Member=(*IteratorLevel.top().Hash).second;
					Type MemberType=Member?Member->Type():Type::Null;
					if(MemberType==Type::Null) JsonString+=L"null";
					else if(MemberType==Type::Int) Converter<<Member->Int(),JsonString+=Converter.str(),Converter.str(std::wstring());
					else if(MemberType==Type::Double) Converter<<Member->Double(),JsonString+=Converter.str(),Converter.str(std::wstring());
					else if(MemberType==Type::Bool) JsonString+=Member->Bool()?L"true":L"false";
					else if(MemberType==Type::String) JsonString+=ToEscapedString(Member->String());
					else if(MemberType==Type::Hash){
						JsonString+=L'{';
						Level.push(Member);
						IteratorLevel.top().Hash++;
						IteratorLevel.push(Iterator());
						IteratorLevel.top().Hash=Member->Hash().cbegin();
						continue;
					}else if(MemberType==Type::Array){
						JsonString+=L'[';
						Level.push(Member);
						IteratorLevel.top().Hash++;
						IteratorLevel.push(Iterator());
						IteratorLevel.top().Array=Member->Array().cbegin();
						continue;
					}else throw std::exception("オブジェクトの型が数値(long long,double)、\n論理値(bool)、文字列(std::wstring)、配列(Json::Array)、\n連想配列(Json::Hash)以外の型です。");
					JsonString+=L',';
					IteratorLevel.top().Hash++;
				}else{
					JsonString[JsonString.length()-1]=L'}';
					Level.pop();
					IteratorLevel.pop();
					if(Level.size()>0) JsonString+=L",";
				}
			}else if(ObjType==Type::Array){
				if(IteratorLevel.top().Array!=Level.top()->Array().cend()){
					Item& Member=*IteratorLevel.top().Array;
					Type MemberType=Member?Member->Type():Type::Null;
					if(MemberType==Type::Null) JsonString+=L"null";
					else if(MemberType==Type::Int) Converter<<Member->Int(),JsonString+=Converter.str(),Converter.str(std::wstring());
					else if(MemberType==Type::Double) Converter<<Member->Double(),JsonString+=Converter.str(),Converter.str(std::wstring());
					else if(MemberType==Type::Bool) JsonString+=Member->Bool()?L"true":L"false";
					else if(MemberType==Type::String) JsonString+=ToEscapedString(Member->String());
					else if(MemberType==Type::Hash){
						JsonString+=L'{';
						Level.push(Member);
						IteratorLevel.top().Array++;
						IteratorLevel.push(Iterator());
						IteratorLevel.top().Hash=Member->Hash().cbegin();
						continue;
					}else if(MemberType==Type::Array){
						JsonString+=L'[';
						Level.push(Member);
						IteratorLevel.top().Array++;
						IteratorLevel.push(Iterator());
						IteratorLevel.top().Array=Member->Array().cbegin();
						continue;
					}else throw std::exception("オブジェクトの型が数値(long long,double)、\n"
						"論理値(bool)、文字列(std::wstring)、配列(Json::Array)、\n連想配列(Json::Hash)以外の型です。");
					JsonString+=L',';
					IteratorLevel.top().Array++;
				}else{
					JsonString[JsonString.length()-1]=L']';
					Level.pop();
					IteratorLevel.pop();
					if(Level.size()>0) JsonString+=L',';
				}
			}else throw std::exception("配列又は連想配列を表す型は、Json::Array\n又はJson::Hashでなければなりません。");
		}
	}
	return JsonString;
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
	}while(Level.size()!=0&&Char!=JsonString.cend());
	return Root;
}
