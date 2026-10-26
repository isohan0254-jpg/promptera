#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <atomic>
#include <string>
#include "Dobby/dobby.h"
#include <android/log.h>

#define NET_LOG_TAG "NexoBlocker"
#define LOG_NET(...) __android_log_print(ANDROID_LOG_INFO, NET_LOG_TAG, __VA_ARGS__)

// ============================================================================
// 🛡️ NEXO NETWORK / IP & PORT BLOCKER ENGINE
// ============================================================================
inline bool g_BlockAllNetwork = false;         // Master IP & Port Block
inline bool g_BlockAntiCheatReports = false;   // Telemetry & Report Port Blocker
inline bool g_BlockUdpTraffic = false;         // UDP Match Sync Freeze
inline bool g_BlockTcpTraffic = false;         // TCP Connection Blocker
inline bool g_BlockUploadLogs = false;         // Log & Screenshot Upload Blocker

inline std::atomic<uint64_t> g_BlockedPackets{0};
inline std::atomic<uint64_t> g_BlockedConnections{0};

// Original function pointers
inline int (*orig_connect)(int sockfd, const struct sockaddr *addr, socklen_t addrlen) = nullptr;
inline ssize_t (*orig_sendto)(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen) = nullptr;
inline ssize_t (*orig_send)(int sockfd, const void *buf, size_t len, int flags) = nullptr;
inline ssize_t (*orig_recv)(int sockfd, void *buf, size_t len, int flags) = nullptr;
inline ssize_t (*orig_recvfrom)(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen) = nullptr;

// Hooked connect (Blocks outgoing TCP/IP connections)
inline int hook_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    if (g_BlockAllNetwork || g_BlockTcpTraffic) {
        g_BlockedConnections++;
        errno = ECONNREFUSED;
        return -1;
    }
    if (g_BlockAntiCheatReports && addr != nullptr && addr->sa_family == AF_INET) {
        const struct sockaddr_in *sin = (const struct sockaddr_in *)addr;
        int port = ntohs(sin->sin_port);
        // Block anti-cheat telemetry and report server ports
        if ((port >= 10000 && port <= 20000) || port == 8080 || port == 9000 || port == 30100) {
            g_BlockedConnections++;
            errno = ECONNREFUSED;
            return -1;
        }
    }
    if (orig_connect) {
        return orig_connect(sockfd, addr, addrlen);
    }
    return -1;
}

// Hooked sendto (Blocks & drops UDP packets / IP Ports)
inline ssize_t hook_sendto(int sockfd, const void *buf, size_t len, int flags, const struct sockaddr *dest_addr, socklen_t addrlen) {
    if (g_BlockAllNetwork || g_BlockUdpTraffic) {
        g_BlockedPackets++;
        // Fake success return to prevent crash while dropping payload
        return (ssize_t)len;
    }
    if (orig_sendto) {
        return orig_sendto(sockfd, buf, len, flags, dest_addr, addrlen);
    }
    return (ssize_t)len;
}

// Hooked send (Blocks & drops TCP stream traffic)
inline ssize_t hook_send(int sockfd, const void *buf, size_t len, int flags) {
    if (g_BlockAllNetwork || g_BlockTcpTraffic || g_BlockUploadLogs) {
        g_BlockedPackets++;
        return (ssize_t)len;
    }
    if (orig_send) {
        return orig_send(sockfd, buf, len, flags);
    }
    return (ssize_t)len;
}

// Hooked recvfrom (Drops incoming UDP packets)
inline ssize_t hook_recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen) {
    if (g_BlockAllNetwork || g_BlockUdpTraffic) {
        errno = EAGAIN;
        return -1;
    }
    if (orig_recvfrom) {
        return orig_recvfrom(sockfd, buf, len, flags, src_addr, addrlen);
    }
    return -1;
}

// Hooked recv (Drops incoming TCP packets)
inline ssize_t hook_recv(int sockfd, void *buf, size_t len, int flags) {
    if (g_BlockAllNetwork || g_BlockTcpTraffic) {
        errno = EAGAIN;
        return -1;
    }
    if (orig_recv) {
        return orig_recv(sockfd, buf, len, flags);
    }
    return -1;
}

// Install socket hooks seamlessly
inline void InstallNetworkBlockerHooks() {
    static bool installed = false;
    if (installed) return;
    
    void *ptr_connect = DobbySymbolResolver("/system/lib/libc.so", "connect");
    if (!ptr_connect) ptr_connect = DobbySymbolResolver("/system/lib64/libc.so", "connect");
    if (!ptr_connect) ptr_connect = DobbySymbolResolver("libc.so", "connect");
    if (ptr_connect) {
        DobbyHook(ptr_connect, (void *)hook_connect, (void **)&orig_connect);
        LOG_NET("Hooked connect successfully");
    }
    
    void *ptr_sendto = DobbySymbolResolver("/system/lib/libc.so", "sendto");
    if (!ptr_sendto) ptr_sendto = DobbySymbolResolver("/system/lib64/libc.so", "sendto");
    if (!ptr_sendto) ptr_sendto = DobbySymbolResolver("libc.so", "sendto");
    if (ptr_sendto) {
        DobbyHook(ptr_sendto, (void *)hook_sendto, (void **)&orig_sendto);
        LOG_NET("Hooked sendto successfully");
    }
    
    void *ptr_send = DobbySymbolResolver("/system/lib/libc.so", "send");
    if (!ptr_send) ptr_send = DobbySymbolResolver("/system/lib64/libc.so", "send");
    if (!ptr_send) ptr_send = DobbySymbolResolver("libc.so", "send");
    if (ptr_send) {
        DobbyHook(ptr_send, (void *)hook_send, (void **)&orig_send);
        LOG_NET("Hooked send successfully");
    }

    void *ptr_recvfrom = DobbySymbolResolver("/system/lib/libc.so", "recvfrom");
    if (!ptr_recvfrom) ptr_recvfrom = DobbySymbolResolver("/system/lib64/libc.so", "recvfrom");
    if (!ptr_recvfrom) ptr_recvfrom = DobbySymbolResolver("libc.so", "recvfrom");
    if (ptr_recvfrom) {
        DobbyHook(ptr_recvfrom, (void *)hook_recvfrom, (void **)&orig_recvfrom);
        LOG_NET("Hooked recvfrom successfully");
    }

    void *ptr_recv = DobbySymbolResolver("/system/lib/libc.so", "recv");
    if (!ptr_recv) ptr_recv = DobbySymbolResolver("/system/lib64/libc.so", "recv");
    if (!ptr_recv) ptr_recv = DobbySymbolResolver("libc.so", "recv");
    if (ptr_recv) {
        DobbyHook(ptr_recv, (void *)hook_recv, (void **)&orig_recv);
        LOG_NET("Hooked recv successfully");
    }
    
    installed = true;
}
