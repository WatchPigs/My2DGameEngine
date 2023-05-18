#pragma once

#include <assert.h>
#include <cstddef>
#include <stdint.h>

namespace Engine {

	typedef uint64_t	ref_counter_t;

	struct ReferenceCounters
	{

		ReferenceCounters(ref_counter_t i_InitialSmartReferences, ref_counter_t i_InitialWeakReferences) :
			m_SmartReferences(i_InitialSmartReferences),
			m_WeakReferences(i_InitialWeakReferences)
		{}

		ref_counter_t SmartRefs() const { return m_SmartReferences; }
		ref_counter_t IncSmartRefs() { return ++m_SmartReferences; }
		ref_counter_t DecSmartRefs() { return --m_SmartReferences; }

		ref_counter_t WeakRefs() const { return m_WeakReferences; }
		ref_counter_t IncWeakRefs() { return ++m_WeakReferences; }
		ref_counter_t DecWeakRefs() { return --m_WeakReferences; }
	private:
		ref_counter_t		m_SmartReferences;
		ref_counter_t		m_WeakReferences;
	};

	template<class T, class DestroyT>
	class WeakPtr;

	template<typename T>
	class StandardDelete
	{
	public:
		static void release(T* i_ptr) { delete i_ptr; }
	};

	template<class T, class DestroyT = StandardDelete<T>>
	class SmartPtr
	{
		template<class U, class DestroyU>
		friend class WeakPtr;

		template<class U, class DestroyU>
		friend class SmartPtr;

	public:
		SmartPtr() :
			pCounters(nullptr),
			pPtr(nullptr)
		{}

		explicit SmartPtr(T* i_ptr) :
			pCounters(i_ptr ? new ReferenceCounters(1, 0) : nullptr),
			pPtr(i_ptr)
		{}

		// Copy Constructor
		SmartPtr(const SmartPtr& i_other) :
			pCounters(i_other.pCounters),
			pPtr(i_other.pPtr)
		{
			if (pCounters)
			{
				assert(pPtr != nullptr);
				pCounters->IncSmartRefs();
			}
			else
			{
				assert(pPtr == nullptr);
			}
		}

		template<class U>
		SmartPtr(const SmartPtr<U>& i_other) :
			pCounters(i_other.pCounters),
			pPtr(i_other.pPtr)
		{
			if (pCounters)
			{
				assert(pPtr != nullptr);
				pCounters->IncSmartRefs();
			}
			else
			{
				assert(pPtr == nullptr);
			}
		}

		// Move Copy Constructor
		SmartPtr(SmartPtr&& i_other) noexcept :
			pCounters(i_other.pCounters),
			pPtr(i_other.pPtr)
		{
			i_other.pCounters = nullptr;
			i_other.pPtr = nullptr;
		}

		template<class U>
		SmartPtr(SmartPtr<U>&& i_other) :
			pCounters(i_other.pCounters),
			pPtr(i_other.pPtr)
		{
			i_other.pCounters = nullptr;
			i_other.pPtr = nullptr;
		}

		SmartPtr(WeakPtr<T, DestroyT>&& i_other) :
			pCounters(i_other.pCounters),
			pPtr(i_other.pPtr)
		{
			i_other.pCounters = nullptr;
			i_other.pPtr = nullptr;
		}

		template<class U, class DestroyU>
		SmartPtr(WeakPtr<U, DestroyU>&& i_other) :
			pCounters(i_other.pCounters),
			pPtr(i_other.pPtr)
		{
			i_other.pCounters = nullptr;
			i_other.pPtr = nullptr;
		}

		// Copy Constructor - From WeakPtr
		SmartPtr(const WeakPtr<T, DestroyT>& i_other) :
			pCounters((i_other.pCounters && (i_other.pCounters->SmartRefs() > 0)) ? i_other.pCounters : nullptr),
			pPtr((i_other.pCounters && (i_other.pCounters->SmartRefs() > 0)) ? i_other.pPtr : nullptr)
		{
			IncrementOwnership();
		}

		template<class U, class DestroyU>
		SmartPtr(const WeakPtr<U, DestroyU>& i_other) :
			pCounters((i_other.pCounters && (i_other.pCounters->SmartRefs() > 0)) ? i_other.pCounters : nullptr),
			pPtr((i_other.pCounters && (i_other.pCounters->SmartRefs() > 0)) ? i_other.pPtr : nullptr)
		{
			IncrementOwnership();
		}

		// Assignment Operator
		SmartPtr& operator=(const SmartPtr& i_other)
		{
			if (pPtr != i_other.pPtr)
			{
				ReleaseOwnership();

				pCounters = i_other.pCounters;
				pPtr = i_other.pPtr;

				IncrementOwnership();
			}

			return *this;
		}

		template<class U>
		SmartPtr& operator=(const SmartPtr<U>& i_other)
		{
			if (pPtr != i_other.pPtr)
			{
				ReleaseOwnership();

				pCounters = i_other.pCounters;
				pPtr = i_other.pPtr;

				IncrementOwnership();
			}

			return *this;
		}

		// Move Assignment Operator
		template<class U, class DestroyU>
		SmartPtr& operator=(SmartPtr<U, DestroyU>&& i_other)
		{
			if (pPtr != i_other.pPtr)
			{
				ReleaseOwnership();

				pCounters = i_other.pCounters;
				pPtr = i_other.pPtr;

				i_other.pCounters = nullptr;
				i_other.pPtr = nullptr;
			}

			return *this;
		}

		// Assignment Operator - Converting
		SmartPtr& operator=(const WeakPtr<T, DestroyT>& i_other)
		{
			if (pPtr != i_other.pPtr)
			{
				ReleaseOwnership();
				pCounters = (i_other.pCounters && (i_other.pCounters->SmartRefs() > 0)) ? i_other.pCounters : nullptr;
				pPtr = (i_other.pCounters && (i_other.pCounters->SmartRefs() > 0)) ? i_other.pPtr : nullptr;

				IncrementOwnership();
			}

			return *this;
		}

		template<class U, class DestroyU>
		SmartPtr& operator=(const WeakPtr<U, DestroyU>& i_other)
		{
			if (pPtr != i_other.pPtr)
			{
				ReleaseOwnership();
				pCounters = (i_other.pCounters && (i_other.pCounters->SmartRefs() > 0)) ? i_other.pCounters : nullptr;
				pPtr = (i_other.pCounters && (i_other.pCounters->SmartRefs() > 0)) ? i_other.pPtr : nullptr;

				IncrementOwnership();
			}

			return *this;
		}

		// Move Assignment Operator - Converting
		template<class U, class DestroyU>
		SmartPtr& operator=(WeakPtr<U, DestroyU>&& i_other)
		{
			if (pPtr != i_other.pPtr)
			{
				ReleaseOwnership();

				pCounters = i_other.pCounters;
				pPtr = i_other.pPtr;

				i_other.pCounters = nullptr;
				i_other.pPtr = nullptr;
			}

			return *this;
		}


		// Assignment Operator - null specific
		SmartPtr& operator=(std::nullptr_t i_null)
		{
			ReleaseOwnership();

			pCounters = nullptr;
			pPtr = nullptr;

			return *this;
		}

		// Assignment Operator - Type pointer direct
		SmartPtr& operator=(T* i_ptr)
		{
			if (pPtr != i_ptr)
			{
				ReleaseOwnership();

				pCounters = i_ptr ? new ReferenceCounters(1, 0) : nullptr;
				pPtr = i_ptr;
			}

			return *this;
		}

		~SmartPtr()
		{
			ReleaseOwnership();
		}

		template<class U>
		inline bool operator==(const SmartPtr<U>& i_other)
		{
			return pPtr == i_other.pPtr;
		}

		template<class U, class DestroyU>
		inline bool operator==(const WeakPtr<U, DestroyU>& i_other)
		{
			return pPtr == i_other.pPtr;
		}

		template<class U>
		inline bool operator!=(const SmartPtr<U>& i_other)
		{
			return pPtr != i_other.pPtr;
		}

		template<class U, class DestroyU>
		inline bool operator!=(const WeakPtr<U, DestroyU>& i_other)
		{
			return pPtr != i_other.pPtr;
		}

		template<class U>
		inline bool operator==(U* i_ptr) const
		{
			return pPtr == i_ptr;
		}

		inline bool operator==(std::nullptr_t nullp) const
		{
			return pPtr == nullptr;
		}

		inline operator bool() const
		{
			return pPtr != nullptr;
		}

		T* operator->()
		{
			return pPtr;
		}

		const T* operator->() const
		{
			return pPtr;
		}

		T& operator*()
		{
			assert(pPtr);

			return *pPtr;
		}

		const T& operator*() const
		{
			assert(pPtr);

			return *pPtr;
		}

	private:
		void ReleaseOwnership()
		{
			if (pCounters)
			{
				assert(pCounters->SmartRefs() > 0);
				if (pCounters->DecSmartRefs() == 0)
				{
					assert(pPtr);

					DestroyT::release(pPtr);
					pPtr = nullptr;

					if (pCounters->WeakRefs() == 0)
					{
						delete pCounters;
						pCounters = nullptr;
					}
				}
			}
			else
			{
				// originated a SmartPtr to nullptr. Still is
				assert(pPtr == nullptr);
			}
		}

		void IncrementOwnership()
		{
			if (pCounters)
			{
				// Started as or became an SmartPtr to something.
				assert(pCounters->SmartRefs() > 0);
				assert(pPtr != nullptr);

				pCounters->IncSmartRefs();
			}
			else
			{
				// originated a SmartPtr to nullptr. Still is
				assert(pPtr == nullptr);
			}
		}

		ReferenceCounters* pCounters;
		T* pPtr;
	};

	template<class T, class DestroyT = StandardDelete<T>>
	class WeakPtr
	{
		template<class U, class DestroyU>
		friend class SmartPtr;
		template<class U, class DestroyU>
		friend class WeakPtr;

	public:
		WeakPtr() :
			pCounters(nullptr),
			pPtr(nullptr)
		{
		}

		template<class U, class DestroyU>
		WeakPtr(const SmartPtr<U, DestroyU>& i_owner) :
			pCounters(i_owner.pCounters),
			pPtr(i_owner.pPtr)
		{
			if (pCounters)
			{
				assert(pCounters->WeakRefs() || pCounters->SmartRefs());
				pCounters->IncWeakRefs();
			}
			else
			{
				assert(i_owner.pPtr == nullptr);
			}
		}

		WeakPtr(const WeakPtr& i_owner) :
			pCounters(i_owner.pCounters),
			pPtr(i_owner.pPtr)
		{
			if (pCounters)
			{
				assert(pCounters->WeakRefs() || pCounters->SmartRefs());
				pCounters->IncWeakRefs();
			}
			else
			{
				assert(i_owner.pPtr == nullptr);
			}
		}

		template<class U, class DestroyU>
		WeakPtr(const WeakPtr<U, DestroyU>& i_owner) :
			pCounters(i_owner.pCounters),
			pPtr(i_owner.pPtr)
		{
			if (pCounters)
			{
				assert(pCounters->WeakRefs() || pCounters->SmartRefs());
				pCounters->IncWeakRefs();
			}
			else
			{
				assert(i_owner.pPtr == nullptr);
			}
		}

		~WeakPtr()
		{
			ReleaseObservership();
		}

		WeakPtr& operator=(const WeakPtr& i_other)
		{
			if (pPtr != i_other.pPtr)
			{
				ReleaseObservership();

				pCounters = i_other.pCounters;
				pPtr = i_other.pPtr;

				IncrementObservership();
			}

			return *this;
		}

		template<class U, class DestroyU>
		WeakPtr& operator=(const WeakPtr<U, DestroyU>& i_other)
		{
			if (pPtr != i_other.pPtr)
			{
				ReleaseObservership();

				pCounters = i_other.pCounters;
				pPtr = i_other.pPtr;

				IncrementObservership();
			}

			return *this;
		}

		template<class U>
		inline WeakPtr& operator=(const SmartPtr<U>& i_other)
		{
			if (pPtr != i_other.pPtr)
			{
				ReleaseObservership();

				pCounters = i_other.pCounters;
				pPtr = i_other.pPtr;

				IncrementObservership();
			}

			return *this;
		}

		WeakPtr<T, DestroyT>& operator=(std::nullptr_t i_null)
		{
			ReleaseObservership();

			pCounters = nullptr;
			pPtr = nullptr;

			return *this;
		}

		inline SmartPtr<T, DestroyT> AcquireOwnership()
		{
			return SmartPtr<T, DestroyT>(*this);
		}

		// comparison operators
		template<class U, class DestroyU>
		inline bool operator==(const SmartPtr<U, DestroyU>& i_other)
		{
			return pPtr == i_other.pPtr;
		}

		template<class U, class DestroyU>
		inline bool operator==(const WeakPtr<U, DestroyU>& i_other)
		{
			return pPtr == i_other.pPtr;
		}

		template<class U, class DestroyU>
		inline bool operator!=(const SmartPtr<U, DestroyU>& i_other)
		{
			return pPtr != i_other.pPtr;
		}

		template<class U, class DestroyU>
		inline bool operator!=(const WeakPtr<U, DestroyU>& i_other)
		{
			return pPtr != i_other.pPtr;
		}

		template<class U>
		inline bool operator==(U* i_ptr)
		{
			return pPtr == i_ptr;
		}

		inline operator bool() const
		{
			return pPtr != nullptr;
		}

	private:
		void ReleaseObservership()
		{
			if (pCounters)
			{
				assert(pCounters->WeakRefs() > 0);
				if (pCounters->DecWeakRefs() == 0)
				{
					if ((pCounters->SmartRefs() == 0))
					{
						delete pCounters;
						pCounters = nullptr;

						pPtr = nullptr;
					}
				}
			}
			else
			{
				assert(pPtr == nullptr);
			}
		}

		void IncrementObservership()
		{
			if (pCounters)
			{
				assert(pCounters->WeakRefs() || pCounters->SmartRefs());

				pCounters->IncWeakRefs();
			}
			else
			{
				assert(pPtr == nullptr);
			}
		}

		ReferenceCounters* pCounters;
		T* pPtr;
	};

} // namespace Engine