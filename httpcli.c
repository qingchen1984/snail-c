#ifdef WIN32
#include "sdns.h"
#else
#include "../src/sdns.h"
#endif

#define YOUR_ID 10000
#define SDNS_DES_ID   YOUR_ID
#define SDNS_DES_KEY  "abcabcababcabcab"
#define BUF_SIZE 102400


/*#pragma commenct(lib,"http_dns.lib")*/

#ifdef WIN32
#pragma comment(lib,"libeay32MT.lib")
#pragma comment(lib,"ssleay32MT.lib")

#pragma comment(lib, "ws2_32.lib")
#endif


int main(int argc, char **argv)
{
    struct addrinfo *answer, hint, *curr;
    char ipstr[16];
    int ret; 
    //int sfd;
    //char http_data[BUF_SIZE];
    char *domain;

    if (argc != 2) {
        //fprintf(stderr, "Usage: %s hostname\n", argv[0]);
        //exit(1);
        domain = "test1.example.com";
    }
    else {
        domain = argv[1];
    }
	
    //init sdnslus environment
    sdns_set_server_port("192.168.5.165", 80);
    sdns_set_publicdns_server_port("192.168.5.127", 53);
    sdns_set_cache_mem(4*1024*1024);
    sdns_set_ttl(90);

//#ifdef ENTERPRISE_EDITION
    //设置企业版加密ID和KEY
    sdns_set_des_id_key(SDNS_DES_ID, SDNS_DES_KEY);
//#endif
	//printf("zhaoqingya...[%s]\n",sdns_des_encrypt("www.example.com"));
	//return 0;
	//printf("1...\n");
    sdns_env_init();
	//printf("2...\n");
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
	//printf("3...\n");
    //first
    ret = sdns_getaddrinfo(domain, "http", &hint, &answer);
	//printf("4...[%d]\n",ret);
    if (ret != 0) {
        fprintf(stderr, "can't resolve  %s\n", domain);
        sdns_env_destroy();
        return 1;
    }
	//printf("5...\n");
    for (curr = answer; curr != NULL; curr = curr->ai_next) {
        inet_ntop(AF_INET, &(((struct sockaddr_in *)(curr->ai_addr))->sin_addr),
            ipstr, sizeof(ipstr));
        printf("%s\n", ipstr);
    }
	//printf("6...\n");
    sdns_freeaddrinfo(answer);
    sdns_env_destroy();
    return 0;
}
