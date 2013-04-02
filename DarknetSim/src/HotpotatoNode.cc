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
#include <cstringtokenizer.h>
#include "HotpotatoNode.h"

Define_Module(HotpotatoNode);

void HotpotatoNode::initialize(int stage)
{

    if(stage == 0)
        DarknetBaseNode::initialize(stage);

    if (stage == 3) {
        //std::string peerID = par("dest_id").stdstringValue();

        std::vector<std::string> v = cStringTokenizer(par("dest_id")).asVector();
        for(std::vector<std::string>::iterator iter = v.begin(); iter != v.end(); iter++) {
            DarknetNode* peer = new DarknetNode;
            std::string peerID = (*iter);
            peer->nodeID = peerID;
            peer->address = IPAddressResolver().resolve(peerID.c_str());
            peer->port = par("dest_port");
            peer->active = true;
            peers.insert(std::pair<std::string, DarknetNode*>(peerID,peer));
        }

        if(nodeID == "host1") {
            cMessage *timer = new cMessage("sendTimer");
                scheduleAt(1.0, timer);
        }
    }
}

void HotpotatoNode::sendMessage(DarknetMessage* msg) {
    if(!peers.size()) {
        // peer list empty -> raise exception?
        EV << "ERROR: empty peer list!";
        return;
    }
    DarknetNode *destPeer;
    if(peers.find(msg->destNodeID) != peers.end()) {
        destPeer = peers[msg->destNodeID];
    }else {
        std::map<std::string, DarknetNode*>::iterator iter = peers.begin();
        std::advance(iter, dblrand() * peers.size());
        destPeer = iter->second;
    }
    sendPacket(msg,destPeer->address,destPeer->port);
}
