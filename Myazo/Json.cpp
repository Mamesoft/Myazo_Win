#include "Json.h"

namespace Json
{
	bool Parser::ParseBool(std::wstring::const_iterator& Char)
	{
		auto Start=Char;
		for(;;Char++) if(*Char==L' '||*Char==L'\t'||*Char==L'\n'||*Char==L'\r'||*Char==L','||*Char==L'}'||*Char==L']') break;
		std::wstring BoolString(Start,Char);
		if(!BoolString.compare(L"true")) return true;
		else if(!BoolString.compare(L"false")) return false;
		throw std::exception();
	}

	std::wstring Parser::ParseString(std::wstring::const_iterator& Char)
	{
		std::wstring String;
		return String;
	}

	bool Parser::ParseNull(std::wstring::const_iterator& Char)
	{
		auto Start=Char;
		for(;;Char++) if(*Char==L' '||*Char==L'\t'||*Char==L'\n'||*Char==L'\r'||*Char==L','||*Char==L'}'||*Char==L']') break;
		std::wstring NullString(Start,Char);
		if(!NullString.compare(L"null")) return true;
		else return false;
	}

	std::wstring Parser::ToEscapeString(const std::wstring& String)
	{
		return String;
	}

	std::wstring Parser::Create(const Item& Root)
	{
		std::stack<const Item&> Level;
		std::stack<Iterator> IteratorLevel;
		std::wstring JsonString;
		std::wostringstream Converter;
		Converter.precision(10);
		Converter.setf(std::ios::scientific);
		JsonString.reserve(1000);
		IteratorLevel.push(Iterator());
		if(Root.Type()==Type::Hash){
			JsonString+=L'{';
			IteratorLevel.top().Hash=Root.Hash().cbegin();
		}else if(Root.Type()==Type::Array){
			JsonString+=L'[';
			IteratorLevel.top().Array=Root.Array().cbegin();
		}else if(Root.Type()==Type::Null) return JsonString;
		else throw std::exception("配列又は連想配列を表す型は、Json::Array\n又はJson::Hashでなければなりません。");
		Level.push(Root);
		while(Level.size()>0){
			Type ObjType=Level.top().Type();
			if(ObjType==Type::Hash){
				if(IteratorLevel.top().Hash!=Level.top().Hash().cend()){
					JsonString+=L'\"';
					JsonString+=(*IteratorLevel.top().Hash).first;
					JsonString+=L"\":";
					const Item& Member=(*IteratorLevel.top().Hash).second;
					switch(Member.Type()){
					case Type::Null:
						JsonString+=L"null";
						break;
					case Type::Int:
						Converter<<Member.Int();
						JsonString+=Converter.str();
						Converter.str(String());
						break;
					case Type::Double:
						Converter<<Member.Double();
						JsonString+=Converter.str();
						Converter.str(String());
						break;
					case Type::Bool:
						JsonString+=Member.Bool()?L"true":L"false";
						break;
					case Type::String:
						JsonString+=ToEscapeString(Member.String());
						break;
					case Type::Hash:
						JsonString+=L'{';
						Level.push(Member);
						IteratorLevel.top().Hash++;
						IteratorLevel.push(Iterator());
						IteratorLevel.top().Hash=Member.Hash().cbegin();
						continue;
					case Type::Array:
						JsonString+=L'[';
						Level.push(Member);
						IteratorLevel.top().Hash++;
						IteratorLevel.push(Iterator());
						IteratorLevel.top().Array=Member.Array().cbegin();
						continue;
					}
					JsonString+=L',';
					IteratorLevel.top().Hash++;
				}else{
					JsonString[JsonString.length()-1]=L'}';
					Level.pop();
					IteratorLevel.pop();
					if(Level.size()>0) JsonString+=L",";
				}
			}else if(ObjType==Type::Array){
				if(IteratorLevel.top().Array!=Level.top().Array().cend()){
					const Item& Member=*IteratorLevel.top().Array;
					switch(Member.Type()){
					case Type::Null:
						JsonString+=L"null";
						break;
					case Type::Int:
						Converter<<Member.Int();
						JsonString+=Converter.str();
						Converter.str(String());
						break;
					case Type::Double:
						Converter<<Member.Double();
						JsonString+=Converter.str();
						Converter.str(String());
						break;
					case Type::Bool:
						JsonString+=Member.Bool()?L"true":L"false";
						break;
					case Type::String:
						JsonString+=ToEscapeString(Member.String());
						break;
					case Type::Hash:
						JsonString+=L'{';
						Level.push(Member);
						IteratorLevel.top().Array++;
						IteratorLevel.push(Iterator());
						IteratorLevel.top().Hash=Member.Hash().cbegin();
						continue;
					case Type::Array:
						JsonString+=L'[';
						Level.push(Member);
						IteratorLevel.top().Array++;
						IteratorLevel.push(Iterator());
						IteratorLevel.top().Array=Member.Array().cbegin();
						continue;
					}
					JsonString+=L',';
					IteratorLevel.top().Hash++;
				}else{
					JsonString[JsonString.length()-1]=L']';
					Level.pop();
					IteratorLevel.pop();
					if(Level.size()>0) JsonString+=L',';
				}
			}else throw std::exception("配列又は連想配列を表す型は、Json::Array\n又はJson::Hashでなければなりません。");
		}
		return JsonString;
	}

	Item Parser::Parse(const std::wstring& JsonString)
	{
		auto Char=JsonString.cbegin();
		std::stack<Item&> Level;
		Item Root;
		std::wistringstream Converter;
		Converter.precision(10);
		do{
			if(*Char==L'{') Root=Item(Type::Hash);
			else if(*Char==L'[') Root=Item(Type::Array);
			else if(*Char!=L' '&&*Char!=L'\t'&&*Char!=L'\n'&&*Char!=L'\r') throw std::exception("不正な文字が含まれています。");
		}while(Root.Type()==Type::Null&&++Char!=JsonString.cend());
		Level.push(Root);
		do{
			Char++;
			if(*Char==L','||*Char==L' '||*Char==L'\t'||*Char==L'\n'||*Char==L'\r') continue;
			Type ObjType=Level.top().Type();
			if(ObjType==Type::Hash){
				std::wstring Key;
				if(*Char==L'\"'){
					Key=ParseString(Char);
					for(;*Char!=L':';Char++) continue;
					for(Char++;*Char==L' '||*Char==L'\t'||*Char==L'\n'||*Char==L'\r';Char++) continue;
				}
				if(*Char==L'\"'){
					Level.top().Hash().insert(std::make_pair(Key,Item(ParseString(Char))));
				}else if(*Char==L'-'||(*Char>=L'0'&&*Char<=L'9')){
					auto Start=Char;
					for(;;Char++) if(*Char==L' '||*Char==L'\t'||*Char==L'\n'||*Char==L'\r'||*Char==L','||*Char==L'}'||*Char==L']') break;
					if(std::regex_match(Start,Char,std::wregex(L"-?\\d+"))){
						Item& Obj=Item(Type::Int);
						Converter.str(std::wstring(Start,Char));
						Converter>>Obj.Int();
						Level.top().Hash().insert(std::make_pair(Key,Obj));
					}else if(std::regex_match(Start,Char,std::wregex(L"-?\\d+\\.\\d+([eE][+-]\\d+)?"))){
						Item& Obj=Item(Type::Double);
						Converter.str(std::wstring(Start,Char));
						Converter>>Obj.Double();
						Level.top().Hash().insert(std::make_pair(Key,Obj));
					}else throw std::exception("数字以外の文字が入っている、または不正な数値形式の文字列です。\nJSONの数値文字列は10進数で記述しなければなりません。");
					Char--;
				}else if(*Char==L't'||*Char==L'f'){
					Level.top().Hash().insert(std::make_pair(Key,Item(ParseBool(Char))));
				}else if(*Char==L'{'){
					Item& Obj=Item(Type::Hash);
					Level.top().Hash().insert(std::make_pair(Key,Obj));
					Level.push(Obj);
				}else if(*Char==L'['){
					Item& Obj=Item(Type::Array);
					Level.top().Hash().insert(std::make_pair(Key,Obj));
					Level.push(Obj);
				}else if(*Char==L'n'){
					Level.top().Hash().insert(std::make_pair(Key,ParseNull(Char)?Item(Type::Null):throw std::exception()));
				}else if(*Char==L'}') Level.pop();
			}else if(ObjType==Type::Array){
				if(*Char==L'\"'){
					Level.top().Array().push_back(Item(ParseString(Char)));
				}else if(*Char==L'-'||(*Char>=L'0'&&*Char<=L'9')){
					auto Start=Char;
					for(;;Char++) if(*Char==L' '||*Char==L'\t'||*Char==L'\n'||*Char==L'\r'||*Char==L','||*Char==L'}'||*Char==L']') break;
					if(std::regex_match(Start,Char,std::wregex(L"-?\\d+"))){
						Item& Obj=Item(Type::Int);
						Converter.str(std::wstring(Start,Char));
						Converter>>Obj.Int();
						Level.top().Array().push_back(Obj);
					}else if(std::regex_match(Start,Char,std::wregex(L"-?\\d+\\.\\d+([eE][+-]\\d+)?"))){
						Item& Obj=Item(Type::Double);
						Converter.str(std::wstring(Start,Char));
						Converter>>Obj.Double();
						Level.top().Array().push_back(Obj);
					}else throw std::exception("数字以外の文字が入っている、または不正な数値形式の文字列です。\nJSONの数値文字列は10進数で記述しなければなりません。");
					Char--;
				}else if(*Char==L't'||*Char==L'f'){
					Level.top().Array().push_back(Item(ParseBool(Char)));
				}else if(*Char==L'{'){
					Item& Obj=Item(Type::Hash);
					Level.top().Array().push_back(Obj);
					Level.push(Obj);
				}else if(*Char==L'['){
					Item& Obj=Item(Type::Array);
					Level.top().Array().push_back(Obj);
					Level.push(Obj);
				}else if(*Char==L'n'){
					Level.top().Array().push_back(ParseNull(Char)?Item(Type::Null):throw std::exception());
				}else if(*Char==L']') Level.pop();
			}
		}while(Level.size()!=0&&Char!=JsonString.cend());
		return Root;
	}
}