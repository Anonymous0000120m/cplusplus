#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <iostream>
#include <stdexcept>
#include "spdlog/spdlog.h"

class PortScanner {
public:
    PortScanner(const char* device, const char* logFile) {
        handle = pcap_open_live(device, 65536, 1, 1000, errbuf);
        if (handle == nullptr) {
            throw std::runtime_error("Couldn't open device: " + std::string(errbuf));
        }

        auto console = spdlog::stdout_color_mt("console");
        log = std::make_shared<spdlog::logger>("portscanner", console->sinks().begin(), console->sinks().end());
        log->set_level(spdlog::level::info);
        log->info("Port Scanner Initialized");
    }

    ~PortScanner() {
        pcap_close(handle);
        log->info("Port Scanner Closed");
    }

    void scan(const char* ipAddress) {
        struct bpf_program fp;
        if (pcap_compile(handle, &fp, "tcp and dst host " + std::string(ipAddress), 0, PCAP_NETMASK_UNKNOWN) == -1) {
            throw std::runtime_error("Failed to compile filter");
        }
        if (pcap_setfilter(handle, &fp) == -1) {
            throw std::runtime_error("Failed to install filter");
        }

        pcap_loop(handle, 0, [](u_char *user, const pcap_pkthdr *pkthdr, const u_char *packet) {
            struct ip *ip_header = (struct ip*)(packet + 14);
            if (ip_header->ip_p == IPPROTO_TCP) {
                struct tcphdr *tcp_header = (struct tcphdr*)(packet + 14 + ip_header->ip_hl * 4);
                log->info("Found TCP packet from: {}:{}", inet_ntoa(ip_header->ip_src), ntohs(tcp_header->th_sport));
            }
        }, nullptr);

        pcap_freecode(&fp);
    }

private:
    pcap_t *handle;
    char errbuf[PCAP_ERRBUF_SIZE];
    std::shared_ptr<spdlog::logger> log;
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        spdlog::error("Usage: ./portscanner <device> <logFile> <IP address>");
        return 1;
    }

    try {
        PortScanner portScanner(argv[1], argv[2]);
        portScanner.scan(argv[3]);
    } catch (const std::exception& e) {
        spdlog::error("An exception occurred: {}", e.what());
        return 1;
    }

    return 0;
}
