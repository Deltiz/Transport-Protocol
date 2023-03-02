#include "Sim_Engine.h"
#include <string.h>
#define SIZE 20
#define BUFF_SIZE 2000
 
struct pkt packet;
float timeOut;
int packet_num;
int ACK0, ACK1;
struct msg buffer[BUFF_SIZE];
int buff_read = 0;
int loopNo = 1;
int i = 0;

// Create new checksum
int calculate_checksum(char *p)
{
  int checkSum = 0;
  for (int i = 0; i < SIZE; i++)
  {
    checkSum += p[i];
  }
  return checkSum;
}

//New pkt 
void A_createpkt(int seqNo, struct msg message)
{
  printf("Create a new packet\n");
  packet.seqnum = seqNo;
  packet.acknum = seqNo;
  strcpy(packet.payload, message.data);
  int int_msg = calculate_checksum(packet.payload);
  packet.checksum = int_msg + packet.seqnum + packet.acknum;
}

/* Called from layer 5, passed the data to be sent to other side */
void A_output(struct msg message)
{
  /* TODO */
  buffer[i] = message; // save a copy of data
  i++;
  if (packet_num == 0 && ACK1 == 1)
  {
    ACK0 = 0;
    A_createpkt(0, buffer[buff_read]);
    buff_read++;
    tolayer3(0, packet);
    printf("\nLoop no:%d, buffer[%d], message: %s\n", loopNo, i - 1, message.data);
    loopNo++;
    starttimer(0, timeOut);
    printf("Packet no:0 sent, timer 0 started\n");
    packet_num = 1;
  }

  if (packet_num == 1 && ACK0 == 1)
  {
    ACK1 = 0;
    A_createpkt(1, buffer[buff_read]);
    buff_read++;
    tolayer3(0, packet);
    printf("\nLoop no:%d, buffer[%d], message: %s\n", loopNo, i - 1, message.data);
    loopNo++;
    starttimer(0, timeOut);
    printf("Packet no:1 sent, timer 1 started");
    packet_num = 0;
  }
}

/* Called from layer 3, when a packet arrives for layer 4 */
void A_input(struct pkt packet)
{
  /* TODO */
  int calc_sum = packet.acknum + packet.seqnum;

  printf("checksum: %d, calculated checksum: %d\n\n", packet.checksum, calc_sum);
  if (packet.checksum == calc_sum)
  {
    if (packet.acknum == 0)
    {
      ACK0 = 1;
      stoptimer(0);
      printf("Received ACK 0, timer 0 stopped\n");
    }
    if (packet.acknum == 1)
    {
      ACK1 = 1;
      stoptimer(0);
      printf("Received ACK 1, timer 1 stopped\n");
    }
  }
}

/* Called when A's timer goes off */
void A_timerinterrupt()
{
  /* TODO */
  printf("Timeout for packet: %d. \n", packet.seqnum);

  // Send package ones more + start timer
  tolayer3(0, packet);
  printf("Packet: %d retransmitted.\n", packet.seqnum);
  starttimer(0, timeOut);
}

/* The following routine will be called once (only) before any other */
/* Host A routines are called. You can use it to do any initialization */
void A_init()
{
  /* TODO */
  packet_num = 0;
  timeOut = 11;
  ACK1 = 1;
  ACK0 = 1;
}
