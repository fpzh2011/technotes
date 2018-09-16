#!/bin/bash -e
#
# docker storage setup
#

devops='zjh'
data='/var/data'
dkr_dir=${data}'/docker'
dkr_mnt=${data}'/mnt'

mirror="${1}"
if [[ -n "${mirror}" ]]; then
    if [[ "${mirror}" != "-m" ]]; then
        echo "unrecognized options" 1>&2
        exit 1
    fi
fi

apt-get update
apt-get install -y curl apt-transport-https ca-certificates software-properties-common
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | apt-key add -
add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable"

apt-get update && apt-get -y install docker-ce && apt-mark hold docker-ce
systemctl stop docker

mkdir -p ${data}
if mkdir -p ${dkr_mnt}; then
    chown ${devops}:${devops} ${dkr_mnt}
fi
if mkdir -p ${dkr_dir}; then
    chown root:root ${dkr_dir}
    chmod 600 ${dkr_dir}
fi

(   echo '{'
    echo '  "data-root": "'${dkr_dir}'",'
    echo '  "log-opts": {'
    echo '    "max-size": "1g"'
    if [[ -n "${mirror}" ]]; then
        echo '  },'
        echo '  "registry-mirrors": ['
        echo '    "https://f62945bb.mirror.aliyuncs.com"'
        echo '  ]'
    else
        echo '  }'
    fi
    echo '}'
) | tee /etc/docker/daemon.json

usermod -G docker -a ${devops}
systemctl start docker

apt-get -y install python-pip
pip install ${mirror:+-i https://mirrors.aliyun.com/pypi/simple} --upgrade pip
pip install ${mirror:+-i https://mirrors.aliyun.com/pypi/simple} --upgrade docker-compose
