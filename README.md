# Foxypad
A device driver for Asus laptops with Numberpad support and experimental support for generic touchpads on any laptop

## Installation
Clone this repo, and make the binary

```bash
mkdir -p ~/GitClones
git clone https://github.com/JustSomeLazyFox/Foxypad ~/GitClones/Foxypad
cd ~/GitClones/Foxypad
cd build
cmake ..
make
sudo mv ./foxypad /usr/bin/foxypad
```

## Configuration
Foxypad used Lua for configuration. By default, `foxypad` looks for a config file at `~/.config/foxypad/config.lua`. You can also provide a different config file path to the program using the `--config-file` argument. The stubs are also provided in the `resources` directory.
To use the stubs, add this to your `~/.foxypad/.luarc.json` file:
```json
{
  "workspace": {
    "library": [
      "/home/<username>/GitClones/Foxypad/resources/stubs.lua"
    ]
  }
}
```

A default config is also provided in the resources directory

## Usage
Foxypad provides several command line arguments:
```bash
foxypad [options...]
        -c      --config-file   Read from a different config file
        -g      --generic       For use with non-Asus laptops or devices without Numberpad support
        -o      --old-model     Compatibility with older models of Asus Numberpad
        -i      --i2c-device    Use a different i2c device path. Default is /dev/i2c-0.
        -l      --log-level     Set the log level. Possible values: ERRORS_ONLY, ERRORS_AND_WARNINGS, SUCCESS_STATUSES, INFO, DEBUG (case-insensitive)
        -d      --dimensions    Detect and print the dimensions of the touchpad device # exits immediately
        -h      --help          Display this help message # exits immediately
        -v      --version       Display the version number of the program # exits immediately
```
