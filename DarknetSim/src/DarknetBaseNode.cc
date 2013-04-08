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

#include <omnetpp.h>
#include <IPAddressResolver.h>
#include "DarknetBaseNode.h"
#include <UDPPacket.h>
#include <cstringtokenizer.h>

void DarknetBaseNode::initialize(int stage) {
    if(stage == 0) {
        nodeID = par("node_id").stdstringValue();
        localPort = par("local_port");
        bindToPort(localPort);
    }else if (stage == 3) {
        const char* port_delimiter = ":";
        std::vector<std::string> v = cStringTokenizer(par("dest_id")).asVector();
        for(std::vector<std::string>::iterator iter = v.begin(); iter != v.end(); iter++) {
            std::vector<std::string> peer_tuple = cStringTokenizer((*iter).c_str(),port_delimiter).asVector(); //split <destID>:<destPort>
            if(peer_tuple.size() == 2) {
                DarknetPeer* peer = new DarknetPeer;
                peer->nodeID = peer_tuple[0];
                peer->address = IPAddressResolver().resolve(peer->nodeID.c_str());
                std::istringstream convert(peer_tuple[1]);
                int port;
                peer->port = convert >> port ? port : 0;  //convert string to int (user 0 on error)
                peers.insert(std::pair<std::string, DarknetPeer*>(peer->nodeID,peer));
            }
        }
    }
}

void DarknetBaseNode::sendPacket(DarknetMessage* dmsg, IPvXAddress& destAddr, int destPort) {
    sendToUDP(dmsg, localPort, destAddr, destPort);
}

void DarknetBaseNode::sendMessage(DarknetMessage* msg) {
    DarknetPeer *destPeer = findNextHop(msg);
    if(destPeer != NULL)
        sendPacket(msg,destPeer->address,destPeer->port);
    else {
        EV << "No next hop found for message: " << msg;
        //TODO: implement proper default error handling here
        delete msg;
    }
}

void DarknetBaseNode::handleMessage(cMessage *msg) {
    if (msg->isSelfMessage()) {
        handleSelfMessage(msg);
    } else if(dynamic_cast<DarknetMessage*>(msg) != NULL) {
        handleIncomingMessage(check_and_cast<DarknetMessage*>(msg));
    } else {
       EV << "received an unknown cMessage: " << msg;
       delete msg;
   }

}

void DarknetBaseNode::handleIncomingMessage(DarknetMessage *msg) {
    if (msg->destNodeID != nodeID) {
            forwardMessage(msg);
    }else {
       EV << "received unknwon DarknetMessage for this node: " << msg;
       delete msg;
    }
}

void DarknetBaseNode::forwardMessage(DarknetMessage* msg) {
    if(msg->TTL > 0) {
        msg->TTL--;
        sendMessage(((DarknetMessage*)msg)->dup());
    }else {
        // TODO: inform simulator/user of droped message
        EV << "dropped message";
    }
    delete msg;
}
