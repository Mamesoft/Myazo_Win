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

	class Item
	{
	private:
		long long* Int_Value;
		double* Double_Value;
		bool* Bool_Value;
		std::wstring* String_Value;
		std::map<std::wstring,Item*>* Hash_Value;
		std::vector<Item*>* Array_Value;
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
		Item(const std::map<std::wstring,Item*>& Hash);
		Item(const std::vector<Item*>& Array);
		Item(long long&& Int);
		Item(double&& Double);
		Item(bool&& Bool);
		Item(std::wstring&& String);
		Item(std::map<std::wstring,Item*>&& Hash);
		Item(std::vector<Item*>&& Array);
		~Item(void);
		Type Type(void);
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
		bool ParseNull(std::wstring NullString);

		std::wstring ToString(long long Int);
		std::wstring ToString(double Double);
		std::wstring ToString(bool Bool);
		std::wstring ToString(std::wstring String);
		std::wstring ToString(void);

	public:
		std::wstring Create(Item* Root);
		Item* Parse(std::wstring JsonString);

	};
	
}