# Installing docker for LINUX
sudo apt-get install docker
# Installing docker for MACOS
sudo brew install docker
# MACOS :: il faut installer Docker Desktop car cet OS est garbage
https://docs.docker.com/desktop/install/mac-install/

# Create zappy image
# pour vérifier qu'elle existe : cmd : docker images
# ! IMPORTANT : run this command where the Dockerfile is !
docker build -t zappy .

# Run the container
docker run -it -p 8080:8080 zappy

# Run the server
./server -p 8080 -x 10 -y 10 -n foo -c 10 -t 25
# Run the client
./client -h 127.0.0.1 -p 8080 -n foo
