docker build -t mattdebian .
docker run --name test -itd mattdebian /bin/bash
docker cp ./Matt_daemon test:/

echo "Launch cmd with : docker exec -it test /bin/bash"