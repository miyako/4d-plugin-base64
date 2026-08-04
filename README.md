# 4d-plugin-base64

The BASE64 plugin adds four commands to 4D for converting between `BLOB` and `Text`: base64 encode/decode, and hexadecimal encode/decode. All conversion happens in-process (no file I/O, no network, no OS-level encoding API) — the plugin implements the base64 and hex alphabets itself in C++.

| Command | Returns | Purpose |
|---|---|---|
| [`BLOB to base64`](#blob-to-base64) | Text | Encode a BLOB as base64 text |
| [`BLOB from base64`](#blob-from-base64) | BLOB | Decode base64 text back to a BLOB |
| [`BLOB to hex`](#blob-to-hex) | Text | Encode a BLOB as hexadecimal text |
| [`BLOB from hex`](#blob-from-hex) | BLOB | Decode hexadecimal text back to a BLOB |

**Platforms:** macOS and Windows. There is no platform-specific branching anywhere in these four commands' implementation, so behavior is identical on both — nothing here depends on an OS-level encoding API.

---

## Requirements & platform notes

- Each command takes exactly **one mandatory parameter**. There is no optional/omitted-parameter form for any of the four.
- All four commands are declared **thread-safe** by the plugin, so 4D may call them concurrently from multiple processes.
- **None of the four commands raise a 4D error.** Invalid or malformed input is never reported back to your code as an error — it silently produces an empty result instead. Check the return value's length/content yourself if you need to detect bad input (see [Error handling](#error-handling--troubleshooting) below).
- An empty BLOB in → empty Text out, and empty Text in → empty BLOB out, for all four commands. This is handled explicitly and does not error.
- This reference documents the plugin's source **as corrected** during code review (see the note under [Error handling](#error-handling--troubleshooting) for what changed and why it matters if you're on an older build).

---

## BLOB to base64

### Syntax
```4d
BLOB to base64 ( blob ) → Text
```

| Parameter | Type | Description |
|---|---|---|
| `blob` | BLOB | Binary data to encode. |
| Result | Text | Base64-encoded representation of `blob`. |

### Description
Encodes `blob` using the standard base64 alphabet (`A`–`Z`, `a`–`z`, `0`–`9`, `+`, `/`), padded with `=` so the output length is a multiple of 4, per standard base64. **Unlike 4D's own base64 handling, a line feed (`\n`) is inserted every 72 characters** — this plugin's own test method calls this out explicitly as the reason to use it over a native equivalent. If you need output with no embedded line breaks, strip `\n` from the result yourself (e.g. `Replace string`).

An empty `blob` produces an empty Text result (`""`), not an error.

### Example
From the plugin's own test method (`Method1.4dm`):
```4d
DOCUMENT TO BLOB:C525(Structure file:C489; $test)
$b64:=BLOB to base64($test)
```

Encoding a BLOB field from a record, and stripping the line breaks:
```4d
$blob:=[Attachments]Data
$b64:=BLOB to base64($blob)
$b64:=Replace string($b64; "\n"; "")
```

---

## BLOB from base64

### Syntax
```4d
BLOB from base64 ( text ) → BLOB
```

| Parameter | Type | Description |
|---|---|---|
| `text` | Text | Base64-encoded text to decode. |
| Result | BLOB | Binary data decoded from `text`. |

### Description
Decodes standard base64 text back into a BLOB. Whitespace (including embedded line feeds, such as those `BLOB to base64` inserts) and `=` padding characters are skipped wherever they appear, so text produced by `BLOB to base64` — with or without its line breaks — decodes correctly either way.

If `text` contains any character outside the base64 alphabet (aside from whitespace and `=`), decoding stops at that point and the command returns an **empty BLOB** — not a partial result, and not a 4D error.

An empty `text` produces an empty BLOB, not an error.

### Example
From the plugin's own test method (`Method1.4dm`):
```4d
$echo:=BLOB from base64($b64)
```

Decoding a literal base64 string:
```4d
$blob:=BLOB from base64("SGVsbG8=")  // decodes to the bytes for "Hello"
```

---

## BLOB to hex

### Syntax
```4d
BLOB to hex ( blob ) → Text
```

| Parameter | Type | Description |
|---|---|---|
| `blob` | BLOB | Binary data to encode. |
| Result | Text | Hexadecimal representation of `blob`. |

### Description
Encodes `blob` as lowercase hexadecimal, two characters per byte, with **no separators and no line breaks** (unlike `BLOB to base64`, which does insert line breaks). An empty `blob` produces an empty Text result, not an error.

### Example
From the plugin's own test method (`Method1.4dm`):
```4d
$hex:=BLOB to hex($test)
```

Encoding the same 5-byte BLOB used in the base64 example above (the bytes for "Hello") produces `48656c6c6f` — no separators, all lowercase:
```4d
$hex:=BLOB to hex($blob)  // $hex = "48656c6c6f"
```

---

## BLOB from hex

### Syntax
```4d
BLOB from hex ( text ) → BLOB
```

| Parameter | Type | Description |
|---|---|---|
| `text` | Text | Hexadecimal text to decode. |
| Result | BLOB | Binary data decoded from `text`. |

### Description
Decodes hexadecimal text back into a BLOB. Both uppercase and lowercase `A`–`F` are accepted, and — usefully — hex digit pairs may be separated by spaces (` `), hyphens (`-`), or colons (`:`); any mix of these separators is silently skipped rather than rejected. This means `"48656c6c6f"`, `"48 65 6c 6c 6f"`, `"48-65-6c-6c-6f"`, and `"48:65:6c:6c:6f"` all decode identically.

If `text` contains any character outside `0`–`9`/`A`–`F`/`a`–`f` and the three permitted separators, the command returns an **empty BLOB**, not a 4D error. If `text` has an **odd number of hex digits**, the final, unpaired digit is silently dropped rather than causing an error.

An empty `text` produces an empty BLOB, not an error.

### Example
From the plugin's own test method (`Method1.4dm`):
```4d
$echo:=BLOB from hex($hex)
```

Decoding hex text with separators:
```4d
$blob:=BLOB from hex("48:65:6c:6c:6f")  // same result as BLOB from hex("48656c6c6f")
```

---

## Worked example — round-trip verification

This is the plugin's own test method (`Method1.4dm`) in full, which round-trips a document through both encodings and confirms integrity with a hash comparison — a good pattern to reuse if you want to sanity-check the plugin after installing it, or after moving between 4D versions:

```4d
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
```

If the plugin is working correctly, `$hash1`, `$hash2`, and `$hash3` are all identical — the base64 and hex round trips are lossless regardless of the line breaks `BLOB to base64` inserts.

---

## Error handling & troubleshooting

- **No command in this plugin raises a 4D error, ever.** Malformed input for either decoder (`BLOB from base64`, `BLOB from hex`) is handled by returning an empty BLOB, not by signaling failure. If you need to distinguish "empty because the input was invalid" from "empty because the input was actually empty," check the input yourself before calling the command.
- **`BLOB from hex` silently drops a trailing odd hex digit.** A string with an odd number of hex characters (after removing permitted separators) decodes everything except the last, unpaired digit — with no indication that anything was dropped.
- **`BLOB to base64`'s line breaks are non-standard for consumers outside this plugin.** The inserted `\n` every 72 characters is a deliberate design choice (per the plugin's own code comment) but is not universal base64 formatting — if you're sending the result to a system that expects unwrapped base64 (e.g. a single-line JSON field, some HTTP headers), strip the line breaks before use.
- **On very large BLOBs, behavior depends on which build of the plugin you're running.** In the source as originally reviewed, an internal allocation failure while encoding/decoding a very large BLOB (multi-gigabyte range) could leave 4D waiting indefinitely for a result that never arrives — an application hang, not a crash or an error message. **This is fixed in the corrected source produced during this review** (each command now guarantees a return value — an empty result — even if that internal failure occurs). If you're not certain your installed build includes that fix, treat extremely large BLOBs with caution until you've confirmed which source you're running.
- **A rare base64-padding quirk was fixed during this review.** For certain specific BLOB sizes, the originally reviewed `BLOB to base64` could emit one extra, non-standard trailing `=` character. This plugin's own `BLOB from base64` tolerates that (it ignores `=` anywhere), so round-tripping through this plugin was never affected — but a stricter, non-4D base64 consumer downstream could have rejected the output for those specific sizes. The corrected source eliminates this. If your base64 output is consumed by another system and you're on an older build, be aware of this edge case.

---

## Quick reference

```4d
// Base64
$b64:=BLOB to base64($blob)
$blob:=BLOB from base64($b64)

// Hex
$hex:=BLOB to hex($blob)
$blob:=BLOB from hex($hex)

// Hex decode tolerates separators:
$blob:=BLOB from hex("48:65:6c:6c:6f")   // same as "48656c6c6f"
```
