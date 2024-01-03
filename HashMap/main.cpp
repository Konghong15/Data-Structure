#include <iostream>
#include <unordered_map>
#include <chrono>
#include <vector>

#include "HashMap.h"

std::string MakeRandomString();
void printDurantion(std::chrono::nanoseconds nano);
void test_insert();
void test_find();
void calculate_insert(size_t mapSize, size_t elementCount);
void calculate_Find(size_t mapSize, size_t elementCount);

int main(void)
{
	test_insert();
	test_find();

	const size_t MAP_SIZE = 977;
	const size_t ELEMENT_COUNT = 500;
	std::cout << "===Test Info===" << std::endl;
	std::cout << "MAP_SIZE: " << MAP_SIZE << std::endl;
	std::cout << "ELEMENT_COUNT : " << ELEMENT_COUNT << std::endl;
	std::cout << "\n";

	calculate_insert(MAP_SIZE, ELEMENT_COUNT);
	std::cout << "\n";
	calculate_Find(MAP_SIZE, ELEMENT_COUNT);

	return 0;
}

std::string MakeRandomString()
{
	const size_t ALPHABET_SIZE = 26;
	const size_t MAX_LENGTH = 64;

	size_t length = rand() % MAX_LENGTH;

	std::string result;

	for (size_t i = 0; i < length; ++i)
	{
		size_t offset = rand() % ALPHABET_SIZE;
		result.push_back('a' + offset);
	}
	//result.push_back('\0');

	return result;
}

void printDurantion(std::chrono::nanoseconds nano)
{
	using namespace std::chrono;

	microseconds micro = duration_cast<microseconds>(nano);
	milliseconds milli = duration_cast<milliseconds>(nano);;

	std::cout << "경과시간(나노초): " << nano.count() << std::endl;
	std::cout << "경과시간(마크초): " << micro.count() << std::endl;
	std::cout << "경과시간(밀리초): " << milli.count() << std::endl;
}

void test_insert()
{
	constexpr const size_t mapSize = 991;
	std::vector<std::string> stringArr(100);

	for (int i = 0; i < stringArr.size(); ++i)
	{
		stringArr[i] = MakeRandomString();
	}

	hong::HashMap<std::string, int> myHashMap(mapSize);

	for (int i = 0; i < stringArr.size(); ++i)
	{
		myHashMap.Insert(stringArr[i], i);
	}

	size_t firstCount = myHashMap.GetCount();

	for (int i = 0; i < stringArr.size(); ++i)
	{
		myHashMap.Insert(stringArr[i], i);
	}

	size_t secondCount = myHashMap.GetCount();

	assert(firstCount == secondCount);
}

void test_find()
{
	constexpr const size_t mapSize = 991;
	std::vector<std::string> stringArr(100);

	for (int i = 0; i < stringArr.size(); ++i)
	{
		stringArr[i] = MakeRandomString();
	}

	hong::HashMap<std::string, int> myHashMap(mapSize);

	for (int i = 0; i < stringArr.size(); ++i)
	{
		myHashMap.Insert(stringArr[i], i);
	}

	for (int i = 0; i < stringArr.size(); ++i)
	{
		int outValue;
		myHashMap.Find(stringArr[i], &outValue);

		assert(outValue == i);
	}
}

void calculate_insert(size_t mapSize, size_t elementCount)
{
	using namespace std::chrono;

	std::vector<std::string> stringArr(elementCount);

	for (int i = 0; i < stringArr.size(); ++i)
	{
		stringArr[i] = MakeRandomString();
	}

	system_clock::time_point start;
	system_clock::time_point end;

	{
		hong::HashMap<std::string, int> myHashMap(mapSize);
		start = system_clock::now();

		for (int i = 0; i < stringArr.size(); ++i)
		{
			myHashMap.Insert(stringArr[i], i);
		}

		end = system_clock::now();

		nanoseconds nano = end - start;

		std::cout << "hong hashmap Insert" << std::endl;
		printDurantion(nano);
	}

	{
		std::unordered_map<std::string, int> unorderedMap;
		unorderedMap.reserve(mapSize);

		start = system_clock::now();

		for (int i = 0; i < stringArr.size(); ++i)
		{
			unorderedMap.insert({ stringArr[i], i });
		}

		end = system_clock::now();

		nanoseconds nano = end - start;

		std::cout << "std hashmap Insert" << std::endl;
		printDurantion(nano);
	}
}

void calculate_Find(size_t mapSize, size_t elementCount)
{
	using namespace std::chrono;

	std::vector<std::string> stringArr(elementCount);

	for (int i = 0; i < stringArr.size(); ++i)
	{
		stringArr[i] = MakeRandomString();
	}

	system_clock::time_point start;
	system_clock::time_point end;

	{
		hong::HashMap<std::string, int> myHashMap(mapSize);

		for (int i = 0; i < stringArr.size(); ++i)
		{
			myHashMap.Insert(stringArr[i], i);
		}

		start = system_clock::now();

		for (int i = 0; i < stringArr.size(); ++i)
		{
			int outValue;
			myHashMap.Find(stringArr[i], &outValue);
		}

		end = system_clock::now();

		nanoseconds nano = end - start;

		std::cout << "hong hashmap Find" << std::endl;
		printDurantion(nano);
	}

	{
		std::unordered_map<std::string, int> unorderedMap;
		unorderedMap.reserve(mapSize);

		for (int i = 0; i < stringArr.size(); ++i)
		{
			unorderedMap.insert({ stringArr[i], i });
		}

		start = system_clock::now();

		for (int i = 0; i < stringArr.size(); ++i)
		{
			auto find = unorderedMap.find(stringArr[i]);
		}

		end = system_clock::now();

		nanoseconds nano = end - start;

		std::cout << "std hashmap Find" << std::endl;
		printDurantion(nano);
	}
}