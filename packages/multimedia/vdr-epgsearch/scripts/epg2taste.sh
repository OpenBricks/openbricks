#
# epg2taste.sh - v.0.1
#
# add this line to your epgsearchcmds.conf:
# folgende zeile in die epgsearchcmds.conf eintragen:
#
# epg2taste : /usr/lib/vdr-plugin-epgsearch/epg2taste.sh

# CONFIG START
  TASTE_FILE="/var/lib/vdr/plugins/taste.conf"

# default taste settings
  REGULAR_EXPRESSION=0 # Regular Expression / Regulär Ausdruck
  IGNORE_CASE=0        # Ignore Case / GroßKleinschreibung ignorieren
# CONFIG END

# add taste
echo "${REGULAR_EXPRESSION}:${IGNORE_CASE}:${1}" >> "${TASTE_FILE}"
echo "Done..."

# jump back
at now <<EOF
perl -l -e "printf \"\n/usr/lib/vdr/svdrpsend.pl HITK BACK\" x 2" | sh
EOF

