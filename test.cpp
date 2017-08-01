#include <cstdio>
#include <iostream>

#include "dpdk_import.h"

#include "dpdk.h"
#include "pcap.h"


int main(int argc, char *argv[])
{
    int ret;
    uint32_t nb_lcores, nb_sys_ports;

    ret = rte_eal_init(argc, argv);

    if (ret < 0) rte_exit(EXIT_FAILURE, "Invalid EAL parameters\n");
    argc -= ret;
    argv += ret;

    nb_lcores = rte_lcore_count();

    std::cout << "nb_lcores = " << nb_lcores << std::endl;
    if (nb_lcores != 1) rte_exit(EXIT_FAILURE, "This application needs 1 core. \n");

    nb_sys_ports = rte_eth_dev_count();
    if (nb_sys_ports <= 0) rte_exit(EXIT_FAILURE, "Cannot find ETH devices\n");
    std::cout << "port num " << nb_sys_ports << std::endl;

    int socket_id = rte_eth_dev_socket_id(1);

    std::cout << "socket id = " << socket_id << std::endl;
    char dev_name[128] = {0};
    rte_eth_dev_get_name_by_port(1, dev_name);
    std::cout << dev_name << std::endl;

    std::cout << rte_get_master_lcore() << std::endl;

    struct rte_eth_dev_info dev_info;
    rte_eth_dev_info_get(1, &dev_info);

    printf("%d, %d, %d   \n",
           dev_info.max_rx_pktlen,
           dev_info.max_rx_queues,
           dev_info.max_tx_queues
          );

    return 0;
}