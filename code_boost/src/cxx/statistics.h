/*
 * Class providing containers and access methods for statistical data collection.
 */
#ifndef CPP_PCAPREADER_STATISTICS_H
#define CPP_PCAPREADER_STATISTICS_H

// Aidmar
#include <vector>

#include <unordered_map>
#include <list>
#include <tuple>
#include <tins/timestamp.h>
#include <tins/ip_address.h>


/*
 * Definition of structs used in unordered_map fields
 */


/*
 * Struct used as data structure for method get_stats_for_ip, represents:
 * - Incoming bandwidth in KBits
 * - Outgoing bandwidth in KBits
 * - Number of incoming packets per second
 * - Number of outgoing packets per second
 * - Average size of sent packets in kbytes
 * - Average size of received packets in kybtes
 * - Average value of TCP option Maximum Segment Size (MSS)
 */
struct ip_stats {
    float bandwidthKBitsIn;
    float bandwidthKBitsOut;
    float packetPerSecondIn;
    float packetPerSecondOut;
    float AvgPacketSizeSent;
    float AvgPacketSizeRecv;
    long AvgMaxSegmentSizeTCP;
};

// Aidmar
/*
 * Struct used to represent a flow by:
 * - IP address A
 * - Port A
 * - IP address B
 * - Port B
 */
struct flow{
    std::string ipAddressA;
    int portA;
    std::string ipAddressB;
    int portB;

    bool operator==(const flow &other) const {
        return ipAddressA == other.ipAddressA
               && portA == other.portA
               &&ipAddressB == other.ipAddressB
               && portB == other.portB;
    }    
}; 


// Aidmar
/*
 * Struct used to represent:
 * - IP address (IPv4 or IPv6)
 * - MSS value
 */
struct ipAddress_mss {
    std::string ipAddress;
    int mssValue;

    bool operator==(const ipAddress_mss &other) const {
        return ipAddress == other.ipAddress
               && mssValue == other.mssValue;
    }
};

// Aidmar
/*
 * Struct used to represent:
 * - IP address (IPv4 or IPv6)
 * - Window size
 */
struct ipAddress_win {
    std::string ipAddress;
    int winSize;

    bool operator==(const ipAddress_win &other) const {
        return ipAddress == other.ipAddress
               && winSize == other.winSize;
    }
};


/*
 * Struct used to represent:
 * - IP address (IPv4 or IPv6)
 * - TTL value
 */
struct ipAddress_ttl {
    std::string ipAddress;
    int ttlValue;

    bool operator==(const ipAddress_ttl &other) const {
        return ipAddress == other.ipAddress
               && ttlValue == other.ttlValue;
    }
};


/*
 * Struct used to represent:
 * - IP address (IPv4 or IPv6)
 * - Protocol (e.g. TCP, UDP, IPv4, IPv6)
 */
struct ipAddress_protocol {
    std::string ipAddress;
    std::string protocol;

    bool operator==(const ipAddress_protocol &other) const {
        return ipAddress == other.ipAddress
               && protocol == other.protocol;
    }
};

/*
 * Struct used to represent:
 * - Number of received packets
 * - Number of sent packets
 * - Data received in kbytes
 * - Data sent in kbytes
 */
struct entry_ipStat {
    long pkts_received;
    long pkts_sent;
    float kbytes_received;
    float kbytes_sent;
    // Aidmar - to calculate tn/r score
    long firstAppearAsSenderPktCount;
    long firstAppearAsReceiverPktCount;
    long sourceAnomalyScore;
    long destinationAnomalyScore;

    bool operator==(const entry_ipStat &other) const {
        return pkts_received == other.pkts_received
               && pkts_sent == other.pkts_sent
               && kbytes_sent == other.kbytes_sent
               && kbytes_received == other.kbytes_received
               // Aidmar
               && firstAppearAsSenderPktCount == other.firstAppearAsSenderPktCount
               && firstAppearAsReceiverPktCount == other.firstAppearAsReceiverPktCount
               && sourceAnomalyScore == other.sourceAnomalyScore
               && destinationAnomalyScore == other.destinationAnomalyScore;
    }
};

// Aidmar
/*
 * Struct used to represent:
 * - Number of packets from A to B
 * - Number of packets from B to A
 */
struct entry_flowStat {
    long pkts_A_B;
    long pkts_B_A;
    std::vector<std::chrono::microseconds> pkts_A_B_timestamp;
    std::vector<std::chrono::microseconds> pkts_B_A_timestamp;
    std::vector<std::chrono::microseconds> pkts_delay;
    //std::chrono::duration<double, std::micro> median_delay;
    std::chrono::microseconds median_delay;
    
    bool operator==(const entry_flowStat &other) const {
        return pkts_A_B == other.pkts_A_B
               && pkts_A_B_timestamp == other.pkts_A_B_timestamp
               && pkts_B_A_timestamp == other.pkts_B_A_timestamp
               && pkts_delay == other.pkts_delay
               && median_delay == other.median_delay;
    }
};

/*
 * Struct used to represent:
 * - IP address (IPv4 or IPv6)
   - Traffic direction (out: outgoing connection, in: incoming connection)
 * - Port number
 */
struct ipAddress_inOut_port {
    std::string ipAddress;
    std::string trafficDirection;
    int portNumber;

    bool operator==(const ipAddress_inOut_port &other) const {
        return ipAddress == other.ipAddress
               && trafficDirection == other.trafficDirection
               && portNumber == other.portNumber;
    }

};

/*
 * Definition of hash functions for structs used as key in unordered_map
 */
namespace std {
    template<>
    struct hash<ipAddress_ttl> {
        std::size_t operator()(const ipAddress_ttl &k) const {
            using std::size_t;
            using std::hash;
            using std::string;
            return ((hash<string>()(k.ipAddress)
                     ^ (hash<int>()(k.ttlValue) << 1)) >> 1);
        }
    };

    // Aidmar
      template<>
    struct hash<ipAddress_mss> {
        std::size_t operator()(const ipAddress_mss &k) const {
            using std::size_t;
            using std::hash;
            using std::string;
            return ((hash<string>()(k.ipAddress)
                     ^ (hash<int>()(k.mssValue) << 1)) >> 1);
        }
    };

    // Aidmar
      template<>
    struct hash<ipAddress_win> {
        std::size_t operator()(const ipAddress_win &k) const {
            using std::size_t;
            using std::hash;
            using std::string;
            return ((hash<string>()(k.ipAddress)
                     ^ (hash<int>()(k.winSize) << 1)) >> 1);
        }
    };
    
    // Aidmar: TO-DO:??
    template<>
    struct hash<flow> {
        std::size_t operator()(const flow &k) const {
            using std::size_t;
            using std::hash;
            using std::string;
            return ((hash<string>()(k.ipAddressA)
                     ^ (hash<int>()(k.portA) << 1)) >> 1)
                     ^ ((hash<string>()(k.ipAddressB)
                     ^ (hash<int>()(k.portB) << 1)) >> 1);
        }
    };
    
    template<>
    struct hash<ipAddress_protocol> {
        std::size_t operator()(const ipAddress_protocol &k) const {
            using std::size_t;
            using std::hash;
            using std::string;
            return ((hash<string>()(k.ipAddress)
                     ^ (hash<string>()(k.protocol) << 1)) >> 1);
        }
    };

    template<>
    struct hash<ipAddress_inOut_port> {
        std::size_t operator()(const ipAddress_inOut_port &k) const {
            using std::size_t;
            using std::hash;
            using std::string;
            return ((hash<string>()(k.ipAddress)
                     ^ (hash<string>()(k.trafficDirection) << 1)) >> 1)
                   ^ (hash<int>()(k.portNumber) << 1);
        }
    };
}

class statistics {
public:
    /*
     * Constructor
     */
    statistics();

    /*
     * Methods
     */
    std::string getFormattedTimestamp(time_t seconds, suseconds_t microseconds) const;

    /*
    * Access methods for containers
    */
    void incrementPacketCount();

    // Adimar
    void incrementMSScount(std::string ipAddress, int mssValue);
    void incrementWinCount(std::string ipAddress, int winSize);
    void addIPEntropy();
    void addFlowStat(std::string ipAddressSender,int sport,std::string ipAddressReceiver,int dport, std::chrono::microseconds timestamp);
    

    void incrementTTLcount(std::string ipAddress, int ttlValue);

    void incrementProtocolCount(std::string ipAddress, std::string protocol);

    void incrementPortCount(std::string ipAddressSender, int outgoingPort, std::string ipAddressReceiver,
                            int incomingPort);

    int getProtocolCount(std::string ipAddress, std::string protocol);

    void setTimestampFirstPacket(Tins::Timestamp ts);

    void setTimestampLastPacket(Tins::Timestamp ts);

    void assignMacAddress(std::string ipAddress, std::string macAddress);

    void addIpStat_packetSent(std::string ipAddressSender, std::string ipAddressReceiver, long bytesSent);

    void addMSS(std::string ipAddress, int MSSvalue);

    void writeToDatabase(std::string database_path);

    void addPacketSize(uint32_t packetSize);

    std::string getCaptureDurationTimestamp() const;

    float getCaptureDurationSeconds() const;

    float getAvgPacketSize() const;

    void printStats(std::string ipAddress);

    /*
     * IP Address-specific statistics
     */
    ip_stats getStatsForIP(std::string ipAddress);


private:
    /*
     * Data fields
     */
    Tins::Timestamp timestamp_firstPacket;
    Tins::Timestamp timestamp_lastPacket;
    float sumPacketSize = 0;
    int packetCount = 0;

    /*
     * Data containers
     */
    // {IP Address, TTL value, count}
    std::unordered_map<ipAddress_ttl, int> ttl_distribution;

    // Aidmar
    // {IP Address, MSS value, count}
    std::unordered_map<ipAddress_mss, int> mss_distribution;
    // {IP Address, Win size, count}
    std::unordered_map<ipAddress_win, int> win_distribution;
    // {IP Address A, Port A, IP Address B, Port B,   #packets_A_B, #packets_B_A}
    std::unordered_map<flow, entry_flowStat> flow_statistics;
    
    // {IP Address, Protocol, count}
    std::unordered_map<ipAddress_protocol, int> protocol_distribution;

    // {IP Address,  #received packets, #sent packets, Data received in kbytes, Data sent in kbytes}
    std::unordered_map<std::string, entry_ipStat> ip_statistics;

    // {IP Address, in_out, Port Number, count}
    std::unordered_map<ipAddress_inOut_port, int> ip_ports;

    // {IP Address, MAC Address}
    std::unordered_map<std::string, std::string> ip_mac_mapping;

    // {IP Address, avg MSS}
    std::unordered_map<std::string, int> ip_sumMss;
};


#endif //CPP_PCAPREADER_STATISTICS_H
