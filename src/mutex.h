#ifndef mutex_h__
#define mutex_h__


#if _MSC_VER <= 1900

#ifndef WIN32_EXPORT
#define WIN32_EXPORT __declspec(dllexport)
#endif

class WIN32_EXPORT mutex
{
public:
	mutex() { ::InitializeCriticalSection(&m_cs); }
	~mutex() { ::DeleteCriticalSection(&m_cs); }

	void lock() { ::EnterCriticalSection(&m_cs); }
	void unlock() { ::LeaveCriticalSection(&m_cs); }

private:
	mutex(const mutex&);
	mutex& operator = (const mutex&);

	CRITICAL_SECTION m_cs;
};
#endif
#endif // mutex_h__
