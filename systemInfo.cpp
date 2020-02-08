
#include <iostream> /* cout */
#include <unistd.h>/* gethostname */
#include <netdb.h> /* struct hostent */
#include <arpa/inet.h> /* inet_ntop */
#include "systemInfo.h"
#include "log.h"
#include "getconfig.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <netinet/in.h>

#define MACLength 17
bool GetHostInfo(std::string& hostName, std::string& Ip) {
	char name[256];
	gethostname(name, sizeof(name)); // @suppress("Invalid arguments")
	hostName = name;

	struct hostent* host = gethostbyname(name); // @suppress("Invalid arguments")
	char ipStr[32];
	const char* ret = inet_ntop(host->h_addrtype, host->h_addr_list[0], ipStr, sizeof(ipStr));
	if (NULL==ret) {
		std::cout << "hostname transform to ip failed";
		return false;
	}
	Ip = ipStr;
	return true;
}

bool get_local_mac(std::string& macStr)
{
//    std::string macStr;
    int sockfd;
    struct ifreq tmp;
    char mac_addr[30];
    char mac[30];
    sockfd = socket(AF_INET, SOCK_STREAM, 0); // @suppress("Symbol is not resolved") // @suppress("Invalid arguments")
    if( sockfd < 0)
    {
        ERROR_LOG("create socket fail\n");
        return false;
    }

    memset(&tmp,0,sizeof(struct ifreq));
    string netBoard = getConfig("trade","NetBoard");
    strncpy(tmp.ifr_name,netBoard.c_str(),sizeof(tmp.ifr_name)-1); // @suppress("Field cannot be resolved") // @suppress("Invalid arguments")
    if( (ioctl(sockfd,SIOCGIFHWADDR,&tmp)) < 0 ) // @suppress("Symbol is not resolved") // @suppress("Function cannot be resolved")
    {
        ERROR_LOG("mac ioctl error\n");
        return false;
    }

    sprintf(mac_addr, "%02x:%02x:%02x:%02x:%02x:%02x",
            (unsigned char)tmp.ifr_hwaddr.sa_data[0],
            (unsigned char)tmp.ifr_hwaddr.sa_data[1],
            (unsigned char)tmp.ifr_hwaddr.sa_data[2],
            (unsigned char)tmp.ifr_hwaddr.sa_data[3],
            (unsigned char)tmp.ifr_hwaddr.sa_data[4],
            (unsigned char)tmp.ifr_hwaddr.sa_data[5]
            );
    close(sockfd);
    memcpy(mac,mac_addr,strlen(mac_addr));
    mac[17] = '\0';
    macStr = std::string(mac);
    return true;
}



