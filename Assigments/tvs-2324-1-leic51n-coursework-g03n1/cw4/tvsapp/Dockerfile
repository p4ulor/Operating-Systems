FROM node:alpine

USER node

ENV NODE_PORT=4003
EXPOSE 4003

# we put "--chown=node:node" because we set the USER before hand, se this gives file permissions to node
# we do individual copy of tvsapp.js to cache it, which makes an extra layer
COPY --chown=node:node /src/tvsapp.js /home/node/app/tvsapp.js

COPY --chown=node:node /src /home/node/app 
WORKDIR /home/node/app

RUN npm install

CMD ["node", "tvsapp.js"]
