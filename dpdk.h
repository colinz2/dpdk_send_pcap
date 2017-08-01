#ifndef DPDK_PORT_H_
#define DPDK_PORT_H_

#include <string>
#include <thread>
#include <mutex>

#include "dpdk_import.h"


class DpdkPort
{
public:
    DpdkPort(uint8_t port_id);
    ~DpdkPort();

    int PortInit(struct rte_mempool* mbuf_pool);
    int PortStart();
    void PortStop();

private:
    uint8_t port_id_;
    int socket_id_;
    int core_id_;
    std::string dev_name_;
    int rx_rings_;
    int tx_rings_;
    int num_rxdesc_;
    int num_txdesc_;
    struct rte_eth_dev_info dev_info_;
    struct rte_eth_conf port_conf_;
};


class DpdkRte
{
public:
    ~DpdkRte() {

    }
    
    static DpdkRte * Instance() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (rte_ == nullptr)
        {
            rte_ = new DpdkRte;
        }
        return rte_;
    }

    int RteInit(int argc, char *argv[]) {
        int ret = rte_eal_init(argc, argv);
        if (ret < 0) rte_exit(EXIT_FAILURE, "Invalid EAL parameters\n");

        core_num = rte_lcore_count();
        port_num = rte_eth_dev_count();
        return ret;
    }

    void Print() {
        printf("core_num = %d\n", core_num);
        printf("port_num = %d\n", port_num  );    
    }
public:
    int core_num;
    int port_num;
private:
    DpdkRte() {}
    DpdkRte & operator=(const DpdkRte & ) = delete;
    DpdkRte(const DpdkRte &) = delete;
private:
    static DpdkRte* rte_;
    static std::mutex mutex_;
};

DpdkRte * DpdkRte::rte_ = nullptr;
std::mutex DpdkRte::mutex_;


#endif


