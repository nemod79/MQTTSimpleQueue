#include "MosquittoPublisher.h"

int Publisher::publishers {0};

Publisher::Publisher(std::string MQTTBrokerAddress, int port, int timeout) : _MQTTBrokerAddress(MQTTBrokerAddress), _port(port), _timeout(timeout) {
    _msgCNT = 0;
    // Since it is not thread safe we will handle lib mosquitto init with care...
    if (Publisher::publishers == 0)
        mosquitto_lib_init();

    _mosq = mosquitto_new(NULL, true, this);
    Publisher::publishers++;
    
    this->Connect();
}

void Publisher::Connect() {
    mosquitto_connect_callback_set(_mosq, [](struct mosquitto *mosq, void *obj, int reason_code) { ((Publisher *)obj)->OnConnectCallback(mosq,obj,reason_code); });
    mosquitto_publish_callback_set(_mosq, [](struct mosquitto *mosq, void *obj, int mid) { ((Publisher *)obj)->OnPublishCallback(mosq,obj,mid); });
    // Connection will loop here up until broker will become available
    mosquitto_connect(_mosq, _MQTTBrokerAddress.c_str(), _port, _timeout);      
    mosquitto_loop_start(_mosq);
}

Publisher::~Publisher() {
    mosquitto_destroy(_mosq);
    _connected = false;

    Publisher::publishers--;

    if (Publisher::publishers == 0)
        mosquitto_lib_cleanup();
}

void Publisher::OnConnectCallback(struct mosquitto *mosq, void *obj, int reason_code) {
    if (!_connected )
        _connected = true;
}

void Publisher::OnPublishCallback(struct mosquitto *mosq, void *obj, int mid) {
    ++Publisher::_msgCNT;
}

void Publisher::Publish(void *Payload,int &PayloadLen, const std::string &Topic,const int &QOS, const bool &retain) {
    mosquitto_publish(_mosq, NULL, Topic.c_str(), PayloadLen, Payload, QOS, retain);
}

bool Publisher::GetConnectionStatus() { return Publisher::_connected; }

unsigned long long Publisher::GetMessageCNT() { return Publisher::_msgCNT; }