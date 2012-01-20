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

		long long* Int;
		double* Double;
		bool* Bool;
		std::wstring* String;
		std::map<std::wstring,Item>* Hash;
		std::vector<Item>* Array;


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
		

	};
}