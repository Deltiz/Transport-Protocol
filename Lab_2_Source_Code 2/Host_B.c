#include "Sim_Engine.h"
#define SIZE 20 // packet data size
int exp_sec; //could be 1 or 2 to handle sequens number
struct pkt ack; //ack/nack packet to send back

/* Called from layer 5, passed the data to be sent to other side */
void B_output( struct msg message) {
  /* DON'T IMPLEMENT */
}

/* Called from layer 3, when a packet arrives for layer 4 */
void B_input(struct pkt packet) {
  /* TODO */
  int payloadCheckSum=0; // for checksum of data in packet
  int checksum; // for new checksum
 
  for (int i = 0; i < SIZE; i++)
    {
      payloadCheckSum += packet.payload[i];
    }
  checksum = packet.seqnum + packet.acknum + payloadCheckSum;
  if (packet.checksum == checksum)
  {
    ack.acknum = packet.acknum;
    ack.seqnum = packet.acknum;
    ack.checksum = ack.acknum + ack.seqnum;

    tolayer3(1, ack);

    printf("Packet %d received, packet.checksum: %d calc_checksum: %d, data: %s\n\n", packet.seqnum, packet.checksum, checksum, packet.payload);
    if (exp_sec == packet.seqnum)
    {
      tolayer5(1, packet.payload);

      if (exp_sec == 0)
      {
        exp_sec = 1;
        printf("ACK 0 sent\n");
      }
      else if (exp_sec == 1)
      {
        exp_sec = 0;
        printf("ACK 1 sent\n");
      }
    }
    else
    {
      printf("Packet %d received\n", packet.seqnum);
      printf("Packet dropped, was expecting packet %d\n", exp_sec);
    }
  }
  else
  {
    printf("Packet %d received\n", packet.seqnum);
    printf("checksum mismatch, packet checksum: %d, calc_checksum: %d\n", packet.checksum, checksum);
  }
}

/* Called when B's timer goes off */
void B_timerinterrupt() {
  /* TODO */
}  

/* The following routine will be called once (only) before any other */
/* Host B routines are called. You can use it to do any initialization */
void B_init() {
  /* TODO */
   exp_sec = 0; // set secqence start number
}
