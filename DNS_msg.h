
#include <stdint.h>

// DNS 报文头部
typedef struct {
    uint16_t id;        // 标识
    uint16_t flags;     // 标志
    uint16_t qdcount;   // 问题数量
    uint16_t ancount;   // 回答数量
    uint16_t nscount;   // 授权数量
    uint16_t arcount;   // 附加数量
} dns_header_t;

// DNS 问题
typedef struct {
    char *qname;        // 问题名称
    uint16_t qtype;     // 问题类型
    uint16_t qclass;    // 问题类
} dns_question_t;

// DNS 资源记录
typedef struct {
    char *name;         // 名称
    uint16_t type;      // 类型
    uint16_t class;     // 类
    uint32_t ttl;       // 生存时间
    uint16_t rdlength;  // 数据长度
    void *rdata;        // 数据
} dns_rr_t;

// DNS 报文
typedef struct {
    dns_header_t header;
    dns_question_t *questions;
    dns_rr_t *answers;
    dns_rr_t *authorities;
    dns_rr_t *additionals;
} dns_msg_t;

dns_msg_t *parse_dns_message(const char *data, size_t len);