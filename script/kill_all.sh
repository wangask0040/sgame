#!/bin/sh

ps -e | grep LogicSrv | awk '{ print $1}' | xargs kill -9
ps -e | grep GateSrv | awk '{ print $1}' | xargs kill -9
ps -e | grep GameSrv | awk '{ print $1}' | xargs kill -9
ps -e | grep RouterSrv | awk '{ print $1}' | xargs kill -9
ps -e | grep GmSrv | awk '{ print $1}' | xargs kill -9
