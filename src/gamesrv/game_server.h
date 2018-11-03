#pragma once

class CGameSrv
{
public:
	CGameSrv();
	~CGameSrv();

	bool Init(int srvtype, int srvidx);
	void Start();

protected:
	int _srv_type;
	int _srv_idx;
};

