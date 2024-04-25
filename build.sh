#!/bin/bash

sudo docker run -v /root/server/judger-wrapper:/root/builder -it -u root builder-core /bin/bash