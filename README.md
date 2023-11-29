# valiants_TracingAndDebugging
team repository for tracing and debugging SDV Hackathon Challenge

## Prerequisites

Docker is installed

## How to run the demo:

1. Jaeger Trace service

Open the new terminal and run:
```
./start_jaeger.sh
```

2. Publisher and subscriber app

Open the neew terminal and run publisher/subscriber app:

```
docker build -t <container_name>  . 
docker run --net=host <container_name>  
```
