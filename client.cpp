#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

static void die(const char *msg) {
    int err = errno;
    fprintf(stderr, "[%s] %s\n", strerror(err), msg);
    abort();
}

int main() {
    // file descriptor
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        die("socket()");
    }

    // connect
    struct sockaddr_in addr = {};
    addr.sin_family = PF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK); // 127.0.0.1
    int rv = connect(fd, (const struct sockaddr *) &addr, sizeof(addr));
    if (rv) {
        die("connect()");
    }

    // write to server
    char msg[] = "hello";
    write(fd, msg, strlen(msg));

    // read from server
    char rbuf[64] = {};
    ssize_t n = read(fd, rbuf, sizeof(rbuf) -1);
    if (n < 0) {
        die("read()");
    }
    printf("server says: %s\n", rbuf);
    close(fd);
}
