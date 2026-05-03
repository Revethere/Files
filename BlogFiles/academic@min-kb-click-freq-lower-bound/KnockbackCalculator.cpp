#include <iostream>
#include <cmath>
#include <iomanip>

class entity {
    bool on_ground;
    double s, h;
    double motion_h, motion_y;
    double click_sd;
    
public:
    bool   isOnGround()                { return on_ground = h <= 0; }
    double isClicking(const bool &val) { return click_sd = val ? 0.6 : 1.0; }
    double getMotionH()                { return motion_h; }
    double getSpatium()                { return s; }
    void move(const int &ticks)        {
        static constexpr double mult = 0.91,
                                spring_modifier = 0.3,
                                // walkSpeed = 0.1,
                                // speedThreshold = 0.05,
                                // jumpMovementFactor = 0.02;
                                resist_coeff[] = {0.02, 0.05, 0.1};
        static constexpr double slip[] = {1.0, 0.6};
        static constexpr double speed_threshold = 0.005;

        isOnGround();

        if(motion_h >= 0) isClicking(false);
        motion_h *= mult * slip[on_ground] * click_sd;
        motion_h += (on_ground ?
                    resist_coeff[2] * pow(0.6 / slip[1], 3) :
                    resist_coeff[0]
                    ) * (1.0 + spring_modifier) * 0.98;
        s += motion_h;

        if(!on_ground) {
            motion_y -= 0.08;
            motion_y *= 0.98;
            if(abs(motion_y) <= speed_threshold) motion_y = 0.0;
        }
        h += motion_y;
        if(h < 0.0) h = 0.0;
    }

    entity(const double &motion_h = 0.0, const double &motion_y = 0.0, const bool &on_ground = true, const bool &is_clicking = false): s(0.0), h(0.0), motion_h(motion_h), motion_y(motion_y), on_ground(on_ground), click_sd(isClicking(is_clicking)) {}
};

using namespace std;

signed main() {
    entity player(-0.8835, 0.3622, true, true);

    double motion_h[2] = {0.0, 0.0};
    for(int ticks = 1; ; ++ticks) {
        int i = ticks & 1, j = !i;
        player.move(ticks);
        if((motion_h[i] = player.getMotionH()) == motion_h[j] && ticks != 1 && player.isOnGround()) break;
        
        cout << "[tick " << ticks << "] " << motion_h[i] << " " << player.getSpatium() << "\n";
    }
    return 0;
}