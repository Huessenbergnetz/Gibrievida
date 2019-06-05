 #!/bin/bash

STARTDIR=$PWD

cd l10n
for LANG in de en en_GB en_US fr it nl nl_BE sv
do
lrelease-qt5 -nounfinished $LANG.ts
done
