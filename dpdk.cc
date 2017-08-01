#include "dpdk.h"

#include <string>


// static const struct rte_eth_conf port_conf_default = {
//     .rxmode = {
//         .mq_mode = ETH_MQ_RX_NONE,
//         .max_rx_pkt_len = ETHER_MAX_LEN,
//     }
// };

DpdkPort::DpdkPort(uint8_t port_id)
    : port_id_(port_id)
{
    port_conf_.rxmode.mq_mode = ETH_MQ_RX_RSS;
    port_conf_.rx_adv_conf.rss_conf.rss_key = NULL; // use defaut DPDK-key
    port_conf_.rx_adv_conf.rss_conf.rss_hf = ETH_RSS_IP | ETH_RSS_TCP | ETH_RSS_UDP;
    // Tune tx
    port_conf_.txmode.mq_mode = ETH_MQ_TX_NONE;
}

DpdkPort::~DpdkPort()
{

}

int DpdkPort::PortInit(struct rte_mempool* mbuf_pool)
{
    int ret = 0;
    char dev_name[128];

    if (rte_eth_dev_is_valid_port(port_id_) == 0) {
        rte_exit(EXIT_FAILURE, "fail to get port name \n");
    }

    ret = rte_eth_dev_get_name_by_port(port_id_, dev_name);
    if (ret != 0) {
        rte_exit(EXIT_FAILURE, "fail to get port name \n");
    }
    dev_name_ = std::string(dev_name);

    socket_id_ = rte_eth_dev_socket_id(port_id_);
    core_id_  = rte_lcore_id();

    rte_eth_dev_info_get(1, &dev_info_);

    if (rx_rings_ > 1) {
        port_conf_.rxmode.mq_mode = ETH_MQ_RX_RSS;
        port_conf_.rx_adv_conf.rss_conf.rss_key = NULL;
        port_conf_.rx_adv_conf.rss_conf.rss_hf = ETH_RSS_PROTO_MASK;
    }
    //发包时是否释放mbuf的阈值，可以配置

    ret = rte_eth_dev_configure(port_id_, rx_rings_, 0, &port_conf_);
    if (ret) {
        //RTE_LOG(ERR, DPDKCAP, "rte_eth_dev_configure(...): %s\n", rte_strerror(-ret));
        return ret;
    }

    for (int q = 0; q < rx_rings_; q++) {
        ret = rte_eth_rx_queue_setup(port_id_, q, num_rxdesc_, socket_id_, NULL, mbuf_pool);
        if (ret) {
            //RTE_LOG(ERR, DPDKCAP, "rte_eth_rx_queue_setup(...): %s\n", rte_strerror(-ret));
            return ret;
        }
    }

    for (int q = 0; q < tx_rings_; q++) {
        ret = rte_eth_tx_queue_setup(port_id_, q, num_txdesc_, socket_id_, NULL);
        if (ret) {
            //RTE_LOG(ERR, DPDKCAP, "rte_eth_tx_queue_setup(...): %s\n", rte_strerror(-ret));
            return ret;
        }
    }

    rte_eth_promiscuous_enable(port_id_);

    return 0;
}

int DpdkPort::PortStart()
{
    int ret = rte_eth_dev_start(port_id_);
    if (ret) {
        rte_exit(EXIT_FAILURE, "Cannot start port %d \n", port_id_);
    }
    return 0;
}

void DpdkPort::PortStop()
{
    rte_eth_dev_stop(port_id_);
}

// int DpdkRte::RteInit(int argc, char *argv[])
// {
//     int ret = rte_eal_init(argc, argv);
//     if (ret < 0) rte_exit(EXIT_FAILURE, "Invalid EAL parameters\n");

//     core_num = rte_lcore_count();
//     port_num = rte_eth_dev_count();
//     return ret;
// }

// void DpdkRte::Print()
// {
//     printf("core_num = %d\n", core_num);
//     printf("port_num = %d\n", core_num);
// }



// DpdkRte* DpdkRte::Instance() 
// {
//     std::lock_guard<std::mutex> lock(mutex_);
//     if (rte_ == NULL) {
//         rte_ = new DpdkRte();
//     }
//     return rte_;
// }