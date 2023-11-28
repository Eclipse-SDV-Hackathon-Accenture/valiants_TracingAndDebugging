#include <ecal/ecal.h>

#include <ecal/msg/protobuf/publisher.h>


#include <iostream>

#include <thread>


#include "test_message.pb.h"


int main(int argc, char** argv)

{

  // Initialize eCAL and create a protobuf publisher

  eCAL::Initialize(argc, argv, "Vehicle Test App Publisher");

  eCAL::protobuf::CPublisher<proto_messages::TestMessage> publisher("test_message_protobuf");


  // Ask the user to input his name


  std::string name = "Q4";
  std::string message = "I am Saqib's fancy car";


  unsigned int id = 0;


  // Infinite loop (using eCAL::Ok() will enable us to gracefully shutdown the

  // Process from another application)

  while (eCAL::Ok())

  {

    // Let the user input a message


    // Create a protobuf message object

    proto_messages::TestMessage test_message;

    test_message.set_name(name);

    test_message.set_msg (message);

    test_message.set_id  (id++);


    // Send the message

    publisher.Send(test_message);

    std::cout << "Sent message!" << std::endl << std::endl;

  }


  // finalize eCAL API

  eCAL::Finalize();

}
