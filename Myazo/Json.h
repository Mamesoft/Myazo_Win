#include "ShareHeaders.h"


namespace Json
{
	enum ItemType
	{
		Int,
		Double,
		Bool,
		String,
		Hash,
		Array,
		Null
	};

	class Item
	{
	private:
		long long* Int_Value;
		double* Double_Value;
		bool* Bool_Value;
		std::wstring* String_Value;
		std::map<std::wstring,Item*>* Hash_Value;
		std::vector<Item*>* Array_Value;
		ItemType Type;
		bool IsNull_Value;

		Item(const Item&);
		
	public:
		Item(void);
		Item(long long& Int);
		Item(double& Double);
		Item(bool& Bool);
		Item(std::wstring& String);
		Item(std::map<std::wstring,Item*>& Hash);
		Item(std::vector<Item*>& Array);
		~Item(void);
		bool IsNull(void)const;
		long long& Int(void);
		double& Double(void);
		bool& Bool(void);
		std::wstring& String(void);
		std::map<std::wstring,Item*>& Hash(void);
		std::vector<Item*>& Array(void);

	};

	class Json
	{
	private:
		long long ParseInt(std::wstring IntString);
		double ParseDouble(std::wstring DoubleString);
		bool ParseBool(std::wstring BoolString);
		std::wstring ParseString(std::wstring String);
		std::nullptr_t ParseNull(std::wstring NullString);

		std::wstring ToString(long long Int);
		std::wstring ToString(double Double);
		std::wstring ToString(bool Bool);
		std::wstring ToString(std::wstring String);
		std::wstring ToString(std::nullptr_t Null);

	public:
		std::wstring Create(Item* Root);
		Item* Parse(std::wstring JsonString);

	};
	
}