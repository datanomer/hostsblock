# hostsblock
Simple website blocker for linux which utilizes the /etc/hosts file to block sites on local machine.
Use at your own responsibility.
## requirement:
* Cmake
* Linux(for now)
* gcc

## Usage
Run the command ```make``` in the cloned directory.
Then execute the program with appropriate permissions ```sudo ./hblock```.

It is possible to use a external domain blocklist: ```sudo ./hblock <blocklist>```.  

## Information and stuff:

- Hostsblock simply writes the input site to the hosts file (found in linux by /etc/hosts) and redirects it to localhost address
### NOTE: You have to refresh the blocked page. You may have to clear the cache and restart the browser in some cases.
