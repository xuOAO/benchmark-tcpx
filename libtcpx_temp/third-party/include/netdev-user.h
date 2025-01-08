/* SPDX-License-Identifier: ((GPL-2.0 WITH Linux-syscall-note) OR BSD-3-Clause) */
/* Do not edit directly, auto-generated from: */
/*	Documentation/netlink/specs/netdev.yaml */
/* YNL-GEN user header */

#ifndef _LINUX_NETDEV_GEN_H
#define _LINUX_NETDEV_GEN_H

#include <stdlib.h>
#include <string.h>
#include <linux/types.h>
#include <linux/netdev.h>

struct ynl_sock;

extern const struct ynl_family ynl_netdev_family;

/* Enums */
const char *netdev_op_str(int op);
const char *netdev_xdp_act_str(enum netdev_xdp_act value);
const char *netdev_xdp_rx_metadata_str(enum netdev_xdp_rx_metadata value);
const char *netdev_xsk_flags_str(enum netdev_xsk_flags value);
const char *netdev_queue_type_str(enum netdev_queue_type value);
// ERROR，所以于1219注释改声明
const char *netdev_qstats_scope_str(enum netdev_qstats_scope value);

/* Common nested types */
struct netdev_page_pool_info {
	struct {
		__u32 id:1;
		__u32 ifindex:1;
	} _present;

	__u64 id;
	__u32 ifindex;
};

struct netdev_queue_id {
	struct {
		__u32 id:1;
		__u32 type:1;
	} _present;

	__u32 id;
	enum netdev_queue_type type;
};

/* ============== NETDEV_CMD_DEV_GET ============== */
/* NETDEV_CMD_DEV_GET - do */
struct netdev_dev_get_req {
	struct {
		__u32 ifindex:1;
	} _present;

	__u32 ifindex;
};

static inline struct netdev_dev_get_req *netdev_dev_get_req_alloc(void)
{
	return (struct netdev_dev_get_req *)calloc(1, sizeof(struct netdev_dev_get_req));
}
void netdev_dev_get_req_free(struct netdev_dev_get_req *req);

static inline void
netdev_dev_get_req_set_ifindex(struct netdev_dev_get_req *req, __u32 ifindex)
{
	req->_present.ifindex = 1;
	req->ifindex = ifindex;
}

struct netdev_dev_get_rsp {
	struct {
		__u32 ifindex:1;
		__u32 xdp_features:1;
		__u32 xdp_zc_max_segs:1;
		__u32 xdp_rx_metadata_features:1;
		__u32 xsk_features:1;
	} _present;

	__u32 ifindex;
	__u64 xdp_features;
	__u32 xdp_zc_max_segs;
	__u64 xdp_rx_metadata_features;
	__u64 xsk_features;
};

void netdev_dev_get_rsp_free(struct netdev_dev_get_rsp *rsp);

/*
 * Get / dump information about a netdev.
 */
struct netdev_dev_get_rsp *
netdev_dev_get(struct ynl_sock *ys, struct netdev_dev_get_req *req);

/* NETDEV_CMD_DEV_GET - dump */
struct netdev_dev_get_list {
	struct netdev_dev_get_list *next;
	struct netdev_dev_get_rsp obj __attribute__((aligned(8)));
};

void netdev_dev_get_list_free(struct netdev_dev_get_list *rsp);

struct netdev_dev_get_list *netdev_dev_get_dump(struct ynl_sock *ys);

/* NETDEV_CMD_DEV_GET - notify */
struct netdev_dev_get_ntf {
	__u16 family;
	__u8 cmd;
	struct ynl_ntf_base_type *next;
	void (*free)(struct netdev_dev_get_ntf *ntf);
	struct netdev_dev_get_rsp obj __attribute__((aligned(8)));
};

void netdev_dev_get_ntf_free(struct netdev_dev_get_ntf *rsp);

/* ============== NETDEV_CMD_PAGE_POOL_GET ============== */
/* NETDEV_CMD_PAGE_POOL_GET - do */
struct netdev_page_pool_get_req {
	struct {
		__u32 id:1;
	} _present;

	__u64 id;
};

static inline struct netdev_page_pool_get_req *
netdev_page_pool_get_req_alloc(void)
{
	return (struct netdev_page_pool_get_req *)calloc(1, sizeof(struct netdev_page_pool_get_req));
}
void netdev_page_pool_get_req_free(struct netdev_page_pool_get_req *req);

static inline void
netdev_page_pool_get_req_set_id(struct netdev_page_pool_get_req *req, __u64 id)
{
	req->_present.id = 1;
	req->id = id;
}

struct netdev_page_pool_get_rsp {
	struct {
		__u32 id:1;
		__u32 ifindex:1;
		__u32 napi_id:1;
		__u32 inflight:1;
		__u32 inflight_mem:1;
		__u32 detach_time:1;
		__u32 dmabuf:1;
	} _present;

	__u64 id;
	__u32 ifindex;
	__u64 napi_id;
	__u64 inflight;
	__u64 inflight_mem;
	__u64 detach_time;
	__u32 dmabuf;
};

void netdev_page_pool_get_rsp_free(struct netdev_page_pool_get_rsp *rsp);

/*
 * Get / dump information about Page Pools.
(Only Page Pools associated with a net_device can be listed.)

 */
struct netdev_page_pool_get_rsp *
netdev_page_pool_get(struct ynl_sock *ys, struct netdev_page_pool_get_req *req);

/* NETDEV_CMD_PAGE_POOL_GET - dump */
struct netdev_page_pool_get_list {
	struct netdev_page_pool_get_list *next;
	struct netdev_page_pool_get_rsp obj __attribute__((aligned(8)));
};

void netdev_page_pool_get_list_free(struct netdev_page_pool_get_list *rsp);

struct netdev_page_pool_get_list *
netdev_page_pool_get_dump(struct ynl_sock *ys);

/* NETDEV_CMD_PAGE_POOL_GET - notify */
struct netdev_page_pool_get_ntf {
	__u16 family;
	__u8 cmd;
	struct ynl_ntf_base_type *next;
	void (*free)(struct netdev_page_pool_get_ntf *ntf);
	struct netdev_page_pool_get_rsp obj __attribute__((aligned(8)));
};

void netdev_page_pool_get_ntf_free(struct netdev_page_pool_get_ntf *rsp);

/* ============== NETDEV_CMD_PAGE_POOL_STATS_GET ============== */
/* NETDEV_CMD_PAGE_POOL_STATS_GET - do */
struct netdev_page_pool_stats_get_req {
	struct {
		__u32 info:1;
	} _present;

	struct netdev_page_pool_info info;
};

static inline struct netdev_page_pool_stats_get_req *
netdev_page_pool_stats_get_req_alloc(void)
{
	return (struct netdev_page_pool_stats_get_req *)calloc(1, sizeof(struct netdev_page_pool_stats_get_req));
}
void
netdev_page_pool_stats_get_req_free(struct netdev_page_pool_stats_get_req *req);

static inline void
netdev_page_pool_stats_get_req_set_info_id(struct netdev_page_pool_stats_get_req *req,
					   __u64 id)
{
	req->_present.info = 1;
	req->info._present.id = 1;
	req->info.id = id;
}
static inline void
netdev_page_pool_stats_get_req_set_info_ifindex(struct netdev_page_pool_stats_get_req *req,
						__u32 ifindex)
{
	req->_present.info = 1;
	req->info._present.ifindex = 1;
	req->info.ifindex = ifindex;
}

struct netdev_page_pool_stats_get_rsp {
	struct {
		__u32 info:1;
		__u32 alloc_fast:1;
		__u32 alloc_slow:1;
		__u32 alloc_slow_high_order:1;
		__u32 alloc_empty:1;
		__u32 alloc_refill:1;
		__u32 alloc_waive:1;
		__u32 recycle_cached:1;
		__u32 recycle_cache_full:1;
		__u32 recycle_ring:1;
		__u32 recycle_ring_full:1;
		__u32 recycle_released_refcnt:1;
	} _present;

	struct netdev_page_pool_info info;
	__u64 alloc_fast;
	__u64 alloc_slow;
	__u64 alloc_slow_high_order;
	__u64 alloc_empty;
	__u64 alloc_refill;
	__u64 alloc_waive;
	__u64 recycle_cached;
	__u64 recycle_cache_full;
	__u64 recycle_ring;
	__u64 recycle_ring_full;
	__u64 recycle_released_refcnt;
};

void
netdev_page_pool_stats_get_rsp_free(struct netdev_page_pool_stats_get_rsp *rsp);

/*
 * Get page pool statistics.
 */
struct netdev_page_pool_stats_get_rsp *
netdev_page_pool_stats_get(struct ynl_sock *ys,
			   struct netdev_page_pool_stats_get_req *req);

/* NETDEV_CMD_PAGE_POOL_STATS_GET - dump */
struct netdev_page_pool_stats_get_list {
	struct netdev_page_pool_stats_get_list *next;
	struct netdev_page_pool_stats_get_rsp obj __attribute__((aligned(8)));
};

void
netdev_page_pool_stats_get_list_free(struct netdev_page_pool_stats_get_list *rsp);

struct netdev_page_pool_stats_get_list *
netdev_page_pool_stats_get_dump(struct ynl_sock *ys);

/* ============== NETDEV_CMD_QUEUE_GET ============== */
/* NETDEV_CMD_QUEUE_GET - do */
struct netdev_queue_get_req {
	struct {
		__u32 ifindex:1;
		__u32 type:1;
		__u32 id:1;
	} _present;

	__u32 ifindex;
	enum netdev_queue_type type;
	__u32 id;
};

static inline struct netdev_queue_get_req *netdev_queue_get_req_alloc(void)
{
	return (struct netdev_queue_get_req *)calloc(1, sizeof(struct netdev_queue_get_req));
}
void netdev_queue_get_req_free(struct netdev_queue_get_req *req);

static inline void
netdev_queue_get_req_set_ifindex(struct netdev_queue_get_req *req,
				 __u32 ifindex)
{
	req->_present.ifindex = 1;
	req->ifindex = ifindex;
}
static inline void
netdev_queue_get_req_set_type(struct netdev_queue_get_req *req,
			      enum netdev_queue_type type)
{
	req->_present.type = 1;
	req->type = type;
}
static inline void
netdev_queue_get_req_set_id(struct netdev_queue_get_req *req, __u32 id)
{
	req->_present.id = 1;
	req->id = id;
}

struct netdev_queue_get_rsp {
	struct {
		__u32 id:1;
		__u32 type:1;
		__u32 napi_id:1;
		__u32 ifindex:1;
		__u32 dmabuf:1;
	} _present;

	__u32 id;
	enum netdev_queue_type type;
	__u32 napi_id;
	__u32 ifindex;
	__u32 dmabuf;
};

void netdev_queue_get_rsp_free(struct netdev_queue_get_rsp *rsp);

/*
 * Get queue information from the kernel. Only configured queues will be reported (as opposed to all available hardware queues).
 */
struct netdev_queue_get_rsp *
netdev_queue_get(struct ynl_sock *ys, struct netdev_queue_get_req *req);

/* NETDEV_CMD_QUEUE_GET - dump */
struct netdev_queue_get_req_dump {
	struct {
		__u32 ifindex:1;
	} _present;

	__u32 ifindex;
};

static inline struct netdev_queue_get_req_dump *
netdev_queue_get_req_dump_alloc(void)
{
	return (struct netdev_queue_get_req_dump *)calloc(1, sizeof(struct netdev_queue_get_req_dump));
}
void netdev_queue_get_req_dump_free(struct netdev_queue_get_req_dump *req);

static inline void
netdev_queue_get_req_dump_set_ifindex(struct netdev_queue_get_req_dump *req,
				      __u32 ifindex)
{
	req->_present.ifindex = 1;
	req->ifindex = ifindex;
}

struct netdev_queue_get_list {
	struct netdev_queue_get_list *next;
	struct netdev_queue_get_rsp obj __attribute__((aligned(8)));
};

void netdev_queue_get_list_free(struct netdev_queue_get_list *rsp);

struct netdev_queue_get_list *
netdev_queue_get_dump(struct ynl_sock *ys,
		      struct netdev_queue_get_req_dump *req);

/* ============== NETDEV_CMD_NAPI_GET ============== */
/* NETDEV_CMD_NAPI_GET - do */
struct netdev_napi_get_req {
	struct {
		__u32 id:1;
	} _present;

	__u32 id;
};

static inline struct netdev_napi_get_req *netdev_napi_get_req_alloc(void)
{
	return (struct netdev_napi_get_req *)calloc(1, sizeof(struct netdev_napi_get_req));
}
void netdev_napi_get_req_free(struct netdev_napi_get_req *req);

static inline void
netdev_napi_get_req_set_id(struct netdev_napi_get_req *req, __u32 id)
{
	req->_present.id = 1;
	req->id = id;
}

struct netdev_napi_get_rsp {
	struct {
		__u32 id:1;
		__u32 ifindex:1;
		__u32 irq:1;
		__u32 pid:1;
	} _present;

	__u32 id;
	__u32 ifindex;
	__u32 irq;
	__u32 pid;
};

void netdev_napi_get_rsp_free(struct netdev_napi_get_rsp *rsp);

/*
 * Get information about NAPI instances configured on the system.
 */
struct netdev_napi_get_rsp *
netdev_napi_get(struct ynl_sock *ys, struct netdev_napi_get_req *req);

/* NETDEV_CMD_NAPI_GET - dump */
struct netdev_napi_get_req_dump {
	struct {
		__u32 ifindex:1;
	} _present;

	__u32 ifindex;
};

static inline struct netdev_napi_get_req_dump *
netdev_napi_get_req_dump_alloc(void)
{
	return (struct netdev_napi_get_req_dump *)calloc(1, sizeof(struct netdev_napi_get_req_dump));
}
void netdev_napi_get_req_dump_free(struct netdev_napi_get_req_dump *req);

static inline void
netdev_napi_get_req_dump_set_ifindex(struct netdev_napi_get_req_dump *req,
				     __u32 ifindex)
{
	req->_present.ifindex = 1;
	req->ifindex = ifindex;
}

struct netdev_napi_get_list {
	struct netdev_napi_get_list *next;
	struct netdev_napi_get_rsp obj __attribute__((aligned(8)));
};

void netdev_napi_get_list_free(struct netdev_napi_get_list *rsp);

struct netdev_napi_get_list *
netdev_napi_get_dump(struct ynl_sock *ys, struct netdev_napi_get_req_dump *req);

/* ============== NETDEV_CMD_QSTATS_GET ============== */
/* NETDEV_CMD_QSTATS_GET - dump */
struct netdev_qstats_get_req_dump {
	struct {
		__u32 ifindex:1;
		__u32 scope:1;
	} _present;

	__u32 ifindex;
	__u64 scope;
};

static inline struct netdev_qstats_get_req_dump *
netdev_qstats_get_req_dump_alloc(void)
{
	return (struct netdev_qstats_get_req_dump *)calloc(1, sizeof(struct netdev_qstats_get_req_dump));
}
void netdev_qstats_get_req_dump_free(struct netdev_qstats_get_req_dump *req);

static inline void
netdev_qstats_get_req_dump_set_ifindex(struct netdev_qstats_get_req_dump *req,
				       __u32 ifindex)
{
	req->_present.ifindex = 1;
	req->ifindex = ifindex;
}
static inline void
netdev_qstats_get_req_dump_set_scope(struct netdev_qstats_get_req_dump *req,
				     __u64 scope)
{
	req->_present.scope = 1;
	req->scope = scope;
}

struct netdev_qstats_get_rsp_dump {
	struct {
		__u32 ifindex:1;
		__u32 queue_type:1;
		__u32 queue_id:1;
		__u32 rx_packets:1;
		__u32 rx_bytes:1;
		__u32 tx_packets:1;
		__u32 tx_bytes:1;
	} _present;

	__u32 ifindex;
	enum netdev_queue_type queue_type;
	__u32 queue_id;
	__u64 rx_packets;
	__u64 rx_bytes;
	__u64 tx_packets;
	__u64 tx_bytes;
};

struct netdev_qstats_get_rsp_list {
	struct netdev_qstats_get_rsp_list *next;
	struct netdev_qstats_get_rsp_dump obj __attribute__((aligned(8)));
};

void netdev_qstats_get_rsp_list_free(struct netdev_qstats_get_rsp_list *rsp);

struct netdev_qstats_get_rsp_list *
netdev_qstats_get_dump(struct ynl_sock *ys,
		       struct netdev_qstats_get_req_dump *req);

/* ============== NETDEV_CMD_BIND_RX ============== */
/* NETDEV_CMD_BIND_RX - do */
struct netdev_bind_rx_req {
	struct {
		__u32 ifindex:1;
		__u32 fd:1;
	} _present;

	__u32 ifindex;
	__u32 fd;
	unsigned int n_queues;
	struct netdev_queue_id *queues;
};

static inline struct netdev_bind_rx_req *netdev_bind_rx_req_alloc(void)
{
	return (struct netdev_bind_rx_req *)calloc(1, sizeof(struct netdev_bind_rx_req));
}
void netdev_bind_rx_req_free(struct netdev_bind_rx_req *req);

static inline void
netdev_bind_rx_req_set_ifindex(struct netdev_bind_rx_req *req, __u32 ifindex)
{
	req->_present.ifindex = 1;
	req->ifindex = ifindex;
}
static inline void
netdev_bind_rx_req_set_fd(struct netdev_bind_rx_req *req, __u32 fd)
{
	req->_present.fd = 1;
	req->fd = fd;
}
static inline void
__netdev_bind_rx_req_set_queues(struct netdev_bind_rx_req *req,
				struct netdev_queue_id *queues,
				unsigned int n_queues)
{
	free(req->queues);
	req->queues = queues;
	req->n_queues = n_queues;
}

struct netdev_bind_rx_rsp {
	struct {
		__u32 id:1;
	} _present;

	__u32 id;
};

void netdev_bind_rx_rsp_free(struct netdev_bind_rx_rsp *rsp);

/*
 * Bind dmabuf to netdev
 */
struct netdev_bind_rx_rsp *
netdev_bind_rx(struct ynl_sock *ys, struct netdev_bind_rx_req *req);

#endif /* _LINUX_NETDEV_GEN_H */
