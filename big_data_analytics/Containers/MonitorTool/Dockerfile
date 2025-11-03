FROM  alpine:3.22 AS build
RUN mkdir /usr/app
COPY . /usr/src/MonitorTool
WORKDIR /usr/src

RUN ./MonitorTool/scripts/dependencies.sh
RUN ./MonitorTool/scripts/mongo.sh
RUN ./MonitorTool/scripts/ftxui.sh
RUN ./MonitorTool/scripts/monitor-tool.sh

FROM alpine:3.22 AS release
COPY --from=build /usr/src/MonitorTool/build/monitor-tool /usr/bin
RUN apk add openssl libstdc++ libgcc openssl-dev musl
ENTRYPOINT [ "monitor-tool" ]
