FROM node:lts-alpine
COPY src/package.json /home/node/app/
COPY src/tvsapp.js /home/node/app/
RUN cd /home/node/app/ && npm install
RUN chown "node:node" /home/node/ -R
WORKDIR /home/node/app/
EXPOSE 4004
USER node
CMD node tvsapp.js