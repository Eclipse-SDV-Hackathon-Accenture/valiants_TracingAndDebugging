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
#include "opentelemetry/sdk/resource/resource.h"
#include <iostream>

#include <thread>


#include "test_message.pb.h"

namespace trace_api      = opentelemetry::trace;
namespace trace_sdk      = opentelemetry::sdk::trace;
namespace nostd = opentelemetry::nostd;
// namespace trace_exporter = opentelemetry::exporter::trace;

std::string appname = "Protobuf Publisher";
int sleep_time = 500;

nostd::shared_ptr<trace_api::Tracer> get_tracer()
{
  auto provider = trace_api::Provider::GetTracerProvider();
  return provider->GetTracer("PublisherTracer", OPENTELEMETRY_SDK_VERSION);
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
  auto resource = opentelemetry::sdk::resource::Resource::Create(
      opentelemetry::sdk::resource::ResourceAttributes{{"service.name", "publisher"}});
  std::shared_ptr<opentelemetry::trace::TracerProvider> provider =
      trace_sdk::TracerProviderFactory::Create(std::move(processor), resource);

  // Set the global trace provider
  trace_api::Provider::SetTracerProvider(provider);
}

void CleanupTracer()
{
  std::shared_ptr<trace_api::TracerProvider> none;
  trace_api::Provider::SetTracerProvider(none);
}
}

int main(int argc, char** argv)

{

  if (argc > 1)
          appname += std::string(argv[1]);

  if (argc > 2)
          sleep_time = atoi(argv[2]);


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
    auto pub_snd_span = get_tracer()->StartSpan(name.c_str());
    auto scope = get_tracer()->WithActiveSpan(pub_snd_span);
    publisher.Send(test_message);
    std::cout << appname << ": Sent message!" << std::endl << std::endl;
    pub_snd_span->End();

    eCAL::Process::SleepMS(sleep_time);

  }

  // finalize eCAL API

  eCAL::Finalize();
  CleanupTracer();

}
