//
// Created by nocnname on 2024/6/30.
//

#include "DNS_msg.h"
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>


// 解析 DNS 报文
dns_msg_t *parse_dns_message(const char *data, size_t len) {
    dns_msg_t* message = (dns_msg_t *)malloc(sizeof(dns_msg_t));
    size_t pos = 0;

    // 解析报文头部
    memcpy(&message->header, data + pos, sizeof(dns_header_t));
    message->header.id = ntohs(message->header.id);
    message->header.flags = ntohs(message->header.flags);
    message->header.qdcount = ntohs(message->header.qdcount);
    message->header.ancount = ntohs(message->header.ancount);
    message->header.nscount = ntohs(message->header.nscount);
    message->header.arcount = ntohs(message->header.arcount);
    pos += sizeof(dns_header_t);

    // 解析问题部分
    message->questions = (dns_question_t *)malloc(sizeof(dns_question_t) * message->header.qdcount);
    for (int i = 0; i < message->header.qdcount; i++) {
        // 解析域名
        // ...
        message->questions[i].qtype = ntohs(*(uint16_t *)(data + pos));
        pos += 2;
        message->questions[i].qclass = ntohs(*(uint16_t *)(data + pos));
        pos += 2;
    }

    // 解析回答、授权和附加部分
    // ...

    return message;
}