/*! \file */ //Copyright 2011-2016 Tyler Gilbert; All Rights Reserved

#ifndef THREAD_HPP_
#define THREAD_HPP_

#ifndef __link

#include <pthread.h>

namespace sys {

/*! \brief Thread Class
 * \details This class creates and manages new threads using POSIX calls.
 *
 */
class Thread {
public:

	/*! \details This constructs a new thread object
	 *
	 * @param stack_size  The stack size of the new thread (default is 4096)
	 * @param detached Whether to create as a detached thread.  If this value is false,
	 * another thread must use join() in order for the thread to terminate correctly.
	 */
	Thread(int stack_size = 4096, bool detached = true);

	/*! \brief Scheduling policies */
	enum {
		OTHER /*! Default scheduling algorithm (SCHED_OTHER), the thread uses a round robin algorithm with a 0 priority value.  */ = SCHED_OTHER,
		ROUND_ROBIN /*! Spend up to the total round robin time on a task before switching, variable priority allowed */ = SCHED_RR,
		FIFO /*! Stay in the thread until the thread yields (usleep(), read()/write(), other Thread::yield()), variable priority allowed */ = SCHED_FIFO
	};

	/*! \details Sets the stacksize (no effect after create) */
	int set_stacksize(int size);

	/*! \details Gets the stacksize */
	int get_stacksize() const;

	/*! \details Sets the thread priority */
	int set_priority(int prio, int policy = ROUND_ROBIN);

	/*! \details Gets the thread priority */
	int get_priority() const;

	/*! \details Get the thread policy */
	int get_policy() const;

	/*! \details Gets the ID of the thread */
	int id() const;

	/*! \details Start the thread */
	int create(void * (*func)(void*), void * args = NULL, int prio = 0, int policy = OTHER);

	/*! \details Check if the thread is running */
	bool is_running() const;

	/*! \details Wait for the thread to complete (joins thread if it is not detached) */
	int wait(void**ret, int interval = 1000);

	/*! \details Yield the processor to another thread */
	static void yield();

	/*! \details Join the current thread to the specified thread
	 *
	 * @param ident the ID of the target thread
	 * @param value_ptr A pointer to the return value of the target thread
	 * @return Zero on success
	 */
	static int join(int ident, void ** value_ptr);

	/*! \details Reset the object (thread must not be running) */
	inline void reset(){ set_id_default(); }

private:
	pthread_attr_t m_pthread_attr;
	pthread_t m_id;

	void set_id_default(){ m_id = -1; }
	void set_id_error(){ m_id = -2; }
};

};

#endif

#endif /* THREAD_HPP_ */