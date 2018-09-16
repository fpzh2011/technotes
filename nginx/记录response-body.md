# 在access.log中记录response body

## nginx版本

系统环境：Ubuntu 16.04 64位

```shell
sudo apt install nginx-extras
```

## nginx.conf

```conf
worker_processes auto;

events {
    worker_connections 768;
    use   epoll;
}


http {
    include       /etc/nginx/mime.types;
    default_type  application/octet-stream;

    log_format  main escape=json
        '[$time_local]\t$bytes_sent\t'
        '$upstream_status\t$upstream_response_time\t$status\t$request_time\t'
        '$resp_body';

    access_log  /var/log/nginx/access.log  main;

    sendfile        on;

    keepalive_timeout  65;
    client_max_body_size 100m;
    server_tokens off;

    include /etc/nginx/conf.d/*.conf;

    server {
      listen 80;
      location / {
        proxy_set_header Access-Control-Allow-Origin *;
        proxy_pass_header Server;
        proxy_set_header Host $http_host;
        proxy_redirect off;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Scheme $scheme;
        proxy_pass http://10.1.0.6:9201;
      }

      lua_need_request_body on;

      set $resp_body "";
      body_filter_by_lua '
        local resp_body = string.sub(ngx.arg[1], 1, 1000)
        ngx.ctx.buffered = (ngx.ctx.buffered or "") .. resp_body
        if ngx.arg[2] then
          ngx.var.resp_body = ngx.ctx.buffered
        end
      ';
    }

    upstream backend {
       server 10.1.0.6:9201;
    }
}
```
