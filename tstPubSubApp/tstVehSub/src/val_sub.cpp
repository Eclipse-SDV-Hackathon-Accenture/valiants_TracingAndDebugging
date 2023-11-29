#include <ecal/ecal.h>
#include <ecal/msg/protobuf/subscriber.h>

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

void TstCallback(const proto_messages::TestMessage& tst_message)
{
  std::cout << tst_message.name() << " sent a message with ID "
            << tst_message.id() << ":" << std::endl
            << tst_message.msg() << std::endl << std::endl;

   // sleep 100 ms
   auto scoped_span = trace_api::Scope(get_tracer()->StartSpan("SubscriberTest"));
    eCAL::Process::SleepMS(500);
}

int main(int argc, char** argv)
{
  // Initialize opentelemetry
  auto provider = opentelemetry::trace::Provider::GetTracerProvider();
  auto tracer = provider->GetTracer("Subscriber", "0.1.0");

  // Initialize eCAL and create a protobuf subscriber
  eCAL::Initialize(argc, argv, "Protobuf Subscriber");
  eCAL::protobuf::CSubscriber<proto_messages::TestMessage> subscriber("test_message_protobuf");

  // Set the Callback
  subscriber.AddReceiveCallback(std::bind(&TstCallback, std::placeholders::_2));
  InitTracer();
  // Just don't exit
  while (eCAL::Ok())
  {

  }

  // finalize eCAL API
  eCAL::Finalize();
  CleanupTracer();
}
