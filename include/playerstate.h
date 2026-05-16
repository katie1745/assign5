#ifndef PLAYERSTATE_H
#define PLAYERSTATE_H

namespace PlayerState{
    enum MoveState{
        MOVE,
        STOP,
        IDLE//待機狀態，完全不按按鈕時角色狀態
    };
}
#endif