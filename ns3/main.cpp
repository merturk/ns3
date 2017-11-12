//
//  main.cpp
//  ns3
//
//  Created by Mehmet Ali Ertürk on 12.11.2017.
//  Copyright © 2017 Mehmet Ali Ertürk. All rights reserved.
//


#include <iostream>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"

using namespace ns3;

int main(int argc, const char * argv[]) {
    
    Time::SetResolution (Time::NS);
    LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    
    NodeContainer nodes;
    nodes.Create (2);
    
    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("5Mbps"));
    pointToPoint.SetChannelAttribute ("Delay", StringValue ("2ms"));
    
    NetDeviceContainer devices;
    devices = pointToPoint.Install (nodes);
    
    InternetStackHelper stack;
    stack.Install (nodes);
    
    Ipv4AddressHelper address;
    address.SetBase ("10.1.1.0", "255.255.255.0");
    
    Ipv4InterfaceContainer interfaces = address.Assign (devices);
    
    UdpEchoServerHelper echoServer (9);
    
    ApplicationContainer serverApps = echoServer.Install (nodes.Get (1));
    serverApps.Start (Seconds (1.0));
    serverApps.Stop (Seconds (10.0));
    
    UdpEchoClientHelper echoClient (interfaces.GetAddress (1), 9);
    echoClient.SetAttribute ("MaxPackets", UintegerValue (1000));
    echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.001)));
    echoClient.SetAttribute ("PacketSize", UintegerValue (2024));
    
    ApplicationContainer clientApps = echoClient.Install (nodes.Get (0));
    clientApps.Start (Seconds (2.0));
    clientApps.Stop (Seconds (100.0));
    
    
    std::cout << "Hello, World!\n";
    return 0;
}

