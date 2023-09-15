FROM gcc:11.2.0

WORKDIR /home/sisop

RUN apt -y update \
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/sisoputnfrba/so-commons-library \
    && cd so-commons-library \
    && make \
    && make install \
    && cd .. \
    && rm -rf so-commons-library

COPY . .