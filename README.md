# INSTALLING DOCKER
## for LINUX
sudo apt-get install docker
## for MACOS
sudo brew install docker
### MACOS :: il faut installer Docker Desktop car cet OS est garbage
https://docs.docker.com/desktop/install/mac-install/

# Create SERVER zappy image
### pour vérifier qu'elle existe : cmd : docker images
### ! IMPORTANT : run this command where the SERVER Dockerfile is !
docker build -f ./Dockerfile_server -t zappy_server .

# Run the SERVER container
docker run -it -p 8080:8080 -e ZPORT="8080" -e ZX="10" -e ZY="10" -e ZTEAM="foo" -e ZCLIENT="10" -e ZT="25" zappy_server

# Create CLIENT zappy image
### ! IMPORTANT : run this command where the CLIENT Dockerfile is !
docker build -f ./Dockerfile_client -t zappy_client .

# Run the CLIENT container
docker run -it -p 8080:8080 -e ZHOST="127.0.0.1" -e ZPORT="8080" -e ZTEAM="foo" zappy_client
