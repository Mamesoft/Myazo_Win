#include "ShareHeaders.h"


namespace Json
{
	enum Type
	{
		Int,
		Double,
		Bool,
		String,
		Hash,
		Array,
		Null
	};

	class Item;
	typedef std::map<std::wstring,Item> Hash;
	typedef std::vector<Item> Array;
	typedef std::wstring String;

	class Item
	{
	private:
		std::shared_ptr<long long> Int_Value;
		std::shared_ptr<double> Double_Value;
		std::shared_ptr<bool> Bool_Value;
		std::shared_ptr<std::wstring> String_Value;
<<<<<<< HEAD
		std::shared_ptr<Json::Hash> Hash_Value;
		std::shared_ptr<Json::Array> Array_Value;
=======
		std::shared_ptr<Hash> Hash_Value;
		std::shared_ptr<Array> Array_Value;
>>>>>>> origin/master
		Type Type_Value;
		bool IsNull_Value;
		
	public:
		Item(void);
		Item(const Item& LeftRef);
		Item(Item&& RightRef);
		Item(Type ItemType);
		Item(const long long& Int);
		Item(const double& Double);
		Item(const bool& Bool);
		Item(const String& String);
<<<<<<< HEAD
		Item(const Json::Hash& Hash);
		Item(const Json::Array& Array);
=======
		Item(const Hash& Hash);
		Item(const Array& Array);
>>>>>>> origin/master
		Item(long long&& Int);
		Item(double&& Double);
		Item(bool&& Bool);
		Item(String&& String);
<<<<<<< HEAD
		Item(Json::Hash&& Hash);
		Item(Json::Array&& Array);
=======
		Item(Hash&& Hash);
		Item(Array&& Array);
>>>>>>> origin/master
		~Item(void);
		Type Type(void)const;
		bool IsNull(void)const;
		long long& Int(void);
		const long long& Int(void)const;
		double& Double(void);
		const double& Double(void)const;
		bool& Bool(void);
		const bool& Bool(void)const;
		std::wstring& String(void);
<<<<<<< HEAD
		const std::wstring& String(void)const;
		Json::Hash& Hash(void);
		const Json::Hash& Hash(void)const;
		Json::Array& Array(void);
		const Json::Array& Array(void)const;

=======
		Hash& Hash(void);
		Array& Array(void);
>>>>>>> origin/master
		Item& operator=(const Item& LeftRef);
		Item& operator=(Item&& RightRef);

	};


	class Parser
	{
	private:
		union Iterator
		{
			Hash::const_iterator& Hash;
			Array::const_iterator& Array;
		};

		long long ParseInt(std::wstring::const_iterator& Char);
		double ParseDouble(std::wstring::const_iterator& Char);
		bool ParseBool(std::wstring::const_iterator& Char);
		std::wstring ParseString(std::wstring::const_iterator& Char);
		bool ParseNull(std::wstring::const_iterator& Char);
		std::wstring ToEscapeString(const std::wstring& String);

	public:
		std::wstring Create(const Item& Root);
		Item Parse(const std::wstring& JsonString);

	};
	
}
