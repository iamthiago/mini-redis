#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/errno.h>
#include <unistd.h>

static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%s] %s\n", strerror(err), msg);
    abort();
}

static void msg(const char *msg) {
    fprintf(stderr, "%s\n", msg);
}

// read and write
static void do_something(int connfd) {
    char rbuf[64] = {};
    ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        msg("read() error");
        return;
    }
    printf("client says: %s\n", rbuf);

    char wbuf[] = "world";
    write(connfd, wbuf, strlen(wbuf));
}

int main() {
    // file descriptor
    int fd = socket(PF_INET, SOCK_STREAM, 0);

    // bind
    int bind_to_same_port = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &bind_to_same_port, sizeof(bind_to_same_port));

    struct sockaddr_in addr = {};
    addr.sin_family = PF_INET;
    addr.sin_port = htons(1234); // port
    addr.sin_addr.s_addr = htonl(0); // wildcard IP 0.0.0.0
    int rv = bind(fd, (const struct sockaddr *) &addr, sizeof(addr));
    if (rv) {
        die("bind()");
    }

    // listen
    rv = listen(fd, SOMAXCONN);
    if (rv) {
        die("listen()");
    }

    while (true) {
        // accept connection
        struct sockaddr_in client_addr = {};
        socklen_t addlen = sizeof(client_addr);
        int connfd = accept(fd, (struct sockaddr *) &client_addr, &addlen);
        if (connfd < 0) {
            continue; // error
        }

        do_something(connfd);

        close(fd);
    }

    return 0;
}
