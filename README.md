# valiants_TracingAndDebugging
team repository for tracing and debugging SDV Hackathon Challenge

## Prerequisites

Docker is installed

## How to run the demo:

1. Start Jaeger Trace service

Open the new terminal and run:
```
./start_jaeger.sh
```

2. Start publisher and subscriber app

Open the neew terminal and run publisher/subscriber app:

```
docker build -t <container_name>  . 
docker run -it --net=host <container_name>  
```
3. Open http://localhost:16686/search in the browser to access Jaeger Tracer

## Data flow diagram

![Service data flow.](img/dataflow.svg)