//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef DARKNETBASENODE_H_
#define DARKNETBASENODE_H_

#include <omnetpp.h>
#include <UDPAppBase.h>
#include "messages/DarknetMessage.h"
#include "messages/MaintenanceMessage.h"

struct DarknetNode{
    std::string nodeID;
    IPvXAddress address;
    int port;
    bool active;
};

class DarknetBaseNode : public UDPAppBase  {
public:
    DarknetBaseNode() {};
    virtual ~DarknetBaseNode() { };
protected:

    std::string nodeID;
    int localPort;
    std::map<std::string, DarknetNode*> peers;


    // higher level interface for DarknetNodes
 //   virtual void connectPeer(IPvXAddress* destAddr, int destPort);

 //   virtual void handleMaintenanceMessage(MaintenanceMessage* msg);
 //   virtual void handleRequestMessage(DarknetMessage* msg);

    virtual void sendMessage(DarknetMessage* msg) = 0;
    virtual void forwardMessage(DarknetMessage* msg);


    virtual void processIncomingMessage(DarknetMessage* msg);

    // most likely only a wrapper around sendToUDP
    virtual void sendPacket(DarknetMessage* pkg, IPvXAddress& destAddr, int destPort);

    virtual void handleMessage(cMessage* msg);
    virtual void initialize(int stage);
};

#endif /* DARKNETBASENODE_H_ */
