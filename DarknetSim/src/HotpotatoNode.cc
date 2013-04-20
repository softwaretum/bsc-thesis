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

    if (stage == 3) {
        std::vector<std::string> v = cStringTokenizer(par("ping_id")).asVector();
        for(std::vector<std::string>::iterator iter = v.begin(); iter != v.end(); iter++) {
            cMessage *timer = new PingTimer((*iter));
                scheduleAt(1.0, timer);
        }
    }
}


DarknetPeer* HotpotatoNode::findNextHop(DarknetMessage* msg) {
    if(!peers.size()) { // peer list empty -> raise exception? (TODO)
        EV << "ERROR: empty peer list!";
        return NULL;
    }
    if(peers.find(msg->getDestNodeID()) != peers.end()) {
        return peers[msg->getDestNodeID()];
    }else {
        std::map<std::string, DarknetPeer*>::iterator iter = peers.begin();
        std::advance(iter, dblrand() * peers.size());
        return iter->second;
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
        EV << "recieved PONG from: " << msg->getSrcNodeID() << endl;
        delete msg;
        break;
     default:
       DarknetBaseNode::handleIncomingMessage(msg);
       break;
    }
}
