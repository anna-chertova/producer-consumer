#pragma once
/*
 * pc_shared_ofstream.h - this header file contains shared ofstream
 * to synchronize output of several threads to console.
 * Locks the mutex on construction, flushes the stream and 
 * releases mutex on destruction. Wrapped inner implementation into
 * unique pointer to avoid construction/destruction on every operator << call.
 * (c) 2020 Anna Chertova
 */

#include <iostream>
#include <memory>
#include <cassert>
#include <windows.h>

// This class holds mutex that synchronizes access to console output
struct pc_shared_ostream {

	std::ostream& shared_ostream;
	HANDLE guard_mutex;
	
	// Scoped lock implementation
	struct guarded_impl {

		guarded_impl() = delete;
		guarded_impl(const guarded_impl&) = delete;
		void operator=(const guarded_impl&) = delete;

		guarded_impl(std::ostream& o, HANDLE gm): ostream(o), guard_mutex(gm)
		{
			DWORD wait_result = WaitForSingleObject(guard_mutex, INFINITE);
			if (wait_result != WAIT_OBJECT_0) {
				std::cerr << "Error adding item: could not lock buffer\n";
			}
		}

		~guarded_impl()
		{
			ostream.flush();
			ReleaseMutex(guard_mutex);
		}

		template<typename T> void write(const T& x)
		{
			ostream << x;
		}

		std::ostream& ostream;
		HANDLE guard_mutex;
	};

	struct impl {

		mutable std::unique_ptr<guarded_impl> unique_impl;
		
		impl() = delete;
		void operator=(const impl&) = delete;
		
		impl(std::ostream& o, HANDLE m): unique_impl(new guarded_impl(o, m))
		{
		}

		impl(const impl& rhs)
		{
			assert(rhs.unique_impl.get());
			unique_impl.swap(rhs.unique_impl);
		}

		template<typename T> impl& operator << (const T& x)
		{
			guarded_impl* p = unique_impl.get();
			assert(p);
			p->write(x);
			return *this;
		}		
	};

	explicit pc_shared_ostream(std::ostream& o): shared_ostream(o),
		guard_mutex(nullptr)
	{
		init();
	}

	~pc_shared_ostream()
	{
		cleanup();
	}

	void init()
	{
		guard_mutex = CreateMutex(NULL, FALSE, NULL);
		if (!guard_mutex) {
			std::cerr << 
				"Error initializing guard mutex for synchronized output\n";
			return;
		}
	}

	void cleanup() 
	{
		if (guard_mutex) {
			ReleaseMutex(guard_mutex);
			CloseHandle(guard_mutex);
			guard_mutex = nullptr;
		}
	}

	template<typename T> impl operator<<(const T& x)
	{
		return impl(shared_ostream, guard_mutex) << x;
	}
};

extern pc_shared_ostream shared_cout;
extern pc_shared_ostream shared_cerr;