//%attributes = {}
DOCUMENT TO BLOB:C525(Structure file:C489; $test)

$hash1:=Generate digest:C1147($test; SHA1 digest:K66:2)

//unlike native commands, inserts \n every 72 characters
$b64:=BLOB to base64($test)
$echo:=BLOB from base64($b64)

$hash2:=Generate digest:C1147($echo; SHA1 digest:K66:2)

$hex:=BLOB to hex($test)
$echo:=BLOB from hex($hex)

$hash3:=Generate digest:C1147($echo; SHA1 digest:K66:2)