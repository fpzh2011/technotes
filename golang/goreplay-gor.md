# goreplay - gor

```
sudo su -
cd /data/code/gor/
ps -ef | grep gor

nohup ./gor --input-raw :8080 --output-http "http://10.26.0.146:8080|40" &
nohup ./gor --input-raw :8080 --output-http "http://10.26.0.146:8080|40%" &
nohup ./gor --input-raw :8080 --output-http "http://10.26.0.146:8080|100%" --output-http-workers 80 &

nohup ./gor --input-raw :8080 --output-file /data/logs/gor_log/requests.log.gz --output-file-append --output-file-size-limit 2g --output-file-queue-limit 0 &
nohup ./gor --input-file "requests.log.gz|200%" --input-file-loop --output-http "http://10.26.0.146:8080" &
nohup ./gor --input-file "requests.log.gz|200%" --output-http "http://10.26.0.146:8080" &
nohup ./gor --input-file "requests.log|500%" --output-http "http://10.26.0.146:8080" &
```

https://github.com/buger/goreplay/wiki/Saving-and-Replaying-from-file#performance-testing
https://github.com/buger/goreplay/wiki/Rate-limiting
https://github.com/buger/goreplay/wiki/FAQ
