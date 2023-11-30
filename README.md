# valiants_TracingAndDebugging
team repository for tracing and debugging SDV Hackathon Challenge

## Prerequisites

[Install Docker](https://docs.docker.com/get-docker/)

## How to run the demo:

1. Start Jaeger Trace service

Open a new terminal and run:
```
./start_jaeger.sh
```

2. Start publisher and subscriber app

Open a new terminal and run publisher/subscriber app:

```
docker build -t <container_name>  . 
docker run -it --net=host <container_name>  
```
3. Open http://localhost:16686/search in the browser to access Jaeger Tracer

## Data flow diagram

![Service data flow.](img/dataflow.svg)
