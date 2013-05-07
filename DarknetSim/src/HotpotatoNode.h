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

#ifndef HOTPOTATONODE_H_
#define HOTPOTATONODE_H_

#include <string>

#include "DarknetBaseNode.h"

class DarknetMessage;
class IPvXAddress;

class PingTimer: public cMessage {
public:
    PingTimer(const std::string& name) : cMessage(name.c_str()) {}
    virtual ~PingTimer();
};


class HotpotatoNode: public DarknetBaseNode {
public:
   HotpotatoNode() {}
   virtual ~HotpotatoNode() {}
protected:

   virtual int numInitStages() const { return 6; }
   virtual DarknetPeer* findNextHop(DarknetMessage* msg);
   virtual void initialize(int stage);
   virtual void handleSelfMessage(cMessage* msg);
   virtual void handleIncomingMessage(DarknetMessage* msg);
   virtual void connectPeer(const std::string& toNodeID, const IPvXAddress& destAddr, int destPort); //do nothing
};

#endif /* HOTPOTATONODE_H_ */
