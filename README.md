Network-Models
==============

This repository contains a simulated implementation of Process per Protocol and
Process per Message network stacks. Each implementation is defined by a class.

Files
-----

* `processperprotocol.h`: Header file for Process per Protocol implementation
* `processperprotocol.cpp`: File containing the implementation of the Process
  per Protocol implementation
* `processpermessage.h`: Header file for Process per Message implementation
* `processpermessage.h`: File containing the implemenation of the Process per
  Message implementation
* `protocolid.h`: Header file containing ID's to be used to describe different
  protocols, as well as headers for each of those protocols
* `combined_application.cpp`: Application that starts one instance of Process
  per Protocol and one instance of Process per Message and transmits 400 
  messages (800 total) between them. Prints total time needed to transmit the
  message
* `protocol_application.cpp`: Application that starts two instance of Process
  per Protocol and transmits 800 total messages between them. Prints total time
  needed to transmit messages
* `message_application.cpp`: Application that starts two instance of Process
  per Message and transmits 800 total messages between them. Prints total time
  needed to transmit messages

Usage
-----

Compile all of the application files using `make`. Then, run each of the 
applications with `./combined_application`, `./protocol_application`, and
`./message_application`. Each application will print out the total time elapsed
between when that application started and when the last message was received.

Timing
------

Below are sample results for testing the time needed for each implementation 
of the network stack:

* Process per Protocol: 62 milliseconds
* Process per Message: 43 milliseconds
* Combined: 57 milliseconds