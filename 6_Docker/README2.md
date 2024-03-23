# Docker Compose
docker-compose!=docker compose

- 1st is the initial docker compose made in python
- 2nd is the final implementation w/ plugin (compose is the plugin name)
## Commands
- docker compose up -d
- docker compose stop
- docker compose down (deletes containers)
- docker compose ps (in the .yml directory)
- docker compose exec -it ariana /bin/sh (images in alpine dont have shell, so it has to be sh)
- nslookup <service_name> <address>
- ping <service_name>
- ip addr

## Networks
Are used to restrict visibility between containers