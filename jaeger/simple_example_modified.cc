// Copyright The OpenTelemetry Authors
// SPDX-License-Identifier: Apache-2.0

#include "opentelemetry/exporters/otlp/otlp_grpc_exporter.h"
#include "opentelemetry/exporters/otlp/otlp_grpc_exporter_options.h"
#include "opentelemetry/sdk/trace/exporter.h"
#include "opentelemetry/sdk/trace/processor.h"
#include "opentelemetry/sdk/trace/simple_processor_factory.h"
#include "opentelemetry/sdk/trace/tracer_provider_factory.h"
#include "opentelemetry/trace/provider.h"

#ifdef BAZEL_BUILD
#include "examples/common/foo_library/foo_library.h"
#else
#include "foo_library/foo_library.h"
#endif

namespace trace_api = opentelemetry::trace;
namespace trace_sdk = opentelemetry::sdk::trace;

namespace {
void InitTracer() {
  opentelemetry::exporter::otlp::OtlpGrpcExporterOptions options;
  options.endpoint = "http://localhost:4317";
  options.use_ssl_credentials = false;
  auto exporter = std::unique_ptr<trace_sdk::SpanExporter>(
      new opentelemetry::exporter::otlp::OtlpGrpcExporter(options));
  auto processor =
      trace_sdk::SimpleSpanProcessorFactory::Create(std::move(exporter));
  std::shared_ptr<opentelemetry::trace::TracerProvider> provider =
      trace_sdk::TracerProviderFactory::Create(std::move(processor));

  // Set the global trace provider
  trace_api::Provider::SetTracerProvider(provider);
}

void CleanupTracer() {
  std::shared_ptr<opentelemetry::trace::TracerProvider> none;
  trace_api::Provider::SetTracerProvider(none);
}
}  // namespace

int main() {
  // Removing this line will leave the default noop TracerProvider in place.
  for (;;) {
    InitTracer();

    foo_library();

    CleanupTracer();
  }
}
