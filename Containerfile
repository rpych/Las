FROM alpine

RUN apk update && \
    apk add --no-cache \
    build-base \
    less \
    cmake \
    git

WORKDIR /las_app

COPY src/ ./src/
COPY test/ ./test/
COPY CMakeLists.txt .


WORKDIR /las_app/build

RUN cmake .. && make

WORKDIR  /run_las

ENTRYPOINT [ "/las_app/build/las" ]
CMD [ "" ]
