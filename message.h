#ifndef _MESSAGE_
#define _MESSAGE_

#include <list>
#include <iterator>
#include <memory.h>

class Message
{
public:
   
    Message( );
    Message(char* msg, size_t len);
    ~Message( );
    void msgAddHdr(char *hdr, size_t length);
    char* msgStripHdr(int len);
    int msgSplit(Message& secondMsg, size_t len);
    void msgJoin(Message& secondMsg);
    size_t msgLen( );
    void msgFlat(char *buffer);

private:
    size_t msglen;
    char *msg_content;
};

#endif