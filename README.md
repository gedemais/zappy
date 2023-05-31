<!-- MACOS -->

<!-- installing docker -->
sudo brew install docker

<!-- il faut installer Docker Desktop car MACOS c'est garbage -->
https://docs.docker.com/desktop/install/mac-install/

<!-- il faut créer l'image zappy -->
<!-- pour vérifier qu'elle existe : cmd : docker images -->
docker build -t zappy .

<!-- il faut lancer l'image dans un container -->
docker run -it -p 8080:8080 zappy

<!-- docker run -it -p 8080:8080 -e SERVER_ARGS="-p 8080 -x 10 -y 10 -n foo -c 10 -t 25" zappy -->

<!-- cmd pour lancer le serveur -->
./server -p 8080 -x 10 -y 10 -n foo -c 10 -t 25
<!-- cmd pour lancer le client (en dehors du container) -->
./client -h 127.0.0.1 -p 8080 -n foo
