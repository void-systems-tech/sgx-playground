# SGX Playground

A collection of simple examples demonstrating Intel SGX enclave using the [OpenEnclave SDK](https://openenclave.io/sdk/).

## Prerequisites

### Hardware Requirements

Some of the examples (e.g. data sealing) require actual SGX-compatible hardware and will not function in simulation mode due to the [documented feature limitations](https://github.com/openenclave/openenclave/blob/master/docs/GettingStartedDocs/install_oe_sdk-Simulation.md). This is further confirmed in [Issue #1851](https://github.com/openenclave/openenclave/issues/1851#issuecomment-498442995) where the maintainer directly states that "_The OE SDK's simulation mode support is kind of limited, which does not cover data-sealing or any report related operations._". Given that this comment has been made in June 2019 and this limitation is still present in March 2026, it appears unlikely to be resolved.

Hardware compatibility can be checked e.g. by using [SGX Detect Tool](https://fortanix.zendesk.com/hc/en-us/articles/4414753648788-SGX-Detect-Tool):
```sh
curl -o sgx-detect https://download.fortanix.com/sgx-detect/ubuntu24.04/sgx-detect
chmod +x sgx-detect
```

Example output on SGX-compatible device:
```
Detecting SGX, this may take a minute...
✔  SGX instruction set
  ✔  CPU support
  ✔  CPU configuration
  ✔  Enclave attributes
  ✔  Enclave Page Cache
  SGX features
    ✘  SGX2  ✘  EXINFO  ✘  ENCLV  ✘  OVERSUB  ✘  KSS
    Total EPC size: 93.5MiB
✔  Flexible launch control
  ✔  CPU support
  ？ CPU configuration
  ✔  Able to launch production mode enclave
✔  SGX system software
  ✔  SGX kernel device (/dev/sgx_enclave)
  ✔  libsgx_enclave_common
  ✔  AESM service
  ✔  Able to launch enclaves
    ✔  Debug mode
    ✔  Production mode
    ✔  Production mode (Intel whitelisted)

You're all set to start running SGX programs!
```

Example output on SGX-incompatible device:
```
Detecting SGX, this may take a minute...
✘  SGX instruction set
  ✘  CPU support
✘  SGX system software
  ✘  SGX kernel device
  ✘  libsgx_enclave_common
  ✘  AESM service

🕮  SGX instruction set > CPU support
It appears your hardware does not have SGX support.

(run with `--verbose` for more details)

More information: https://edp.fortanix.com/docs/installation/help/#cpu-support

🕮  SGX system software > SGX kernel device
The SGX device (/dev/sgx/enclave, /dev/sgx or /dev/isgx) is not present.

It could not be detected whether you have an SGX driver installed. Please make sure the SGX driver is installed and loaded correctly.

(run with `--verbose` for more details)

More information: https://edp.fortanix.com/docs/installation/help/#sgx-driver

🕮  SGX system software > AESM service
AESM could not be contacted. AESM is needed for launching enclaves and generating attestations.

Please check your AESM installation.

(run with `--verbose` for more details)

More information: https://edp.fortanix.com/docs/installation/help/#aesm-service
```

### OpenEnclave SDK Installation

OpenEnclave SDK has strict (and somewhat outdated - e.g. requires `clang-11`) requirements on the toolchain. The easiest way to satisfy those requirements is to use an [official Docker image](https://github.com/openenclave/openenclave/blob/master/DOCKER_IMAGES.md) and [build the SDK inside it](https://github.com/openenclave/openenclave/blob/master/docs/GettingStartedDocs/Contributors/BuildingInADockerContainer.md).

This repository assumes that the OpenEnclave SDK has already been built and that it is available at the path pointed to by `OE_INSTALL` environment variable:
```sh
export OE_INSTALL="../path/to/openenclave/installation/directory"
```

### SGX Platform Software

Running enclaves on real SGX hardware requires Intel's SGX Platform Software (PSW) installed on the host OS. This is separate from the OE SDK build environment and must be set up before running examples with `OE_SIMULATE=0`.

Add the Intel SGX APT repository (must be run as root):
```sh
curl -fsSL https://download.01.org/intel-sgx/sgx_repo/ubuntu/intel-sgx-deb.key -o /etc/apt/keyrings/intel-sgx-keyring.asc
echo "deb [signed-by=/etc/apt/keyrings/intel-sgx-keyring.asc arch=amd64] https://download.01.org/intel-sgx/sgx_repo/ubuntu $(lsb_release -cs) main" > /etc/apt/sources.list.d/intel-sgx.list
apt-get update
```

Install the runtime packages:
```sh
apt-get install -y libsgx-quote-ex libsgx-dcap-ql
```

These two packages pull in all required runtime dependencies, including `libsgx-enclave-common` (the shared library the OE SDK loads at runtime) and `sgx-aesm-service` (the AESM daemon).

Enable and start the AESM service:
```sh
systemctl enable aesmd
systemctl start aesmd
```

The AESM daemon (`aesmd`) manages the architectural enclaves responsible for enclave launch and sealing key derivation. It must be running for `oe_seal` / `oe_unseal` to work.

Add the current user to the `sgx` group (required to access `/dev/sgx_enclave`):
```sh
sudo usermod -aG sgx ${USER}
newgrp sgx   # apply without re-login
```

## Usage

Use provided [./build.sh](build.sh) script to build the examples:
```
./build.sh
```

Use provided [./run.sh](run.sh) script to run the examples (`OE_SIMULATE` defaults to `1`):
```
OE_SIMULATE=0 ./run.sh
```


## Examples

### Hello SGX

The simple example demonstrating basic principles. It runs correctly in simulation mode. See the [example output](./hello.out).

**Structure:**
```
src/
├─ enclave/
│   ├─ hello.edl       (interface: ecall enclave_hello; ocalls host_hello)
│   └─ hello/
│       ├─ main.c      (trusted side)
│       └─ sgx.conf    (enclave config)
└─ host/
    └─ hello.c         (untrusted side: main + ocall implementations)
```

**What it does:**

The host loads the signed enclave and makes a single ecall into it. Inside the enclave, three things happen in sequence:

1. **Ocall `host_hello()`**: Demonstrates the simplest possible scenario. Enclave calls back to host.

2. **Ocall `host_get_const('e')`**: Demonstrates passing arguments and return value to/from the host.

3. **Dynamic heap allocation**: The enclave allocates maximum possible amount of heap memory, demonstrating that the enclave has its own isolated heap. There is no overcommit nor OOM killer inside the enclave: `malloc` fails with `NULL` when the enclave heap is exhausted.

### Data Sealing

Demonstrates SGX data sealing: encrypting data so it can only be decrypted by a specific enclave (or class of enclaves) on the same platform. Requires actual SGX-compatible hardware — does not work in simulation mode. See the [example output](./datasealing.out).

**Structure:**

```
src/
├─ datasealing/
│   └─ common.c                (transform_data(): shared between trusted and untrusted worlds)
├─ enclave/
│   ├─ datasealing.edl         (interface: ecalls enclave_seal_data, enclave_unseal_data)
│   └─ datasealing/
│       ├─ dispatcher.c/h      (seal_data / unseal_data: shared by all three enclave variants)
│       ├─ a_v1/               (Enclave A, ProductID=1, SecurityVersion=1, signing key A)
│       ├─ a_v2/               (Enclave A, ProductID=1, SecurityVersion=2, signing key A)
│       └─ b/                  (Enclave B, ProductID=1, SecurityVersion=1, signing key B)
└─ host/
    └─ datasealing.c           (host side: loads all three enclaves, drives 8 seal/unseal test cases)
```

**What it does:**

The example seals and unseals a fixed plaintext (`"test plaintext"`) across three enclave identities under two sealing policies, producing 8 test cases in total.

Before sealing, `transform_data()` transforms the plaintext into `"<original> - <reversed>"` format (e.g. `"test plaintext - txetnialpset tset"`). This function is compiled separately for both the trusted (enclave) and untrusted (host) worlds from the same source — demonstrating that common code can be shared across the trust boundary.

The two sealing policies differ in how the sealing key is derived:

```
[ POLICY_UNIQUE  ]   key = f(enclave code measurement)
                     → only the exact same enclave binary can unseal

[ POLICY_PRODUCT ]   key = f(ProductID, SecurityVersion, signing key)
                     → any enclave with the same product identity can unseal, subject to SVN constraints
```

Test matrix (seal enclave → unseal enclave):

```
[ POLICY_UNIQUE ]
  A-v1 -> A-v1   PASS   (same binary)
  A-v1 -> A-v2   FAIL   (different code => different measurement => different key)
  A-v2 -> A-v1   FAIL   (same as A-v1 → A-v2, included only for completeness)
  A-v1 -> B      FAIL   (different signing key)

[ POLICY_PRODUCT ]
  A-v1 -> A-v1   PASS   (same ProductID, same SVN, same signing key)
  A-v1 -> A-v2   PASS   (same ProductID+key; higher SVN=2 can unseal data from lower SVN=1)
  A-v2 -> A-v1   FAIL   (OE_INVALID_ISVSVN: lower SVN=1 cannot unseal data from higher SVN=2)
  A-v1 -> B      FAIL   (different signing key, even though ProductID and SVN match)
```

The optional message (`"optional sealing message"`) is passed as additional authenticated data to `oe_seal` / `oe_unseal` — it is not encrypted, but the unsealing will fail if it does not match what was provided at sealing time.


