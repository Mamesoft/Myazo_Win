#include "Json.h"

namespace Json
{
	Item::Item(void)
	{
		IsNull_Value=true;
		Type_Value=Type::Null;
		return;
	}

	Item::Item(const Item& LeftRef)
	{
		Type_Value=LeftRef.Type();
		switch(Type_Value){
		case Type::Int:
			Int_Value=LeftRef.Int_Value;
			break;
		case Type::Double:
			Double_Value=LeftRef.Double_Value;
			break;
		case Type::Bool:
			Bool_Value=LeftRef.Bool_Value;
			break;
		case Type::String:
			String_Value=LeftRef.String_Value;
			break;
		case Type::Hash:
			Hash_Value=LeftRef.Hash_Value;
			break;
		case Type::Array:
			Array_Value=LeftRef.Array_Value;
			break;
		}
		IsNull_Value=Type_Value==Type::Null?true:false;
		return;
	}

	Item::Item(Item&& RightRef)
	{
		Type_Value=RightRef.Type();
		switch(Type_Value){
		case Type::Int:
			Int_Value=std::move(RightRef.Int_Value);
			break;
		case Type::Double:
			Double_Value=std::move(RightRef.Double_Value);
			break;
		case Type::Bool:
			Bool_Value=std::move(RightRef.Bool_Value);
			break;
		case Type::String:
			String_Value=std::move(RightRef.String_Value);
			break;
		case Type::Hash:
			Hash_Value=std::move(RightRef.Hash_Value);
			break;
		case Type::Array:
			Array_Value=std::move(RightRef.Array_Value);
			break;
		}
		IsNull_Value=Type_Value==Type::Null?true:false;
		return;
	}

	Item::Item(Json::Type ItemType)
	{
		Type_Value=ItemType;
		switch(ItemType){
		case Type::Int:
			Int_Value.reset(new long long);
			break;
		case Type::Double:
			Double_Value.reset(new double);
			break;
		case Type::Bool:
			Bool_Value.reset(new bool);
			break;
		case Type::String:
			String_Value.reset(new Json::String);
			break;
		case Type::Hash:
			Hash_Value.reset(new Json::Hash);
			break;
		case Type::Array:
			Array_Value.reset(new Json::Array);
			break;
		}
		IsNull_Value=ItemType==Type::Null?true:false;
		return;
	}

	Item::Item(const long long& Int):Int_Value(new long long(Int))
	{
		Type_Value=Type::Int;
		return;
	}

	Item::Item(const double& Double):Double_Value(new double(Double))
	{
		Type_Value=Type::Double;
		return;
	}

	Item::Item(const bool& Bool):Bool_Value(new bool(Bool))
	{
		Type_Value=Type::Bool;
		return;
	}

	Item::Item(const Json::String& String):String_Value(new Json::String(String))
	{
		Type_Value=Type::String;
		return;
	}

	Item::Item(const Json::Hash& Hash):Hash_Value(new Json::Hash(Hash))
	{
		Type_Value=Type::Hash;
		return;
	}

	Item::Item(const Json::Array& Array):Array_Value(new Json::Array(Array))
	{
		Type_Value=Type::Array;
		return;
	}

	Item::Item(long long&& Int):Int_Value(new long long(std::move(Int)))
	{
		Type_Value=Type::Int;
		return;
	}

	Item::Item(double&& Double):Double_Value(new double(std::move(Double)))
	{
		Type_Value=Type::Double;
		return;
	}

	Item::Item(bool&& Bool):Bool_Value(new bool(std::move(Bool)))
	{
		Type_Value=Type::Bool;
		return;
	}

	Item::Item(Json::String&& String):String_Value(new Json::String(std::move(String)))
	{
		Type_Value=Type::String;
		return;
	}

	Item::Item(Json::Hash&& Hash):Hash_Value(new Json::Hash(std::move(Hash)))
	{
		Type_Value=Type::Hash;
		return;
	}

	Item::Item(Json::Array&& Array):Array_Value(new Json::Array(std::move(Array)))
	{
		Type_Value=Type::Array;
		return;
	}

	Type Item::Type(void)const
	{
		return Type_Value;
	}

	bool Item::IsNull(void)const
	{
		return IsNull_Value;
	}

	long long& Item::Int(void)
	{
		if(Int_Value) return *Int_Value;
		else throw std::exception();
	}

	double& Item::Double(void)
	{
		if(Double_Value) return *Double_Value;
		else throw std::exception();
	}

	bool& Item::Bool(void)
	{
		if(Bool_Value) return *Bool_Value;
		else throw std::exception();
	}

	String& Item::String(void)
	{
		if(String_Value) return *String_Value;
		else throw std::exception();
	}

	Hash& Item::Hash(void)
	{
		if(Hash_Value) return *Hash_Value;
		else throw std::exception();
	}

	Array& Item::Array(void)
	{
		if(Array_Value) return *Array_Value;
		else throw std::exception();
	}

	Item& Item::operator=(const Item& LeftRef)
	{
		Json::Type ItemType=LeftRef.Type();
		switch(ItemType){
		case Type::Int:
			Int_Value=LeftRef.Int_Value;
			break;
		case Type::Double:
			Double_Value=LeftRef.Double_Value;
			break;
		case Type::Bool:
			Bool_Value=LeftRef.Bool_Value;
			break;
		case Type::String:
			String_Value=LeftRef.String_Value;
			break;
		case Type::Hash:
			Hash_Value=LeftRef.Hash_Value;
			break;
		case Type::Array:
			Array_Value=LeftRef.Array_Value;
			break;
		}
		IsNull_Value=ItemType==Type::Null?true:false;
		return *this;
	}

	Item& Item::operator=(Item&& RightRef)
	{
		Type_Value=RightRef.Type();
		switch(Type_Value){
		case Type::Int:
			Int_Value=std::move(RightRef.Int_Value);
			break;
		case Type::Double:
			Double_Value=std::move(RightRef.Double_Value);
			break;
		case Type::Bool:
			Bool_Value=std::move(RightRef.Bool_Value);
			break;
		case Type::String:
			String_Value=std::move(RightRef.String_Value);
			break;
		case Type::Hash:
			Hash_Value=std::move(RightRef.Hash_Value);
			break;
		case Type::Array:
			Array_Value=std::move(RightRef.Array_Value);
			break;
		}
		IsNull_Value=Type_Value==Type::Null?true:false;
		return *this;
	}
}