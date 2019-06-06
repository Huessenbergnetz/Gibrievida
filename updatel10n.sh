#!/bin/bash

lupdate-qt5 -source-language en -no-obsolete -locations none common sailfishos -ts l10n/gibrievida.ts

for LANG in en en_GB en_US
do
lupdate-qt5 -source-language en -target-language $LANG -pluralonly -no-obsolete -locations none common sailfishos -ts l10n/$LANG.ts
done

# tx push -s
# echo 'Waiting 10 seconds to pull translations'
# sleep 10
# tx pull -a
