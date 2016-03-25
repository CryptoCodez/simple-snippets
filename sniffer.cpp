// =====================================================================================
//
//       Filename:  sniffer.cpp
//
//    Description:  simple packet sniffer
//                  Code is a recoded & slightly modified C++ version of BinaryTides
//                  C Linux Packet Sniffer 
//                  Checkout: http://www.binarytides.com/packet-sniffer-code-c-linux/
//
//        Version:  1.0
//        Created:  03/25/2016 07:50:49 PM
//       Revision:  none
//       Compiler:  g++
//
//         Author:  Marc Puttkammer (MP), CryptoCodez@protonmail.com
//   Organization:  
//
// =====================================================================================
//
// TODO:
// Implement threads for recv, packet processing, and output.
// OOP
//

// Network header files
//
#include <netinet/ip_icmp.h> 	// Provides declarations for icmp header
#include <netinet/udp.h>	// Provides declarations for udp header
#include <netinet/tcp.h>	// Provides declarations for tcp header
#include <netinet/ip.h>		// Provides declarations for ip header

#include <sys/socket.h>		// Hmm whats this... ;)
#include <arpa/inet.h>		//

// C header files
//
#include <stdio.h> 		// Standard stuff
#include <stdlib.h>		// malloc, ...
#include <string.h>		// memset, ...
#include <unistd.h>		// close

// C++ header files
//
#include <iostream>		// cout, cerr, endl, ...

// using directives
using std::cout;
using std::cerr;		
using std::endl;

void processPacket 	( unsigned char*, int ); 	// Packet processing

void printIpHeader 	( unsigned char*, int );	// Ip header console output
void printTcpPacket 	( unsigned char*, int );	// Tcp packet console output
void printUdpPacket 	( unsigned char*, int );	// Udp packet console output
void printIcmpPacket 	( unsigned char*, int );	// Icmp packet console  output

void printData 		( unsigned char*, int );	// Prints Hexa data

FILE *logfile;
int tcp = 0, udp = 0, icmp = 0, others = 0, igmp = 0, total = 0, i, j;
struct sockaddr_in source, dest;

int main()
{
	int saddrSize, dataSize;
	struct sockaddr saddr;
	struct in_addr in;

	unsigned char *buffer = (unsigned char*)malloc(65536);

	logfile = fopen("log.txt", "w");

	if (logfile == NULL)
		cerr << "Unable to create file." << endl;
	cout << "Starting" << endl;

	// Create a RAW socket for sniffing
	int sockRaw = socket (AF_INET, SOCK_RAW, IPPROTO_TCP);
	if (sockRaw < 0)
	{
		cout << "Socket error" << endl;
		return 1;
	}
	saddrSize = sizeof saddr;
	while (true)
	{
		

		// Receive a packet
		dataSize = recvfrom(sockRaw, buffer, 65536, 0, &saddr, (unsigned int*) &saddrSize);
		if (dataSize < 0)
		{
			cout << "Recvfrom error, failed to get packets" << endl;
			return 1;
		}

		processPacket(buffer, dataSize);
	}

	close (sockRaw);
	cout << "Finished!" << endl;


	return 0;
}

void processPacket (unsigned char *buffer, int size)
{
	// Get the IP header
	struct iphdr *iph = (struct iphdr*) buffer;
	++total;

	switch (iph->protocol) //Check the Protocol...
    	{
        case 1:  //ICMP Protocol
            ++icmp;
            printIcmpPacket(buffer,size);
            break;
         
        case 2:  //IGMP Protocol
            ++igmp;
            break;
         
        case 6:  //TCP Protocol
            ++tcp;
            printTcpPacket(buffer , size);
            break;
         
        case 17: //UDP Protocol
            ++udp;
            printUdpPacket(buffer , size);
            break;
         
        default: //Some Other Protocol like ARP etc.
            ++others;
            break;
    	}
	cout << "TCP : " << tcp 
	     << " UDP : " << udp 
	     << " ICMP : " << icmp 
	     << " IGMP : " << igmp 
	     << " Others : " << others 
	     << " Total : " << total 
	     << "\r" << endl;
}

void printIpHeader (unsigned char* buffer, int size)
{
	unsigned short iphdrlen;

	struct iphdr *iph = (struct iphdr*) buffer;
	iphdrlen = iph->ihl * 4;

	memset (&source, 0, sizeof(source));
	source.sin_addr.s_addr = iph->saddr;

	memset (&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = iph->daddr;

	fprintf (logfile, "\n");
	fprintf (logfile, "IP Header\n");
	fprintf (logfile, "   |-IP Version        : %d\n", (unsigned int)iph->version);
	fprintf (logfile, "   |-IP Header Length  : %d DWORDS or %d Bytes\n", (unsigned int)iph->ihl, ((unsigned int)(iph->ihl))*4);
    	fprintf (logfile, "   |-Type Of Service   : %d\n", (unsigned int)iph->tos);
    	fprintf (logfile, "   |-IP Total Length   : %d  Bytes(Size of Packet)\n", ntohs(iph->tot_len));
    	fprintf (logfile, "   |-Identification    : %d\n", ntohs(iph->id));
    	// fprintf (logfile, "   |-Reserved ZERO Field   : %d\n",(unsigned int)iphdr->ip_reserved_zero);
	// fprintf (logfile, "   |-Dont Fragment Field   : %d\n",(unsigned int)iphdr->ip_dont_fragment);
    	// fprintf (logfile, "   |-More Fragment Field   : %d\n",(unsigned int)iphdr->ip_more_fragment);
    	fprintf (logfile, "   |-TTL      : %d\n", (unsigned int)iph->ttl);
    	fprintf (logfile, "   |-Protocol : %d\n", (unsigned int)iph->protocol);
    	fprintf (logfile, "   |-Checksum : %d\n", ntohs(iph->check));
    	fprintf (logfile, "   |-Source IP        : %s\n", inet_ntoa(source.sin_addr));
    	fprintf (logfile, "   |-Destination IP   : %s\n", inet_ntoa(dest.sin_addr));
}

void printTcpPacket (unsigned char* buffer, int size)
{
	unsigned short iphdrlen;
	
	struct iphdr *iph = (struct iphdr*) buffer;
	iphdrlen = iph->ihl * 4;

	struct tcphdr *tcph = (struct tcphdr*)(buffer + iphdrlen);
	fprintf(logfile,"\n\n***********************TCP Packet*************************\n");    
         
    	printIpHeader (buffer, size);
         
    	fprintf (logfile,"\n");
    	fprintf (logfile,"TCP Header\n");
    	fprintf (logfile,"   |-Source Port      : %u\n",ntohs(tcph->source));
    	fprintf (logfile,"   |-Destination Port : %u\n",ntohs(tcph->dest));
    	fprintf (logfile,"   |-Sequence Number    : %u\n",ntohl(tcph->seq));
    	fprintf (logfile,"   |-Acknowledge Number : %u\n",ntohl(tcph->ack_seq));
    	fprintf (logfile,"   |-Header Length      : %d DWORDS or %d BYTES\n" , (unsigned int)tcph->doff, (unsigned int)tcph->doff*4);
    	// fprintf (logfile,"   |-CWR Flag : %d\n", (unsigned int)tcph->cwr);
    	// fprintf (logfile,"   |-ECN Flag : %d\n", (unsigned int)tcph->ece);
    	fprintf (logfile,"   |-Urgent Flag          : %d\n",(unsigned int)tcph->urg);
    	fprintf (logfile,"   |-Acknowledgement Flag : %d\n",(unsigned int)tcph->ack);
    	fprintf (logfile,"   |-Push Flag            : %d\n",(unsigned int)tcph->psh);
    	fprintf (logfile,"   |-Reset Flag           : %d\n",(unsigned int)tcph->rst);
    	fprintf (logfile,"   |-Synchronise Flag     : %d\n",(unsigned int)tcph->syn);
    	fprintf (logfile,"   |-Finish Flag          : %d\n",(unsigned int)tcph->fin);
    	fprintf (logfile,"   |-Window         : %d\n",ntohs(tcph->window));
    	fprintf (logfile,"   |-Checksum       : %d\n",ntohs(tcph->check));
    	fprintf (logfile,"   |-Urgent Pointer : %d\n",tcph->urg_ptr);
    	fprintf (logfile,"\n");
    	fprintf (logfile,"                        DATA Dump                         ");
    	fprintf (logfile,"\n");
         
    	fprintf (logfile,"IP Header\n");
    	printData (buffer,iphdrlen);
         
    	fprintf (logfile,"TCP Header\n");
    	printData (buffer + iphdrlen, tcph->doff * 4);
         
    	fprintf (logfile,"Data Payload\n");  
    	printData (buffer + iphdrlen + tcph->doff * 4 , (size - tcph->doff * 4 - iph->ihl * 4));
                         
    	fprintf(logfile,"\n###########################################################\n");
}

void printUdpPacket (unsigned char *buffer, int size)
{
	unsigned short iphdrlen;
	struct iphdr *iph = (struct iphdr*) buffer;
	iphdrlen = iph->ihl * 4;
	struct udphdr *udph = (struct udphdr*)(buffer + iphdrlen);
     
    	fprintf (logfile, "\n\n***********************UDP Packet*************************\n");
     
    	printIpHeader (buffer, size);           
     
    	fprintf (logfile,"\nUDP Header\n");
    	fprintf (logfile,"   |-Source Port      : %d\n" , ntohs(udph->source));
    	fprintf (logfile,"   |-Destination Port : %d\n" , ntohs(udph->dest));
    	fprintf (logfile,"   |-UDP Length       : %d\n" , ntohs(udph->len));
    	fprintf (logfile,"   |-UDP Checksum     : %d\n" , ntohs(udph->check));
     
    	fprintf (logfile, "\n");
    	fprintf (logfile, "IP Header\n");
    	printData (buffer, iphdrlen);
         
    	fprintf (logfile,"UDP Header\n");
    	printData (buffer + iphdrlen , sizeof udph);
         
    	fprintf (logfile,"Data Payload\n");  
    	printData (buffer + iphdrlen + sizeof udph, ( size - sizeof udph - iph->ihl * 4 ));
     
    	fprintf (logfile,"\n###########################################################");
}

void printIcmpPacket (unsigned char* buffer , int size)
{
    	unsigned short iphdrlen;
     
    	struct iphdr *iph = (struct iphdr *)buffer;
    	iphdrlen = iph->ihl*4;
     
    	struct icmphdr *icmph = (struct icmphdr *)(buffer + iphdrlen);
             
    	fprintf(logfile,"\n\n***********************ICMP Packet*************************\n");   
     
    	printIpHeader(buffer , size);
             
    	fprintf(logfile,"\n");
         
    	fprintf(logfile,"ICMP Header\n");
    	fprintf(logfile,"   |-Type : %d",(unsigned int)(icmph->type));
             
    	if((unsigned int)(icmph->type) == 11) 
    	    	fprintf(logfile,"  (TTL Expired)\n");
    	else if((unsigned int)(icmph->type) == ICMP_ECHOREPLY) 
        	fprintf(logfile,"  (ICMP Echo Reply)\n");
    	fprintf(logfile,"   |-Code : %d\n",(unsigned int)(icmph->code));
    	fprintf(logfile,"   |-Checksum : %d\n",ntohs(icmph->checksum));
    	//fprintf(logfile,"   |-ID       : %d\n",ntohs(icmph->id));
    	//fprintf(logfile,"   |-Sequence : %d\n",ntohs(icmph->sequence));
    	fprintf(logfile,"\n");
 
    	fprintf(logfile,"IP Header\n");
    	printData(buffer,iphdrlen);
         
    	fprintf(logfile,"UDP Header\n");
    	printData(buffer + iphdrlen , sizeof icmph);
         
    	fprintf(logfile,"Data Payload\n");  
    	printData(buffer + iphdrlen + sizeof icmph , (size - sizeof icmph - iph->ihl * 4));
     
    	fprintf(logfile,"\n###########################################################");
}

void printData (unsigned char *data, int size)
{
	for (i=0; i < size; i++)
	{
		if( i != 0 && i % 16 == 0)
		{
            		fprintf(logfile,"         ");
            		for(j=i-16; j<i; j++)
            		{
                		if(data[j]>=32 && data[j]<=128)
                    		fprintf(logfile,"%c",(unsigned char)data[j]); //if its a number or alphabet
                 
                		else fprintf(logfile,"."); //otherwise print a dot
            		}
            		fprintf(logfile,"\n");
       		} 
         
        	if(i%16==0) fprintf(logfile,"   ");
            		fprintf(logfile," %02X",(unsigned int)data[i]);
                 
        	if( i==size-1)  //print the last spaces
        	{
            		for(j=0;j<15-i%16;j++) 
				fprintf(logfile,"   "); //extra spaces
             
            		fprintf(logfile,"         ");
             
            		for(j=i-i%16 ; j<=i ; j++)
            		{
                		if(data[j]>=32 && data[j]<=128) 
					fprintf(logfile,"%c",(unsigned char)data[j]);
                		else 
					fprintf(logfile,".");
            		}
            	fprintf(logfile,"\n");
        	}	
	}
}
