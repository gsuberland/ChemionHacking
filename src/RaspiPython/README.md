# Simple Python class to interact with CHEMION glasses

Based on @notpike's work

## Example

```python
from glasses import Glasses

# Static Display "0w0 ?"
buffer = [
    b'\xfa\x03\x00\x39\x01\x00\x06\x00\x00' +
    b'\x00\x00\x00\x00\x3c\x00\x00\x3c\x00\x3c\xf3',
    b'\x00\x00\xf3\x00\xc3\xf3\x3c\x0c\xf3' +
    b'\x00\xc3\xf3\x3c\xcc\xf3\x00\x0c\xf3\x3c\xcc',
    b'\xf3\x00\x0c\xf3\x3c\xcc\xf3\x00\x00' +
    b'\x3c\x0f\x30\x3c\x00\x0c\x00\x00\x00\x00\x00',
    b'\x00\xc8\x55\xa9'
]

glasses_address = Glasses.discover()

if glasses_address:
    glasses = Glasses(glasses_address)
    glasses.connect()

    # To display the buffer during 5 seconds
    glasses.display(buffer, 5)
    glasses.disconnect()
```
