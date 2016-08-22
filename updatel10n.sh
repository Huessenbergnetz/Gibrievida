#!/bin/bash

lupdate-qt5 -source-language en -no-obsolete . -ts l10n/gibrievida.ts

for LANG in en en_GB en_US
do
lupdate-qt5 -source-language en -target-language $LANG -pluralonly -no-obsolete . -ts l10n/$LANG.ts
done

tx push -s
sleep 10
echo 'Waiting 10 seconds to pull translations'
tx pull -a
