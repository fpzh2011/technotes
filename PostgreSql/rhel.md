# redhat下postgresql安装配置

rhel7.4
postgresql-server-9.2.21
```shell
sudo passwd postgres
sudo postgresql-setup initdb
systemctl enable postgresql.service
systemctl start postgresql.service
sudo -u postgres psql template1
ALTER USER postgres with encrypted password 'zjh@yun';
vim /var/lib/pgsql/data/pg_hba.conf # ident -> trust
systemctl restart postgresql.service
su postgres
createdb zjh
psql -U postgres -h localhost -p 5432 -d zjh -W
```
