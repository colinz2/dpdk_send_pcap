#include <cstdio>
#include <iostream>

#include "dpdk_import.h"

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
    

    return 0;
}
