#include "MosquittoPublisher.h"
#include "MosquittoSubscriber.h"
#include "SimpleQueue.h"
#include <iostream>
#include <memory>
#include <thread>
#include <queue>
#include <chrono>
#include <string>
#include <stdint.h>
#include <fstream>

void PublishingDemoEntityOne() {
    // This demo function is provided for mimiching typical "sensor" in a standard MQTT environment
    // we are sending out MSGToBeSent messages to broker
    Publisher publisher("broker.hivemq.com",1883,30);
    int MSGToBeSent = 10;
    // The following loop is required in order to avoid a static wait, after broker connection the first message can be sent only 1sec later.
    while(! publisher.GetConnectionStatus() ){
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    for (int i=0; i< MSGToBeSent; i++){
        std::string message = "Entity #1 MSG # "+ std::to_string(i) + " demo payload";   
        std::vector<uint8_t> tmpVector(message.c_str(), message.c_str() + message.size() + 1);
        void *myPTR = static_cast<void *>(tmpVector.data());
        int size = tmpVector.size();
        publisher.Publish(myPTR, size,"exampleDAG",0,false);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "E1 MSG Sent # " << publisher.GetMessageCNT() << " messages to the broker" <<  std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void PublishingDemoEntityTwo() {
    // This demo function is provided for mimiching typical "sensor" in a standard MQTT environment
    // We are sending MSGToBeSent messages to the broker

    Publisher publisher("broker.hivemq.com",1883,30);
    int MSGToBeSent = 10;
    // The following loop is required in order to avoid a static wait, after broker connection the first message can be sent only 1sec later.
    while(! publisher.GetConnectionStatus() ){
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    for (int i=0; i< MSGToBeSent; i++){
        std::string message = "Entity #2 MSG # "+ std::to_string(i) + " demo payload";   
        std::vector<uint8_t> tmpVector(message.c_str(), message.c_str() + message.size() + 1);
        void *myPTR = static_cast<void *>(tmpVector.data());
        int size = tmpVector.size();
        publisher.Publish(myPTR, size,"exampleDAG",0,false);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "E2 MSG Sent # " << publisher.GetMessageCNT() << " messages to the broker" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}



int main(int argc, char* argv[]){

    std::string address{"broker.hivemq.com"};
    int port{1883};
    int timeout{60};
    int QOS{0};
    std::string topic{"exampleDAG"};
    bool logging{false};

    // The only user input supported are :
    // -h / -H     --> this will print available options
    // -l filename --> log to file messages popped from the queue

    if (argc == 1)
        logging = false;
    else if (argc == 2){
            std::string option(argv[1]);
            if (option == "-h" || option == "-H") 
                std::cout << "For logging functionality please provide add -l filename to the program name" << std::endl;
            else if (option == "-l")
                std::cout << "Filename is missing, loggin option disabled !!!" << std::endl;
            else
                std::cout << "Requested option not recognized !!!" << std::endl;
    }

    else if (argc == 3){
            std::string option(argv[1]);
            if (option == "-l")
                logging = true;
    }
    else{
        std::cout << "Requested option not recognized !!!" << std::endl;
        std::cout << "For logging functionality please provide add -l filename to the program name" << std::endl;
        std::cout << "Log to file disabled..." << std::endl;
        logging = false;
    }



    // Preparing here the queue struture 
    SimpleQueue<std::string> *msgQueue = new(SimpleQueue<std::string>); 

    std::thread subscriberThread([&address,&port,&timeout,&QOS,&topic,msgQueue]() { Subscriber sub(address,port,timeout,QOS,topic,msgQueue); });
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    // Offloading publishing work to separated threads
    std::thread entityOne(PublishingDemoEntityOne);
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::thread entityTwo(PublishingDemoEntityTwo);

    // Dequeueing here stored messages
    while(true) {
        // This delay is required for provide queue population and demonstrate that we accessing real queued messages
        std::this_thread::sleep_for(std::chrono::seconds(2));

        // If we are not providing the "-l filename" option we are currently logging to video 
        // if "-l filename" has been provided we are also storing the queue element within the provided filename
        // FIXME: in order to properly log data also a timestamp will be required. Not provided here since it is out of scope

        if (! msgQueue->IsEmpty()){
            std::string element = msgQueue->GetElement();
            std::cout << "Dequeued --> " << element << std::endl;
            std::cout << "remaining items: " << std::to_string(msgQueue->GetQueueSize()) << std::endl;

            if (logging){
                std::ofstream logToFile;
                std::string logfilename(argv[2]);   
                try{
                    logToFile.open(logfilename.c_str(), std::ios_base::app);
                    logToFile << element << std::endl;
                    logToFile.close();
                }
                catch(...){
                    std::cout << "generic error on accessing the file " << logfilename << std::endl;
                }
            }
            
        }
    }

    subscriberThread.join();
    entityOne.join();
    entityTwo.join();
}