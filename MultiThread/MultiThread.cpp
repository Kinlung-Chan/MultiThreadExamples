#include <iostream>
#include <vector>
#include <thread>


// Windowsͬ������
// Event��Mutex
// CRITICAL_SECTION
// Semaphore
// SRW Lock

//#define WITH_CRITICAL_SECTION
//#define WITH_MUTEX
//#define WITH_STD_MUTEX

#if defined WITH_CRITICAL_SECTION || defined WITH_MUTEX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#ifdef WITH_CRITICAL_SECTION
CRITICAL_SECTION cs;
#endif
#ifdef WITH_MUTEX
HANDLE hWinMutex;
#endif

#endif

#ifdef WITH_STD_MUTEX
#include <mutex>
std::mutex stdMutex;
#endif

void print_thread_id(int id)
{
#ifdef WITH_CRITICAL_SECTION
    EnterCriticalSection(&cs);
    // ͬһ���߳��У����Զ�ν����ٽ�����������Ҫ��֤�뿪�ٽ����ĵ��ô����ͽ���Ĵ���һ��
    EnterCriticalSection(&cs);
#endif
#ifdef WITH_MUTEX
    DWORD dwResult = WaitForSingleObject(hWinMutex, INFINITE);
    if (dwResult != WAIT_OBJECT_0)
        return;
#endif
#ifdef WITH_STD_MUTEX
    stdMutex.lock();
#endif

    std::cout << "Thread #" << id << '\n';

#ifdef WITH_CRITICAL_SECTION
    LeaveCriticalSection(&cs);
    // �������������EnterCriticalSection������Ҳ������LeaveCriticalSection
    LeaveCriticalSection(&cs);
#endif
#ifdef WITH_MUTEX
    ReleaseMutex(hWinMutex);
#endif
#ifdef WITH_STD_MUTEX
    stdMutex.unlock();
#endif
}

int main()
{
#ifdef WITH_CRITICAL_SECTION
    InitializeCriticalSection(&cs);
#endif
#ifdef WITH_MUTEX
    hWinMutex = CreateMutex(NULL, FALSE, NULL);
#endif

    std::vector<std::thread> threads;

    // ��������߳�
    for (int i = 0; i < 100; ++i)
        threads.emplace_back(print_thread_id, i + 1);

    // �ȴ������߳̽���
    for (auto& t : threads)
        t.join();

#ifdef WITH_CRITICAL_SECTION
    DeleteCriticalSection(&cs);
#endif
#ifdef WITH_MUTEX
     CloseHandle(hWinMutex);
#endif
    return 0;
}
