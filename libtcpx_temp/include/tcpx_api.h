#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
// #include <cuda.h>

#include "../third-party/include/netdev-user.h"
#include "../third-party/include/ynl.h"
#include "./common.h"

#define MSG_SOCK_DEVMEM 0x2000000

// int tcpx_flow_steering(char *ifname,int fd,int queue, int rule_idx) {
//     struct sockaddr_in client_addr;
//     struct sockaddr_in server_addr;
//     socklen_t server_addr_len;
//     socklen_t client_addr_len;
//     char server_ip[32], client_ip[32];
//     memset(server_ip,0,32);
//     memset(client_ip,0,32);
//     if(getsockname(fd, (struct sockaddr *)&server_addr, &server_addr_len)<0){
//         // perror("Fail to get sock name\n");
//     }
//     if(getpeername(fd, (struct sockaddr *)&client_addr, &client_addr_len)<0){
//         // perror("Fail to get peer name\n");
//     }
//     inet_ntop(server_addr.sin_family, &server_addr.sin_addr, server_ip, sizeof(server_ip));
//     inet_ntop(client_addr.sin_family, &client_addr.sin_addr, client_ip, sizeof(client_ip));
//     int server_port = ntohs(server_addr.sin_port);
//     int client_port = ntohs(client_addr.sin_port);
//     // YE_DEBUG_241("Link fd = %5d",fd);
//     // YE_DEBUG_241("Server IP = %-20s,Server port = %5d", server_ip,server_port);
//     // YE_DEBUG_241("Client IP = %-20s,Client port = %5d", client_ip,client_port);
//     run_command("sudo ethtool -N %s flow-type tcp4 "
//                      "src-ip %s src-port %d queue %d loc %d",
//                      ifname, client_ip,client_port, queue, rule_idx);
//     // run_command("sudo ethtool -N %s flow-type tcp4 "
//     //                  "src-port %d dst-port %d queue %d loc %d",
//     //                  ifname, client_port, server_port, queue, rule_idx);
//     return 0;
// }

struct dmabuf_cmsg {
        __u64 frag_offset;      /* offset into the dmabuf where the frag starts.
                                 */
        __u32 frag_size;        /* size of the frag. */
        __u32 frag_token;       /* token representing this frag for
                                 * DEVMEM_DONTNEED.
                                 */
        __u32  dmabuf_id;       /* dmabuf id this frag belongs to. */
        __u32 flags;            /* Currently unused. Reserved for future
                                 * uses.
                                 */
};

struct dmabuf_token {
    __u32 token_start;
    __u32 token_count;
};

typedef uint64_t base_addr;

enum buf_type { GPU_MEM, HOST_MEM };

/* debug */
#ifdef __cplusplus
extern "C"{
#endif

/* tcpx */
//export dmabuf
// int export_gpu_dmabuf( size_t dmabuf_size, void** p_buf);
int export_user_dmabuf( size_t dmabuf_size, int* dev_fd, int* mem_fd);
//bind
int bind_rx_queue( unsigned int ifindex, unsigned int dmabuf_fd, struct netdev_queue_id *queues, unsigned int num_queues, struct ynl_sock **ys );
int tcpx_preprocess( int ifindex, int start_queue, int num_queues, size_t dmabuf_size, void** p_buf, enum buf_type type );
//flow steering
int tcpx_flow_steering(char *ifname,int fd,int queue, int rule_idx);
void open_nic_rfs(char *ifname, char *src_ip, int src_port, char *dst_ip, int dst_port, int queue_id, int rule_idx);
void close_nic_rfs( char *ifname, int rule_idx );
//recv & send
int tcpx_recv_test(int socketfd, void *buf/*incomplete*/, size_t size, int flags/*incomplete*/, void* ctrl_data/*will be deleted soon*/, size_t ctrl_len/*will be deleted soon*/, struct dmabuf_token *token, int maxLen);
int tcpx_recv(int socketfd, void *buf/*incomplete*/, size_t size, int flags/*incomplete*/, void* ctrl_data/*will be deleted soon*/, size_t ctrl_len/*will be deleted soon*/);
void release_tcpx_buf( int fd, struct dmabuf_token *token, int num );
#ifdef __cplusplus
}
#endif
