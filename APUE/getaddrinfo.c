#include "getaddrinfo.h"

int netinfo(const char *host, const char*service, struct addrinfo **res)
{
    struct addrinfo hint;
    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_family = AF_INET;
    hint.ai_flags = AI_CANONNAME;
    return getaddrinfo(host, service, &hint, res);
}
