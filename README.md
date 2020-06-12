# 4d-plugin-base64
Convert BLOB to text (hex or base64)

Unlike the native command, a new line ``\n`` is inserted every 72 characters.

### Platform

| carbon | cocoa | win32 | win64 |
|:------:|:-----:|:---------:|:---------:|
||<img src="https://cloud.githubusercontent.com/assets/1725068/22371562/1b091f0a-e4db-11e6-8458-8653954a7cce.png" width="24" height="24" />|<img src="https://cloud.githubusercontent.com/assets/1725068/22371562/1b091f0a-e4db-11e6-8458-8653954a7cce.png" width="24" height="24" />|<img src="https://cloud.githubusercontent.com/assets/1725068/22371562/1b091f0a-e4db-11e6-8458-8653954a7cce.png" width="24" height="24" />|

### Version

<img width="32" height="32" src="https://user-images.githubusercontent.com/1725068/73986501-15964580-4981-11ea-9ac1-73c5cee50aae.png"> <img src="https://user-images.githubusercontent.com/1725068/73987971-db2ea780-4984-11ea-8ada-e25fb9c3cf4e.png" width="32" height="32" />

## Syntax

```
b64:=BLOB to base64 (data)
hex:=BLOB to hex (data)
```

Parameter|Type|Description
------------|------------|----
data|BLOB|In
b64|TEXT|Out
hex|TEXT|Out

```
data:=BLOB from base64 (b64)
data:=BLOB from hex (hex)
```

Parameter|Type|Description
------------|------------|----
data|BLOB|Out
b64|TEXT|In
hex|TEXT|In
