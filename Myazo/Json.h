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
		std::shared_ptr<Hash> Hash_Value;
		std::shared_ptr<Array> Array_Value;
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
		Item(const Hash& Hash);
		Item(const Array& Array);
		Item(long long&& Int);
		Item(double&& Double);
		Item(bool&& Bool);
		Item(String&& String);
		Item(Hash&& Hash);
		Item(Array&& Array);
		~Item(void);
		Type Type(void)const;
		bool IsNull(void)const;
		long long& Int(void);
		double& Double(void);
		bool& Bool(void);
		std::wstring& String(void);
		Hash& Hash(void);
		Array& Array(void);
		Item& operator=(const Item& LeftRef);
		Item& operator=(Item&& RightRef);

	};


	class Parser
	{
	private:
		long long ParseInt(std::wstring::const_iterator& Char);
		double ParseDouble(std::wstring::const_iterator& Char);
		bool ParseBool(std::wstring::const_iterator& Char);
		std::wstring ParseString(std::wstring::const_iterator& Char);
		bool ParseNull(std::wstring::const_iterator& Char);

		std::wstring ToString(const long long Int);
		std::wstring ToString(const double Double);
		std::wstring ToString(const bool Bool);
		std::wstring ToEscapedString(const std::wstring String);
		std::wstring ToString(void);

	public:
		std::wstring Create(const Item& Root);
		Item Parse(const std::wstring& JsonString);

	};
	
}