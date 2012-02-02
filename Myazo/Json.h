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
	typedef std::map<std::wstring,Item*> Hash;
	typedef std::vector<Item*> Array;

	class Item
	{
	private:
		long long* Int_Value;
		double* Double_Value;
		bool* Bool_Value;
		std::wstring* String_Value;
		Hash* Hash_Value;
		Array* Array_Value;
		Type Type_Value;
		bool IsNull_Value;

		Item(const Item&);
		void Clear(void);
		
	public:
		Item(void);
		Item(const long long& Int);
		Item(const double& Double);
		Item(const bool& Bool);
		Item(const std::wstring& String);
		Item(const Hash& Hash);
		Item(const Array& Array);
		Item(long long&& Int);
		Item(double&& Double);
		Item(bool&& Bool);
		Item(std::wstring&& String);
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

	};

	class Json
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
		std::wstring Create(Item* const& Root);
		Item* Parse(const std::wstring& JsonString);

	};
	
}