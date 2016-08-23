 #!/bin/bash
 
STARTDIR=$PWD

cd l10n
for LANG in de en en_GB en_US nl
do
lrelease-qt5 -nounfinished $LANG.ts
done
