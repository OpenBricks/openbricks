#!/bin/sh

old_IFS=$IFS

server_conf=/srv/www/htdocs/www-config
# cleaning
[ -f $server_conf ] && rm $server_conf
[ -f /tmp/tmp_setHref ] && rm /tmp/tmp_setHref
[ -f /tmp/tmp_table ] && rm /tmp/tmp_table
[ -f /tmp/index.html ] && rm /tmp/index.html

WWW=`ls /srv/www/htdocs/*-www`

# condition for www-config
if [ -z "$WWW" ] ; then
  cp /srv/www/htdocs/index-empty.html /srv/www/htdocs/index.html
  echo " No default config found, starting with a basic index.html file !"
  exit 0
else
  for i in $WWW ; do
    cat $i >> $server_conf
  done
fi

create_setHref () {
# $1 is name
# $2 is port
# $3 is file
cat >> /tmp/tmp_setHref <<EOF
        document.getElementById('$1-href').href = window.location.protocol + "//" + window.location.hostname + ":$2/$3";
EOF
}

create_table () {
# $1 is file
# $2 is description
# $3 is logo
# $4 is name

if [ -z "$4" ] ; then
  cat >> /tmp/tmp_table << EOF
          <tr>
            <td align="right"><img src="$3" border="0"/></td>
            <td><a href="$1">$2</a></td>
          </tr>
EOF
else
  cat >> /tmp/tmp_table << EOF
          <tr>
            <td align="right"><img src="$3" border="0"/></td>
            <td><a href="$1" id="$4-href">$2</a></td>
          </tr>
EOF
fi

}


while IFS=: read name port file logo description
do
  if [ -z $port ] ; then 
    create_table "$file" "$description" "$logo"
  else
    create_setHref "$name" "$port" "$file"
    create_table "$file" "$description" "$logo" "$name"
  fi
done < $server_conf

# Generating index.html
cat > /tmp/index.html << EOF
<html>
  <head>
    <script>
      function setHref() {
EOF
cat /tmp/tmp_setHref >> /tmp/index.html
cat >>/tmp/index.html << EOF
      }
    </script>
  </head>

  <body onload="setHref()" bgcolor="#efefef">
    <table width="100%">
      <tr><td align="left">
        <img src="logo_g_256x64.png" border="0"/>
      </td></tr>

      <tr><td>
        <hr>
      </td></tr>

      <tr><td>
        <table>
EOF
cat /tmp/tmp_table >> /tmp/index.html
cat >> /tmp/index.html << EOF
        </table>
      </td></tr>
    </table>
  </body>
</html>
EOF

cp /tmp/index.html /srv/www/htdocs/index.html