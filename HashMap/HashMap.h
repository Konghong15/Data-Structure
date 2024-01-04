#pragma once

#include <string>
#include <vector>
#include <list>
#include <memory>
#include <functional>
#include <cassert>

namespace hong
{
	template <typename KeyType>
	struct Hash
	{
		size_t operator()(KeyType key)
		{
			assert(false);
			throw 0;
		};
	};

	template <>
	struct Hash<std::string>
	{
		size_t operator()(const std::string& key)
		{
			// FNV-1 hash https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
			const constexpr size_t PRIME = 16777619;
			const constexpr size_t OFFSET = 2166136261U;

			size_t hash = OFFSET;

			for (auto ch : key)
			{
				hash *= PRIME;
				hash ^= ch;
			}

			return hash;
		}
	};

	template <typename KeyType, typename ValueType>
	struct Data
	{
		size_t Hash; 
		KeyType Key;
		ValueType Value;
	};

	template <typename KeyType, typename ValueType, typename Hash = Hash<KeyType>>
	class HashMap
	{
	public:
		HashMap(size_t capacity);
		~HashMap() = default;

		bool Insert(KeyType key, ValueType value);
		bool Find(KeyType key, ValueType* outValue);

		size_t GetCount() const { return mCount; }

	private:
		size_t mCount;
		size_t mCapacity;

		std::vector<std::list<Data<KeyType, ValueType>>> mArr;
	};

	template <typename KeyType, typename ValueType, typename Hash>
	HashMap<KeyType, ValueType, Hash>::HashMap(size_t capacity)
		: mCount(0)
		, mCapacity(capacity)
		, mArr(capacity)
	{
		assert(mArr.size() == mCapacity);
	}

	template <typename KeyType, typename ValueType, typename Hash>
	bool HashMap<KeyType, ValueType, Hash>::Insert(KeyType key, ValueType value)
	{
		assert(mArr.size() == mCapacity);

		size_t hash = Hash()(key);
		size_t index = hash % mCapacity;
		std::list<Data<KeyType, ValueType>>& list = mArr[index];

		for (const Data<KeyType, ValueType>& element : list)
		{
			// value 동치비교 전에 hash끼리 비교하여 연산 속도를 증가시킴
			if (element.Hash != hash)
			{
				// hash가 같지 않으면 키값이 같을 수가 없음
				continue;
			}
			if (element.Key == key)
			{
				return false;
			}
		}

		list.push_back({ hash, key, value });
		++mCount;

		return true;
	}

	template <typename KeyType, typename ValueType, typename Hash>
	bool HashMap<KeyType, ValueType, Hash>::Find(KeyType key, ValueType* outValue)
	{
		assert(mArr.size() == mCapacity);

		size_t hash = Hash()(key);
		size_t index = hash % mCapacity;
		std::list<Data<KeyType, ValueType>>& list = mArr[index];

		for (const Data<KeyType, ValueType>& element : list)
		{
			if (element.Hash != hash)
			{
				continue;
			}
			if (element.Key == key)
			{
				*outValue = element.Value;
				return true;
			}
		}

		return false;
	}
}
