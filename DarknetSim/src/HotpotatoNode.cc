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

#include <IPAddressResolver.h>
#include "HotpotatoNode.h"

Define_Module(HotpotatoNode);

void HotpotatoNode::initialize(int stage) {
    DarknetBaseNode::initialize(stage);

    if (stage == 5) {
        std::vector<std::string> v = cStringTokenizer(par("ping_id")).asVector();
        for(std::vector<std::string>::iterator iter = v.begin(); iter != v.end(); iter++) {
            cMessage *timer = new PingTimer((*iter));
                scheduleAt(1.0, timer);
        }
    }
}

void HotpotatoNode::connectPeer(std::string nodeID, IPvXAddress* destAddr, int destPort) {
    DarknetMessage *dm = new DarknetMessage();
    dm->setType(DM_CON_SYN);
    dm->setSrcNodeID(this->nodeID.c_str());
    dm->setDestNodeID(nodeID.c_str());
    sendPacket(dm, *destAddr, destPort);
//    sendMessage(dm);
};

DarknetPeer* HotpotatoNode::findNextHop(DarknetMessage* msg) {
    if(!connections.size()) { // peer list empty -> raise exception? (TODO)
        EV << "ERROR: empty peer list!";
        return NULL;
    }
    if(peers.find(msg->getDestNodeID()) != peers.end() and connections.find(msg->getDestNodeID()) != connections.end()) {
        return peers[msg->getDestNodeID()];
    }else {
        std::map<std::string, DarknetConnection*>::iterator iter = connections.begin();
        std::advance(iter, dblrand() * connections.size());
        return peers[iter->first];
    }
}

void HotpotatoNode::handleSelfMessage(cMessage *msg) {
    if(dynamic_cast<PingTimer*>(msg) != NULL) {
        sendMessage(makeRequest(msg->getName()));
    }
    delete msg;
}

void HotpotatoNode::handleIncomingMessage(DarknetMessage *msg) {
    switch(msg->getType()) {
    case DM_RESPONSE:
        EV << "received PONG from: " << msg->getSrcNodeID() << endl;
        delete msg;
        break;
    case DM_CON_SYN: {
        if(peers.find(msg->getSrcNodeID()) != peers.end()) {
            DarknetPeer *peer = peers[msg->getSrcNodeID()];
            EV << "received CON_SYN from: " << msg->getSrcNodeID() << endl;
            DarknetMessage *dm = new DarknetMessage();
            dm->setType(DM_CON_ACK);
            dm->setSrcNodeID(msg->getDestNodeID());
            dm->setDestNodeID(msg->getSrcNodeID());
            sendPacket(dm,peer->address,peer->port);
        }
        break;
    }
    case DM_CON_ACK: {
        DarknetConnection *dc = new DarknetConnection;
        dc->nodeID = msg->getSrcNodeID();
        dc->lastSeen=0; //TODO fix
        connections.insert(std::pair<std::string,DarknetConnection*>(msg->getSrcNodeID(),dc));
        EV << "connection to " << dc->nodeID << "established" << endl;
        delete msg;
        break;
    }
    default:
       DarknetBaseNode::handleIncomingMessage(msg);
       break;
    }
}
