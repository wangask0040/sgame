#!/bin/sh

CFGFILE=/home/dev1/src/test/server/server.xml
BINPATH=/home/dev1/src/Debug/bin

${BINPATH}/LogicSrv ${CFGFILE} &
${BINPATH}/GateSrv ${CFGFILE} &
${BINPATH}/GameSrv ${CFGFILE} &
${BINPATH}/RouterSrv ${CFGFILE} &
${BINPATH}/GmSrv ${CFGFILE} &