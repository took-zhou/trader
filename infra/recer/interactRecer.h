/*
 * interactRecer.h
 *
 *  Created on: 2020Äê8ÔÂ29ÈÕ
 *      Author: Administrator
 */

#ifndef WORKSPACE_TRADER_INFRA_INTERACTRECER_H_
#define WORKSPACE_TRADER_INFRA_INTERACTRECER_H_

#include "trader/infra/define.h"

struct ZmqBase;

struct InteractRecer
{
    bool init(){
        return true;
    };

    MsgStruct rece();

    ZmqBase* zmq{nullptr};

};



#endif /* WORKSPACE_TRADER_INFRA_INTERACTRECER_H_ */
