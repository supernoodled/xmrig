# XMRig

XMRig is a high performance, open source, cross platform RandomX, KawPow, CryptoNight and AstroBWT unified CPU/GPU miner and [RandomX benchmark](https://xmrig.com/benchmark). Official binaries are available for Windows, Linux, macOS and FreeBSD.

## Mining backends
- **CPU** (x64/ARMv8)
- **OpenCL** for AMD GPUs.
- **CUDA** for NVIDIA GPUs via external [CUDA plugin](https://github.com/xmrig/xmrig-cuda).

## Usage
The preferred way to configure the miner is the [JSON config file](src/config.json) as it is more flexible and human friendly. The [command line interface](https://xmrig.com/docs/miner/command-line-options) does not cover all features, such as mining profiles for different algorithms. Important options can be changed during runtime without miner restart by editing the config file or executing API calls.

* **[Wizard](https://xmrig.com/wizard)** helps you create initial configuration for the miner.
* **[Workers](http://workers.xmrig.info)** helps manage your miners via HTTP API.

## Donations
* Default donation 0%, fully disabled

if ye wanna donate:
* XMR: `49D4P67NLspFfELR9WFTALTQTdza36wAEaVmRQpaAniSDQ9TBR2Lf8cTQWZdJbY5qf9jQ5M4RFZL9Te3dsoi6Tcd77c38qf`

## Versions
*xmrig x64 - v6.5.3 
(as of commit 89454c6d30ad0b2d8efd0026daec07bcdfa42952)

*openssl x64 - v3.0.0-alpha9-dev 
(as of commit 5053394aa6bc989e1ce8f0e47578c691aee6aa47)

*libuv x64 - v1.40.0 
(as of commit fc2c1a2341c0bedfca3bded96ac7dce8b3575b53)

*hwloc x64 - v2.3.0 
(https://www-lb.open-mpi.org/software/hwloc/v2.3/)

