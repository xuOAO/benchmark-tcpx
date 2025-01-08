//Debug
#define PRINTFUNCLINEFILE   \
printf(DEBUG_HEAD "DEBUGINFO :" DEBUG_TAIL " func : %-20s, line : %04d, file : %s\r\n", __FUNCTION__, __LINE__, __FILE__);

//Ye Debug
static const int YE_START_QUEUE = 10;
static const int YE_FLAG = 0;

#define DEBUG_HEAD "\033[31;1m"
#define DEBUG_TAIL "\033[0m"

#define YE_DEBUG_241(string, ...)   \
{printf(DEBUG_HEAD "[DEBUGINFO] : " DEBUG_TAIL string "\n", ##__VA_ARGS__);}

static inline int tcpx_show_link_info(int fd) {
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
        // perror("Fail to get peer name\n");
    }
    inet_ntop(server_addr.sin_family, &server_addr.sin_addr, server_ip, sizeof(server_ip));
    inet_ntop(client_addr.sin_family, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    int server_port = ntohs(server_addr.sin_port);
    int client_port = ntohs(client_addr.sin_port);
    YE_DEBUG_241("Link fd = %5d",fd);
    YE_DEBUG_241("Server IP = %-20s,Server port = %5d", server_ip,server_port);
    YE_DEBUG_241("Client IP = %-20s,Client port = %5d", client_ip,client_port);
    return 0;
}

//Xu Debug
#define XU_DEBUG(string, ...)   \
({printf("DEBUGINFO : " string, ##__VA_ARGS__);})      \

//utilities
#define run_command(cmd, ...)                                   \
({                                                              \
    char command[256];                                          \
    memset(command, 0, sizeof(command));                        \
    snprintf(command, sizeof(command), cmd, ##__VA_ARGS__);     \
    printf("Running:%s\n", command);                            \
    system(command);                                            \
})

#ifdef __cplusplus
extern "C"{
#endif

// static inline void cudaDriverCheckError(CUresult cudaStatus)
// {
//     const char *err;
//     cuGetErrorString(cudaStatus, &err);
//     if (cudaStatus != CUDA_SUCCESS) {
//         fprintf(stderr, "CUDA Error: %s at %s:%d\n", err, __FILE__, __LINE__);
//         exit(EXIT_FAILURE);
//     }
// }

#ifdef __cplusplus
}
#endif