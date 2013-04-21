#!/usr/bin/python3

import fileinput
import re
import string

pattern = re.compile("^(?P<node>\d+):(?P<edges>(\d+(;\d+)*)?)")

nodes = {}
ignored_lines = []

for line in fileinput.input():
    match = pattern.search(line)
    if match != None:
        nodes[match.group("node")] = match.group("edges").split(';')
    else:
        ignored_lines.append(line[:-1])


#print("ignored_lines:")
#print("\n".join(ignored_lines))
#print("\n\n-------------\n")

def ned_make_hosts(nodes):
    ret = []
    for node in nodes.keys():
        ret.append("\thost"+node+": StandardHost {\n\t}")
    return ret

def ned_make_router_connections(nodes):
    ret = []
    for node in nodes.keys():
        ret.append("\trouter.pppg++ <--> DatarateChannel <--> host"+node+".pppg++;")
    return ret

def ini_conf_hosts(nodes):
    ret = []
    default_port = 8080
    for node in nodes.keys():
        ret.append("**.host"+node+".udpApp[0].node_id=\"host"+node+"\"")
        peers=[]
        for peer in nodes[node]:
            peers.append("host"+peer+":"+str(default_port))
        ret.append("**.host"+node+".udpApp[0].dest_id=\""+(" ".join(peers))+"\"")
    return ret



print("\n".join(ned_make_hosts(nodes)))
print("\n-----\n")
print("\n".join(ned_make_router_connections(nodes)))
print("\n-----\n")
print("\n".join(ini_conf_hosts(nodes)))
print("\n-----\n")
