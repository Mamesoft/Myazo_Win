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
		Item(const unsigned long long& Int);
		Item(const int& Int);
		Item(const unsigned int& Int);
		Item(const double& Double);
		Item(const float& Float);
		Item(const bool& Bool);
		Item(const JsonString& String);
		Item(const JsonHash& Hash);
		Item(const JsonArray& Array);
		Item(long long&& Int);
		Item(unsigned long long&& Int);
		Item(int&& Int);
		Item(unsigned int&& Int);
		Item(double&& Double);
		Item(float&& Float);
		Item(bool&& Bool);
		Item(JsonString&& String);
		Item(JsonHash&& Hash);
		Item(JsonArray&& Array);
		~Item(void);
		Type Type(void)const;
		bool IsNull(void)const;
		long long& Int(void);
		double& Double(void);
		bool& Bool(void);
		std::wstring& String(void);
		JsonHash& Hash(void);
		JsonArray& Array(void);
		const long long& Int(void)const;
		const double& Double(void)const;
		const bool& Bool(void)const;
		const std::wstring& String(void)const;
		const JsonHash& Hash(void)const;
		const JsonArray& Array(void)const;
		Item& operator=(const Item& LeftRef);
		Item& operator=(Item&& RightRef);
		operator long long(void);
		operator unsigned long long(void);
		operator int(void);
		operator unsigned int(void);
		operator double(void);
		operator float(void);
		operator bool(void);
		operator JsonString(void);
		operator JsonHash(void);
		operator JsonArray(void);
		operator const long long(void)const;
		operator const unsigned long long(void)const;
		operator const int(void)const;
		operator const unsigned int(void)const;
		operator const double(void)const;
		operator const float(void)const;
		operator const bool(void)const;
		operator const JsonString(void)const;
		operator const JsonHash(void)const;
		operator const JsonArray(void)const;
		Item& operator()(const std::wstring& Key);
		Item& operator()(const wchar_t*& Key);
		Item& operator()(const int& Index);
		const Item& operator()(const std::wstring& Key)const;
		const Item& operator()(const wchar_t*& Key)const;
		const Item& operator()(const int& Index)const;

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

	bool operator==(const Item& Left,const Item& Right);
	bool operator!=(const Item& Left,const Item& Right);

}