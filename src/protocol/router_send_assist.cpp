#include "router_send_assist.h"

void CRouterSend::GmToGame(int src_srv_idx, int dst_srv_idx, protocol::MSG_CMD cmd, unsigned int uid)
{
	MSG_INNER_INFO frame;
	auto& head = frame.head;
	head.cmd = cmd;
	head.uid = uid;
	head.src_srv_id = ROUTER_INFO::GenRouterId(static_cast<int>(SRV_TYPE::rt_gmsrv), src_srv_idx);
	head.dst_srv_id = ROUTER_INFO::GenRouterId(static_cast<int>(SRV_TYPE::rt_gamesrv), dst_srv_idx);

	protocol::msg_info info;
	info.mutable_head()->set_cmd(frame.Cmd());
	info.SerializeToString(&frame.data);
	auto size = CZmqRouterClientWrapperSgt::Instance()->SendFrame(frame);
	NLOGDEBUG << size;
}