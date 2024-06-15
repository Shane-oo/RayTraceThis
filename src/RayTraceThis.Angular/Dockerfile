#Set Base Image
FROM node:20.12.2-alpine3.19 as build
#Set a folder in it for our source code.
WORKDIR /app/src
#Copy only package.json and package.lock.json files to our source code folder. Note have to be working in this directory
# i.e. cd src/RayTraceThis.Angular/
COPY package*.json ./
#Run npm ci for clean install.
RUN npm ci
#Copy all other source code files.
COPY . ./
RUN npm run build


FROM node:20.12.2-alpine3.19
WORKDIR /usr/app
COPY --from=build /app/src/dist/ray-trace-this.angular/ ./
CMD node server/server.mjs
EXPOSE 4000
