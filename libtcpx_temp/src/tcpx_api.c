#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <error.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/udmabuf.h>

#include <tcpx_api.h>

#define TEST_PREFIX "tcpx_api"

//export dmabuf
// int export_gpu_dmabuf(size_t dmabuf_size, void** p_buf)
// {
//     int dmabuf_fd;
//     int dev_id = 0;
//     CUcontext ctx;
//     cudaDriverCheckError(cuInit(0));
//     cudaDriverCheckError(cuCtxCreate(&ctx, 0, (CUdevice)dev_id));
//     cudaDriverCheckError(cuMemAlloc((CUdeviceptr*)p_buf, dmabuf_size));
//     // cudaDriverCheckError(cuMemAlloc((void**)&test->dst_ptr, dmabuf_size));
//     cudaDriverCheckError(cuMemGetHandleForAddressRange((void*)&dmabuf_fd, (CUdeviceptr)(*p_buf), dmabuf_size, CU_MEM_RANGE_HANDLE_TYPE_DMA_BUF_FD, 0));
//     return dmabuf_fd;
// }

int export_user_dmabuf(size_t dmabuf_size, int *devfd, int *memfd) {
    int dmabuf_fd;

    struct udmabuf_create create;
    int ret;

    *devfd = open("/dev/udmabuf", O_RDWR);
    if (*devfd < 0) {
        error(70, 0,
            "%s: [skip,no-udmabuf: Unable to access DMA buffer device file]\n",
            TEST_PREFIX);
    }

    *memfd = memfd_create("udmabuf-test", MFD_ALLOW_SEALING);
    if (*memfd < 0)
        error(70, 0, "%s: [skip,no-memfd]\n", TEST_PREFIX);

    /* Required for udmabuf */
    ret = fcntl(*memfd, F_ADD_SEALS, F_SEAL_SHRINK);
    if (ret < 0)
        error(73, 0, "%s: [skip,fcntl-add-seals]\n", TEST_PREFIX);

    ret = ftruncate(*memfd, dmabuf_size);
    if (ret == -1)
        error(74, 0, "%s: [FAIL,memfd-truncate]\n", TEST_PREFIX);

    memset(&create, 0, sizeof(create));

    create.memfd = *memfd;
    create.offset = 0;
    create.size = dmabuf_size;
    dmabuf_fd = ioctl(*devfd, UDMABUF_CREATE, &create);
    if (dmabuf_fd < 0)
        error(75, 0, "%s: [FAIL, create udmabuf]\n", TEST_PREFIX);
    return dmabuf_fd;
}

int bind_rx_queue(unsigned int ifindex, unsigned int dmabuf_fd, struct netdev_queue_id *queues, unsigned int num_queues, struct ynl_sock **ys)
{
    struct netdev_bind_rx_req *req = NULL;
    struct netdev_bind_rx_rsp *rsp = NULL;
    struct ynl_error yerr;

    *ys = ynl_sock_create(&ynl_netdev_family, &yerr);
    if (!*ys) {
        fprintf(stderr, "YNL: %s\n", yerr.msg);
        return -1;
    }

    req = netdev_bind_rx_req_alloc();
    netdev_bind_rx_req_set_ifindex(req, ifindex);
    netdev_bind_rx_req_set_fd(req, dmabuf_fd);
    __netdev_bind_rx_req_set_queues(req, queues, num_queues);

    rsp = netdev_bind_rx(*ys, req);
    if (!rsp) {
        perror("netdev_bind_rx");
        goto err_close;
    }

    if (!rsp->_present.id) {
        perror("id not present");
        goto err_close;
    }

    // printf("got dmabuf id=%d\n", rsp->id);
    dmabuf_fd = rsp->id;

    netdev_bind_rx_req_free(req);
    netdev_bind_rx_rsp_free(rsp);

    return 0;

    err_close:
        fprintf(stderr, "YNL failed: %s\n", (*ys)->err.msg);
        netdev_bind_rx_req_free(req);
        ynl_sock_destroy(*ys);
    return -1;
}

int tcpx_preprocess(int ifindex, int start_queue, int num_queues, size_t dmabuf_size, void **p_buf, enum buf_type type)
{
    int ret = 0;
    int dmabuf_fd;

    int devfd, memfd;/*for udmabuf, we dont do close right now*/

    struct ynl_sock *ys;
    struct netdev_queue_id *queues;

    queues = (struct netdev_queue_id*)malloc(sizeof(*queues) * num_queues);
    for (int i = 0; i < num_queues; i++) {
        queues[i]._present.type = 1;
        queues[i]._present.id = 1;
        queues[i].type = NETDEV_QUEUE_TYPE_RX;
        queues[i].id = start_queue + i;
    }

    switch (type) 
    {
        case GPU_MEM:
        // dmabuf_fd = export_gpu_dmabuf(dmabuf_size, p_buf);
        ret = bind_rx_queue(ifindex, dmabuf_fd, queues, num_queues, &ys);
        break;
        case HOST_MEM:
        dmabuf_fd = export_user_dmabuf(dmabuf_size, &devfd, &memfd);
        ret = bind_rx_queue(ifindex, dmabuf_fd, queues, num_queues, &ys);
        break; 
    }

    return ret;
}

/* tcpx */
int tcpx_flow_steering(char *ifname,int fd,int queue, int rule_idx) {
    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;
    socklen_t server_addr_len;
    socklen_t client_addr_len;
    char server_ip[32], client_ip[32];
    memset(server_ip,0,32);
    memset(client_ip,0,32);
    if(getsockname(fd, (struct sockaddr *)&server_addr, &server_addr_len)<0){
        // perror("Fail to get sock name\n");
    }
    if(getpeername(fd, (struct sockaddr *)&client_addr, &client_addr_len)<0){
        perror("Fail to get peer name\n");
    }
    inet_ntop(server_addr.sin_family, &server_addr.sin_addr, server_ip, sizeof(server_ip));
    inet_ntop(client_addr.sin_family, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    int server_port = ntohs(server_addr.sin_port);
    int client_port = ntohs(client_addr.sin_port);
    // YE_DEBUG_241("Link fd = %5d",fd);
    // YE_DEBUG_241("Server IP = %-20s,Server port = %5d", server_ip,server_port);
    // YE_DEBUG_241("Client IP = %-20s,Client port = %5d", client_ip,client_port);
    run_command("sudo ethtool -N %s flow-type tcp4 "
                     "src-ip %s src-port %d queue %d loc %d",
                     ifname, client_ip,client_port, queue, rule_idx);
    // run_command("sudo ethtool -N %s flow-type tcp4 "
    //                  "src-port %d dst-port %d queue %d loc %d",
    //                  ifname, client_port, server_port, queue, rule_idx);
    return 0;
}

void
open_nic_rfs(char *ifname, char *src_ip, int src_port, char *dst_ip, int dst_port, int queue_id, int rule_idx)
{
    // int ret = run_command("sudo ethtool -N %s flow-type tcp4 dst-ip %s dst-port %d queue %d", ifname, dst_ip, port, queue_id);
    int ret;
    int offset = 0;
    char command[256];
    memset(command, 0, sizeof(command));
    offset += snprintf(command, sizeof(command), "sudo ethtool -N %s flow-type tcp4", ifname) ;
    if(src_ip) offset += snprintf(command + offset, sizeof(command) - offset, " src-ip %s", src_ip);
    if(src_port) offset += snprintf(command + offset, sizeof(command) - offset, " src-port %d", src_port);
    if(dst_ip) offset += snprintf(command + offset, sizeof(command) - offset, " dst-ip %s", dst_ip);
    if(dst_port) offset += snprintf(command + offset, sizeof(command) - offset, " dst-port %d", dst_port);
    offset += snprintf(command + offset, sizeof(command) - offset, " queue %d loc %d", queue_id, rule_idx);

    // printf("Running:%s\n", command);
    ret = system(command);
    if(ret)
        error(1, 0, "rfs err\n");
}

void
close_nic_rfs(char *ifname, int rule_idx)
{
    run_command("sudo ethtool -N %s delete %d", ifname, rule_idx);
}

void
insert_token(struct dmabuf_token *token, int maxLen, unsigned int frag_token, int fd)
{
    // XU_DEBUG("frag_token = %d, maxLen = %d\n", frag_token, maxLen);
#define MAX_TOKEN 1280
    for(int i = 0; i < maxLen; i++) {
        if(token[i].token_count == 0) {
            token[i].token_start = frag_token;
            token[i].token_count = 1;
            return;
        }
        if(token[i].token_start + token[i].token_count == frag_token) {
            token[i].token_count++;
            if(token[i].token_count == MAX_TOKEN) {
                // XU_DEBUG("release!\n");
                release_tcpx_buf(fd, token, maxLen);
                // XU_DEBUG("release success!\n");
                memset(token, 0, sizeof(struct dmabuf_token) * maxLen);
                // XU_DEBUG("memset success!\n");
            }
            return;
        }
    }

    release_tcpx_buf(fd, token, maxLen);
    memset(token, 0, sizeof(struct dmabuf_token) * maxLen);
    token[0].token_start = frag_token;
    token[0].token_count = 1;
    return;
}

int
tcpx_recv_test(int socketfd, void *buf, size_t size, int flags, void *ctrl_data, size_t ctrl_len, struct dmabuf_token *token, int maxLen)
{
    struct iovec iov = {.iov_base = NULL, .iov_len = 0};
    struct dmabuf_cmsg *dmabuf_cmsg = NULL;
    struct cmsghdr *cm = NULL;
    struct msghdr msg = {0};

    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = ctrl_data;
    msg.msg_controllen = ctrl_len;

    int offset = 0;

    int bytes = 0;
    do {
        iov.iov_len = size - offset;

        bytes = recvmsg(socketfd, &msg, MSG_SOCK_DEVMEM);
        // XU_DEBUG("offset = %d, bytes = %d\n", offset, bytes);
        if(bytes == 0) {
            return -1;
        }
        if(bytes == -1) {
            if (errno != EINTR && errno != EWOULDBLOCK && errno != EAGAIN) {
                return -1;
            } else {
                continue;
            }
        }

        for(cm = CMSG_FIRSTHDR(&msg); cm; cm = CMSG_NXTHDR(&msg, cm)) {
            dmabuf_cmsg = (struct dmabuf_cmsg *)CMSG_DATA(cm);

            insert_token(token, maxLen, dmabuf_cmsg->frag_token, socketfd); 
        }

        (offset) += bytes;

    } while(bytes > 0 && (offset) < size);

    
    return bytes;
}

int
tcpx_recv(int socketfd, void *buf, size_t size, int flags, void *ctrl_data, size_t ctrl_len)
{
    struct iovec iov = {.iov_base = NULL, .iov_len = 0};
    struct dmabuf_cmsg *dmabuf_cmsg = NULL;
    struct cmsghdr *cm = NULL;
    struct msghdr msg = {0};

    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = ctrl_data;
    msg.msg_controllen = ctrl_len;

    struct dmabuf_token token[128];
    int tidx = 0;

    int offset = 0;
    int recvmsg_count = 0;

    int bytes = 0;
    do {
        iov.iov_len = size - offset;

        bytes = recvmsg(socketfd, &msg, MSG_SOCK_DEVMEM);
        recvmsg_count++;
        // XU_DEBUG("offset = %d, bytes = %d\n", offset, bytes);
        if(bytes == 0) {
            return 0;
        }
        if(bytes == -1) {
            if (errno != EINTR && errno != EWOULDBLOCK && errno != EAGAIN) {
                perror("recvmsg");
                XU_DEBUG("wrong\n");
                return -1;
            } else {
                // XU_DEBUG("non-blocking\n");
                continue;
            }
        }

        for(cm = CMSG_FIRSTHDR(&msg); cm; cm = CMSG_NXTHDR(&msg, cm)) {
            dmabuf_cmsg = (struct dmabuf_cmsg *)CMSG_DATA(cm);

            token[tidx].token_start = dmabuf_cmsg->frag_token;
            token[tidx].token_count = 1;
            tidx++;

            if(tidx == 128) {
                release_tcpx_buf(socketfd, token, 128);
                tidx = 0;
            }

        }

        (offset) += bytes;

    } while(/*bytes > 0 && (offset) < size*/0);
    // XU_DEBUG("recvmsg_count = %d\n", recvmsg_count);

    if(tidx) {
        release_tcpx_buf(socketfd, token, tidx);
        tidx = 0;
    }

    
    return bytes;
}

// int
// tcpx_recv(int socketfd, void *buf, size_t size, int flags, void *ctrl_data, size_t ctrl_len)
// {
//     struct iovec iov = {.iov_base = NULL, .iov_len = 0};
//     struct dmabuf_cmsg *dmabuf_cmsg = NULL;
//     struct cmsghdr *cm = NULL;
//     struct msghdr msg = {0};

//     msg.msg_iov = &iov;
//     msg.msg_iovlen = 1;
//     msg.msg_control = ctrl_data;
//     msg.msg_controllen = ctrl_len;

//     struct dmabuf_token token[128];
//     int tidx = 0;

//     int offset = 0;
//     int recvmsg_count = 0;

//     int bytes = 0;
//     int ret;

//     while(1) {
//         iov.iov_len = size - bytes;
//         ret = recvmsg(socketfd, &msg, MSG_SOCK_DEVMEM);
//         if(ret < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
//             continue;
//         if(ret < 0) {
//             perror("recvmsg");
//             continue;
//         }
//         if(ret == 0) {
//             XU_DEBUG("client exited\n");
//             return 0;
//         }

//         bytes += ret;
//         for (cm = CMSG_FIRSTHDR(&msg); cm; cm = CMSG_NXTHDR(&msg, cm)) {
// 			if (cm->cmsg_level != SOL_SOCKET ||
// 			    (cm->cmsg_type != SCM_DEVMEM_DMABUF &&
// 			     cm->cmsg_type != SCM_DEVMEM_LINEAR)) {
// 				fprintf(stdout, "skipping non-devmem cmsg\n");
// 				continue;
// 			}

// 			dmabuf_cmsg = (struct dmabuf_cmsg *)CMSG_DATA(cm);

// 			if (cm->cmsg_type == SCM_DEVMEM_LINEAR) {
// 				/* TODO: process data copied from skb's linear
// 				 * buffer.
// 				 */
// 				fprintf(stdout,
// 					"SCM_DEVMEM_LINEAR. dmabuf_cmsg->frag_size=%u\n",
// 					dmabuf_cmsg->frag_size);

// 				continue;
// 			}

// 			token[tidx].token_start = dmabuf_cmsg->frag_token;
// 			token[tidx].token_count = 1;
// 			++tidx;

// 			if (tidx == 128) {
// 				release_tcpx_buf(socketfd, token, tidx);
// 				tidx = 0;
// 			}
// 		}
//         if(bytes == size) break;
//     }
//     return bytes;
// }

void
release_tcpx_buf(int fd, struct dmabuf_token *token, int num)
{ 
  int ret = setsockopt(fd, SOL_SOCKET, SO_DEVMEM_DONTNEED, token,
                       sizeof(*token) * num);
    // XU_DEBUG("release ret = %d\n", ret);
//   if (ret != num){error(1, 0, "SO_DEVMEM_DONTNEED not enough tokens, and ret = %d\n", ret);}
}
