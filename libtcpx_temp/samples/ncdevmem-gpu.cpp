#include <arpa/inet.h>
#include <bits/time.h>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <net/if.h>

#include <tcpx_api.h>
#include <time.h>

void ncdevmem(char *ifname, char *ip, int port)
{
    size_t ctrl_len = sizeof(int) * 20000;
    char ctrl_data[ctrl_len];
    int sockfd, new_socket;
    struct sockaddr_in client_addr, server_addr;
    socklen_t addrlen = sizeof(server_addr);
    char buf[1024];
    int opt = 1;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0) {
        std::cout << "Socket creation failed" << std::endl;
        return;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    // setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    // setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

    if(bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cout << "bind err" << std::endl;
        return;
    }

    if(listen(sockfd, 3) < 0) {
        std::cout << "listen err" << std::endl;
        return;
    }

    std::cout << "Server listening on " << ip << ":" << port << "..." << std::endl;

    if((new_socket = accept(sockfd, (struct sockaddr *)&client_addr, &addrlen)) < 0) {
        std::cout << "accept err" << std::endl;
        return;
    }

#define NUM_PAGES 16384
    tcpx_preprocess(if_nametoindex(ifname), 7, 1, 4096 * NUM_PAGES, NULL, HOST_MEM);
    // tcpx_flow_steering(ifname, new_socket, 7, 0);

    std::cout << "Client connected." << std::endl;

#define KB 1024 * 8
#define MB 1024 * KB
#define GB 1024 * MB
    int ret = 0;
    size_t max_recv = 1 * MB;

    do {
        ret = tcpx_recv(new_socket, NULL, max_recv, 0, ctrl_data, ctrl_len);
        XU_DEBUG("ret = %d\n", ret);
    }while(ret != 0 && ret != -1);

// char iobuf[819200];
// bool is_devmem;
// size_t i = 0;
// size_t print = 0, recv_count = 0, cmsg_count=0;
// long long total_received = 0;

// struct timespec start, end;
// clock_gettime(CLOCK_MONOTONIC, &start);

//     while (1) {
// 		struct iovec iov = { .iov_base = iobuf,
// 				     .iov_len = sizeof(iobuf) };
// 		struct dmabuf_cmsg *dmabuf_cmsg = NULL;
// 		struct cmsghdr *cm = NULL;
// 		struct msghdr msg = { 0 };
// 		struct dmabuf_token token[128];
// 		int tidx = 0;
// 		ssize_t ret;

// 		is_devmem = false;
// 		// printf("\n\n");

// 		msg.msg_iov = &iov;
// 		msg.msg_iovlen = 1;
// 		msg.msg_control = ctrl_data;
// 		msg.msg_controllen = sizeof(ctrl_data);
// 		ret = recvmsg(new_socket, &msg, MSG_SOCK_DEVMEM);
// 		++recv_count;
// 		// printf("recvmsg ret=%ld\n", ret);
// 		if (ret < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
// 			continue;
// 		if (ret < 0) {
// 			perror("recvmsg");
// 			continue;
// 		}
// 		if (ret == 0) {
// 			printf("client exited\n");
//             return;
// 		}

//         i++;
// 		for (cm = CMSG_FIRSTHDR(&msg); cm; cm = CMSG_NXTHDR(&msg, cm)) {
// 			if (cm->cmsg_level != SOL_SOCKET ||
// 			    (cm->cmsg_type != SCM_DEVMEM_DMABUF &&
// 			     cm->cmsg_type != SCM_DEVMEM_LINEAR)) {
// 				fprintf(stdout, "skipping non-devmem cmsg\n");
// 				continue;
// 			}

// 			dmabuf_cmsg = (struct dmabuf_cmsg *)CMSG_DATA(cm);
// 			is_devmem = true;

// 			if (cm->cmsg_type == SCM_DEVMEM_LINEAR) {
// 				/* TODO: process data copied from skb's linear
// 				 * buffer.
// 				 */
// 				// fprintf(stdout,
// 				// 	"SCM_DEVMEM_LINEAR. dmabuf_cmsg->frag_size=%u\n",
// 				// 	dmabuf_cmsg->frag_size);

// 				continue;
// 			}
// 			++cmsg_count;

// 			token[tidx].token_start = dmabuf_cmsg->frag_token;
// 			token[tidx].token_count = 1;
// 			++tidx;


// 			total_received += dmabuf_cmsg->frag_size;
// 			// printf("received frag_page=%llu, in_page_offset=%llu, frag_offset=%llu, frag_size=%u, token=%u, total_received=%lu, dmabuf_id=%u\n",
// 			//        dmabuf_cmsg->frag_offset >> PAGE_SHIFT,
// 			//        dmabuf_cmsg->frag_offset % getpagesize(),
// 			//        dmabuf_cmsg->frag_offset, dmabuf_cmsg->frag_size,
// 			//        dmabuf_cmsg->frag_token, total_received,
// 			//        dmabuf_cmsg->dmabuf_id);

// 			// sync.flags = DMA_BUF_SYNC_READ | DMA_BUF_SYNC_START;
// 			// ioctl(buf, DMA_BUF_IOCTL_SYNC, &sync);

// 			// if (do_validation)
// 			// 	validate_buffer(
// 			// 		((unsigned char *)buf_mem) +
// 			// 			dmabuf_cmsg->frag_offset,
// 			// 		dmabuf_cmsg->frag_size);
// 			// else
// 			// 	print_nonzero_bytes(
// 			// 		((unsigned char *)buf_mem) +
// 			// 			dmabuf_cmsg->frag_offset,
// 			// 		dmabuf_cmsg->frag_size);

// 			// sync.flags = DMA_BUF_SYNC_READ | DMA_BUF_SYNC_END;
// 			// ioctl(buf, DMA_BUF_IOCTL_SYNC, &sync);

// 			if (tidx == 128) {
// 				release_tcpx_buf(new_socket, token, tidx);
// 				tidx = 0;
// 			}
// 		}
// 		if (tidx) {
// 			release_tcpx_buf(new_socket, token, tidx);
// 		}

//         if(total_received >= (long long)GB) {
//             clock_gettime(CLOCK_MONOTONIC, &end);
//             long long elapsed_time = ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec));
//             printf("elapsed_time = %lld ns\n", elapsed_time);
//             double sec = elapsed_time * 1.0 / 1e9;
//             printf("elapsed_time = %.6f s\n", sec);
//             printf("bindwidth = %.6f GB/s\n", total_received * 1.0 / GB / sec);

//             clock_gettime(CLOCK_MONOTONIC, &start);
//             total_received = 0;
//         }

// 		// printf("total_received=%lu\n", total_received);
// 		// ++print;
// 		// if (!(print % 30000)) {
// 		// 	printf("recv bytes:    %lu\n"
// 		// 	       "syscall count: %lu\n"
// 		// 	       "cmsg count:    %lu\n"
// 		// 	       , total_received, recv_count, cmsg_count);
// 		// 	printf("\n");
// 		// }
// 	}

    return;
}

int main()
{
    char *ifname = "eth0";
    char *ip = "192.168.1.239";
    int port = 5201;

    ncdevmem(ifname, ip, port);
    return 0;
}