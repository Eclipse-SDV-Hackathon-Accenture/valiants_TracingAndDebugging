#include <ecal/ecal.h>
#include <ecal/msg/protobuf/subscriber.h>

#include <iostream>
#include <thread>

#include "proto_messages/test_message.pb.h"




void TstCallback(const proto_messages::test_message& tst_message)
{
  std::cout << tst_message.name() << " sent a message with ID "
            << tst_message.id() << ":" << std::endl
            << tst_message.msg() << std::endl << std::endl;

   // sleep 100 ms
    eCAL::Process::SleepMS(500);          
}

int main(int argc, char** argv)
{
  // Initialize eCAL and create a protobuf subscriber
  eCAL::Initialize(argc, argv, "Protobuf Subscriber");
  eCAL::protobuf::CSubscriber<proto_messages::test_message> subscriber("test_message_protobuf");

  // Set the Callback
  subscriber.AddReceiveCallback(std::bind(&TstCallback, std::placeholders::_2));

  // Just don't exit
  while (eCAL::Ok())
  {

  }

  // finalize eCAL API
  eCAL::Finalize();
}