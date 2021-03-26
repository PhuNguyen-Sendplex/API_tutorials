# API_tutorials


## Update and Upgrade Ubuntu packages
```bash
sudo apt-get update && apt-get upgrade -y 
```


## Install required libraries and packages
```bash
sudo apt-get install libcurl4-openssl-dev libcurlpp-dev wget g++ -y
```


## Clone this repo
```bash
git clone https://github.com/PhuNguyen-Sendplex/API_tutorials
```


## Compile the code
```bash
g++ -o api_login_logout api_login_logout.cpp -lcurl -lcurlpp
```


## Execute the code
```bash
./api_login_logout
```
