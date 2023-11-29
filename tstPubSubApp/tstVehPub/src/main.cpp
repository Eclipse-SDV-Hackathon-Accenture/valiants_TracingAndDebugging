#include <ecal/ecal.h>

#include <ecal/msg/protobuf/publisher.h>

#include "opentelemetry/exporters/otlp/otlp_grpc_exporter.h"
#include "opentelemetry/exporters/otlp/otlp_grpc_exporter_options.h"
// #include "opentelemetry/exporters/ostream/span_exporter_factory.h"
#include "opentelemetry/sdk/trace/exporter.h"
#include "opentelemetry/sdk/trace/processor.h"
#include "opentelemetry/sdk/trace/simple_processor_factory.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"
#include "opentelemetry/trace/provider.h"

#include <iostream>

#include <thread>


#include "test_message.pb.h"

namespace trace_api      = opentelemetry::trace;
namespace trace_sdk      = opentelemetry::sdk::trace;
namespace nostd = opentelemetry::nostd;
// namespace trace_exporter = opentelemetry::exporter::trace;

nostd::shared_ptr<trace_api::Tracer> get_tracer()
{
  auto provider = trace_api::Provider::GetTracerProvider();
  return provider->GetTracer("PublishTest", OPENTELEMETRY_SDK_VERSION);
}

namespace
{
void InitTracer()
{
  // auto exporter  = trace_exporter::OStreamSpanExporterFactory::Create();
  opentelemetry::exporter::otlp::OtlpGrpcExporterOptions options;
  options.endpoint = "http://localhost:4317";
  options.use_ssl_credentials = false;
  auto exporter = std::unique_ptr<trace_sdk::SpanExporter>(new opentelemetry::exporter::otlp::OtlpGrpcExporter(options));
  auto processor = trace_sdk::SimpleSpanProcessorFactory::Create(std::move(exporter));
  std::shared_ptr<opentelemetry::trace::TracerProvider> provider =
      trace_sdk::TracerProviderFactory::Create(std::move(processor));

  // Set the global trace provider
  trace_api::Provider::SetTracerProvider(provider);
}

void CleanupTracer()
{
  std::shared_ptr<opentelemetry::trace::TracerProvider> none;
  trace_api::Provider::SetTracerProvider(none);
}
}

int main(int argc, char** argv)

{
  // Initialize opentelemetry
  auto provider = opentelemetry::trace::Provider::GetTracerProvider();
  auto tracer = provider->GetTracer("foo_library", "1.0.0");

  // Initialize eCAL and create a protobuf publisher

  eCAL::Initialize(argc, argv, "Vehicle Test App Publisher");

  eCAL::protobuf::CPublisher<proto_messages::TestMessage> publisher("test_message_protobuf");


  // Ask the user to input his name


  std::string name = "Q4";
  std::string message = "I am Saqib's fancy car";


  unsigned int id = 0;


  // Infinite loop (using eCAL::Ok() will enable us to gracefully shutdown the

  // Process from another application)
  InitTracer();

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
    auto scoped_span = trace_api::Scope(get_tracer()->StartSpan("PublisherTest"));

    eCAL::Process::SleepMS(1000);

  }


  // finalize eCAL API

  eCAL::Finalize();
  CleanupTracer();

}
