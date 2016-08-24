// libuvTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include <stdio.h>
#include "uv.h"

#pragma comment(lib,"libuv.lib")
#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"Iphlpapi.lib")
#pragma comment(lib,"Userenv.lib")

int64_t counter = 0;

void wait_for_a_while(uv_idle_t* handle) {
	counter++;

	if (counter >= 10e6)
		uv_idle_stop(handle);
}

int main() {
	uv_idle_t idler;

	uv_idle_init(uv_default_loop(), &idler);
	uv_idle_start(&idler, wait_for_a_while);

	printf("Idling...\n");
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	uv_loop_close(uv_default_loop());
	return 0;
}