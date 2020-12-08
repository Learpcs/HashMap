#include <iostream>
#include <string>
#include <random>
using namespace std;

//Примечания: 
//если не хватает размера, он вбрасывает исключение.
template <class key, class value> //БЕЗЗНАКОВЫЕ ТИПЫ И БЕЗ СТРОК - НЕ ЗАБЫТЬ СДЕЛАТЬ СПЕЦИАЛИЗАЦИИ 
class HashMap
{
	//вложенный класс пар
	template<class key, class value>
	struct pairs 
	{
		key Key;
		value Value;

		pairs& operator= (const value& value)
		{
			this->Value = value;
			return *this;
		}
	};

	pairs<key, value>* arr;
	size_t Size = 0;

	key KEY_MIN; //минимально значения типа или пустота если это строка
	value VALUE_MIN;

	size_t mod = 0;
	
	//Хэш-функции
	size_t HashFunction(const key& k) //O(log n).
	{
		string s;
		if constexpr (is_same_v<key, string>)
			s = k;
		else
			s = to_string(k);
		size_t n = s.size();
		unsigned long long sum = 0;
		for (size_t i = 0; i < n; i++)
			sum += s[i];
		return sum % mod;
	}

public:
	//Конструктор
	HashMap(const size_t& n)
	{
		Size = n, mod = n;
		if constexpr(is_same_v<key, string>)
			KEY_MIN = "";
		else
			KEY_MIN = -(1 << sizeof(key) * 8 - 1);
		if constexpr(is_same_v<value, string>)
			VALUE_MIN = "";
		else
			KEY_MIN = -(1 << sizeof(key) * 8 - 1);
		arr = new pairs<key, value>[n];
		for (size_t i = 0; i < n; i++)
		{
			arr[i].Key = KEY_MIN;
			arr[i].Value = VALUE_MIN; //по идее это не нужно
		}
	}

	void insert(const key& Key, const value& Value) //В среднем O(1), В худшем O(n)
	{
		size_t hash_index = HashFunction(Key), cycle_cnt = 0;
		do 
		{
			if (arr[hash_index].Key == KEY_MIN)
			{
				arr[hash_index].Key = Key;
				arr[hash_index].Value = Value;
				break;
			}
			hash_index++;
			if (hash_index == Size)
			{
				hash_index = 0;
				cycle_cnt++;
			}
		} while (cycle_cnt < 2);

		//обработка ошибок. (не хватило места)
		if(cycle_cnt == 2)
			throw "SmallSize";
	}

	void remove(const key& Key) //O(1) 
	{             
		size_t hash_index = HashFunction(Key);
		arr[hash_index].Key = KEY_MIN;
		arr[hash_index].Value = VALUE_MIN;
	}

	void reconstruct(size_t size) //стоит ли делать хз
	{
		
	}

	value& operator[](const key& i) //В среднем O(1), в худшем O(n) 
	{
		size_t hash_index = HashFunction(i), cycle_cnt = 0;
		do
		{
			if (arr[hash_index].Key == i)
			{
				return arr[hash_index].Value;
			}
			hash_index++;
			if (hash_index == Size)
			{
				hash_index = 0;
				cycle_cnt++;
			}
		} while (cycle_cnt < 2);

		//обработка ошибок. (элемент не найден)
		throw "NotFound";
	}

	size_t count_collisions() //O(n * log n)
	{
		size_t cnt = 0;
		for (size_t i = 0; i < Size; i++)
			if (arr[i].Key != KEY_MIN && i != HashFunction(arr[i].Key))
				cnt++;
		return cnt;
	}

	~HashMap()
	{
		delete[] arr;
	}
};



int main()
{
	HashMap<string, int> prices(10);
	prices.insert("апельсины", 100);
	cout << prices["апельсины"];
}