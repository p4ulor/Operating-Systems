## Layers
- A modified (a modified file system) read-only image
- Layers w/ the same configs/modifications aren't duplicated, they are re-used

## Commands

### Run image into new container
- docker run 

## Run existing container
- docker start

## Params for running containers to allow interaction and I/O
1. `-it`
- docker run -a -i (-a for ouput, -i for input)

## Misc commands
- docker images
- docker container prune
- docker inspect <img> | less //consult layers
- docker build -t tvs02 . --build-arg DATE="$(date)"
- docker save -o file.tar imageName (and `tar tf file.tar`)

## Dockerfile reserved words
- RUN -> executed after the container is running. For each RUN, a new layer is made, so it's good to agregatte commands into 1 or a few. But having a few layers, can be useful to cache layers if there's a case that justifies it
- CMD -> executed 1 time after the container is running
- ENTRYPOINT -> sets initial executable file, per example -> ENTRYPOINT ["/hi"]
- ENV -> set environment variable, per example -> ENV HOME /home/tvs
- USER -> set a different user, per example -> USER root

## Dockerfile instructions
```dockerfile
RUN data > date.txt
```
