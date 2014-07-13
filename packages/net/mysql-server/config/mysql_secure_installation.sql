-- set root password
UPDATE mysql.user SET Password=PASSWORD('openbricks') WHERE User='root';

-- remove anonymous user
DELETE FROM mysql.user WHERE User='';

-- restrict root logins to localhost
DELETE FROM mysql.user WHERE User='root' AND Host NOT IN ('localhost', '127.0.0.1', '::1');

-- drop useless test database
DROP DATABASE test;
DELETE FROM mysql.db WHERE Db='test' OR Db='test\\_%';

-- refresh privileges
FLUSH PRIVILEGES;
