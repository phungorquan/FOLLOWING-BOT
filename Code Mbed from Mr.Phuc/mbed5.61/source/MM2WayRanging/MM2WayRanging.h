// by sonlh5@vng.com.vn,
// based on the work by Matthias Grob & Manuel Stalder - ETH Zürich - 2015

#ifndef MM2WAYRANGING_H
#define MM2WAYRANGING_H

#include "mbed.h"
#include "DW1000.h"

// conversion between the decawave timeunits (ca 15.65ps) to microseconds.
#define TIMEUNITS_TO_US       (1/(128*499.2))
// conversion between microseconds to the decawave timeunits (ca 15.65ps).
#define US_TO_TIMEUNITS       (128*499.2)
// decimal value of 2^40 to correct timeroverflow between timestamps
#define MMRANGING_2POWER40          1099511627776
// timeout (in second) for each request ranging
#define TIMEOUT_S 1.5f
// timeout (in second) to reset Timer
#define TIMER_TIMEOUT_S 1500.0f

//Predefined delay for the critical answers in the ranging algorithm
//HAS TO BE BIGGER THAN THE PROCESSING TIME OF THE FRAME ON THE NODE
//2500 works for 110kbps, 900 for 6.8Mbps
#define ANSWER_DELAY_US             2500
#define ANSWER_DELAY_TIMEUNITS      ANSWER_DELAY_US * (128*499.2)

extern Serial pc;

class MM2WayRanging
{
public:
    MM2WayRanging(DW1000& DW);
    void requestRanging(uint8_t destination);
    void requestRangingAll();

    //TODO: Better capsulation on those?
    bool isAnchor;
    // Identifies the nodes as source and destination in rangingframes
    uint8_t address; 

    //TODO: Make those PRIVATE!
    // Array containing the finally calculated Distances to the anchors
    float distances[10];
    // Array containing the time calculated Distances to the anchors
    float measuretimes[10];
    // TRUE if counter overflows while ranging
    bool overflow;

    // flag to indicate if ranging has succeeded
    bool acknowledgement[10];

private:
    DW1000& dw;
    Timer LocalTimer;

    void callbackRX();
    void callbackTX();
    void sendPingFrame(uint8_t destination);
    void sendDelayedAnswer(uint8_t destination, uint8_t type, uint64_t sysTimestamp);
    void sendTransferFrame(uint8_t destination, int timestamp);

    inline float calibratedDistance(uint8_t destination);

    // These two functions correct the timestamps if the counter had an overflow between measurements
    void correctReceiverTimestamps(uint8_t source);
    void correctSenderTimestamps(uint8_t source);

    int timediffRec;
    int timediffSend;

    // Array containing last time request ranging
    float lasttimes[10];
    // Array containing the round trip times to the anchors or the timeout which occured
    float roundtriptimes[10];

    enum FrameType {
        PING=1,
        ANCHOR_RESPONSE,
        BEACON_RESPONSE,
        TRANSFER_FRAME,
        DISTANCES_FRAME
    };

    // The packed attribute makes sure the types only use their respective size in memory (8 bit for uint8_t),
    // otherwise they would always use 32 bit
    //IT IS A GCC SPECIFIC DIRECTIVE
    struct __attribute__((packed, aligned(1))) RangingFrame {
        uint8_t source;
        uint8_t destination;
        uint8_t type;
        int signedTime;
    };

    // buffer in class for sending a frame (not made locally because then we can recall in the interrupt what was sent)
    RangingFrame rangingFrame;                  
    RangingFrame receivedFrame;
    uint64_t sysTimestamp;
    uint64_t senderTimestamps[10][3];
    uint64_t receiverTimestamps[10][3];
    // Array containing time of flights for each node (index is address of node)
    uint32_t tofs[10];                          
};
#endif
