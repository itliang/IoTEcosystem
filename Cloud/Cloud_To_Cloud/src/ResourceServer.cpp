#include "Util.h"
#include "DeviceManager.h"

int g_auth_provider = 0;
char *g_auth_code = NULL;
struct event_base *g_event_base = NULL;
static bool is_sign_in = false;

void show_help()
{
    const char *help = "resource server bridged with 3rd cloud\n" \
                    "-p <mandatory> OAuth2 authorization provider(jd, wulian)\n" \
                    "-c <optional> OAuth2 authorization code, which's mandatory for authorization code grant(e.x jd)\n" \
                    "-h print this help and exit\n" \
                    "\n";
    fprintf(stderr, "%s", help);
}

void signal_handler(int sig)
{
    switch (sig) {
        case SIGTERM:
        case SIGHUP:
        case SIGQUIT:
        case SIGINT:
        case SIGKILL:
        {
            event_base_loopbreak(g_event_base);
            break;
        }
    }
}

static void event_debug_cb(int severity, const char *msg)
{
    const char *s;

    switch (severity) {
    case _EVENT_LOG_DEBUG: 
        s = "debug";
        break;
    case _EVENT_LOG_MSG:
        s = "msg";
        break;
    case _EVENT_LOG_WARN:
        s = "warn";
        break;
    case _EVENT_LOG_ERR:
        s = "error";
        break;
    default:
        s = "?";
        break; /* never reached */
    }

    printf("[event][%s] %s\n", s, msg);
}

static void conn_read_cb(struct bufferevent *bev, void *ctx)
{
    char buf[32] = {0};
    struct evbuffer *input = bufferevent_get_input(bev);
    size_t len = evbuffer_get_length(input);
    printf("evbuffer length: %d\n", (int)len);

    if (len < 32) {
        bufferevent_read(bev, buf, len);
        printf("recv: %s\n", buf);

        char *p = strtok(buf, ":");
        char *q = strtok(NULL, ":");

        if (!p || !q) {
            fprintf(stderr, "invalid message\n");
            return;
        }

        std::string authProvider(p);
        std::string authCode(q);
        printf("[%s: %s]\n", p, q);

        if ("samsung" == authProvider) {
            if (is_sign_in) {
                printf("Already signed in cloud, exit");
                return;
            }

            DeviceManager *pDeviceManager = DeviceManager::getInstance();
            RSCode ret = pDeviceManager->signIn(authProvider, authCode);
            if (ret != RS_OK) {
                fprintf(stderr, "failed to sign in CI server\n");
                return;
            }

            printf("sign in CI server successfully\n");
            is_sign_in = true;

            if (1 == g_auth_provider) {
                pDeviceManager->registerResources();
            } else {
                std::string jdAuthCode(g_auth_code);
                pDeviceManager->registerResources(jdAuthCode);
            }
        } else {
            fprintf(stderr, "unsupported message\n");
        }
    } else {
        fprintf(stderr, "unknown message, just ignore it!\n");
    }
}

static void conn_event_cb(struct bufferevent *bev, short events, void *ctx)
{
    if (events & BEV_EVENT_ERROR) {
        fprintf(stderr, "Error from bufferevent");
    }

    if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
        bufferevent_free(bev);
    }
}

static void accept_conn_cb(struct evconnlistener *listener, evutil_socket_t fd, 
                        struct sockaddr *address, int socklen, void *ctx)
{
    char hbuf[512] = {0};
    char sbuf[512] = {0};

    getnameinfo(address, socklen, hbuf, sizeof(hbuf), sbuf, sizeof(sbuf), 
                NI_NUMERICHOST | NI_NUMERICSERV);
    printf("(host=%s, port=%s) has been connected\n", hbuf, sbuf);

    struct event_base *base = evconnlistener_get_base(listener);
    struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

    bufferevent_setcb(bev, conn_read_cb, NULL, conn_event_cb, NULL);
    bufferevent_enable(bev, EV_READ|EV_WRITE);
}

static void accept_error_cb(struct evconnlistener *listener, void *ctx)
{       
    struct event_base *base = evconnlistener_get_base(listener);
    int err = EVUTIL_SOCKET_ERROR();

    fprintf(stderr, "Got an error %d (%s) on the listener. Shutting down.\n", 
            err, evutil_socket_error_to_string(err));
    event_base_loopexit(base, NULL);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        show_help();
        exit(EXIT_FAILURE);        
    }

    int c;
    char *code = NULL;
    char *provider = NULL;
    
    while ((c = getopt(argc, argv, "p:c::h")) != -1) {
        switch (c) {
        case 'c' :
            code = optarg;
            break;
        case 'p':
            provider = optarg;
            break;
        case 'h':
            show_help();
            exit(EXIT_SUCCESS);
        case '?':
            fprintf(stderr, "unknown option: %s\n", optarg);
            break;
        }
    }

    if (!provider) {
        fprintf(stderr, "provider is null\n");
        exit(EXIT_FAILURE);
    }

    printf("provider: %s\n", provider);

    if (0 == strncasecmp(provider, "jd", 2)) {
        if (!code) {
            fprintf(stderr, "auth code is missed for JD\n");
            exit(EXIT_FAILURE);
        }

        printf("code: %s\n", code);
        g_auth_code = code;
        g_auth_provider = 0;
    } else if (0 == strncasecmp(provider, "wulian", 6)) {
        g_auth_provider = 1;
    } else {
        fprintf(stderr, "unsupported provider\n");
        exit(EXIT_FAILURE);
    }
    
    //system signal
    signal(SIGHUP, signal_handler);
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    signal(SIGQUIT, signal_handler);
    signal(SIGKILL, signal_handler);

    //receive easy setup from rich device
    struct evconnlistener *tcp_listener;
    struct sockaddr_in sin;

    struct event_base *base = event_base_new();
    if (!base) {
        fprintf(stderr, "can't open event base");
        exit(EXIT_FAILURE);
    }

    g_event_base = base;

    //redirect all libevent log to local
    event_set_log_callback(event_debug_cb);
    
    DeviceManager *pDeviceManager = DeviceManager::getInstance();
    pDeviceManager->init();
    printf("init done\n");

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    sin.sin_port = htons(9000);

    tcp_listener = evconnlistener_new_bind(base, accept_conn_cb, NULL, 
                                    LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, 
                                    -1, (struct sockaddr*)&sin, sizeof(sin));
    if (!tcp_listener) {
        fprintf(stderr, "it cant create tcp listener");
        exit(EXIT_FAILURE);
    }

    printf("listening for auth code...\n");

    evconnlistener_set_error_cb(tcp_listener, accept_error_cb);
    event_base_dispatch(base);

    printf("resource server is stop\n");
    evconnlistener_free(tcp_listener);
    event_base_free(base);

    if (is_sign_in) {
        pDeviceManager->unregisterResources();
        pDeviceManager->signOut();
    }
    
    pDeviceManager->destroy();

    return 0;
}