#include "MM2WayRanging.h"

MM2WayRanging::MM2WayRanging(DW1000& DW) : dw(DW) {
    isAnchor = true;
    overflow = false;
    address = 0;
    sysTimestamp = 0;
    timediffRec = 0;
    timediffSend = 0;
    for (int i = 0; i < 10; i++) {
        acknowledgement[i] = true;
        lasttimes[i] = 0.0f;
    }
    LocalTimer.start();
    dw.setCallbacks(this, &MM2WayRanging::callbackRX, &MM2WayRanging::callbackTX);
    dw.startRX();
}

void MM2WayRanging::callbackRX() {
    dw.readRegister(DW1000_RX_BUFFER, 0, (uint8_t*)&receivedFrame, dw.getFramelength());
    pc.printf("Received type %d source %d destination %d at %f\r\n", receivedFrame.type, receivedFrame.source, receivedFrame.destination, LocalTimer.read());
//    pc.printf("%f\r\n", LocalTimer.read());
    if (receivedFrame.destination == address)
        switch (receivedFrame.type) {
            case PING:
                // Save the first timestamp on the receiving node/anchor (T_rp)
                sysTimestamp = dw.getSYSTimestamp();
//                receiverTimestamps[receivedFrame.source][0] = sysTimestamp;
                receiverTimestamps[receivedFrame.source][0] = dw.getRXTimestamp();
                sendDelayedAnswer(receivedFrame.source, ANCHOR_RESPONSE, sysTimestamp);
                break;
            case ANCHOR_RESPONSE:
                sysTimestamp = dw.getSYSTimestamp();
                // Save the second timestamp on the sending node/beacon (T_rr)
//                senderTimestamps[receivedFrame.source][1] = sysTimestamp;
                senderTimestamps[receivedFrame.source][1] = dw.getRXTimestamp();
                sendDelayedAnswer(receivedFrame.source, BEACON_RESPONSE, sysTimestamp);
                break;
            case BEACON_RESPONSE:
                // Save the third timestamp on the receiving node/anchor (T_rf)
                sysTimestamp = dw.getSYSTimestamp();
//                receiverTimestamps[receivedFrame.source][2] = sysTimestamp;
                receiverTimestamps[receivedFrame.source][2] = dw.getRXTimestamp();
                // Correct the timestamps for the case of a counter overflow
                correctReceiverTimestamps(receivedFrame.source);                
                // Calculation of the summand on the receiving node/anchor
                timediffRec = -2 * receiverTimestamps[receivedFrame.source][1] + receiverTimestamps[receivedFrame.source][0] + receiverTimestamps[receivedFrame.source][2];
                sendTransferFrame(receivedFrame.source, timediffRec);
                break;
            case TRANSFER_FRAME:
                // Calculation of the summand on the sending node/beacon
                timediffSend = 2 * senderTimestamps[receivedFrame.source][1] - senderTimestamps[receivedFrame.source][0] - senderTimestamps[receivedFrame.source][2];
                //calculation of the resulting sum of all four ToFs.
//                pc.printf("Compute %llu %llu %llu, %d, %d\r\n", senderTimestamps[receivedFrame.source][0], senderTimestamps[receivedFrame.source][1], senderTimestamps[receivedFrame.source][2], timediffSend, receivedFrame.signedTime);
                tofs[receivedFrame.source] = receivedFrame.signedTime + timediffSend;
                acknowledgement[receivedFrame.source] = true;
                measuretimes[receivedFrame.source] = LocalTimer.read();
                roundtriptimes[receivedFrame.source] = measuretimes[receivedFrame.source] - lasttimes[receivedFrame.source];
                distances[receivedFrame.source] = calibratedDistance(receivedFrame.source);

//                pc.printf("Distance to %d at %f: %f\r\n", receivedFrame.source, LocalTimer.read(), distances[receivedFrame.source]);
                break;
            default: 
                break;
        }

    dw.startRX();
}

void MM2WayRanging::callbackTX() {
    pc.printf("Send type %d source %d destination %d at %f\r\n", rangingFrame.type, rangingFrame.source, rangingFrame.destination, LocalTimer.read());
    switch (rangingFrame.type) {
    case PING:
        // Save the first timestamp on the sending node/beacon (T_sp)
//        senderTimestamps[rangingFrame.destination][0] = dw.getSYSTimestamp();
        senderTimestamps[rangingFrame.destination][0] = dw.getTXTimestamp();
        break;
    case ANCHOR_RESPONSE:
        // Save the second timestamp on the receiving node/anchor (T_sr)
//        receiverTimestamps[rangingFrame.destination][1] = dw.getSYSTimestamp();
        receiverTimestamps[rangingFrame.destination][1] = dw.getTXTimestamp();
        break;
    case BEACON_RESPONSE:
        // Save the third timestamp on the sending node/beacon (T_sr)
//        senderTimestamps[rangingFrame.destination][2] = dw.getSYSTimestamp();
        senderTimestamps[rangingFrame.destination][2] = dw.getTXTimestamp();
        // Correct the timestamps for the case of a counter overflow
        correctSenderTimestamps(rangingFrame.destination);                      
        break;
    default:
        break;
    }
}

// Get the distance to the Anchor with address @param destination.
void MM2WayRanging::requestRanging(uint8_t destination) {
    bool doit = false;
    if (acknowledgement[destination]) {
        doit = true;
    }
    else {
        float now = LocalTimer.read();
        if (now - lasttimes[destination] > TIMEOUT_S) {
            doit = true;
        }
    }

    doit = true;
    if (doit) {
        acknowledgement[destination] = false;
//        distances[destination] = -1.0f;
        lasttimes[destination] = LocalTimer.read();
        if (lasttimes[destination] > TIMER_TIMEOUT_S) {
            pc.printf("Reset LocalTimer at %f\r\n", lasttimes[destination]);
            LocalTimer.reset();
            for (int i = 0; i < 10; i++) {
                lasttimes[i] = 0.0f;
            }
            LocalTimer.start();
        }
//        pc.printf("MM2WayRanging request ranging for %d start at %f\r\n", destination, LocalTimer.read());
        sendPingFrame(destination);
    }
}

inline float MM2WayRanging::calibratedDistance(uint8_t destination) {
    float rawDistance = (tofs[destination] * 300 * TIMEUNITS_TO_US / 4);

    // Chỉnh với thẻ hiện tại 26012018
//    rawDistance = rawDistance * 0.89483678f - 0.384264041401f;
    rawDistance = rawDistance * 0.88840095f - 0.373507873656f;
    return rawDistance;

}

void MM2WayRanging::requestRangingAll() {
    for (int i = 0; i <= 0; i++) {  
        requestRanging(i);
    }
}

void MM2WayRanging::sendPingFrame(uint8_t destination) {
    rangingFrame.source = address;
    rangingFrame.destination = destination;
    rangingFrame.type = PING;
    rangingFrame.signedTime = 0;
    dw.sendFrame((uint8_t*)&rangingFrame, sizeof(rangingFrame));
}

void MM2WayRanging::sendTransferFrame(uint8_t destination, int timeDiffsReceiver) {
    rangingFrame.source = address;
    rangingFrame.destination = destination;
    rangingFrame.type = TRANSFER_FRAME;
    // Cast the time difference
    rangingFrame.signedTime = timeDiffsReceiver;                      
    dw.sendFrame((uint8_t*)&rangingFrame, sizeof(rangingFrame));
}

void MM2WayRanging::sendDelayedAnswer(uint8_t destination, uint8_t type, uint64_t sysTimestamp) {

    rangingFrame.source = address;
    rangingFrame.destination = destination;
    rangingFrame.type = type;

    if(sysTimestamp + ANSWER_DELAY_TIMEUNITS > MMRANGING_2POWER40)
        dw.sendDelayedFrame((uint8_t*)&rangingFrame, sizeof(rangingFrame), sysTimestamp + ANSWER_DELAY_TIMEUNITS - MMRANGING_2POWER40);
    else
        dw.sendDelayedFrame((uint8_t*)&rangingFrame, sizeof(rangingFrame), sysTimestamp + ANSWER_DELAY_TIMEUNITS);
}

void MM2WayRanging::correctReceiverTimestamps(uint8_t source){
    if(receiverTimestamps[source][0] > receiverTimestamps[source][1]){
        receiverTimestamps[source][1] += MMRANGING_2POWER40;
        receiverTimestamps[source][2] += MMRANGING_2POWER40;
    }

    if(receiverTimestamps[source][1] > receiverTimestamps[source][2]){
        receiverTimestamps[source][2] += MMRANGING_2POWER40;
    }
}

void MM2WayRanging::correctSenderTimestamps(uint8_t source){
    if(senderTimestamps[source][0] > senderTimestamps[source][1]) {
        senderTimestamps[source][1] += MMRANGING_2POWER40;
        senderTimestamps[source][2] += MMRANGING_2POWER40;
        overflow = true;
    }
    else if (senderTimestamps[source][1] > senderTimestamps[source][2]) {
        senderTimestamps[source][2] += MMRANGING_2POWER40;
        overflow = true;
    }
    else {
        overflow = false;
    }
}
