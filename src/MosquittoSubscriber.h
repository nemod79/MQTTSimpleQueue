#ifndef MOSQUITTO_SUBSCRIBER_H_
#define MOSQUITTO_SUBSCRIBER_H_

#include <string>
#include <mosquitto.h>
#include "SimpleQueue.h"
  


class Subscriber{
    private:
        // the following variables are used for storing 
        std::string _MQTTBrokerAddress;
        int _port; 
        int _timeout; 
        int _QOS;   
        
        // Generic Mosquitto object, used in several callbacks
        struct mosquitto *_mosq;                    
        bool _subscribed;
        bool _connected;
        unsigned long long _msgCNT;
        std::string _topic;

        // FIXME: in future release the subscriber must be independent from parsed object
        SimpleQueue<std::string> *_localQueue;    

        void OnConnectCallback(struct mosquitto *mosq, void *obj, int reason_code);
        void OnSubscribeCallback(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos);
        void OnMessageCallback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);                    
        void Connect();

        // Only one topic per program instance can be "subscribed"
        // Subscription is used here in order to inform the MQTT Broker that we are interested on receiving 
        // those message that belongs to a specific TOPIC

        void Subscribe(const int &QOS, const bool &retain); 
           
        // On message reception we'll use PushMSG method for queueing messages  
        // FIXME: in future release the subscriber must be independent from parsed object
        void PushMSG(std::vector<std::string> *_queue, const struct mosquitto_message *msg); 

    public:
        // counter of instantiated object of this class, used only in concurrent execution due to not thread safe execution of mosquitto library
        static int subscribers;     
        Subscriber(std::string MQTTBrokerAddress, int port, int timeout, int QOS, std::string Topic, SimpleQueue<std::string> *localqueue);
        ~Subscriber();
};   

#endif