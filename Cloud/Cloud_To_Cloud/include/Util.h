#ifndef UTIL_H_
#define UTIL_H_

#include <glib.h>
#include <event.h>
#include <event2/listener.h>
#include <event2/event_struct.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

extern int g_auth_provider; //0-jd, 1-weilian
extern char *g_auth_code;
extern struct event_base *g_event_base;


#endif /* UTIL_H_ */