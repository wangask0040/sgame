#pragma once
#include "msg_info.pb.h"
#include "msg_cmd.pb.h"
#include "common/struct_def.h"
#include "common/logging.h"
#include "common/zmqex_connect_wrapper.h"

class CRouterSend
{
public:
	CRouterSend() {}
	~CRouterSend() {}

	static void GmToGame(int src_srv_idx, int dst_srv_idx, protocol::MSG_CMD cmd, unsigned int uid);

private:

};