docker image build -t tadahdtentity:latest .
docker container run  --interactive --env CODECOV_TOKEN=$CODECOV_TOKEN --tty --rm --name tadahdtentity tadahdtentity:latest

