# Use modern Linux base so we can get avr-gcc v7.3.0 or later to be able to compile for ATmega4809
FROM alpine:latest

RUN apk add --no-cache \
    gcc-avr \
    binutils-avr \
    avr-libc \
    avrdude \
    make \
    vim \
    tio

WORKDIR /app
