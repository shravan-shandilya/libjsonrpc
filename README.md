# [WIP] mini-stratum
This is an attempt to write stratum server in C for fun!

# How to compile?
```
$ make clean
$ make all
$ ./server
```
* JSON-RPC 2.0 is implemented from scratch(WIP). 
* JSONRPC API
  * **jsonrpc_server_start**    : starts the server at specific port
  * **jsonrpc_server_stop**     : stops the server
  * **jsonrpc_register_method** : adds a rpc method to the jsonrpc server
  * **jsonrpc_client_init**     : initialises client side connection
  * **jsonrpc_invoke_method**   : calls a remote method on jsonrpc server
  
# stratum [WIP]
  
