#pragma once

class CRouterSrv
{
public:
	CRouterSrv();
	~CRouterSrv();
	bool Init(int srv_type, int srv_idx);
	void Start();

protected:
	int _srv_type;
	int _srv_idx;
};