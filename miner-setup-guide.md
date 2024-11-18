## Usage of Miner Setup and Update Script

This script is designed to quickly install and update the TOKI-ccminerARM mining software. It supports multiple languages (English, Traditional Chinese, Simplified Chinese, Japanese, and Korean). The script automatically configures the environment and generates the required configuration file for mining.

### Download the Script
Download the script from the following link:
https://github.com/TokiZeng/TOKI-ccminerARM/releases/download/latest/miner-setup-update.sh

### Pre-Requisites
1. Ensure the following dependencies are installed on your system:
   - wget: Used for downloading required files.
   - bash: Script execution environment (included in most Linux systems).
2. The default environment is Ubuntu on UserLAnd. If running on other platforms, you may need to manually adjust paths or dependencies.

### Installation Process
```
sudo apt update
```
```
sudo apt install -y wget nano ca-certificates
```
```
wget https://github.com/TokiZeng/TOKI-ccminerARM/releases/download/latest/miner-setup-update.sh
```
```
chmod +x miner-setup-update.sh
```
```
./miner-setup-update.sh
```

### Feature Options
After starting the script, you will be prompted to select an option:

1. Full Installation  
   Installs the latest version of the mining software and generates the necessary configuration file.
   
2. Update Binary Files  
   Updates only the mining software's binary file (for systems with existing configuration files).

### Language Selection
The script supports the following languages and will prompt for selection at the start:
- 1) English
- 2) 繁體中文 (Traditional Chinese)
- 3) 简体中文 (Simplified Chinese)
- 4) 日本語 (Japanese)
- 5) 한국어 (Korean)

### Installation Process
1. Select Architecture:  
   Choose based on your device's architecture:
   - A53
   - A55
2. Automatic Downloads:  
   The script will download the appropriate binary file and dependencies.
3. Configuration File Generation:  
   The script will prompt for the following parameters to generate config.json (defaults can be used):  
   - Number of threads
   - Mining pool URL
   - Wallet address
   - Miner name

### Configuration File
The script generates ~/config.json with the following format:
```json
{
      "algo" : "verus",
      "threads" : 8,
      "cpu-priority" : 3,
      "max-log-rate": 60,
      "quiet" : false,
      "debug" : false,
      "protocol" : false,
      "url" : "stratum+tcp://us.vipor.net:5040",
      "user" : "your wallet address.your miner name",
      "pass" : "x"
}
```
If adjustments are needed, manually edit this file.

### Start Mining
1. Use the following command to start mining:
   ./start.sh
2. To reconfigure, rerun the script:
   ./miner-setup-update.sh

### Support and Feedback
If you encounter any issues, refer to the following resources:
- GitHub Repository: https://github.com/TokiZeng/TOKI-ccminerARM
- YouTube Channel: 熔爐 FORGE THE MAKER
