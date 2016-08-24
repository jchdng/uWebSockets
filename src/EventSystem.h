#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

#include <uv.h>
#include <vector>
#include "mutex.h"
#include "Network.h"

namespace uWS {

enum LoopType {
    MASTER,
    WORKER
};

class WIN32_EXPORT EventSystem
{
    friend class Server;
    friend class WebSocket;
    LoopType loopType;
    uv_loop_t *loop;
    uv_async_t *asyncPollChange;
	std::vector<uv_poll_t *> pollsToChange;
    mutex pollsToChangeMutex;
    pthread_t tid;

    void changePollAsync(uv_poll_t *p);
	static void uvasync_callback(uv_async_t* a);
public:
    EventSystem(LoopType loopType = MASTER);
    ~EventSystem();
    void run();
};

}

#endif // EVENTSYSTEM_H
