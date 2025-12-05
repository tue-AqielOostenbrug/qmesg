# This document was produced by reading the unrealircd and docker wiki, in combination with help from ChatGPT.
# It helped with the parts where I used sed, mapfile, and debugging
FROM ubuntu:jammy
ARG USERNAME=ubuntu
ARG USER_UID=1000
ARG USER_GID=$USER_UID
RUN groupadd --gid $USER_GID $USERNAME && \
    useradd -s /bin/bash --uid $USER_UID --gid $USER_GID -m $USERNAME
RUN apt-get update
RUN apt-get install -y build-essential pkg-config gdb libssl-dev libpcre2-dev libargon2-dev libsodium-dev libc-ares-dev libcurl4-openssl-dev wget
ADD --unpack=true https://www.unrealircd.org/downloads/unrealircd-latest.tar.gz temp
RUN mv temp/* /home/ubuntu/setup
RUN rm -rf temp
RUN chown -R $USERNAME /home/ubuntu/setup
USER ubuntu
WORKDIR /home/ubuntu/setup
RUN yes "" | ./Config
RUN make
RUN make install
WORKDIR /home/ubuntu/unrealircd
EXPOSE 6667
RUN sed -i 's/bobsmith/admin/' conf/unrealircd.conf
RUN sed -i 's/password "$argon2id..etc..";/password "password";/' conf/unrealircd.conf 
RUN bash -c 'mapfile -t CLOAKS < <(./unrealircd gencloak | grep -oP "(?<=\")[^\"]+(?=\")") && \
sed -i "/cloak-keys {/,/}/c\
cloak-keys {\
\"${CLOAKS[0]}\";\
\"${CLOAKS[1]}\";\
\"${CLOAKS[2]}\";\
}" conf/unrealircd.conf'
RUN sed -i "s/kline-address 'set.this.to.email.address';/kline-address 'example@example.com';/" conf/unrealircd.conf
