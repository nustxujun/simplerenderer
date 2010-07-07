#ifndef _Any_H_
#define _Any_H_
#include "Prerequisites.h"
#include <typeinfo>
namespace RCP
{
	class Any
	{
	public:
		class Content
		{
		public:
			virtual ~Content()
			{}

			virtual Content* clone() const = 0;
			virtual size_t getSize()const = 0; 
			virtual void* getContentPtr() = 0;
		};

		template<typename Type>
		class RealContent:public Content
		{
		public:
			RealContent(const Type& value):
				mValue(value)
			{}

			virtual RealContent* clone() const 
			{
				return new RealContent(mValue);
			}

			virtual size_t getSize()const
			{
				return sizeof(mValue);
			}

			virtual void* getContentPtr()
			{
				return &mValue;
			}


			Type mValue;
		};
	public:
		Any():
			mContent(NULL)
		{}

		template<typename T>
		Any(const T& value):
			mContent(new RealContent<T>(value))
		{

		}

		Any(const Any& any):
			mContent(any.mContent?any.mContent->clone():NULL)
		{

		}

		~Any()
		{
			SAFE_DELETE(mContent);
		}

	public:
		Any& swap(Any& any)
		{
			std::swap(mContent,any.mContent);
			return *this;
		}

		Any& operator=(const Any& any)
		{
			Any(any).swap(*this);
			return *this;
		}

		template<typename T>
		Any& operator=(const T& t)
		{
			Any(t).swap(*this);
			return *this;
		}

		bool isEmpty()const
		{
			return mContent == NULL;
		}

		void setEmpty()
		{
			SAFE_DELETE(mContent);
			
		}

		size_t getSize()const
		{
			return mContent->getSize();
		}

		void* getContentPtr()const
		{
			return mContent?mContent->getContentPtr():NULL;
		}


		Content* mContent;
	};

	template<typename T>
	T* any_cast(Any* any)
	{
		assert(any);
		const type_info& info = typeid(static_cast<Any::RealContent<T>*>(any->mContent)->mValue);
		if (info == typeid(T))
			return &static_cast<Any::RealContent<T>*>(any->mContent)->mValue;
		else
			return NULL;
	}

	template<typename T>
	const T* any_cast(const Any* any)
	{
		return any_cast<T>(const_cast<Any*>(any));
	}

	template<typename T>
	T any_cast(const Any& any)
	{
		const T* result = any_cast<T>(&any);
		if (result == NULL)
			assert(0);
		return *result;
	}



}
#endif//_Any_H_
