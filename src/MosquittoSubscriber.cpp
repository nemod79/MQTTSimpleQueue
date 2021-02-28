#include "MosquittoSubscriber.h"
#include <iostream>

int Subscriber::subscribers {0};


Subscriber::Subscriber(std::string MQTTBrokerAddress, int port, int timeout, int QOS, std::string Topic, SimpleQueue<std::string> *localqueue) : _MQTTBrokerAddress(MQTTBrokerAddress),_port(port),_timeout(timeout), _QOS(QOS), _topic(Topic) {
    _msgCNT = 0;
    _localQueue = localqueue;
    if (Subscriber::subscribers == 0)
        mosquitto_lib_init();

    _mosq = mosquitto_new(NULL, true, this);

    Subscriber::subscribers++;
    
    this->Connect();
    this->Subscribe(_QOS,false);    // FIXME : in future release we should handle also message retention feature
}


void Subscriber::Connect() {
    // Set of callbacks triggered by several MOSQUITTO events
    // change hereunder for implementing or tailoring features

    mosquitto_connect_callback_set(_mosq, [](struct mosquitto *mosq, void *obj, int reason_code) { ((Subscriber *)obj)->OnConnectCallback(mosq,obj,reason_code); });
    mosquitto_subscribe_callback_set(_mosq, [](struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos) { ((Subscriber *)obj)->OnSubscribeCallback(mosq,obj,mid,qos_count,granted_qos); });
    mosquitto_message_callback_set(_mosq, [](struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) { ((Subscriber *)obj)->OnMessageCallback(mosq,obj,msg); });
    mosquitto_connect(_mosq, _MQTTBrokerAddress.c_str(), _port, _timeout);      // Connection will loop here up until broker will become available
    mosquitto_threaded_set(_mosq,true);
}

Subscriber::~Subscriber() {
    mosquitto_destroy(_mosq);
    _connected = false;
    Subscriber::subscribers--;

    if (Subscriber::subscribers == 0)
        mosquitto_lib_cleanup();

    _localQueue = NULL;
}

void Subscriber::OnConnectCallback(struct mosquitto *mosq, void *obj, int reason_code) {
    // Called right after connection, any functionality related to CONNECT state handling have to be implemented here
}

void Subscriber::OnSubscribeCallback(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos) {
    // Called right after SUBSCRIBE, any functionality related to SUBSCRIPTION state have to be implemented here
}

void Subscriber::OnMessageCallback(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg) {
    // The following object need to be adapted according to the queue object type, in future try to figure out how to 
    // abstract this as well, in order to define the conversion from void * to queue object with a template like approach

    _localQueue->PushElement((char *)(msg->payload));
}

void Subscriber::Subscribe(const int &QOS, const bool &retain) {
    mosquitto_subscribe(_mosq, NULL, _topic.c_str(), 0);
    mosquitto_loop_forever(_mosq,-1,1);
    // After subscription the MQTT network function will be handled by MOSQUITTO THREAD
}