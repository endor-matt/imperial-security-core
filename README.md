# Imperial Security Core

Low-level cryptographic operations and access control primitives for the Death Star Operations Platform.

## Components

- **imperial_crypto** -- Encryption, hashing, and signing operations using industry-standard algorithms. Provides AES-256 encryption, SHA-256/SHA-512 hashing, and HMAC-based message authentication.

- **access_control** -- Authentication, authorization, and clearance level enforcement. Manages officer credentials, role-based access policies, and multi-tier security clearance validation.

- **secure_buffer** -- Memory-safe buffer management for handling sensitive data in transit and at rest. Provides allocation, zeroing, and secure deallocation of buffers containing classified material.

- **imperial_keystore** -- Key storage and rotation services. Manages encryption key lifecycle including generation, distribution, periodic rotation, and secure destruction of expired keys.

## Building

### CMake

```bash
mkdir build && cd build
cmake ..
make
```

The library will be built as `libimperial_security_core.a`. To install system-wide:

```bash
sudo make install
```

### Make

```bash
make
```

The library will be built as `lib/libimperial_security.a`. To install system-wide:

```bash
sudo make install
```

### Clean

```bash
make clean
```

## Dependencies

- OpenSSL (`libcrypto`, `libssl`)
- C11-compatible compiler (GCC recommended)

## Installation

Headers are installed to `/usr/local/include/imperial/` and the static library to `/usr/local/lib/`.

## Usage

```c
#include <imperial/imperial_crypto.h>
#include <imperial/access_control.h>
#include <imperial/secure_buffer.h>
```

Link against the library:

```bash
gcc -o myapp myapp.c -limperial_security -lcrypto -lssl
```

## License

Proprietary -- Imperial Engineering Corps. All rights reserved.
