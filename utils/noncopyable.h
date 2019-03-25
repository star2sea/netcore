#ifndef __NONCOPYABLE_H
#define __NONCOPYABLE_H

namespace netcore
{
	class NonCopyable
	{
	public:
		NonCopyable(const NonCopyable &) = delete;
		NonCopyable & operator=(const NonCopyable &) = delete;

	protected:
		NonCopyable() = default;
		~NonCopyable() = default;
	};
}

#endif