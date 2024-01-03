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
		enum { INVALID = -1 };

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
			// FNV-1 해시 참고 https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
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
		KeyType Key;
		ValueType Value;
	};

	template <typename KeyType, typename ValueType>
	class HashMap
	{
	public:
		HashMap(size_t capacity);
		~HashMap() = default;

		bool Insert(KeyType key, ValueType value);
		bool Find(KeyType key, ValueType* outValue);

		void SetHashFunction(std::function<size_t(KeyType)> hashFunc);

		size_t GetCount() const { return mCount; }

	private:
		std::function<size_t(KeyType)> mHashFunc;
		size_t mCount;
		size_t mCapacity;

		std::vector<std::list<Data<KeyType, ValueType>>> mArr;
	};

	template <typename KeyType, typename ValueType>
	HashMap<KeyType, ValueType>::HashMap(size_t capacity)
		: mHashFunc(Hash<KeyType>())
		, mCount(0)
		, mCapacity(capacity)
		, mArr(capacity)
	{
		assert(mArr.size() == mCapacity);
	}

	template <typename KeyType, typename ValueType>
	bool HashMap<KeyType, ValueType>::Insert(KeyType key, ValueType value)
	{
		assert(mArr.size() == mCapacity);

		size_t index = mHashFunc(key) % mCapacity;
		std::list<Data<KeyType, ValueType>>& list = mArr[index];

		for (const Data<KeyType, ValueType>& element : list)
		{
			if (element.Key == key)
			{
				return false;
			}
		}

		list.push_back({ key, value });
		++mCount;

		return true;
	}

	template <typename KeyType, typename ValueType>
	bool HashMap<KeyType, ValueType>::Find(KeyType key, ValueType* outValue)
	{
		assert(mArr.size() == mCapacity);

		size_t index = mHashFunc(key) % mCapacity;
		std::list<Data<KeyType, ValueType>>& list = mArr[index];

		for (const Data<KeyType, ValueType>& element : list)
		{
			if (element.Key == key)
			{
				*outValue = element.Value;
				return true;
			}
		}

		return false;
	}

	template <typename KeyType, typename ValueType>
	void HashMap<KeyType, ValueType>::SetHashFunction(std::function<size_t(KeyType)> hashFunc)
	{
		mHashFunc = hashFunc;
	}
}
