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

#include "HotpotatoNode.h"

HotpotatoNode::HotpotatoNode() {
    // TODO Auto-generated constructor stub

}

HotpotatoNode::~HotpotatoNode() {
    // TODO Auto-generated destructor stub
}

void HotpotatoNode::sendMessage(DarknetMessage* msg) {
    if(!peers.size()) {
        // peer list empty -> raise exception?
        return;
    }
    DarknetNode* destPeer;
    if(peers.find(msg->destNodeID) != peers.end()) {
        destPeer = peers[msg->destNodeID];
    }else {
        std::map::iterator it = peers.begin();
        std::advance(it, rand() % peers.size());
        destPeer = (*it).second;
    }
    sendPacket(msg,destPeer->address,destPeer->port);
}
