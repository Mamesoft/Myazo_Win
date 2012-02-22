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
	typedef std::map<std::wstring,Item> JsonHash;
	typedef std::vector<Item> JsonArray;
	typedef std::wstring JsonString;

	class Item
	{
	private:
		std::shared_ptr<long long> Int_Value;
		std::shared_ptr<double> Double_Value;
		std::shared_ptr<bool> Bool_Value;
		std::shared_ptr<JsonString> String_Value;
		std::shared_ptr<JsonHash> Hash_Value;
		std::shared_ptr<JsonArray> Array_Value;
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
		Item(const JsonString& String);
		Item(const JsonHash& Hash);
		Item(const JsonArray& Array);
		Item(long long&& Int);
		Item(double&& Double);
		Item(bool&& Bool);
		Item(JsonString&& String);
		Item(JsonHash&& Hash);
		Item(JsonArray&& Array);
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
		const std::wstring& String(void)const;
		JsonHash& Hash(void);
		const JsonHash& Hash(void)const;
		JsonArray& Array(void);
		const JsonArray& Array(void)const;
		Item& operator=(const Item& LeftRef);
		Item& operator=(Item&& RightRef);
		bool operator==(const Item& LeftRef)const;
		bool operator!=(const Item& LeftRef)const;

	};

	class Parser
	{
	private:
		struct IteratorContainer
		{
			Type IteratorType;
			JsonHash::const_iterator Hash;
			JsonArray::const_iterator Array;
			IteratorContainer(JsonHash::const_iterator HashIterator)
			{
				IteratorType=Type::Hash;
				Hash=HashIterator;
				return;
			}
			IteratorContainer(JsonArray::const_iterator ArrayIterator)
			{
				IteratorType=Type::Array;
				Array=ArrayIterator;
				return;
			}
		};

		bool ParseBool(std::wstring::const_iterator& Char);
		std::wstring ParseString(std::wstring::const_iterator& Char);
		bool ParseNull(std::wstring::const_iterator& Char);
		std::wstring ToEscapeString(const std::wstring& String);

	public:
		std::wstring Create(const Item& Root);
		Item Parse(const std::wstring& JsonString);

	};
	
}