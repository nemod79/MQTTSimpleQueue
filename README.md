As a Capstone project for my Udacity C++ course ( https://www.udacity.com/course/c-plus-plus-nanodegree--nd213 ) 
I've implemented classes and methods with the aim of providing an easy to use abstraction to the MOSQUITTO library, for dealing with MQTT protocol.

Hereunder you can see the overall strcture and the general idea of the project.

![Structure](/structure.gif)

In order to explain how the provided classes actually works a SampleMQTT.cpp has been provided.

_**MosquittoPubilsher.h/.cpp**_
This file actually contains all the required classes and methods for connecting to an MQTT Broker and sending to it messages

_**MosquittoSubscriber.h/.cpp**_
This file actually contains all the required classes and methods for connecting to an MQTT Broker and subscribind specific TOPIC.
You can consider "TOPIC" like a common subject, shared by several MosquittoPublisher, in order to let the MQTT Broker to "group" messages togheter.
In real case valid TOPICS can be : "sensors/", "temperature/", "acceleration/", "kitchen/", "bedroom/", etc.

_**SimpleQueue.h**_
Only h file is provided here since we are using 'inline' for proper template implementation. The aim of this file is to provided classes and methods
for proper handling the queue abstraction used for providing to the user a simple (and concurrent capable) way for handling / parsing / modifying / etc. 
data, received by MosquittoPublisher.

_**SampleMQTT.cpp**_
In order to demonstrate the functionalities provided a SampleMQTT program has been provided.
It implements the following feature:
* Two publisher are instantiated and ran through separated threads
* One subscriber is instantiated and ran within a separated thread
* One queue is created within the main program in order to grant concurrent access to the subscriber and to the main loop

As an example of proper queue elements handling a 'while(true)' has been provided. Withing such loop there are two options:
* popping elements from the queue and provide video output that demonstrates the dequeueing
* add file logging if the program has been invoked with "-l filename" option

![Structure](/SampleMQTT.gif)

_**Installation instructions**_

* mkdir build
* cd build
* cmake ..
* make

_**Run instructions**_
For running the program without logging to file: 
> ./SampleMQTT

Enabling loggin feature:
> ./SampleMQTT -l logfile.log

_**UDACITY Rubric**_
* README (All Rubric Points REQUIRED) -> Properly assessed
* Compiling and Testing (All Rubric Points REQUIRED) -> Properly assessed
* Loops, Functions, I/O -> SampleMQTT.cpp 
* Object Oriented Programming -> Basically all over the project, templates used in SimpleQueue.h
* Memory Management -> All over the project
* Concurrency -> Implemented in SampleMQTT.cpp but also used in SimpleQueue.h
