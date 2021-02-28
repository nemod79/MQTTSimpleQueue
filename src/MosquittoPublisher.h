#ifndef MOSQUITTO_PUBLISHER_H_
#define MOSQUITTO_PUBLISHER_H_

#include <string>
#include <mosquitto.h>

class Publisher{
    private:
        // Underneath variables are used for broker detail connection
        std::string _MQTTBrokerAddress;
        int _port;     
        int _timeout;
       
        struct mosquitto *_mosq;                                                         // Generic Mosquitto object
        bool _connected;
        unsigned long long _msgCNT;

        // Connection callback, attached to connection state
        void OnConnectCallback(struct mosquitto *mosq, void *obj, int reason_code);   

        // Publish callback, invoked after ecah message published
        void OnPublishCallback(struct mosquitto *mosq, void *obj, int mid);             
        void Connect();

    public:
        // counter of instantiated object of this class. This is used for proper concurrent operation handling, libmosquitto is not thread safe
        static int publishers;                                                          

        Publisher(std::string MQTTBrokerAddress, int port, int timeout);

        ~Publisher();

        // Publish method is used in order to send messages to the MQTT Broker
        void Publish(void *Payload, int &PayloadLen, const std::string &Topic, const int &QOS, const bool &retain);

        // This method is used for wating CONNACK sent by broker on connreq.
        bool GetConnectionStatus();       

        // For statistic purposes can be useful to track the amount of sent messages
        unsigned long long GetMessageCNT();      
};   

#endif